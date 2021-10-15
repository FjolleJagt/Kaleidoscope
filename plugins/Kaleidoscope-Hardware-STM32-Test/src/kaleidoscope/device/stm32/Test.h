/* -*- mode: c++ -*-
 * Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2021  Keyboard.io, Inc.
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

#ifdef KBIO_TEST

#include <Arduino.h>

#include "kaleidoscope/device/Base.h"
#include "kaleidoscope/driver/hid/RCMComposite.h"

namespace kaleidoscope {
namespace device {
namespace stm32 {

struct TestProps: public kaleidoscope::device::BaseProps {
  typedef kaleidoscope::driver::hid::RCMCompositeProps HIDProps;
  typedef kaleidoscope::driver::hid::RCMComposite<HIDProps> HID;

  static constexpr const char *short_name = "KBIOTest";
};

class Test: public kaleidoscope::device::Base<TestProps> {
 public:
  auto serialPort() -> decltype(kaleidoscope::driver::hid::rcmcomposite::CompositeSerial) & {
    return kaleidoscope::driver::hid::rcmcomposite::CompositeSerial;
  }
};

#define PER_KEY_DATA(dflt,                                           \
         R0C0, R0C1                                                  \
  )                                                                  \
         R0C0, R0C1

} // namespace stm32
} // namespace device

EXPORT_DEVICE(kaleidoscope::device::stm32::Test)

} // namespace kaleidoscope

#endif
