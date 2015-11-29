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
#ifndef MutatorCOAPJSONH
#define MutatorCOAPJSONH
#include "Mutator.h"
#include "COAP.h"
//---------------------------------------------------------------------------
class TMutatorCOAPJSON : public TMutator
{
public:
  static TPacket* __fastcall Wrap(TPacket* Packet)
  {
    Packet->SetData(TCOAP::JSONWrap(Packet->GetData(), Packet->GetConfirm()));
    return Packet;
  }
  static TPacket* __fastcall Unwrap(TPacket* Packet)
  {
    Packet->SetData(TCOAP::JSONUnwrap(Packet->GetData()));
    return Packet;
  }
};
//---------------------------------------------------------------------------
#endif
