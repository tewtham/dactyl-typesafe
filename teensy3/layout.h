#ifndef Layout_h
#define Layout_h

#include <stdint.h>
#include "./reader.h"
#include "./constants.h"
#include "./keyboard.h"

class LayoutSender {
  private:
    Reader reader;
    uint8_t currentLayer;
    bool mouseButtons [3];
    void btnPress(uint8_t, uint8_t);
    void btnRelease(uint8_t, uint8_t);
    void setMouseButtons(void);
    uint8_t mouseModifiers;
    int pressedLayer[NUM_KEYS];
  public:
    LayoutSender();
    bool state[NUM_ROWS][NUM_COLUMNS];
    void init(void);
    void updateState();
    uint8_t getLayer();
    void setLayer(uint8_t);
};

extern LayoutSender Layout;

#endif
