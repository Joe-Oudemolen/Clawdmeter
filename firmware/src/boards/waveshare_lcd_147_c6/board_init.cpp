#include "board.h"
#include <Arduino.h>

// Nothing to bring up: no I2C peripherals, no IO expander, no power-hold line.
extern "C" void board_init(void) {}
