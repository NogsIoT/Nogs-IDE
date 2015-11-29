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
#include "TargetSettingsDlg.h"
#include "MainFrm.h"
#include <System.StrUtils.hpp>
#include "Usul.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TDialogTargetSettings::ApplicationEvents1Idle(TObject *Sender, bool &Done)
{
  ComboBoxAutomaticUploadProject->Enabled = RadioGroupAutomaticUploadMode->ItemIndex;
  RadioButtonUploadExplicit->Enabled = ComboBoxAutomaticUploadProject->Items->Count;
}
//---------------------------------------------------------------------------
void __fastcall TDialogTargetSettings::PaintBoxPaint(TObject *Sender)
{
  TListView *ListView = (TListView*) ListItem->ListView;
  ListView->SmallImages->Draw(PaintBox->Canvas, 0, 0, ListItem->ImageIndex);
}
//---------------------------------------------------------------------------
void __fastcall TDialogTargetSettings::PageControlChange(TObject *Sender)
{
  Usul()->Ini->WriteInteger("UI@DialogTargetSettings", "Page", PageControl->ActivePageIndex);
}
//---------------------------------------------------------------------------
void __fastcall TDialogTargetSettings::ButtonCancelClick(TObject *Sender)
{
  ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TDialogTargetSettings::ButtonOkClick(TObject *Sender)
{
  TTarget *Target = TTarget::Get(ListItem);
  Target->SetAutomaticUploadMode(RadioGroupAutomaticUploadMode->ItemIndex);
  Target->SetProjectPath(ComboBoxAutomaticUploadProject->Items->Strings[ComboBoxAutomaticUploadProject->ItemIndex]);
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TDialogTargetSettings::ComboBoxAutomaticUploadProjectChange(TObject *Sender)
{
  RadioButtonUploadExplicit->Checked = ComboBoxAutomaticUploadProject->ItemIndex >= 0;
}
//---------------------------------------------------------------------------
void __fastcall TDialogTargetSettings::RadioButtonUploadFirstClick(TObject *Sender)
{
  ComboBoxAutomaticUploadProject->ItemIndex = -1;
}
//---------------------------------------------------------------------------
void __fastcall TDialogTargetSettings::RadioButtonUploadExplicitClick(TObject *Sender)
{
  ComboBoxAutomaticUploadProject->ItemIndex = 0;
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
__fastcall TDialogTargetSettings::TDialogTargetSettings(TComponent* Owner, TListItem *_ListItem)
  : TForm(Owner), ListItem(_ListItem)
{
  for (int I = 0; I < RadioGroupAutomaticUploadMode->Items->Count; I++) {
    String S = RadioGroupAutomaticUploadMode->Items->Strings[I];
    S = ReplaceStr(S, "\\n", "\n");
    S = ReplaceStr(S, "\\t", String::StringOfChar(32, 4));
    RadioGroupAutomaticUploadMode->Items->Strings[I] = S;
  }

  PageControl->ActivePageIndex = Usul()->Ini->ReadInteger("UI@DialogTargetSettings", "Page", 0);
  TTarget *Target = TTarget::Get(ListItem);

  Caption = ListItem->SubItems->Strings[0];
  Caption = Target->GetStartup();
  EditName->Text = ListItem->SubItems->Strings[3];
  DateTimePickerFirstDate->Date = Target->GetFirstTime();
  DateTimePickerFirstTime->Time = Target->GetFirstTime();
  DateTimePickerLastDate->Date = Target->GetLastTime();
  DateTimePickerLastTime->Time = Target->GetLastTime();

  TabSheetUpload->TabVisible = ListItem->ImageIndex == 2;

  RadioGroupAutomaticUploadMode->ItemIndex = Target->GetAutomaticUploadMode();
  {
    TStringList *ProjectPathes = FormMain->FrameProject->GetProjects();
    ComboBoxAutomaticUploadProject->Items->Assign(ProjectPathes);
    delete ProjectPathes;
    int Index = ComboBoxAutomaticUploadProject->Items->IndexOf(Target->GetProjectPath(false));
    ComboBoxAutomaticUploadProject->ItemIndex = Index;
    RadioButtonUploadFirst->Checked = Index < 0;
    RadioButtonUploadExplicit->Checked = Index >= 0;
  }
}
//---------------------------------------------------------------------------

