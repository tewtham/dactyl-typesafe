#include "./mcp.h"
#include "./layout.h"
#include "./adns.h"

void setup() {
  Layout.init();
  Adns.init();
}

void loop() {
  Layout.updateState();
  Adns.updateState();
}
