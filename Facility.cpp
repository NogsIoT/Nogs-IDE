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
#include "Facility.h"
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
#define FRONT_COLOR (TColor) RGB(255, 255, 255)
#define BACK_COLOR  (TColor) RGB(25, 25, 25)
//---------------------------------------------------------------------------
TFacility *Facility = 0;
//---------------------------------------------------------------------------
__fastcall TFacility::TItem::TItem()
{}
//---------------------------------------------------------------------------
__fastcall TFacility::TItem::TItem(const char* _Name, bool _Visible, TColor _FontColor, TColor _BackColor):
  Name(_Name), Visible(_Visible), FontColor(_FontColor), BackColor(_BackColor)
{}
//---------------------------------------------------------------------------
__fastcall TFacility::TFacility()
{
  Reset();
}
//---------------------------------------------------------------------------
TFacility* __fastcall TFacility::Get()
{
  if (!Facility) Facility = new TFacility;
  return Facility;
}
//---------------------------------------------------------------------------
TFacility::TItem* __fastcall TFacility::operator [] (int Index)
{
  return (Index >= 0) && (Index < 24) ? &Items[Index] : 0;
}
//---------------------------------------------------------------------------
void __fastcall TFacility::Reset()
{
  Items[ 0] = TItem("KERN", true, FRONT_COLOR, BACK_COLOR);
  Items[ 1] = TItem("USER", true, FRONT_COLOR, BACK_COLOR);
  Items[ 2] = TItem("MAIL", true, FRONT_COLOR, BACK_COLOR);
  Items[ 3] = TItem("DAEM", true, FRONT_COLOR, BACK_COLOR);
  Items[ 4] = TItem("AUTH", true, FRONT_COLOR, BACK_COLOR);
  Items[ 5] = TItem("SYSL", true, FRONT_COLOR, BACK_COLOR);
  Items[ 6] = TItem("LPR ", true, FRONT_COLOR, BACK_COLOR);
  Items[ 7] = TItem("NEWS", true, FRONT_COLOR, BACK_COLOR);
  Items[ 8] = TItem("UUCP", true, FRONT_COLOR, BACK_COLOR);
  Items[ 9] = TItem("CRON", true, FRONT_COLOR, BACK_COLOR);
  Items[10] = TItem("AUTH", true, FRONT_COLOR, BACK_COLOR);
  Items[11] = TItem("PRTF", true, (TColor) RGB(196, 255, 0), (TColor) RGB(0, 0, 0));
  Items[12] = TItem("PERF", false,(TColor) RGB(0, 255, 0), (TColor) RGB(0, 128, 0));
  Items[13] = TItem("LUAE", true, (TColor) RGB(255, 64, 0), (TColor) RGB(0, 0, 0));
  Items[14] = TItem("14  ", true, FRONT_COLOR, BACK_COLOR);
  Items[15] = TItem("15  ", true, FRONT_COLOR, BACK_COLOR);
  Items[16] = TItem("LOC0", true, FRONT_COLOR, BACK_COLOR);
  Items[17] = TItem("LOC1", true, FRONT_COLOR, BACK_COLOR);
  Items[18] = TItem("LOC2", true, FRONT_COLOR, BACK_COLOR);
  Items[19] = TItem("LOC3", true, FRONT_COLOR, BACK_COLOR);
  Items[20] = TItem("LOC4", true, FRONT_COLOR, BACK_COLOR);
  Items[21] = TItem("LOC5", true, FRONT_COLOR, BACK_COLOR);
  Items[22] = TItem("LOC6", true, FRONT_COLOR, BACK_COLOR);
  Items[23] = TItem("LOC7", true, FRONT_COLOR, BACK_COLOR);
}
//---------------------------------------------------------------------------
TColor __fastcall TFacility::GetBackColor()
{
  return BACK_COLOR;
}
//---------------------------------------------------------------------------
void __fastcall TFacility::Load(TIniFile *Ini)
{
  for (int I = 0;I < 24;I++) {
    String Section = Items[I].Name;
    Items[I].Visible = Ini->ReadInteger(Section, "Enabled", (int) Items[I].Visible);
    Items[I].FontColor = (TColor) Ini->ReadInteger(Section, "FontColor", (int) Items[I].FontColor);
    Items[I].BackColor = (TColor) Ini->ReadInteger(Section, "BackColor", (int) Items[I].BackColor);
  }
}
//---------------------------------------------------------------------------
void __fastcall TFacility::Save(TIniFile *Ini)
{
  for (int I = 0;I < 24;I++) {
    String Section = Items[I].Name;
    Ini->WriteInteger(Section, "Enabled", (int) Items[I].Visible);
    Ini->WriteInteger(Section, "FontColor", (int) Items[I].FontColor);
    Ini->WriteInteger(Section, "BackColor", (int) Items[I].BackColor);
  }
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
