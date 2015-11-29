--[[
 *
 *
 *  Copyright (C) Nogs GmbH, Andre Riesberg
 *  
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, 
 *  write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
--]]
--[[

A simple treffic light


                              ----------------
                             |        |     | |
                            -|        |     | |- +3.3V   LEDs  120 Ohm
                            -|        |     | |-              _____
                            -|         - - -  |-  PA0 --|<|--|_____|--+- +3.3V  red
                            -|                |-  PA1 --|<|--|_____|--+         yellow
                      AREF  -|                |-  PA2 --|<|--|_____|--+         green
                      AGND  -|                |-  PA3
                      PB12  -|                |-  PA4
                      PB11  -|                |-  PA7
                      PB10  -|                |-  PA8
                      PB3   -|                |-  PA9
                      PB2   -|                |-  PA10
                      PB1   -|                |-  PA15
                      PB0   -|                |-  PA16
                      PA21  -|                |-  PA17
                      PA22  -|                |-  PA18
                      PA23  -|                |-  PA19
                      PA24  -|                |-  PA20
                      GND   -|                |-  +5V
                              ----------------
--]]

cn = require('cn')
timers = require('timers')


timers.add(
  0.2,
  coroutine.create(
    function()
      local red = sys.pin.config("A0", "LOW")
      local yellow = sys.pin.config("A1", "LOW")
      local green = sys.pin.config("A2", "LOW")
      while true do

        sys.pin.set(red, true)

        coroutine.yield(10.0)  -- Wait 10 seconds

        sys.pin.set(yellow, true)

        coroutine.yield(2.0) -- Wait 2 seconds

        sys.pin.set(red, false)
        sys.pin.set(yellow, false)
        sys.pin.set(green, true)

        coroutine.yield(10.0) -- Wait 10 seconds

        sys.pin.set(red, false)
        sys.pin.set(yellow, true)
        sys.pin.set(green, false)

        coroutine.yield(2.0) -- Wait 2 seconds

        sys.pin.set(yellow, false)

      end
    end
  )
)


while sys.idle() do end
