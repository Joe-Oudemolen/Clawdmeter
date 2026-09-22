#include "power_hal.h"

// Weak defaults for optional power_hal functions. A board that supports the
// gesture defines a strong version in its own power.cpp, which overrides this.

__attribute__((weak)) bool power_hal_pwr_double_pressed(void) { return false; }
