#include <Arduino.h>
#include <Keyboard.h>
#include "./layout.h"
#include "./keys.h"
#include "./adns.h"

// Use this to restart the teensy
#define CPU_RESTART_ADDR (uint32_t *)0xE000ED0C
#define CPU_RESTART_VAL 0x5FA0004
#define CPU_RESTART (*CPU_RESTART_ADDR = CPU_RESTART_VAL);

const Key LAYER_KEYS [NUM_LAYERS][NUM_KEYS] = {{
// layer 0, left
esc    , key1   , key2   , key3   , key4   , key5   ,
tab    , keyQ   , keyW   , keyE   , keyR   , keyT   ,
shift  , keyA   , keyS   , keyD   , keyF   , keyG   ,
shift  , keyZ   , keyX   , keyC   , keyV   , keyB   ,
                  gui    , tilde  , equal  , alt    ,
                                    backsp , ctrl,
                           home   , end    , layerU ,
// layer 0, right
key6   , key7   , key8   , key9   , key0   , minus  ,
keyY   , keyU   , keyI   , keyO   , keyP   , backsl ,
keyH   , keyJ   , keyK   , keyL   , semicl , quote  ,
keyN   , keyM   , comma  , period , slash  , shift  ,
lftBrc , rgtBrc , alt , gui ,
enter  ,
unused , space
},{
// layer 1, left
transp , unused , unused , unused , unused , unused,
unused , scrollX, unused , up     , unused , unused,
transp , scrollY, left   , down   , right  , unused,
transp , unused , transp , transp , transp , unused,
                  transp , unused , unused , transp,
                                    transp , transp,
                           transp , transp , unused,
// layer 1, right
unused , unused , unused , unused , unused , unused,
scrollX, unused , unused , unused , unused , unused,
scrollY, mouseL , mouseM , mouseR , unused , unused,
unused , unused , unused , unused , unused , unused,
unused , unused , transp , transp ,
transp ,
layerD , transp
}};

// Map state col/row to an index in the layout
const int8_t KEY_MAP [NUM_ROWS][NUM_COLUMNS] = {
  { 0, 1, 2, 3, 4, 5,     33,34,35,36,37,38},
  { 6, 7, 8, 9,10,11,     39,40,41,42,43,44},
  {12,13,14,15,16,17,     45,46,47,48,49,50},
  {18,19,20,21,22,23,     51,52,53,54,55,56},
  {24,25,26,27,-1,28,     -1,-1,57,58,59,60},
  {-1,-1,30,31,32,29,     63,62,61,-1,-1,-1}
};

LayoutSender::LayoutSender() {
  mouseModifiers = 0x00;
  keyModifiers = 0x00;
  currentLayer = 1;
  mouseButtons[0] = 0;
  mouseButtons[1] = 0;
  mouseButtons[2] = 0;
  reader = Reader();
}

void LayoutSender::init(void) {
  reader.init();
  reader.updateState();
}

void LayoutSender::updateState() {
  reader.updateState();
  bool shouldSend = false;

  for(uint8_t i = 0; i < NUM_ROWS; i++) {
    for(uint8_t j = 0; j < NUM_COLUMNS; j++) {
      // if the state of the key has changed
      if(state[i][j] != reader.state[i][j]) {
        // if the key is pressed
        if(reader.state[i][j]) {
          Serial.print(F("press: "));
          Serial.print(i, DEC);
          Serial.print(F(" "));
          Serial.println(j, DEC);
          btnPress(currentLayer, KEY_MAP[i][j]);
        } else {
          Serial.print(F("release: "));
          Serial.print(i, DEC);
          Serial.print(F(" "));
          Serial.println(j, DEC);
          btnRelease(currentLayer, KEY_MAP[i][j]);
        }
        shouldSend = true;
      }
      state[i][j] = reader.state[i][j];
    }
  }
  if(shouldSend) {
    Keyboard.send_now();
  }
}

void LayoutSender::btnPress(uint8_t layer, uint8_t index) {
  Key key = LAYER_KEYS[layer][index];

  switch(key.type) {
    case STANDARD_KEY:
      Keyboard.press(key.code);
      break;
    case TRANSP:
      if(layer == 0) break;
      btnPress(layer - 1, index);
      break;
    case MOUSE_MOD:
      mouseModifiers |= key.code;
      Adns.setModifiers(mouseModifiers);
      break;
    case MOUSE_BUTTON:
      mouseButtons[key.code] = 1;
      setMouseButtons();
      break;
    case KEY_MOD:
      keyModifiers |= key.code;
      Serial.println(keyModifiers, HEX);
      Keyboard.set_modifier(keyModifiers);
      break;
    case DOWN_LAYER:
      currentLayer = currentLayer == 0 ? 0 : currentLayer - 1;
      break;
    case UP_LAYER:
      currentLayer = currentLayer == NUM_LAYERS ? NUM_LAYERS : currentLayer + 1;
      break;
  }
}

uint8_t LayoutSender::getLayer() {
  return currentLayer;
}

void LayoutSender::btnRelease(uint8_t layer, uint8_t index) {
  Key key = LAYER_KEYS[layer][index];

  switch(key.type) {
    case STANDARD_KEY:
      Keyboard.release(key.code);
      break;
    case TRANSP:
      if(layer == 0) break;
      btnRelease(layer - 1, index);
      break;
    case MOUSE_MOD:
      mouseModifiers &= ~key.code;
      Adns.setModifiers(mouseModifiers);
      break;
    case MOUSE_BUTTON:
      mouseButtons[key.code] = 0;
      setMouseButtons();
      break;
    case KEY_MOD:
      keyModifiers &= ~key.code;
      Serial.println(keyModifiers, HEX);
      Keyboard.set_modifier(keyModifiers);
      break;
  }
}

void LayoutSender::setMouseButtons(void) {
  Mouse.set_buttons(mouseButtons[MOUSE_L], mouseButtons[MOUSE_M], mouseButtons[MOUSE_R]);
}

LayoutSender Layout = LayoutSender();
