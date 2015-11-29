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


local function execute(source)
  print("NOGS_IDE: Execute("..source..")")
  local app, error = load(source)
  source = nil
  if app then
    local success, result = pcall(app)
    if success then
      print('NOGS_IDE: Success', result)
      if result == nil then
        return '["Success"]'
      else
        return '[{"Success":"'..string.gsub(result, '"', '\\"')..'"}]'
      end
    else
      print('NOGS_IDE: Error during pcall', result)
      return '[{"error":"'..string.gsub(result,'%"',' ')..'"}]'
    end
  else
    print('NOGS_IDE: Error during load:',error)
    return '[{"error":"'..string.gsub(error, '%"',' ')..'"}]'
  end
end


function Port11111Handler(socket, ip, port, events, status)
  --print("Port11111Recv", socket, ip, port, events, status)
  sys.w5500.open(socket)
  while true do
    coroutine.yield()
    local recv, peerIP = sys.w5500.recv(socket);
    if recv then
      sys.w5500.send(socket, execute(recv), peerIP, 11110)
    end
  end
end


local socket = sys.w5500.create("UDP", 11111, coroutine.create(Port11111Handler))

