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

Make colorfull effect on a Neo Pixel strip

                              ----------------
                             |        |     | |
                             |        |     | |                 _______________
                             |        |     | |          +5V --|
                            -|         - - -  |-  PA0 -- DI  --| Neo Pixel strip
                            -|                |-  PA1    GND --|___________
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


cn = require 'cn'
timers = require 'timers'
color = require 'color'

local a0 = sys.pin.config('A0', 'LOW')
local a1 = sys.pin.config('A1', 'LOW')

timers.add(
  0.1,
  coroutine.create(
    function()
      local rgb = {}
      for i = 1, 64 do
        rgb[i] = {0, 0, 0}
      end
      local s = 0
      while true do
        for i = 1, 64 do
          local r, g, b = color.hsv((i + s) / 64.0, 1, 0.8)
          rgb[i] = {r, g, b}
        end
        sys.ws2812.send(a0, rgb)
        for i = 1, 64 do
          local r, g, b = color.hsv((i + s) / 512.0, 1, 0.1)
          rgb[i] = {r, g, b}
        end
        sys.ws2812.send(a1, rgb)
        s = s + 4
        coroutine.yield()
      end
    end
  )
)


while sys.idle() do end
