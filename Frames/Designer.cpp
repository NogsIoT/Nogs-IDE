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
#include "Designer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TFrameDesigner::TFrameDesigner(TComponent* Owner)
  : TFrame(Owner)
{
}
//---------------------------------------------------------------------------


/*
  TCanvas *C = PaintBoxBackground->Canvas;
  C->Brush->Color = clBlack;
  C->FillRect(Bounds(0,0,PaintBoxBackground->Width,PaintBoxBackground->Height));
  if (CheckBoxGrid->Checked) {
    C->Pen->Color = (TColor) RGB(32,16,0);
    for (int X = 0;X < PaintBoxBackground->Width;X += 20) {
      C->MoveTo(X,0);
      C->LineTo(X,PaintBoxBackground->Height);
    }
    for (int Y = 0;Y < PaintBoxBackground->Height;Y += 20) {
      C->MoveTo(0,Y);
      C->LineTo(PaintBoxBackground->Width,Y);
    }
  }
  if (CheckBoxAndroidTopTray->Checked) C->Draw(0,0,OverlayTop);
  if (CheckBoxAndroidRightTray->Checked) C->Draw(PaintBoxBackground->Width - OverlayRight->Width,0,OverlayRight);
*/
void __fastcall TFrameDesigner::PaintBoxPaint(TObject *Sender)
{
  TCanvas *C = PaintBox->Canvas;
  C->Brush->Color = (TColor) RGB(15, 67, 143);
  C->FillRect(Bounds(0,0,PaintBox->Width,PaintBox->Height));
  if (CheckBoxGrid->Checked) {
    C->Pen->Color = (TColor) RGB(15 / 2, 67 / 2, 143 / 2);
    for (int X = 0;X < PaintBox->Width;X += 20) {
      C->MoveTo(X,0);
      C->LineTo(X,PaintBox->Height);
    }
    for (int Y = 0;Y < PaintBox->Height;Y += 20) {
      C->MoveTo(0,Y);
      C->LineTo(PaintBox->Width,Y);
    }
  }
}
//---------------------------------------------------------------------------
