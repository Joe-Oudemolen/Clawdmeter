#include "../../hal/input_hal.h"

// BOOT is claimed by the PWR-role handler in power.cpp, so no button is
// exposed as PRIMARY/SECONDARY. This disables the HID Space (PTT) and
// Shift+Tab keys on this board, a deliberate tradeoff for a one-button device.

void input_hal_init(void) {}

bool input_hal_is_held(InputButton btn) {
    (void)btn;
    return false;
}
