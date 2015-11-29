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
#include "LogFrm.h"
#include "MainFrm.h"
#include "JSON.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SysLog"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
void __fastcall TFrameLog::TreeViewLogCustomDrawItem(TCustomTreeView *Sender, TTreeNode *TreeNode, TCustomDrawState State, bool &DefaultDraw)
{
  TColor Colors[] = {clWhite, clLime, clYellow, (TColor) RGB(255, 32, 0), clAqua, clFuchsia};
  TCanvas *C = TreeViewLog->Canvas;
  C->Brush->Color = (TColor) RGB(25, 25, 25);
  TreeViewLog->Canvas->Font->Color = Colors[(int) TreeNode->Data];
}
//---------------------------------------------------------------------------
void __fastcall TFrameLog::IdSyslogServerSyslog(TObject *Sender, TIdSysLogMessage *SysLogMessage, TIdSocketHandle *Binding)
{
  int Page = TabControl->Tabs->IndexOf(Binding->PeerIP);
  if (Page < 0) {
    Page = TabControl->Tabs->Add(Binding->PeerIP);
    TFrameSysLog *FrameSysLog = new TFrameSysLog(this);
    FrameSysLog->Name = "FrameSysLog" + FormMain->UniqueId.Get();
    FrameSysLog->Align = alClient;
    FrameSysLog->Parent = TabControl;
    TabControl->Tabs->Objects[Page] = FrameSysLog;
    TabControl->Visible = true;
  }
//  for (int I = 0; I < TabControl->Tabs->Count; I++) {
//    TFrameSysLog *FrameSysLog = (TFrameSysLog*) TabControl->Tabs->Objects[I];
//    FrameSysLog->Visible = I == Page;
//  }
  TFrameSysLog *FrameSysLog = (TFrameSysLog*) TabControl->Tabs->Objects[Page];
  FrameSysLog->Syslog(SysLogMessage, Binding);

//  TTabSheet *TabSheet = 0;
//  for (int I = 0; I < PageControlSysLog->PageCount; I++) {
//    TTabSheet *TabSheet2 = PageControlSysLog->Pages[I];
//    if (TabSheet2->Caption == Binding->PeerIP) {
//      TabSheet = TabSheet2;
//      break;
//    }
//  }
//  if (!TabSheet) {
//    PageControlSysLog->Visible = true;
//    TabSheet = new TTabSheet(this);
//    TabSheet->Caption = Binding->PeerIP;
//    TabSheet->PageControl = PageControlSysLog;
//    TFrameSysLog *FrameSysLog = new TFrameSysLog(this);
//    FrameSysLog->Align = alClient;
//    FrameSysLog->Parent = TabSheet;
//    FrameSysLog->Visible = true;
//    FrameSysLog->Invalidate();
//    PageControlSysLog->Invalidate();
//  }
//  TFrameSysLog *FrameSysLog = dynamic_cast<TFrameSysLog*>(TabSheet->Controls[0]);
//  FrameSysLog->Syslog(SysLogMessage, Binding);
}
//---------------------------------------------------------------------------
void __fastcall TFrameLog::PopupMenuPopup(TObject *Sender)
{
  MenuItemOpen->Caption = "Open";
  MenuItemOpen->Enabled = false;
  TTreeNode *TreeNode = TreeViewLog->Selected;
  if (TreeNode) {
    try {
      String S = TreeNode->Level ? TreeNode->Text : TreeNode->Text.SubString(22, MaxInt).Trim();
      TJSONArray JSONArray(S);
      MenuItemOpen->Caption = "Open in JSON editor";
      MenuItemOpen->Enabled = true;
      return;
    }
    catch (TJSONScanner::TError) {}
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameLog::MenuItemClearClick(TObject *Sender)
{
  TreeViewLog->Items->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TFrameLog::MenuItemOpenClick(TObject *Sender)
{
  TTreeNode *TreeNode = TreeViewLog->Selected;
  if (TreeNode) {
    try {
      String S = TreeNode->Level ? TreeNode->Text : TreeNode->Text.SubString(22, MaxInt).Trim();
      TJSONArray JSONArray(S);
      TJSONFormatter JSONFormatter;
      FormMain->FrameLuaEditor->NewJSON(JSONArray.ToString(JSONFormatter));
      return;
    }
    catch (TJSONScanner::TError) {}

  }
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
__fastcall TFrameLog::TFrameLog(TComponent* Owner)
  : TFrame(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrameLog::Init()
{
  //FrameSysLog->Init();
  IdSyslogServer->Active = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrameLog::LogClear()
{
  TreeViewLog->Items->Clear();
}
//---------------------------------------------------------------------------
TTreeNode* __fastcall TFrameLog::Log(int Level, String S)
{                                                   // TTreeView
  S = StringReplace(S, "\n", " ", TReplaceFlags() << rfReplaceAll);
  S = StringReplace(S, "\t", "\\t", TReplaceFlags() << rfReplaceAll);
  S = StringReplace(S, "\r", "\\r", TReplaceFlags() << rfReplaceAll);
  AnsiString DateTimeString = Now().DateTimeString();
  TTreeNode *TreeNode = TreeViewLog->Items->Add(0, DateTimeString + "  " + S);
  TreeNode->Data = (void*) Level;
  TreeViewLog->Selected = TreeNode;
  TreeViewLog->Update();
  return TreeNode;
}
//---------------------------------------------------------------------------
TTreeNode* __fastcall TFrameLog::Log(TTreeNode* TreeNode, int Level, String S)
{
  if (!TreeNode) return Log(Level, S);
  S = StringReplace(S, "\n", " ", TReplaceFlags() << rfReplaceAll);
  S = StringReplace(S, "\t", "\\t", TReplaceFlags() << rfReplaceAll);
  S = StringReplace(S, "\r", "\\r", TReplaceFlags() << rfReplaceAll);
  TTreeNode *TreeNode2 = TreeViewLog->Items->AddChild(TreeNode, S);
  if ((int) TreeNode->Data < Level) TreeNode->Data = (void*) Level;
  TreeNode2->Data = (void*) Level;
  TreeViewLog->Selected = TreeNode;
  if (Level >= 2) TreeNode->Expanded = true;
  TreeViewLog->Update();
  return TreeNode2;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------





