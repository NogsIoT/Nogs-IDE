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

Blink LED with delay function.


                              ----------------
                             |        |     | |
                            -|        |     | |- +3.3V     LED   120 Ohm
                            -|        |     | |-               _____
                            -|         - - -  |-  PA0 --|<|--|_____|---- +3.3V
                            -|                |-  PA1
                      AREF  -|                |-  PA2
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

sys.pin.config("A0", "LOW")

while sys.idle() do
  sys.pin.set("A0", true)
  sys.sleep(0.5)
  sys.pin.set("A0", false)
  sys.sleep(0.5)
end
