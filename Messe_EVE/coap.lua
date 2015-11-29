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

-- coap module
-- Author: Andre Riesberg
-- V3.0a

print("CoAP")

local string = require 'string'
local math = require 'math'

local coap = {}

coap.port = 5683

coap.TypeNumber = {
  ["CON"] = 0;
  ["NON"] = 1;
  ["ACK"] = 2;
  ["RES"] = 3;
}

coap.CodeNumber = {
  ["GET"]                       = 1;
  ["POST"]                      = 2;
  ["PUT"]                       = 3;
  ["DELETE"]                    = 4;
  ["Created"]                   = (2 * 32) +  1;
  ["Deleted"]                   = (2 * 32) +  2;
  ["Valid"]                     = (2 * 32) +  3;
  ["Changed"]                   = (2 * 32) +  4;
  ["Content"]                   = (2 * 32) +  5;
  ["BadRequest"]                = (4 * 32) +  0;
  ["Unauthorized"]              = (4 * 32) +  1;
  ["BadOption"]                 = (4 * 32) +  2;
  ["Forbidden"]                 = (4 * 32) +  3;
  ["NotFound"]                  = (4 * 32) +  4;
  ["MethodNotAllowed"]          = (4 * 32) +  5;
  ["NotAcceptable"]             = (4 * 32) +  6;
  ["PreconditionFailed"]        = (4 * 32) + 12;
  ["RequestEntityTooLarge"]     = (4 * 32) + 13;
  ["UnsupportedContentFormat"]  = (4 * 32) + 15;
  ["InternalServerError"]       = (5 * 32) +  0;
  ["NotImplemented"]            = (5 * 32) +  1;
  ["BadGateway"]                = (5 * 32) +  2;
  ["SrviceUnavailable"]         = (5 * 32) +  3;
  ["GatewayTimeout"]            = (5 * 32) +  4;
  ["ProxyingNotSupported"]      = (5 * 32) +  5;
}

coap.OptionNumber = {
  ["IfMatch"]       =  1;
  ["UriHost"]       =  3;
  ["ETag"]          =  4;
  ["IfNoneMatch"]   =  5;
  ["UriPort"]       =  7;
  ["LocationPath"]  =  8;
  ["UriPath"]       = 11;
  ["ContentFormat"] = 12;
  ["MaxAge"]        = 14;
  ["UriQuery"]      = 15;
  ["Accept"]        = 16;
  ["LocationQuery"] = 20;
  ["ProxyUri"]      = 35;
  ["ProxyScheme"]   = 39
};

coap.ContentFormatNumber = {
  ["TextPlain"]              =  0;
  ["ApplicationLinkFormat"]  = 40;
  ["ApplicationXML"]         = 41;
  ["ApplicationOctetStream"] = 42;
  ["ApplicationEXI"]         = 47;
  ["ApplicationJSON"]        = 50
};

-----------------------------------------------------------------------------
-- Build CoAP header
-----------------------------------------------------------------------------

coap.header = function(typeNumber, codeNumber, tokenLength, messageId)
  return string.char(
    1 * 64 + typeNumber * 16 + tokenLength,     -- Version 1
    codeNumber,
    math.floor(messageId / 256 % 256),messageId % 256 -- Byte order HL
  )
end

-----------------------------------------------------------------------------
-- Add dummy token
-----------------------------------------------------------------------------

coap.addToken = function(tokenLength)
  return string.char(0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA)
end

-----------------------------------------------------------------------------
-- Add an option byte
-----------------------------------------------------------------------------

coap.addOptionByte = function(number, byte)
  return string.char(number * 16 + 1,byte)
end

coap.wrap = {}
coap.unwrap = {}

coap.wrap.NONJSON = function(jsonString)
  coap.messageId = (coap.messageId or 1234) + 1
  local send =
    coap.header(coap.TypeNumber.NON, coap.CodeNumber.PUT, 8, coap.messageId) ..
    coap.addToken(8) ..
    coap.addOptionByte(coap.OptionNumber.ContentFormat, coap.ContentFormatNumber.ApplicationJSON) ..
    string.char(0xFF) ..
    jsonString ..
    string.char(0)
  return send
end

coap.wrap.CONJSON = function(jsonString)
  coap.messageId = (coap.messageId or 1234) + 1
  local send =
    coap.header(coap.TypeNumber.CON, coap.CodeNumber.PUT, 8, coap.messageId) ..
    coap.addToken(8) ..
    coap.addOptionByte(coap.OptionNumber.ContentFormat, coap.ContentFormatNumber.ApplicationJSON) ..
    string.char(0xFF) ..
    jsonString ..
    string.char(0)
  return send
end

coap.unwrap.JSON = function(recv)
  -- Process header
  local header, code, messageIdL, messageIdH = string.byte(recv, 1, 4)

  -- Process tokens
  local tokenLength = header % 16
  local p = 5 + tokenLength

  -- Process options
  local option = 0
  local contentFormat = nil
  while string.byte(recv, p) ~= 0xFF do
    local optionHeader = string.byte(recv, p)
    local option = option + math.floor(optionHeader / 16)
    local optionLength = optionHeader % 16
    p = p + 1
    if option == coap.OptionNumber.ContentFormat then
      contentFormat = string.byte(recv, p)
    end
    -- handle other
    p = p + optionLength
  end

  -- Process payload
  if contentFormat == coap.ContentFormatNumber.ApplicationJSON then
    local payLoad = string.sub(recv, p + 1)
    return payLoad
  end

  return nil
end




--[[
-----------------------------------------------------------------------------
-- Send a confirmed message with json payload
-----------------------------------------------------------------------------
coap.conJSON = function(ip, jsonString)

  --print('------>', jsonString)

  coap.messageId = (coap.messageId or 1234) + 1

  local send =
    coap.header(coap.TypeNumber.CON, coap.CodeNumber.PUT, 8, coap.messageId) ..
    coap.addToken(8) ..
    coap.addOptionByte(coap.OptionNumber.ContentFormat, coap.ContentFormatNumber.ApplicationJSON) ..
    string.char(0xFF) ..
    jsonString ..
    string.char(0)

  socket.try(udp:sendto(send, ip, coap.port))

  -- socket.settimeout(0.5)
  local recv, result = udp:receive();

  --print('------>', recv, result)

  -- Process herader
  local header, code, messageIdL, messageIdH = string.byte(recv, 1, 4)

  -- Process tokens
  local tokenLength = header % 16
  local p = 5 + tokenLength

  -- Process options
  local option = 0
  local contentFormat = nil
  while string.byte(recv, p) ~= 0xFF do
    local optionHeader = string.byte(recv, p)
    local option = option + math.floor(optionHeader / 16)
    local optionLength = optionHeader % 16
    p = p + 1
    if option == coap.OptionNumber.ContentFormat then
      contentFormat = string.byte(recv, p)
    end
    -- handle other
    p = p + optionLength
  end

  -- Process payload
  if contentFormat == coap.ContentFormatNumber.ApplicationJSON then
    local payLoad = string.sub(recv, p + 1)
    return payLoad
  end

  return nil
end

-----------------------------------------------------------------------------
-- Send a non confirmed message with json payload
-----------------------------------------------------------------------------

coap.nonJSON = function(ip, jsonString)

  --base.print(jsonString)

  coap.messageId = coap.messageId + 1

  local send =
    coap.header(TypeNumber.CON,CodeNumber.PUT,8,messageId) ..
    coap.addToken(8) ..
    coap.addOptionByte(OptionNumber.ContentFormat,ContentFormatNumber.ApplicationJSON) ..
    string.char(0xFF) ..
    jsonString ..
    string.char(0)

  socket.try(udp:sendto(send,ip,COAPPort))

end
--]]
-----------------------------------------------------------------------------
--
-----------------------------------------------------------------------------

return coap

