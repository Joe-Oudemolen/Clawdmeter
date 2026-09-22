#include "../../hal/power_hal.h"
#include "board.h"
#include <Arduino.h>

// No PMU and no battery sensing. The BOOT button (GPIO 9, active LOW) is the
// only control, so all its gestures are synthesized here from a polled GPIO:
//
//   short press   - fires once the double-press window has closed after a
//                   release: cycle animation / brightness
//   double press  - two short presses within DOUBLE_WINDOW_MS: toggle
//                   splash <-> usage
//   long press    - fires once when a hold crosses PWR_LONG_MS; release fires
//                   on every release edge. main.cpp uses these for the
//                   hold-3s-then-release pairing gesture.
//
// GPIO 9 is also the chip's download-mode strap, but that is sampled only at
// reset, so using it as a button afterwards is safe.

#define PWR_POLL_MS      20
#define PWR_LONG_MS      1500
#define DOUBLE_WINDOW_MS 350   // max gap between the two taps of a double-press

static bool     pwr_pressed_flag  = false;
static bool     pwr_double_flag   = false;
static bool     pwr_long_flag     = false;
static bool     pwr_released_flag = false;
static bool     last_pwr_state    = false;
static bool     long_fired        = false;
static bool     tap_pending       = false;   // one tap seen, waiting to see if a second follows
static uint32_t press_started_ms  = 0;
static uint32_t tap_released_ms   = 0;
static uint32_t last_poll_ms      = 0;

void power_hal_init(void) {
    pinMode(BTN_PWR_GPIO, INPUT_PULLUP);
}

void power_hal_tick(void) {
    uint32_t now = millis();
    if (now - last_poll_ms < PWR_POLL_MS) return;
    last_poll_ms = now;

    bool down = (digitalRead(BTN_PWR_GPIO) == LOW);
    if (down && !last_pwr_state) {                // press edge
        press_started_ms = now;
        long_fired = false;
    } else if (down && last_pwr_state) {          // held
        if (!long_fired && now - press_started_ms >= PWR_LONG_MS) {
            long_fired    = true;
            pwr_long_flag = true;
            tap_pending   = false;
        }
    } else if (!down && last_pwr_state) {         // release edge
        pwr_released_flag = true;
        if (!long_fired) {
            if (tap_pending && now - tap_released_ms <= DOUBLE_WINDOW_MS) {
                tap_pending     = false;
                pwr_double_flag = true;
            } else {
                tap_pending     = true;
                tap_released_ms = now;
            }
        }
    }
    last_pwr_state = down;

    // A lone tap becomes a short press once the double-press window closes.
    if (tap_pending && !down && now - tap_released_ms > DOUBLE_WINDOW_MS) {
        tap_pending      = false;
        pwr_pressed_flag = true;
    }
}

int  power_hal_battery_pct(void) { return -1; }
bool power_hal_is_charging(void) { return false; }
bool power_hal_is_vbus_in(void)  { return false; }

bool power_hal_pwr_pressed(void) {
    if (pwr_pressed_flag) { pwr_pressed_flag = false; return true; }
    return false;
}

bool power_hal_pwr_double_pressed(void) {
    if (pwr_double_flag) { pwr_double_flag = false; return true; }
    return false;
}

bool power_hal_pwr_long_pressed(void) {
    if (pwr_long_flag) { pwr_long_flag = false; return true; }
    return false;
}

bool power_hal_pwr_released(void) {
    if (pwr_released_flag) { pwr_released_flag = false; return true; }
    return false;
}
