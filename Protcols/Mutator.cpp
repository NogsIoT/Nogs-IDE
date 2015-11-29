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
#include "Mutator.h"
//---------------------------------------------------------------------------
__fastcall TMutator::TPacket::TPacket():
  Confirm(false)
{}
//---------------------------------------------------------------------------
__fastcall TMutator::TPacket::TPacket(const AnsiString _Data):
  Data(_Data), Confirm(false)
{}
//---------------------------------------------------------------------------
__fastcall TMutator::TPacket::TPacket(const AnsiString _Data, bool _Confirm):
  Data(_Data), Confirm(_Confirm)
{}
//---------------------------------------------------------------------------
__fastcall TMutator::TPacket::~TPacket()
{}
//---------------------------------------------------------------------------
AnsiString __fastcall TMutator::TPacket::GetData()
{
  return Data;
}
//---------------------------------------------------------------------------
void __fastcall TMutator::TPacket::SetData(const AnsiString _Data)
{
  Data = _Data;
}
//---------------------------------------------------------------------------
bool __fastcall TMutator::TPacket::GetConfirm()
{
  return Confirm;
}
//---------------------------------------------------------------------------
TMutator::TPacket* __fastcall TMutator::Wrap(TPacket* Packet)
{
  return Packet;
}
//---------------------------------------------------------------------------
TMutator::TPacket* __fastcall TMutator::Unwrap(TPacket* Packet)
{
  return Packet;
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
