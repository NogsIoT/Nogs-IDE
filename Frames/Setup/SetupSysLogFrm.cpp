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
#include "SetupSysLogFrm.h"
#include "Facility.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SetupBaseFrm"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
void __fastcall TFrameSetupSysLog::ListViewClick(TObject *Sender)
{
  TFacility::TItem *Item = (*Facility)[ListView->ItemIndex];
  ColorListBoxFontColor->Enabled = true;
  ColorListBoxBackColor->Enabled = true;
  CheckBoxVisible->Enabled = true;
  if (Item) {
    ColorListBoxFontColor->Selected = Item->FontColor;
    ColorListBoxBackColor->Selected = Item->BackColor;
    CheckBoxVisible->Checked = Item->Visible;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameSetupSysLog::ListViewCustomDrawItem(TCustomListView *Sender, TListItem *ListItem, TCustomDrawState State, bool &DefaultDraw)
{
  TListView *ListView = dynamic_cast<TListView*>(Sender);
  ListView->Canvas->Brush->Color = TFacility::GetBackColor();
  ListView->Canvas->Font->Color = clGray;

}
//---------------------------------------------------------------------------
void __fastcall TFrameSetupSysLog::ListViewCustomDrawSubItem(TCustomListView *Sender, TListItem *ListItem, int SubItem, TCustomDrawState State, bool &DefaultDraw)
{
  TListView *ListView = dynamic_cast<TListView*>(Sender);
  TFacility::TItem *Item = (TFacility::TItem*) ListItem->Data;
  if (SubItem == ListItem->SubItems->Count) {
    ListView->Canvas->Brush->Color = Item->BackColor;
    if (!Item->Visible) ListView->Canvas->Font->Style = ListView->Canvas->Font->Style << fsStrikeOut;
    ListView->Canvas->Font->Color = Item->FontColor;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameSetupSysLog::CheckBoxVisibleClick(TObject *Sender)
{
  TFacility::TItem *Item = (*Facility)[ListView->ItemIndex];
  Item->Visible = CheckBoxVisible->Checked;
  ListView->Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TFrameSetupSysLog::ColorListBoxFontColorClick(TObject *Sender)
{
  TFacility::TItem *Item = (*Facility)[ListView->ItemIndex];
  Item->FontColor = ColorListBoxFontColor->Selected;
  ListView->Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TFrameSetupSysLog::ColorListBoxBackColorClick(TObject *Sender)
{
  TFacility::TItem *Item = (*Facility)[ListView->ItemIndex];
  Item->BackColor = ColorListBoxBackColor->Selected;
  ListView->Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TFrameSetupSysLog::Button2Click(TObject *Sender)
{
  Facility->Reset();
  ListView->Invalidate();
  ListViewClick(0);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
__fastcall TFrameSetupSysLog::TFrameSetupSysLog(TComponent* Owner)
  : TFrameSetupBase(Owner)
{
  Facility = TFacility::Get();
}
//---------------------------------------------------------------------------
void __fastcall TFrameSetupSysLog::Init()
{
  for (int I = 0;I < 24;I++) {
    TFacility::TItem *Item = (*Facility)[I];
    TListItem *ListItem = ListView->Items->Add();
    ListItem->Data = Item;
    ListItem->Caption = Item->Name;
    ListItem->SubItems->Add(Item->Name);
    ListItem->ImageIndex = -1; //I & 7;
  }
}
//---------------------------------------------------------------------------


