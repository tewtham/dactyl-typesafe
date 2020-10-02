#include <Arduino.h>
#include <stdint.h>

#include "./reader.h"
#include "./mcp.h"
#include "./constants.h"

#define TEENSY_START_COLUMN NUM_COLUMNS/2

#define readTeensyRow(row) \
pinMode(TEENSY_ROW_##row, OUTPUT); \
delayMicroseconds(10); \
state[row][TEENSY_START_COLUMN    ] = digitalRead(TEENSY_COL_0) != HIGH; \
state[row][TEENSY_START_COLUMN + 1] = digitalRead(TEENSY_COL_1) != HIGH; \
state[row][TEENSY_START_COLUMN + 2] = digitalRead(TEENSY_COL_2) != HIGH; \
state[row][TEENSY_START_COLUMN + 3] = digitalRead(TEENSY_COL_3) != HIGH; \
state[row][TEENSY_START_COLUMN + 4] = digitalRead(TEENSY_COL_4) != HIGH; \
state[row][TEENSY_START_COLUMN + 5] = digitalRead(TEENSY_COL_5) != HIGH; \
pinMode(TEENSY_ROW_##row, INPUT); \
delayMicroseconds(10);

Reader::Reader() {
  mcp = Mcp23017();  
}

void Reader::init(void) {
  mcp.init();

  pinMode(TEENSY_COL_0, INPUT_PULLUP);
  pinMode(TEENSY_COL_1, INPUT_PULLUP);
  pinMode(TEENSY_COL_2, INPUT_PULLUP);
  pinMode(TEENSY_COL_3, INPUT_PULLUP);
  pinMode(TEENSY_COL_4, INPUT_PULLUP);
  pinMode(TEENSY_COL_5, INPUT_PULLUP);

  pinMode(TEENSY_ROW_0, INPUT);
  pinMode(TEENSY_ROW_1, INPUT);
  pinMode(TEENSY_ROW_2, INPUT);
  pinMode(TEENSY_ROW_3, INPUT);
  pinMode(TEENSY_ROW_4, INPUT);
  pinMode(TEENSY_ROW_5, INPUT);
}

void Reader::updateState(void) {
  readTeensyRow(0);
  readTeensyRow(1);
  readTeensyRow(2);
  readTeensyRow(3);
  readTeensyRow(4);
  readTeensyRow(5);

  mcp.updateState(state);
}
