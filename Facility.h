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
#ifndef FacilityH
#define FacilityH
#include <System.IniFiles.hpp>
//---------------------------------------------------------------------------
class TFacility
{
public:
  struct TItem {
    const char* Name;
    bool Visible;
    TColor FontColor, BackColor;
    __fastcall TItem();
    __fastcall TItem(const char* _Name, bool _Visible, TColor _FontColor, TColor _BackColor);
  };
private:
  TItem Items[24];
  __fastcall TFacility();
public:
  static TFacility* __fastcall Get();

  TItem* __fastcall operator [] (int Index);

  void __fastcall Reset();

  static TColor __fastcall GetBackColor();

  void __fastcall Load(TIniFile *Ini);
  void __fastcall Save(TIniFile *Ini);
};
//---------------------------------------------------------------------------
#endif
