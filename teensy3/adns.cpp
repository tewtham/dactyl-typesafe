// https://github.com/pepijndevos/Dwergmuis/blob/master/dwergmuis.ino
#include <Mouse.h>
#include <Arduino.h>
#include <avr/pgmspace.h>
#include <tgmath.h>

#include "./adns.h"
#include "./adnsfirmware.h"
#include "./spi4teensy3.h"
#include "./constants.h"
#include "./layout.h"

#define beginCommunication() digitalWrite(CS_PIN, LOW);
#define endCommunication() digitalWrite(CS_PIN, HIGH);

#define RESOLUTION_SETTING 0x0A // 0x29 max, 0x09 default
#define RESOLUTION_SETTING_SCROLL 0x01

#define POWER_UP_RESET 0x3A
#define CONFIGURATION_I 0x0F
#define CONFIGURATION_IV 0x39
#define SROM_ENABLE 0x13
#define SROM_LOAD_BURST 0x62
#define LASER_CTRL0 0x20

#define MOTION_BURST 0x50
#define MOTION 0x02
#define DELTA_X_LOW 0x03
#define DELTA_X_HIGH 0x04
#define DELTA_Y_LOW 0x05
#define DELTA_Y_HIGH 0x06

#define SCROLL_X_ENABLED (modifiers & MOUSEMOD_SCROLLX) == MOUSEMOD_SCROLLX
#define SCROLL_Y_ENABLED (modifiers & MOUSEMOD_SCROLLY) == MOUSEMOD_SCROLLY
#define SCROLL_ENABLED (SCROLL_X_ENABLED || SCROLL_Y_ENABLED)
#define SCROLL_ADJ 10.0 // when scrolling, we'll divide the movement by this much

Adns9800::Adns9800() {
  modifiers = 0x00;
}

void Adns9800::init(void) {
  pinMode(CS_PIN, OUTPUT);
  pinMode(MOT_PIN, INPUT);
  attachInterrupt(MOT_PIN, handleInterrupt, FALLING);

  // 48MHz / 24 = 2MHz = fSCLK
  spi4teensy3::init(5,1,1);

  // ensure that the serial port is reset
  endCommunication();
  beginCommunication();
  endCommunication();

  // tell it to reboot
  adnsWrite(POWER_UP_RESET, 0x5A);
  delay(50);

  // read registers 0x02 to 0x06 (and discard the data)
  adnsRead(MOTION);
  adnsRead(DELTA_X_LOW);
  adnsRead(DELTA_X_HIGH);
  adnsRead(DELTA_Y_LOW);
  adnsRead(DELTA_Y_HIGH);

  // upload the firmware
  uploadFirmware();
  delay(10);

  //enable laser(bit 0 = 0b), in normal mode (bits 3,2,1 = 000b)
  // reading the actual value of the register is important because the real
  // default value is different from what is said in the datasheet, and if you
  // change the reserved bytes (like by writing 0x00...) it would not work.
  uint8_t laserCtrl0 = adnsRead(LASER_CTRL0);
  adnsWrite(LASER_CTRL0, laserCtrl0 & 0xf0 );
  
  delay(100);

  configure(RESOLUTION_SETTING);
}


bool _somethingHappened = false;
void Adns9800::handleInterrupt(void) {
  _somethingHappened = true;
}

void Adns9800::setModifiers(uint8_t mods) {
  modifiers = mods;
  if (SCROLL_ENABLED) {
    configure(RESOLUTION_SETTING_SCROLL);
  } else {
    configure(RESOLUTION_SETTING);
  }
}

uint8_t _xSkips = 0, _ySkips = 0;
void Adns9800::updateState() {
  if(!_somethingHappened) {
    _somethingHappened = false; 
    return;
  }
  _somethingHappened = false;

  uint8_t data[6];
  
  beginCommunication();
  
  // send address of the register, with MSBit = 0 to indicate it's a read
  spi4teensy3::send(MOTION_BURST & 0x7F);
  delayMicroseconds(100); // tSRAD
  // read data
  spi4teensy3::receive(data, 6);

  int16_t y = (data[3] << 8) | data[2];
  int16_t x = (data[5] << 8) | data[4];

  // apply bounds -127 - 127
  x = fmin(fmax(x, -127), 127);
  y = fmin(fmax(y, -127), 127);
  
  delayMicroseconds(1); // tSCLK-NCS for read operation is 120ns
  endCommunication();
  delayMicroseconds(19); //  tSRW/tSRR (=20us) minus tSCLK-NCS

  /*
  // small twist
  double r = sqrt(pow(x, 2.0) + pow(y, 2.0));
  double a = atan2(y, x) + 0.8;
  x = floor(r * cos(a));
  y = floor(r * sin(a));

  if ( x != 0 || y != 0 ) {
    Serial.print(x, DEC);
    Serial.print(F(","));
    Serial.println(y);
  }
  */

  if (Layout.getLayer() != 0x01) {
    return;
  }
  if (!SCROLL_ENABLED) {
    Mouse.move(x, y);
  } else {
    _xSkips++;
    _ySkips++;
    
    // The lowest resolution setting on the adns 9800 is still too high for scrolling, so we need to slow it down even more
    y = round(-(y/SCROLL_ADJ) * _ySkips);
    x = round(x/SCROLL_ADJ * _xSkips);

    // if after adjusting the movement we're not at 0, reset skips
    if (y != 0) {
      _ySkips = 0;
    }
    if (x != 0) {
      _xSkips = 0;
    }

    Mouse.scroll(
      SCROLL_Y_ENABLED ? y : 0,
      SCROLL_X_ENABLED ? x : 0
    );
  }
}

uint8_t Adns9800::adnsRead(uint8_t address) {
  beginCommunication();
  
  // send adress of the register, with MSBit = 0 to indicate it's a read
  spi4teensy3::send(address & 0x7F );
  delayMicroseconds(100); // tSRAD
  // read data
  uint8_t data = spi4teensy3::receive();
  
  delayMicroseconds(1); // tSCLK-NCS for read operation is 120ns
  endCommunication();
  delayMicroseconds(19); //  tSRW/tSRR (=20us) minus tSCLK-NCS

  return data;
}

void Adns9800::adnsWrite(uint8_t address, uint8_t data) {
  beginCommunication();
  
  //send adress of the register, with MSBit = 1 to indicate it's a write
  spi4teensy3::send(address | 0x80 );
  //sent data
  spi4teensy3::send(data);
  
  delayMicroseconds(20); // tSCLK-NCS for write operation
  endCommunication();
  delayMicroseconds(100); // tSWW/tSWR (=120us) minus tSCLK-NCS. Could be shortened, but is looks like a safe lower bound 
}

void Adns9800::configure(uint8_t setting) {
  adnsWrite(CONFIGURATION_I, setting);
}

void Adns9800::uploadFirmware(void) {
  // set the configuration_IV register in 3k firmware mode
  adnsWrite(CONFIGURATION_IV, 0x02); // bit 1 = 1 for 3k mode, other bits are reserved 
  
  // write 0x1d in SROM_enable reg for initializing
  adnsWrite(SROM_ENABLE, 0x1d); 
  
  // wait for more than one frame period
  delay(10); // assume that the frame rate is as low as 100fps... even if it should never be that low
  
  // write 0x18 to SROM_enable to start SROM download
  adnsWrite(SROM_ENABLE, 0x18); 
  
  // write the SROM file (=firmware data) 
  beginCommunication();
  spi4teensy3::send(SROM_LOAD_BURST | 0x80); // write burst destination adress
  delayMicroseconds(15);
  
  // send all bytes of the firmware
  unsigned char c;
  for(int i = 0; i < firmware_length; i++){ 
    c = (unsigned char)pgm_read_byte(firmware_data + i);
    spi4teensy3::send(c);
    delayMicroseconds(15);
  }
  endCommunication();
}

Adns9800 Adns = Adns9800();
