#ifndef Mcp_h
#define Mcp_h

#include "./constants.h"

class Mcp23017 {
  private:
    void writeOne(uint8_t, uint8_t);
    void writeBoth(uint8_t, uint8_t, uint8_t);
    uint8_t readOne(uint8_t);
  public:
    Mcp23017();
    void init(void);
    void updateState(bool [NUM_ROWS][NUM_COLUMNS]);
};

#endif
