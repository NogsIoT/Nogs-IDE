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
#ifndef MutatorH
#define MutatorH
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
//
// CON
//             Mutator        Mutator        Mutator        Transmitter
//             --------       --------       --------       --------
// Packet --> | Warp   | --> | Warp   | --> | Warp   | --> | Send   |
// Packet <-- | Unwrap | <-- | Unwrap | <-- | Unwrap | <-- | Recv   |
//             --------       --------       --------       --------
//
// NON
//             Mutator        Mutator        Mutator        Transmitter
//             --------       --------       --------       --------
// Packet --> | Warp   | --> | Warp   | --> | Warp   | --> | Send   |
//            | Unwrap |     | Unwrap |     | Unwrap |     |        |
//             --------       --------       --------       --------
//
//---------------------------------------------------------------------------
class TMutator
{
public:
  class TPacket
  {
    AnsiString Data;
    bool Confirm;
  public:
    __fastcall TPacket();
    __fastcall TPacket(const AnsiString _Data);
    __fastcall TPacket(const AnsiString _Data, bool _Confirm);
    virtual __fastcall ~TPacket();
    AnsiString __fastcall GetData();
    void __fastcall SetData(const AnsiString _Data);
    bool __fastcall GetConfirm();
  };

  static TPacket* __fastcall Wrap(TPacket* Packet);
  static TPacket* __fastcall Unwrap(TPacket* Packet);
};
//---------------------------------------------------------------------------
#endif
