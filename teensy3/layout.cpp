#include <Arduino.h>
#include <Keyboard.h>
#include "./layout.h"
#include "./keys.h"
#include "./adns.h"

// Use this to restart the teensy
#define CPU_RESTART_ADDR (uint32_t *)0xE000ED0C
#define CPU_RESTART_VAL 0x5FA0004
#define CPU_RESTART (*CPU_RESTART_ADDR = CPU_RESTART_VAL);

// these macros (LAYER_KEYS and KEY_MAP) are from either "4x6.h" or "5x6.h" depending on what's in "keyboard.h"
const Key layerKeys [3][NUM_KEYS] = LAYER_KEYS;
const int8_t keyMap [NUM_ROWS][NUM_COLUMNS] = KEY_MAP;

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
        int index = keyMap[i][j];
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
  Key key = layerKeys[layer][index];

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
  Key key = layerKeys[layer][index];

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
