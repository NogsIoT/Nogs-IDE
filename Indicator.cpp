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
#include "Indicator.h"
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
__fastcall TIndicator::TIndicator()
{}
//---------------------------------------------------------------------------
__fastcall TIndicator::~TIndicator()
{}
//---------------------------------------------------------------------------
void __fastcall TIndicator::Step()
{}
//---------------------------------------------------------------------------
bool __fastcall TIndicator::IsFinished()
{
  return true;
}
//---------------------------------------------------------------------------
String __fastcall TIndicator::GetText()
{
  return EmptyStr;
}
//---------------------------------------------------------------------------
TColor __fastcall TIndicator::GetFgColor()
{
  return clWhite;
}
//---------------------------------------------------------------------------
TColor __fastcall TIndicator::GetBgColor()
{
  return RGB(25, 25, 25);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
__fastcall TIndicatorStatic::TIndicatorStatic(const String _Text, TColor _Color):
  TIndicator(), Text(_Text), Color(_Color)
{}
//---------------------------------------------------------------------------
bool __fastcall TIndicatorStatic::IsFinished()
{
  return false;
}
//---------------------------------------------------------------------------
String __fastcall TIndicatorStatic::GetText()
{
  return Text;
}
//---------------------------------------------------------------------------
TColor __fastcall TIndicatorStatic::GetFgColor()
{
  return clBlack;
}
//---------------------------------------------------------------------------
TColor __fastcall TIndicatorStatic::GetBgColor()
{
  return Color;
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
__fastcall TIndicatorFadeOut::TIndicatorFadeOut(const String _Text, TColor Color):
  TIndicator(), Text(_Text), Level(100)
{
  int RGB = ::ColorToRGB(Color);
  R = RGB & 0xFF;
  G = (RGB >> 8) & 0xFF;
  B = (RGB >> 16) & 0xFF;
}
//---------------------------------------------------------------------------
void __fastcall TIndicatorFadeOut::Step()
{
  if (Level > 0) Level -= 10;
  if (Level < 0) Level = 0;
}
//---------------------------------------------------------------------------
bool __fastcall TIndicatorFadeOut::IsFinished()
{
  return Level <= 0;
}
//---------------------------------------------------------------------------
String __fastcall TIndicatorFadeOut::GetText()
{
  return Text;
}
//---------------------------------------------------------------------------
TColor __fastcall TIndicatorFadeOut::GetFgColor()
{
  return clBlack;
}
//---------------------------------------------------------------------------
TColor __fastcall TIndicatorFadeOut::GetBgColor()
{
  return RGB(R * Level / 100, G * Level / 100, B * Level / 100);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
#pragma package(smart_init)
