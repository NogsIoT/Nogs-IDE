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

#ifndef MemoryTrackerFrmH
#define MemoryTrackerFrmH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include "MainFrm.h"
//---------------------------------------------------------------------------
class TFormMemoryTracker : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
  TPaintBox *PaintBox;
  TSplitter *Splitter1;
  TEdit *Edit;
  TListBox *ListBoxLog;
  TSplitter *Splitter2;
  TPanel *Panel10;
  TPanel *Panel11;
  TDrawGrid *Grid;
  void __fastcall PaintBoxPaint(TObject *Sender);
  void __fastcall GridDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
  void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);

private:	// Anwender-Deklarationen
  struct TItem {
    uint32_t Count;
    uint32_t Size;
    uint32_t ByteNumber;
    __fastcall TItem():
      Count(0), ByteNumber(0)
    {}
  };
  DynamicArray<TItem> Memory;

  void __fastcall Area(TCanvas *Canvas, uint32_t Address, uint32_t Size, TColor Color);
  void __fastcall DoAllocate(uint32_t Count, uint32_t Address, uint32_t Size);
  void __fastcall DoFree(uint32_t Count, uint32_t Address, uint32_t Size);



public:		// Anwender-Deklarationen
  struct TBuffer {
    uint32_t Count;
    char Type;
    char Pad;
    uint16_t Flags;
    uint32_t Address1;
    uint32_t Address2;
    uint32_t Size1;
    uint32_t Size2;
    uint32_t Line;
    char File[128];
  };

  DynamicArray<TBuffer> Tracker;
  uint32_t TrackerP;

  __fastcall TFormMemoryTracker(TComponent* Owner);
  void __fastcall Add(TBuffer *Buffer);
};
//---------------------------------------------------------------------------
#endif
