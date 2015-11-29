--[[
 *  Color utils
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

local color = {}


function color.rgbToValue(r, g, b)
   return math.floor(r * 255.0) * 65536 + math.floor(g * 255.0) * 256 + math.floor(b * 255.0)
end


-- h 0..1, s 0..1 v 0..1
function color.rgb(r, g ,b)
  return r, g, b
end


function color.hsv(h, s, v)
  local r,g,b
  local i = math.floor(h * 6)
  local f = h * 6 - i
  local p = v * (1 - s)
  local q = v * (1 - f * s)
  local t = v * (1 - (1 - f) * s)
  local switch = i % 6
  if switch <= 0 then return v, t, p end
  if switch <= 1 then return q, v, p end
  if switch <= 2 then return p, v, t end
  if switch <= 3 then return p, q, v end
  if switch <= 4 then  return t, p, v end
  return v, p, q
end


return color

