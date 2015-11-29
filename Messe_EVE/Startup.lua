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
                              ----------------
                             |        |     | |
                             |        |     | |
                             |        |     | |
                            -|         - - -  |-  PA0
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
    (3) MISO -- MISO  PA22  -|                |-  PA18
    (2) MOSI -- MOSI  PA23  -|                |-  PA19
    (1) SCK  -- SCK   PA24  -|                |-  PA20
    (4) CS#  -- CS    GND   -|                |-  +5V
    (6) +5V                   ----------------
    (9) GND
   (10) GND

--]]

require 'cn'
require 'cn.timers'
--require 'Nogs-IDE'
local json = require 'json'
local coap = require 'coap'

function tprint (tbl, indent)
  if not indent then indent = 0 end
  for k, v in pairs(tbl) do
    formatting = string.rep("  ", indent) .. k .. ": "
    if type(v) == "table" then
      print(formatting)
      tprint(v, indent+1)
    elseif type(v) == 'boolean' then
      print(formatting .. tostring(v))
    else
      print(formatting .. v)
    end
  end
end


print('*** Sample EVE ***')

local timers = require 'cn.timers'

local e = sys.eve.open(0)
local count = 0

--sys.eve.load(0, '2:/lenaface40.raw')
--sys.eve.load(0, '2:/lenaface40_16M.bmp')
--sys.eve.load(0, '2:/lenaface40_256.bmp')

local t1 = 0.0

function getCommandList()
  count = count + 1
  return {
    e.CMD_DLSTART(),
    e.CLEAR_RGB(0, 0, 0),
    e.CLEAR(1, 1, 1),
    e.CMD_BGCOLOR(0, 0, 128),
    e.CMD_GAUGE(112, 81, 80, 0, 4, 8, math.floor(t1), 100),
    e.CMD_BGCOLOR(59, 177, 0),
    e.CMD_GAUGE(350, 201, 46, 0, 4, 10, math.floor(t1), 40),
    e.CMD_TEXT(295, 50, 28, 0, t1),
    e.CMD_TEXT(295, 70, 28, 0, count)
  }
end

timers.add(
  0.01,
  function()
    sys.eve.set(getCommandList())
    -- print('xxx')
  end
)



function COAPHandler(socket, ip, port, events, status)
  print('COAPHandler', socket, ip, port, events, status)
  local recv = sys.w5500.recv(socket)
  print('Recv', recv)
  local jsonString = coap.unwrap.JSON(recv)
  print(jsonString)
  local t = json.decode(jsonString)
  print('-->', string.match(t[1]['10-000802BE51BB-E1'], '%d+%.%d'))
  print('-->', t[2]['10-0008021B102E-B7'])
  print('-->', t[3]['10-000800C64986-FF'])

  t1 = 0.0 + string.match(t[1]['10-000802BE51BB-E1'], '%d+%.%d')


  --sys.show(t)
end


local socketCOAP = sys.w5500.create('UDP', 12345, COAPHandler)
sys.w5500.open(socketCOAP)

--local send = coap.wrap.CONJSON('["sys.introduce()"]')
local send = coap.wrap.CONJSON('["ow.getTemp()"]')

print('-->', send)

timers.add(
  2,
  function()
    sys.w5500.send(socketCOAP, send, sys.w5500.stringToIP('169.254.111.111'), coap.port)
    --print('Send', send)
  end
)


while true do
  sys.idle()
  collectgarbage()
end
