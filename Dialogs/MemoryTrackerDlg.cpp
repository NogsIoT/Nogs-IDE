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
#include "MemoryTrackerDlg.h"
#pragma package(smart_init)
#pragma link "ModelessBaseDlg"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
#define BASE 0x20000000
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TDialogMemoryTracker::Area(TCanvas *Canvas, uint32_t Address, uint32_t Size, TColor Color)
{
  Address -= BASE;
  int X1 = Address % 256;
  int Y1 = Address / 256;
  int X2 = (Address + Size - 1) % 256;
  int Y2 = (Address + Size - 1) / 256;
  Canvas->Pen->Color = Color;
  Canvas->Brush->Color = Color;
  if (Y1 == Y2) {
    Canvas->MoveTo(X1, Y1);
    Canvas->LineTo(X2 + 1, Y1);
  } else {
    Canvas->MoveTo(X1, Y1);
    Canvas->LineTo(256, Y1);
    Canvas->FillRect(TRect(0, Y1 + 1, 256, Y2));
    Canvas->MoveTo(0, Y2);
    Canvas->LineTo(X2 + 1, Y2);
  }
}
//---------------------------------------------------------------------------
void __fastcall TDialogMemoryTracker::DoAllocate(uint32_t Count, uint32_t Address, uint32_t Size)
{
  for (int I = 0; I < Size; I++) {
    int Index = Address - BASE + I;
    if (Memory[Index].Count) {
      String S;
      S.sprintf(L"[%08d] Not free, allocated by [%08d]", Count, Memory[Index].Count);
      ListBoxLog->Items->Add(S);
    }
    Memory[Index].Count = Count;
    Memory[Index].Size = Size;
    Memory[Index].ByteNumber = I;
  }
}
//---------------------------------------------------------------------------
void __fastcall TDialogMemoryTracker::DoFree(uint32_t Count, uint32_t Address, uint32_t Size)
{
  if (Address) {
    int Index = Address - BASE;
    if (!Size) Size = Memory[Index].Size;
    for (int I = 0; I < Size; I++) {
      int Index = Address - BASE + I;
      if (!Memory[Index].Count || (Memory[Index].ByteNumber != I)) {
        String S;
        S.sprintf(L"[%08d] Not used, byte number %u", Count, Memory[Index].ByteNumber);
        ListBoxLog->Items->Add(S);
      }
      Memory[Index].Count = 0;
      Memory[Index].ByteNumber = 0;
    }
  }
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TDialogMemoryTracker::PaintBoxPaint(TObject *Sender)
{
  TCanvas *C = PaintBox->Canvas;
  C->Brush->Color = clBlack;
  C->FillRect(Bounds(0, 0, 256, 512));
  C->Brush->Color = RGB(16, 16, 16);
  C->FillRect(Bounds(256, 0, 32767, 512));
  C->FillRect(Bounds(0, 512, 32767, 32768));

  C->Pen->Color = clWhite;
  C->Font->Color = clWhite;
  int H0 = C->TextHeight("0");

  for (int I = 0; I < 128 * 1024; I += 4096) {
    int Y = I / 256;
    C->MoveTo(256, Y);
    C->LineTo(256 + 7, Y);
    C->TextOut(256 + 8,Y - 3, "0x" + String::IntToHex(BASE + I, 8));
  }
}
//---------------------------------------------------------------------------
void __fastcall TDialogMemoryTracker::GridDrawCell(TObject *Sender, int Col, int Row, TRect &Rect, TGridDrawState State)
{
  String S;
  if ((Row >= 1) && (Row < Tracker.Length)) {
    TBuffer *Buffer = &Tracker[Row];
    S.sprintf(
      L"[%08d] %c 0x%08X %8d 0x%08X %8d %u",
      Buffer->Count,
      (unsigned char) Buffer->Type > 32 ? Buffer->Type : 32,
      Buffer->Address1,
      Buffer->Size1,
      Buffer->Address2,
      Buffer->Size2,
      Buffer->Line
    );
  }
  Grid->Canvas->Brush->Color = clBackground;
  Grid->Canvas->FillRect(Rect);
  Grid->Canvas->TextOut(Rect.Left, Rect.Top, S);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
__fastcall TDialogMemoryTracker::TDialogMemoryTracker(TComponent* Owner)
  : TDialogModeless(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TDialogMemoryTracker::Add(TBuffer *Buffer)
{
  switch (Buffer->Type) {

    case 'M' :
      Edit->Text = Buffer->Count;
      Edit->Refresh();
      Area(PaintBox->Canvas, Buffer->Address1, Buffer->Size1, clYellow);
      DoAllocate(Buffer->Count, Buffer->Address1, Buffer->Size1);
      break;

    case 'R' :
      Edit->Text = Buffer->Count;
      Edit->Refresh();
      Area(PaintBox->Canvas, Buffer->Address1, Buffer->Size1, clBlack);
      Area(PaintBox->Canvas, Buffer->Address2, Buffer->Size2, clYellow);
      DoFree(Buffer->Count, Buffer->Address1, Buffer->Size1);
      DoAllocate(Buffer->Count, Buffer->Address2, Buffer->Size2);
      break;

    case 'F' :
      Edit->Text = Buffer->Count;
      Edit->Refresh();
      Area(PaintBox->Canvas, Buffer->Address1, Buffer->Size1, clBlack);
      DoFree(Buffer->Count, Buffer->Address1, Buffer->Size1);
      break;

  }
  if (Buffer->Count < Tracker.Length) {
    Tracker[Buffer->Count] = *Buffer;
    if (Grid->RowCount < Buffer->Count) Grid->RowCount = Buffer->Count + 4096;
  }
}
//---------------------------------------------------------------------------


