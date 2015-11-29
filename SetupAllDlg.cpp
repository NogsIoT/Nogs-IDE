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
#include "SetupAllDlg.h"
#include "SetupBaseFrm.h"
#include "SetupSysLogFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TDialogSetupAll::Add(const String Name, TFrameSetupBase *FameSetupBase)
{
  int Index = ListBox->Items->Add(Name);
  FameSetupBase->Align = alClient;
  FameSetupBase->Init();
  ListBox->Items->Objects[Index] = FameSetupBase;
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TDialogSetupAll::ButtonCloseClick(TObject *Sender)
{
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
__fastcall TDialogSetupAll::TDialogSetupAll(TComponent* Owner)
  : TForm(Owner), ActiveFameSetupBase(0)
{
  Add("Sys Log", new TFrameSetupSysLog(this));


}
//---------------------------------------------------------------------------
void __fastcall TDialogSetupAll::ListBoxClick(TObject *Sender)
{
  if (ActiveFameSetupBase) {
    ActiveFameSetupBase->Parent = 0;
    ActiveFameSetupBase = 0;
  }

  int Index = ListBox->ItemIndex;
  if (Index >= 0) {
    ActiveFameSetupBase = (TFrameSetupBase*) ListBox->Items->Objects[Index];
    ActiveFameSetupBase->Parent = PanelFrames;


  }

}
//---------------------------------------------------------------------------
