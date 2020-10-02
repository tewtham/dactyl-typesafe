#ifndef Adns_h
#define Adns_h

#include <stdint.h>

class Adns9800 {
  private:
    static void handleInterrupt(void);
    void adnsWrite(uint8_t, uint8_t);
    uint8_t adnsRead(uint8_t);
    void uploadFirmware(void);
    void configure(uint8_t);
    uint8_t modifiers;
  public:
    Adns9800();
    void init(void);
    void updateState();
    void setModifiers(uint8_t);
};

extern Adns9800 Adns;

#endif
