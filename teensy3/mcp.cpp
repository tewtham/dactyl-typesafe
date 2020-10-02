#include <Wire.h>
#include <WireIMXRT.h>
#include <WireKinetis.h>
#include "./mcp.h"
#include "./constants.h"

#define DEVICE 0x20 //default addrress of IO chip

#define GPIOA 0x12 // i/o pins
#define GPIOB 0x13

#define GPPUA 0x0C // pull up resistors
#define GPPUB 0x0D

#define IODIRA 0x00  // i/o direction
#define IODIRB 0x01

#define OLATA  0x14  // output latch
#define OLATB  0x15

Mcp23017::Mcp23017() {}

void Mcp23017::init(void) {
  Wire.begin();
  Wire.setSDA(SDA_PIN);
  Wire.setSCL(SCL_PIN);

  /* set memory pointer GPPU - pull up resistors
   * enable for the columns, disable for the rows
   */
  writeBoth(GPPUA, 0xFF, 0x00);

  /* set memory pointer IODIR - pin mode
   * set output for rows, input for columns
   */
  writeBoth(IODIRA, 0xFF, 0x00);

  /* set memory pointer OLAT - output latches
   * enable for colunms, doesn't matter for the rows
   */
  writeBoth(OLATA, 0xFF, 0xFF);
}

void Mcp23017::updateState(bool state[NUM_ROWS][NUM_COLUMNS]) {
  for (uint8_t row = 0; row < NUM_ROWS; row++) {
    // set the row we want to read to low
    writeOne(GPIOB, 0xFF & ~(1 << row));

    // read the column
    uint8_t data = readOne(GPIOA);

    for (uint8_t col = 0; col < NUM_COLUMNS/2; col++) {
      state[row][col] = ( (~data) & (1 << col) ) == (1 << col);
    }
  }

  // reset all the rows
  writeOne(GPIOB, 0xFF);
}

void Mcp23017::writeOne(uint8_t reg, uint8_t data) {
  Wire.beginTransmission(DEVICE);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}

void Mcp23017::writeBoth(uint8_t reg, uint8_t a, uint8_t b) {
  Wire.beginTransmission(DEVICE);
  Wire.write(reg);
  Wire.write(a);
  Wire.write(b);
  Wire.endTransmission();
}

uint8_t Mcp23017::readOne(uint8_t reg) {
  Wire.beginTransmission(DEVICE);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(DEVICE, 1);
  return Wire.read();
}
