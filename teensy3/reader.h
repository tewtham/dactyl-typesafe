#ifndef Reader_h
#define Reader_h

#include "./constants.h"
#include "./mcp.h"
#include "./keyboard.h"

class Reader {
  private:
    Mcp23017 mcp;
  public:
    Reader();
    void init(void);
    bool state[NUM_ROWS][NUM_COLUMNS];
    void updateState(void);
};

#endif
