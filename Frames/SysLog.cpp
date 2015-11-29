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
#include "SysLog.h"
#include "JSON.h"
#include "MainFrm.h"
#include "SetupOneDlg.h"
#include "SetupSysLogFrm.h"
#include "Facility.h"
#include "Utils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cgauges"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
String __fastcall TabToSpace(const String S)
{
  String T;
  for (int I = 1;I <= S.Length();I++)
    T += S[I] == '\t' ? String::StringOfChar(32, 8 - T.Length() % 8) : String(S[I]);
  return T;
}
//---------------------------------------------------------------------------
// TFrameSysLog
//---------------------------------------------------------------------------
void __fastcall TFrameSysLog::ListViewCustomDrawItem(TCustomListView *Sender, TListItem *ListItem, TCustomDrawState State, bool &DefaultDraw)
{
  TListView *ListView = dynamic_cast<TListView*>(Sender);
  ListView->Canvas->Brush->Color = TFacility::GetBackColor();
  ListView->Canvas->Font->Color = clGray;
}
//---------------------------------------------------------------------------
void __fastcall TFrameSysLog::ListViewCustomDrawSubItem(TCustomListView *Sender, TListItem *ListItem, int SubItem, TCustomDrawState State, bool &DefaultDraw)
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
void __fastcall TFrameSysLog::MenuItemSettingsClick(TObject *Sender)
{
  TDialogSetupOne *DialogSetupOne = 0;
  try {
    DialogSetupOne = new TDialogSetupOne(this, new TFrameSetupSysLog(this));
    DialogSetupOne->ShowModal();
  }
  __finally {
    delete DialogSetupOne;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameSysLog::MenuItemClearClick(TObject *Sender)
{
  ListView->Clear();
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
__fastcall TFrameSysLog::TFrameSysLog(TComponent* Owner)
  : TFrame(Owner)
{
  Facility = TFacility::Get();
  ClearOnText = "\"Startup\"";
}
//---------------------------------------------------------------------------
void __fastcall TFrameSysLog::Init()
{
}
//---------------------------------------------------------------------------
void __fastcall TFrameSysLog::Syslog(TIdSysLogMessage *SysLogMessage, TIdSocketHandle *Binding)
{
  AnsiString S = SysLogMessage->RawMessage;
  if (ClearOnText.Length() && S.Pos(ClearOnText))
    ListView->Items->Clear();
  int Type = 0;
  int P = S.Pos(">");
  if (P && (S[1] == '<')) {
    Type = StrToIntDef(S.SubString(2, P - 2), 0);
    S.Delete(1, P);
  }
  TFacility::TItem *Item = (*Facility)[Type >> 3];
  if (Item) {
    if (S.SubString(1, 16) == "Jan  1 00:00:00 ")
      S.Delete(1, 16);
    else
      S.Delete(1, 20);                 // TODO Auf verschieden syslog formate anpassen
    S = TabToSpace(S);
    if (Type >> 3 == 0) {
      //String Startup, Type, IP, Mask, MAC;
      try {
        TJSONArray JSONArray(S);
        FormMain->FrameTargets->Set(&JSONArray);
      } catch (...)
      {}
    } else if (Type >> 3 == 12) {
      int P = S.Pos(":");
      if (P) {
        int UsedMemory = atoi(S.SubString(P + 1, 10).c_str());
        if (UsedMemory > CGauge->MaxValue) CGauge->MaxValue = UsedMemory;
        CGauge->Progress = UsedMemory;
        PanelGauge->Visible = true;
        static bool Toggle;
        CGauge->ForeColor = Toggle ? clLime : clYellow;
        Toggle = !Toggle;
      }
    } else if (Type >> 3 == 13) {
      int P = S.Pos(":/");
      if (P) {
        S.Delete(1, P + 1);
        P = S.Pos(":");
        if (P) {
          String File = S.SubString(1, P - 1);
          int LineNumber = TUtils::AToI(S.SubString(P + 1, 12));
          FormMain->FrameLuaEditor->SetErrorPosition(File, LineNumber);
        }
      }
    }

    if (Item->Visible) {
      ListView->Items->BeginUpdate();
      TListItem *ListItem = ListView->Items->Add();
      ListItem->Data = Item;
      ListItem->Caption = Item->Name;
      ListItem->SubItems->Add(S);
      ListItem->MakeVisible(false);
      ListItem->ImageIndex = Type & 7;
      ListView->Items->EndUpdate();
    }
  }
}
//---------------------------------------------------------------------------

