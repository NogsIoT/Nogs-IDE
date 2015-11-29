/*
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
 */
//---------------------------------------------------------------------------
#pragma hdrstop
#include "LuaSyntaxChecker.h"
extern "C" {
  #include "lua.h"
  #include "lualib.h"
  #include "lauxlib.h"
}
//---------------------------------------------------------------------------
#ifndef LUA_WIN
  #error LUA_WIN
#endif
//---------------------------------------------------------------------------
AnsiString __fastcall TLuaSyntaxChecker::Check(const AnsiString Source)
{
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  int Result = luaL_loadstring(L, Source.c_str());
  AnsiString Error;
  if (Result)
    Error = lua_tostring(L, -1);
  lua_close(L);
  return Error;
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
