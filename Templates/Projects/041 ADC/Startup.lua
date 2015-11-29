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
                            -|        |     | |- +3.3V
                            -|        |     | |-
                            -|         - - -  |-  PA0
                            -|                |-  PA1
                      AREF  -|                |-  PA2
                      AGND  -|                |-  PA3
                      PB12  -|                |-  PA4
                      PB11  -|                |-  PA7
                      PB10  -|                |-  PA8           10k                        10k
                      PB3   -|                |-  PA9         ____/__                    ____/__
                      PB2   -|                |-  PA10  0V --|___/___|-- +3.3V     0V --|___/___|-- +3.3V
                      PB1   -|                |-  PA15          /                          /
                      PB0   -|                |-  PA16         |                          |
                      PA21  -|                |-  PA17 --------                           |
                      PA22  -|                |-  PA18 -----------------------------------
                      PA23  -|                |-  PA19
                      PA24  -|                |-  PA20
                      GND   -|                |-  +5V
                              ----------------
--]]
cn = require 'cn'

if not sys.adc then
  print('*** Sorry! This device has no sys.adc.* support! ***')
  sys.stop()
end

sys.adc.open()

sys.adc.config(0, 'UNIPOLAR')
sys.adc.config(1, 'UNIPOLAR')

while true do

  -- Read out channel by channel
  print('A', sys.adc.get(0), sys.adc.get(1))
  sys.sleep(0.2)

  -- Read out channels with one call
  print('B', sys.adc.get(0, 1))
  sys.sleep(0.2)

  -- Bulk read out all enabled channels
  print('C', sys.adc.get())
  sys.sleep(0.2)

end

while sys.idle() do end
