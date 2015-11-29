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
#include "LiveCodingTargetsDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TDialogLiveCodingTargets::TDialogLiveCodingTargets(TComponent* Owner, TStringList *_LiveCodingTargets)
  : TForm(Owner), LiveCodingTargets(_LiveCodingTargets)
{
  for (int I = 0; I < FormMain->FrameTargets->ListView->Items->Count; I++) {
    TListItem *ListItem1 = FormMain->FrameTargets->ListView->Items->Item[I];
    if ((ListItem1->ImageIndex == 2) || (ListItem1->ImageIndex == 3)) {
      TListItem *ListItem2 = ListView->Items->Add();
      ListItem2->SubItems->Add(ListItem1->SubItems->Strings[1]);
      ListItem2->SubItems->Add(ListItem1->SubItems->Strings[3]);
      ListItem2->ImageIndex = ListItem1->ImageIndex;
      ListItem2->Checked = true;
    }
  }
  if (!LiveCodingTargets) 
    LiveCodingTargets = (TStringList*) 1;
  switch ((int) LiveCodingTargets) {
    case 1 : 
      RadioButtonUnicast->Checked = true; 
      break;
    case 2 : 
      RadioButtonBroadcast->Checked = true; 
      break;
    default : 
      RadioButtonSelected->Checked = true; 
      {
        for (int I = 0; I < ListView->Items->Count; I++) {
          TListItem *ListItem = ListView->Items->Item[I];
          ListItem->Checked = LiveCodingTargets->IndexOf(ListItem->SubItems->Strings[0]) >= 0;
        }
      }
  }
}
//---------------------------------------------------------------------------
void __fastcall TDialogLiveCodingTargets::ButtonCancelClick(TObject *Sender)
{
  if ((int) LiveCodingTargets >= 0xFF) 
    delete LiveCodingTargets;
  if (RadioButtonUnicast->Checked) 
    LiveCodingTargets = (TStringList*) 1;
  else if (RadioButtonBroadcast->Checked) 
    LiveCodingTargets = (TStringList*) 2;
  else {
    LiveCodingTargets = new TStringList;
    for (int I = 0; I < ListView->Items->Count; I++) {
      TListItem *ListItem = ListView->Items->Item[I];
      LiveCodingTargets->Add(ListItem->SubItems->Strings[0]);
    }
  }
  ModalResult = mrOk;  
}
//---------------------------------------------------------------------------
void __fastcall TDialogLiveCodingTargets::ListViewClick(TObject *Sender)
{
  RadioButtonSelected->Checked = true;
}
//---------------------------------------------------------------------------

