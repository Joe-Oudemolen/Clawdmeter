#pragma once

// Waveshare ESP32-C6-LCD-1.47 — 1.47" 172x320 ST7789 TFT on an ESP32-C6FH8.
//
// Minimal hardware: no touch controller, no PMU/battery sensing, no IMU, no
// speaker. Verified on hardware: an I2C scan across every free GPIO pair finds
// no devices. The only user input is the BOOT button (GPIO 9); RESET is wired
// to EN and is invisible to firmware. There is no PSRAM (8 MB embedded flash).
//
// Pins confirmed on hardware with a hello-world sketch (Arduino_GFX ST7789).

#define BOARD_NAME           "Waveshare LCD 1.47 (C6)"

// ---- Display geometry (landscape) ----
// The panel is natively 172x320 portrait; rotation 3 makes it 320x172 (rotation 1 is the same, upside down). LCD_WIDTH
// / LCD_HEIGHT are the post-rotation size the UI sees; the NATIVE pair is what
// the ST7789 driver is constructed with.
#define LCD_WIDTH            320
#define LCD_HEIGHT           172
#define LCD_NATIVE_WIDTH     172
#define LCD_NATIVE_HEIGHT    320
// 3 = landscape; 1 is the same orientation rotated 180 degrees.
#define LCD_ROTATION         3
// The 172-wide window sits at column 34 of the ST7789's 240x320 GRAM.
#define LCD_COL_OFFSET       34
#define LCD_ROW_OFFSET       0

// ---- SPI display pins (ST7789, 4-wire SPI) ----
#define LCD_MOSI             6
#define LCD_SCLK             7
#define LCD_CS               14
#define LCD_DC               15
#define LCD_RST              21
#define LCD_BL               22    // backlight, LEDC PWM (TFT has no brightness cmd)

// ---- Buttons ----
// BOOT is the only usable button and doubles as the PWR-role button (see
// power.cpp): short = cycle animation/brightness, double = toggle splash <->
// usage, hold 3s + release = pairing. It is NOT wired as the HID Space key.
#define BTN_PWR_GPIO         9

// ---- Capability flags ----
#define BOARD_HAS_SECONDARY_BUTTON 0
#define BOARD_HAS_ROTATION         0
#define BOARD_HAS_IMU              0
#define BOARD_HAS_BATTERY          0
#define BOARD_HAS_IO_EXPANDER      0
