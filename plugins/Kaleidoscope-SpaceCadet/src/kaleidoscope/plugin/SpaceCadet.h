/* -*- mode: c++ -*-
 * Kaleidoscope-SpaceCadet -- Space Cadet Shift Extended
 * Copyright (C) 2016, 2017, 2018  Keyboard.io, Inc, Ben Gemperline
 * Copyright (C) 2019  Keyboard.io, Inc
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

#include "kaleidoscope/Runtime.h"
#include "kaleidoscope/KeyEventTracker.h"
#include <Kaleidoscope-Ranges.h>

#ifndef SPACECADET_MAP_END
#define SPACECADET_MAP_END (kaleidoscope::plugin::SpaceCadet::KeyBinding) { Key_NoKey, Key_NoKey, 0 }
#endif

#define Key_SpaceCadetEnable  Key(kaleidoscope::ranges::SC_FIRST)
#define Key_SpaceCadetDisable Key(kaleidoscope::ranges::SC_LAST)

namespace kaleidoscope {
namespace plugin {

class SpaceCadet : public kaleidoscope::Plugin {
 public:
  // Internal Class
  // Declarations for the modifier key mapping
  class KeyBinding {
   public:
    // Empty constructor; set the vars separately
    KeyBinding() {}
    // Constructor with input and output
    KeyBinding(Key input, Key output);
    // Constructor with all three set
    KeyBinding(Key input, Key output, uint16_t timeout);
    // The key that is pressed
    Key input;
    // the key that is sent
    Key output;
    // The timeout (default to global timeout)
    uint16_t timeout = 0;
    // to check for the end of a list (SPACECADET_MAP_END)
    bool isEmpty() const {
      return (input == Key_NoKey && output == Key_NoKey && timeout == 0);
    }
  };

  SpaceCadet(void);

  // Methods
  static void enable(void);
  static void disable(void);
  static bool active(void);

  // Publically accessible variables
  static uint16_t time_out;  //  The global timeout in milliseconds
  static SpaceCadet::KeyBinding * map;  // The map of key bindings

  EventHandlerResult onPhysicalKeyEvent(KeyEvent &event);
  EventHandlerResult afterEachCycle();

 private:
  static bool disabled;

  static KeyEventTracker event_tracker_;

  static int8_t pending_map_index_;
  static KeyEvent pending_event_;
  static uint16_t start_time_;

  int8_t getSpaceCadetKeyIndex(Key key) const;
};
}

}

extern kaleidoscope::plugin::SpaceCadet SpaceCadet;
