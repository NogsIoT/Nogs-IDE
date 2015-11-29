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
#include <vcl.h>
#pragma hdrstop
#ifndef IndicatorH
#define IndicatorH
//---------------------------------------------------------------------------
class TIndicator
{
public:
  __fastcall TIndicator();
  virtual __fastcall ~TIndicator();
  virtual void __fastcall Step();
  virtual bool __fastcall IsFinished();
  virtual String __fastcall GetText();
  virtual TColor __fastcall GetFgColor();
  virtual TColor __fastcall GetBgColor();
};
//---------------------------------------------------------------------------
class TIndicatorStatic : public TIndicator
{
  String Text;
  TColor Color;
public:
  __fastcall TIndicatorStatic(const String _Text, TColor _Color);
  virtual bool __fastcall IsFinished();
  virtual String __fastcall GetText();
  virtual TColor __fastcall GetFgColor();
  virtual TColor __fastcall GetBgColor();
};
//---------------------------------------------------------------------------
class TIndicatorFadeOut : public TIndicator
{
  String Text;
  int R, G, B;
  int Level;
public:
  __fastcall TIndicatorFadeOut(const String _Text, TColor Color);
  virtual void __fastcall Step();
  virtual bool __fastcall IsFinished();
  virtual String __fastcall GetText();
  virtual TColor __fastcall GetFgColor();
  virtual TColor __fastcall GetBgColor();
};
//---------------------------------------------------------------------------
#endif
