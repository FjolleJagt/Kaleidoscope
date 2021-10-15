/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-Model100 -- Keyboardio Model100 hardware support for Kaleidoscope
 * Copyright (C) 2017-2021  Keyboard.io, Inc
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#ifdef ARDUINO_keyboardio_model_100

#define _INIT_HID_GETSHORTNAME // warning "TODO: remove this directive once we have HID"

#ifndef EEPROM_EMULATION_SIZE
#define EEPROM_EMULATION_SIZE 4096
#endif

#include <Arduino.h>

#define CRGB(r,g,b) (cRGB){b, g, r}

struct cRGB {
  uint8_t b;
  uint8_t g;
  uint8_t r;
};


#include "kaleidoscope/driver/keyscanner/Base.h"
#include "kaleidoscope/driver/hid/RCMComposite.h"
#include "kaleidoscope/driver/keyboardio/Model100Side.h"
#include "kaleidoscope/driver/led/Base.h"
#include "kaleidoscope/device/Base.h"

namespace kaleidoscope {
namespace device {
namespace keyboardio {

struct Model100StorageProps: public kaleidoscope::driver::storage::GD32FlashProps {
  static constexpr uint16_t length = EEPROM_EMULATION_SIZE;
};


struct Model100LEDDriverProps : public kaleidoscope::driver::led::BaseProps {
  static constexpr uint8_t led_count = 64;
  static constexpr uint8_t key_led_map[] PROGMEM = {
    3, 4, 11, 12, 19, 20, 26, 27,     36, 37, 43, 44, 51, 52, 59, 60,
    2, 5, 10, 13, 18, 21, 25, 28,     35, 38, 42, 45, 50, 53, 58, 61,
    1, 6, 9, 14, 17, 22, 24, 29,     34, 39, 41, 46, 49, 54, 57, 62,
    0, 7, 8, 15, 16, 23, 31, 30,     33, 32, 40, 47, 48, 55, 56, 63,
  };
};

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
class Model100LEDDriver : public kaleidoscope::driver::led::Base<Model100LEDDriverProps> {
 public:
  static void syncLeds();
  static void setCrgbAt(uint8_t i, cRGB crgb);
  static cRGB getCrgbAt(uint8_t i);
  static void setBrightness(uint8_t brightness);
  static uint8_t getBrightness();

  static void enableHighPowerLeds();

 private:
  static bool isLEDChanged;
};
#else // ifndef KALEIDOSCOPE_VIRTUAL_BUILD
class Model100LEDDriver;
#endif // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

struct Model100KeyScannerProps : public kaleidoscope::driver::keyscanner::BaseProps {
  static constexpr uint8_t matrix_rows = 4;
  static constexpr uint8_t matrix_columns = 16;
  typedef MatrixAddr<matrix_rows, matrix_columns> KeyAddr;
};

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
class Model100KeyScanner : public kaleidoscope::driver::keyscanner::Base<Model100KeyScannerProps> {
 private:
  typedef Model100KeyScanner ThisType;
 public:
  static void setup();
  static void scanMatrix();
  static void readMatrix();
  static void actOnMatrixScan();

  static bool isKeyswitchPressed(KeyAddr key_addr);
  static uint8_t pressedKeyswitchCount();

  static bool wasKeyswitchPressed(KeyAddr key_addr);
  static uint8_t previousPressedKeyswitchCount();

  static void setKeyscanInterval(uint8_t interval);

 protected:
  static driver::keyboardio::keydata_t leftHandState;
  static driver::keyboardio::keydata_t rightHandState;
  static driver::keyboardio::keydata_t previousLeftHandState;
  static driver::keyboardio::keydata_t previousRightHandState;

  static void actOnHalfRow(byte row, byte colState, byte colPrevState, byte startPos);
  static void enableScannerPower();
  static void disableScannerPower();
};
#else // ifndef KALEIDOSCOPE_VIRTUAL_BUILD
class Model100KeyScanner;
#endif // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

struct Model100Props : public kaleidoscope::device::BaseProps {
  typedef Model100LEDDriverProps  LEDDriverProps;
  typedef Model100LEDDriver LEDDriver;
  typedef Model100KeyScannerProps KeyScannerProps;
  typedef Model100KeyScanner KeyScanner;
  typedef Model100StorageProps StorageProps;
  typedef kaleidoscope::driver::hid::RCMCompositeProps HIDProps;
  typedef kaleidoscope::driver::hid::RCMComposite<HIDProps> HID;
  static constexpr const char *short_name = "kbio100";
};

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD

class Model100 : public kaleidoscope::device::Base<Model100Props> {
 public:
  void setup();

  auto serialPort() -> decltype(kaleidoscope::driver::hid::rcmcomposite::CompositeSerial) & {
    return kaleidoscope::driver::hid::rcmcomposite::CompositeSerial;
  }

  static void enableHardwareTestMode();
};

#endif // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

} // namespace keyboardio
} // namespace device

EXPORT_DEVICE(kaleidoscope::device::keyboardio::Model100)

}

#define PER_KEY_DATA_STACKED(dflt,                                    \
               r0c0, r0c1, r0c2, r0c3, r0c4, r0c5, r0c6,                \
               r1c0, r1c1, r1c2, r1c3, r1c4, r1c5, r1c6,                \
               r2c0, r2c1, r2c2, r2c3, r2c4, r2c5,                      \
               r3c0, r3c1, r3c2, r3c3, r3c4, r3c5, r2c6,                \
               r0c7, r1c7, r2c7, r3c7,                                  \
               r3c6,                                                    \
                                                                        \
               r0c9,  r0c10, r0c11, r0c12, r0c13, r0c14, r0c15,         \
               r1c9,  r1c10, r1c11, r1c12, r1c13, r1c14, r1c15,         \
                      r2c10, r2c11, r2c12, r2c13, r2c14, r2c15,         \
               r2c9,  r3c10, r3c11, r3c12, r3c13, r3c14, r3c15,         \
               r3c8,  r2c8,  r1c8, r0c8,                                \
               r3c9, ...)                                               \
                                                                        \
    r0c0, r0c1, r0c2, r0c3, r0c4, r0c5, r0c6, r0c7, r0c8, r0c9, r0c10, r0c11, r0c12, r0c13, r0c14, r0c15, \
    r1c0, r1c1, r1c2, r1c3, r1c4, r1c5, r1c6, r1c7, r1c8, r1c9, r1c10, r1c11, r1c12, r1c13, r1c14, r1c15, \
    r2c0, r2c1, r2c2, r2c3, r2c4, r2c5, r2c6, r2c7, r2c8, r2c9, r2c10, r2c11, r2c12, r2c13, r2c14, r2c15, \
    r3c0, r3c1, r3c2, r3c3, r3c4, r3c5, r3c6, r3c7, r3c8, r3c9, r3c10, r3c11, r3c12, r3c13, r3c14, RESTRICT_ARGS_COUNT((r3c15), 64, KEYMAP_STACKED, ##__VA_ARGS__)

#define PER_KEY_DATA(dflt,                                                                                 \
  r0c0, r0c1, r0c2, r0c3, r0c4, r0c5, r0c6,        r0c9,  r0c10, r0c11, r0c12, r0c13, r0c14, r0c15, \
  r1c0, r1c1, r1c2, r1c3, r1c4, r1c5, r1c6,        r1c9,  r1c10, r1c11, r1c12, r1c13, r1c14, r1c15, \
  r2c0, r2c1, r2c2, r2c3, r2c4, r2c5,                     r2c10, r2c11, r2c12, r2c13, r2c14, r2c15, \
  r3c0, r3c1, r3c2, r3c3, r3c4, r3c5, r2c6,        r2c9,  r3c10, r3c11, r3c12, r3c13, r3c14, r3c15, \
              r0c7, r1c7, r2c7, r3c7,                             r3c8,  r2c8,  r1c8, r0c8,         \
                          r3c6,                                          r3c9, ...)                      \
    r0c0, r0c1, r0c2, r0c3, r0c4, r0c5, r0c6, r0c7, r0c8, r0c9, r0c10, r0c11, r0c12, r0c13, r0c14, r0c15, \
    r1c0, r1c1, r1c2, r1c3, r1c4, r1c5, r1c6, r1c7, r1c8, r1c9, r1c10, r1c11, r1c12, r1c13, r1c14, r1c15, \
    r2c0, r2c1, r2c2, r2c3, r2c4, r2c5, r2c6, r2c7, r2c8, r2c9, r2c10, r2c11, r2c12, r2c13, r2c14, r2c15, \
    r3c0, r3c1, r3c2, r3c3, r3c4, r3c5, r3c6, r3c7, r3c8, r3c9, r3c10, r3c11, r3c12, r3c13, r3c14, RESTRICT_ARGS_COUNT((r3c15), 64, KEYMAP, ##__VA_ARGS__)

#endif
