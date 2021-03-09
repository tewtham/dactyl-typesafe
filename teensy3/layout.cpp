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
unused , unused , unused , unused , unused , unused ,
tab    , keyQ   , keyW   , keyE   , keyR   , keyT   ,
minus  , keyA   , keyS   , keyD   , keyF   , keyG   ,
shift  , keyZ   , keyX   , keyC   , keyV   , keyB   ,
                  gui    , tilde  , equal  , unused ,
                           backsp , alt    , home   ,
                           ctrl   , end    ,
// layer 0, right
unused , unused , unused , unused , unused , unused ,
keyY   , keyU   , keyI   , keyO   , keyP   , backsl ,
keyH   , keyJ   , keyK   , keyL   , semicl , quote  ,
keyN   , keyM   , comma  , period , slash  , shift  ,
lftBrc , rgtBrc , alt , gui ,
enter  ,
layer  , space
},{
// layer 1, left
unused , unused , unused , unused , unused , unused,
esc    , scrollX, unused , up     , unused , unused,
minus  , scrollY, left   , down   , right  , unused,
shift  , unused , keyX   , keyC   , keyV   , unused,
                  gui    , unused , unused , unused,
                           backsp , alt    , home  ,
                           ctrl   , end    ,
// layer 1, right
unused , unused , unused , unused , unused , unused,
scrollX, unused , unused , unused , unused , unused,
scrollY, mouseL , mouseM , mouseR , unused , unused,
unused , unused , unused , unused , unused , unused,
unused , unused , alt    , gui    ,
enter  ,
layer  , space
},{
// layer 2, left
unused , unused , unused , unused , unused , unused ,
minus  , bang   , at     , hash   , dola   , perc   ,
esc    , key1   , key2   , key3   , key4   , key5   ,
shift  , ply    , vdwn   , vup    , prev   , nex    ,
                  gui    , unused , unused , unused ,
                           backsp , alt    , home   ,
                           ctrl   , end    ,
// layer 2, right
unused , unused , unused , unused , unused , unused ,
cart   , amp    , ast    , lpar   , rpar   , unde  ,
key6   , key7   , key8   , key9   , key0   , minus ,
unused , unused , unused , unused , unused , unused ,
unused , unused , alt    , gui    ,
unused ,
layer  , unused
}};

// Map state col/row to an index in the layout
const int8_t KEY_MAP [NUM_ROWS][NUM_COLUMNS] = {
  { 0, 1, 2, 3, 4, 5,     33,34,35,36,37,38},
  { 6, 7, 8, 9,10,11,     39,40,41,42,43,44},
  {12,13,14,15,16,17,     45,46,47,48,49,50},
  {18,19,20,21,22,23,     51,52,53,54,55,56},
  {24,25,26,27,-1,28,     -1,-1,57,58,59,60},
  {-1,-1,29,30,32,31,     63,62,61,-1,-1,-1}
};

LayoutSender::LayoutSender() {
  mouseModifiers = 0x00;
  currentLayer = 1;
  mouseButtons[0] = 0;
  mouseButtons[1] = 0;
  mouseButtons[2] = 0;
  reader = Reader();
}

void LayoutSender::init(void) {
  reader.init();
  reader.updateState();
  for (int i = 0; i < NUM_KEYS; i++) {
    pressedLayer[i] = -1;
  }
}

void LayoutSender::updateState() {
  reader.updateState();
  for(uint8_t i = 0; i < NUM_ROWS; i++) {
    for(uint8_t j = 0; j < NUM_COLUMNS; j++) {
      // if the state of the key has changed
      if(state[i][j] != reader.state[i][j]) {
        int index = KEY_MAP[i][j];
        // if the key is pressed
        if(reader.state[i][j]) {
          Serial.print(F("press: "));
          Serial.print(i, DEC);
          Serial.print(F(" "));
          Serial.println(j, DEC);
          pressedLayer[index] = currentLayer;
          btnPress(currentLayer, index);
        } else {
          Serial.print(F("release: "));
          Serial.print(i, DEC);
          Serial.print(F(" "));
          Serial.println(j, DEC);
          if (pressedLayer[index] == -1) pressedLayer[index] = currentLayer;
          btnRelease(pressedLayer[index], index);
          pressedLayer[index] = -1;
        }
        
        state[i][j] = reader.state[i][j];
      }
    }
  }
}

void LayoutSender::btnPress(uint8_t layer, uint8_t index) {
  Key key = LAYER_KEYS[layer][index];

  switch(key.type) {
    case STANDARD_KEY:
    case KEY_MOD:
      Keyboard.press(key.code);
      break;
    case MOUSE_MOD:
      mouseModifiers |= key.code;
      Adns.setModifiers(mouseModifiers);
      break;
    case MOUSE_BUTTON:
      mouseButtons[key.code] = 1;
      setMouseButtons();
      break;
    case TO_LAYER:
      currentLayer = 2;
      break;
  }
}

uint8_t LayoutSender::getLayer() {
  return currentLayer;
}


void LayoutSender::setLayer(uint8_t layer) {
  currentLayer = layer;
}

void LayoutSender::btnRelease(uint8_t layer, uint8_t index) {
  Key key = LAYER_KEYS[layer][index];

  switch(key.type) {
    case STANDARD_KEY:
    case KEY_MOD:
      Keyboard.release(key.code);
      break;
    case MOUSE_MOD:
      mouseModifiers &= ~key.code;
      Adns.setModifiers(mouseModifiers);
      break;
    case MOUSE_BUTTON:
      mouseButtons[key.code] = 0;
      setMouseButtons();
      break;
    case TO_LAYER:
      currentLayer = 0;
      break;
  }
}

void LayoutSender::setMouseButtons(void) {
  Mouse.set_buttons(mouseButtons[MOUSE_L], mouseButtons[MOUSE_M], mouseButtons[MOUSE_R]);
}

LayoutSender Layout = LayoutSender();
