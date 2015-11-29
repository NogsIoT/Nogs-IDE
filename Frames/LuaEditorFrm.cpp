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
#include "LuaEditorFrm.h"
#include "MainFrm.h"
#include "Usul.h"
#include "LuaSyntaxChecker.h"
#include "LiveCodingTargetsDlg.h"
#include "Vcl.Clipbrd.hpp"
#include <System.StrUtils.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvMemo"
#pragma link "AdvmCSHS"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
static void __fastcall Sort(int &A, int &B)
{
  if (A > B) {
    int T = A;
    A = B;
    B = T;
  }
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
String __fastcall TFrameLuaEditor::ShortPath(const String Path)
{
  String RootPath = Usul()->Path;
  if (Path.SubString(1, RootPath.Length()) == RootPath)
    return "." + Path.SubString(RootPath.Length() + 1, 32767);
  return Path;
}
//---------------------------------------------------------------------------
TStringList* __fastcall TFrameLuaEditor::GetLiveCodingTargets()
{
  TInfo *Info = GetInfo();
  if (!Info->GetLiveCodingTargets()) {
    TDialogLiveCodingTargets *DialogLiveCodingTargets = new TDialogLiveCodingTargets(this, Info->GetLiveCodingTargets());
    DialogLiveCodingTargets->ShowModal();
    Info->SetLiveCodingTargets(DialogLiveCodingTargets->Get());
    delete DialogLiveCodingTargets;
  }
  return Info->GetLiveCodingTargets();
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::ApplicationEvents1Idle(TObject *Sender, bool &Done)
{
  TInfo *Info = GetInfo();
  if (Info) {
    String Path = ShortPath(Info->Path);
    if (Path.IsEmpty()) Path = "<no name>";
    if (Info->MemoSource->Modified) Path += "*";
    TabControl->Tabs->Strings[Info->Index] = Path;
    ButtonSave->Enabled = Info->MemoSource->Modified;
  }
  ButtonSaveAll->Enabled = false;
  MenuItemUndo->Enabled = Memo->CanUndo();
  MenuItemRedo->Enabled = Memo->CanRedo();
  MenuItemCut->Enabled = Memo->CanCut();
  MenuItemCopy->Enabled = Memo->CanCopy();
  MenuItemPaste->Enabled = Memo->CanPaste();
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::ButtonNewClick(TObject *Sender)
{
  New();
//  if (MemoLua->Modified) {
//    int Result = MessageBox("'" + Path + "'\nwas modifyed, but not saved.\n\nDo you want to save this file?","Modified",MB_ICONQUESTION|MB_YESNOCANCEL);
//    switch (Result) {
//      case IDCANCEL : return;
//      case IDYES : Save(Path); break;
//    }
//  }
//  MemoLua->Lines->Clear();
//  Path = EmptyStr;
//  MemoLua->Modified = false;
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::ButtonOpenbClick(TObject *Sender)
{
//  OpenDialog->DefaultExt = Info->MemoSource->SyntaxStyler->DefaultExtension;
//  OpenDialog->Filter = Info->MemoSource->SyntaxStyler->Filter;
//  OpenDialog->FileName = Info->Path;
  if (OpenDialog->Execute()) {
    New();
    TInfo *Info = GetInfo();
    Info->Path = OpenDialog->FileName;
    TabControl->Tabs->Strings[Info->Index] = Info->Path;
    Info->MemoSource->Lines->LoadFromFile(Info->Path);
    Memo->MemoSource = Info->MemoSource;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::ButtonSaveClick(TObject *Sender)
{
  TInfo *Info = GetInfo();
  if (Info->Path.Length()) {
    Memo->MemoSource->Lines->SaveToFile(Info->Path);
    Info->MemoSource->Modified = false;
  } else
    ButtonSaveAsClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::ButtonSaveAllClick(TObject *Sender)
{
//  for (int I = 0; I < TabControl->Tabs->Count; I++) {
//    TInfo *Info = GetInfo(I);
//    if (Info->Path.IsEmpty()) {
//      if (!SaveDialog->Execute()) return;
//      Info->Path = SaveDialog->FileName;
//    }
//    if (Info->MemoSource->Modified) {
//      Memo->MemoSource->Lines->SaveToFile(Info->Path);
//      Info->MemoSource->Modified = false;
//    }
//  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::ButtonSaveAsClick(TObject *Sender)
{
  TInfo *Info = GetInfo();
  SaveDialog->DefaultExt = Info->MemoSource->SyntaxStyler->DefaultExtension;
  SaveDialog->Filter = Info->MemoSource->SyntaxStyler->Filter;
  SaveDialog->FileName = Info->Path;
  if (SaveDialog->Execute()) {
    Info->Path = SaveDialog->FileName;
    ButtonSaveClick(Sender);
    FormMain->FrameProject->Refresh();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::ButtonRebootClick(TObject *Sender)
{
  FormMain->Backdoor(GetLiveCodingTargets(), "sys.reboot()"); 
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::ButtonRestartClick(TObject *Sender)
{
  FormMain->Backdoor(GetLiveCodingTargets(), "sys.restart()");
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::ButtonRebootMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if (Button == mbRight) {
    TInfo *Info = GetInfo();
    TDialogLiveCodingTargets *DialogLiveCodingTargets = new TDialogLiveCodingTargets(this, Info->GetLiveCodingTargets());
    DialogLiveCodingTargets->ShowModal();
    Info->SetLiveCodingTargets(DialogLiveCodingTargets->Get());
    delete DialogLiveCodingTargets;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::TabControlChange(TObject *Sender)
{
  TInfo *Info = GetInfo();
  if (Info) {
    Memo->MemoSource = Info->MemoSource;
    FormMain->FrameProject->HighlightFromEditor(Info->Path);
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::TabControlChanging(TObject *Sender, bool &AllowChange)
{
  LoadContext.Clear();
  Memo->ClearErrors();
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::TabControlGetImageIndex(TObject *Sender, int TabIndex, int &ImageIndex)
{
  if (TabIndex < TabControl->Tabs->Count) {
    int Index = FormMain->FileExtensionToImageIndex(ExtractFileExt(TabControl->Tabs->Strings[TabIndex]));
    if (Index >= 0)
      ImageIndex = Index;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::MenuItemCloseTabClick(TObject *Sender)
{
  TInfo *Info = GetInfo();
  if (Info->MemoSource->Modified) {
    int Result = MessageBox("'" + Info->Path + "'\nwas modifyed, but not saved.\n\nDo you want to save this file?","Modified", MB_ICONQUESTION | MB_YESNOCANCEL);
    switch (Result) {

      case IDCANCEL :
        return;

      case IDYES :
        ButtonSaveClick(0);
        break;

    }
  }
  
  TabControl->Tabs->Delete(Info->Index);
  if (!TabControl->Tabs->Count)
    New();
  else {
    TabControl->TabIndex = 0;
    TabControlChange(0);
  }
  delete Info->MemoSource;
  delete Info;
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::MemoLineBkColor(TObject *Sender, int LineNo,  TColor &BkColor)
{
  if ((LineNo >= LoadContext.LineStartNo) && (LineNo <= LoadContext.LineEndNo))
    BkColor = LoadContext.Error.IsEmpty() ? RGB(0, 48, 16) : RGB(64, 0, 0);
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::MemoMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if (Button == mbRight) {
    int CurX, CurY;
    Memo->MouseToCursor(X, Y, CurX, CurY);
    Memo->CurX = CurX; // + Memo->LeftCol;
    Memo->CurY = CurY + Memo->TopLine;
  }

  Memo->Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::MenuItemUndoClick(TObject *Sender)
{
  Memo->Undo();
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::MenuItemRedoClick(TObject *Sender)
{
  Memo->Redo();
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::MenuItemCutClick(TObject *Sender)
{
  Memo->CutToClipBoard();
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::MenuItemCopyClick(TObject *Sender)
{
  Memo->CopyToClipBoard();
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::MenuItemPasteClick(TObject *Sender)
{
  Memo->PasteFromClipBoard();
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::MenuItemSelectAllClick(TObject *Sender)
{
  Memo->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::MenuItemPasteEVECoprocessorListClick(TObject *Sender)
{
  TStringList *Lines = new TStringList;
  Lines->Text = Clipboard()->AsText;
  for (int I = 0; I < Lines->Count; I++) {
    AnsiString S = Lines->Strings[I].Trim();
    if (S.Length()) {
      AnsiString T = "  e.";
      int P = S.Pos("(");
      if (P) {
        T += S.SubString(1, P);
        S.Delete(1, P);
        bool EndOfFunction = false;
        do {
          int P = S.Pos(",");
          if (!P) {
            P = S.Pos(")");
            EndOfFunction = true;
          }
          AnsiString W = S.SubString(1, P - 1).Trim();
          S.Delete(1, P);
          if (W.Length()) {
            if ((W[1] == '"') || (W[1] == '-') || (W[1] == '+') || ((W[1] >= '0') && (W[1] <= '9')))
              T += W + (EndOfFunction ? ")" : ", ");
            else
              T += "'" + W + "'" + (EndOfFunction ? ")" : ", ");
          } else
            T += EndOfFunction ? ")" : ", ";
        } while (!EndOfFunction);
      }
      Lines->Strings[I] = T + ",";
    }
  }
  Clipboard()->AsText = Lines->Text;
  delete Lines;
  Memo->PasteFromClipBoard();
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::MenuItemToggleCommendClick(TObject *Sender)
{
  TInfo *Info = GetInfo();
  int Y1 = Memo->SelStartY;
  int Y2 = Memo->SelEndY;
  Sort(Y1, Y2);
  for (int I = Y1; I <= Y2; I++) {
    String S = Info->MemoSource->Lines->Strings[I];
    if (S.SubString(1, 2) == "--")
      S.Delete(1, 2);
    else
      S = "--" + S;
    Info->MemoSource->Lines->Strings[I] = S;
  }
  //Memo->ClearSelection();
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::CopyHTML1Click(TObject *Sender)
{
  Memo->CopyHTMLToClipboard();
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::MenuItemLoadClick(TObject *Sender)
{
  FormMain->Backdoor(GetLiveCodingTargets(), LoadContext.Source);
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::MenuItemToggleBookmarkXClick(TObject *Sender)
{
  TMenuItem *MenuItem = dynamic_cast<TMenuItem*>(Sender);

  //Memo->Bookmarks[0] = Memo->CurY;
  Memo->BookmarkIndex[Memo->CurY] = MenuItem->Tag;
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::MenuItemSetLiveCodingTargetsClick(TObject *Sender)
{
  TInfo *Info = GetInfo();
  TDialogLiveCodingTargets *DialogLiveCodingTargets = new TDialogLiveCodingTargets(this, Info->GetLiveCodingTargets());
  DialogLiveCodingTargets->ShowModal();
  Info->SetLiveCodingTargets(DialogLiveCodingTargets->Get());
  delete DialogLiveCodingTargets;
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::PopupMenuMemoPopup(TObject *Sender)
{
  LoadContext.Clear();
  String Caption;
  try {
    TInfo *Info = GetInfo();
    if (Info) {
      String Line = Info->MemoSource->Lines->Strings[Memo->CurY].Trim();
      if (Line.IsEmpty() || (Line.SubString(1, 2) == "--")) return;

      if (Line.Pos("function")) {
        String Line = Info->MemoSource->Lines->Strings[Memo->CurY];
        LoadContext.LineStartNo = Memo->CurY;
        LoadContext.Source = Line;
        Caption = "Live coding: " + Line + "...";
        for (int Y = Memo->CurY + 1; Y < Memo->Lines->Count; Y++) {
          String Line = Info->MemoSource->Lines->Strings[Y];
          LoadContext.Source += "\n" + Line;
          LoadContext.LineEndNo = Y;
          if (Line.SubString(1, 3) == "end") break;
        }
        LoadContext.Error = TLuaSyntaxChecker::Check(LoadContext.Source);
      } else {
        LoadContext.LineStartNo = Memo->CurY;
        LoadContext.LineEndNo = Memo->CurY;
        LoadContext.Source = Info->MemoSource->Lines->Strings[Memo->CurY];
        Caption = "Live coding: " + LoadContext.Source;
        LoadContext.Error = TLuaSyntaxChecker::Check(LoadContext.Source);
      }
    }
  }
  __finally {
    MenuItemLoad->Caption = LoadContext.Error.Length() ? LoadContext.Error : Caption;
    Memo->RefreshMemo();
  }
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
__fastcall TFrameLuaEditor::TFrameLuaEditor(TComponent* Owner)
  : TFrame(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::Init()
{
  TIniFile *Ini = Usul()->Ini;
  int Count = Ini->ReadInteger("LuaEditor", "OpenCount", 0);
  if (Count) {
    for (int I = 0; I < Count; I++) {
      String Section = "LuaEditor.Open" + String(I);
      Load(Ini->ReadString(Section, "Path", EmptyStr));
//  Info->CurX = Memo->CurX;
//  Info->CurY = Memo->CurY;
//  Info->LeftCol = Memo->LeftCol;
//  Info->TopLine = Memo->TopLine;
//  Info->SelStartX = Memo->SelStartX;
//  Info->SelStartY = Memo->SelStartY;
//  Info->SelEndX = Memo->SelEndX;
//  Info->SelEndX = Memo->SelEndX;
    }
    TabControl->TabIndex = Ini->ReadInteger("LuaEditor", "Active", 0);
    TabControlChange(0);

    String V = Memo->Version;

  } else
    New();
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::Finish()
{
  TIniFile *Ini = Usul()->Ini;
  Ini->WriteInteger("LuaEditor", "OpenCount", TabControl->Tabs->Count);
  Ini->WriteInteger("LuaEditor", "Active", TabControl->TabIndex);
  for (int I = 0; I < TabControl->Tabs->Count; I++) {
    String Section = "LuaEditor.Open" + String(I);
    TInfo *Info = GetInfo(I);
    Ini->WriteString(Section, "Path", Info->Path);
//    String S;
//    S.sprintf(L"%d;%d;%d;%d;%d;%d;%d;%d", Info->CurX, Info->CurY, Info->LeftCol, Info->TopLine, Info->SelStartX, Info->SelStartY, Info->SelEndX, Info->SelEndY);
//    Ini->WriteString(Section, "State", S);
  }

}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::New()
{
  int Index = TabControl->Tabs->Add("<no name>.lua");
  TInfo *Info = new TInfo;
  TabControl->Tabs->Objects[Index] = (TObject*) Info;
  Info->MemoSource = new TAdvMemoSource(this);
  Info->MemoSource->SyntaxStyler = AdvLuaMemoStyler;
  Memo->MemoSource = Info->MemoSource;
  TabControl->TabIndex = Index;
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::Load(const String Path)
{
  if (Path.Length() && FileExists(Path)) {
    for (int I = 0; I < TabControl->Tabs->Count; I++) {
      TInfo *Info = GetInfo(I);
      if (Path == Info->Path) {
        TabControl->TabIndex = I;
        TabControlChange(0);
        return;
      }
    }
    int Index = TabControl->Tabs->Add(ShortPath(Path));
    TabControl->TabIndex = Index;
    TInfo *Info = new TInfo;
    TabControl->Tabs->Objects[Index] = (TObject*) Info;
    Info->Path = Path;
    Info->MemoSource = new TAdvMemoSource(this);
    Info->MemoSource->Lines->LoadFromFile(Path);
    Info->MemoSource->SyntaxStyler = AdvLuaMemoStyler;
    Memo->MemoSource = Info->MemoSource;
    FormMain->FrameProject->HighlightFromEditor(Info->Path);
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::NewJSON(const String S)
{
  int Index = TabControl->Tabs->Add("<no name>.json");
  TInfo *Info = new TInfo;
  TabControl->Tabs->Objects[Index] = (TObject*) Info;
  Info->MemoSource = new TAdvMemoSource(this);
  Info->MemoSource->SyntaxStyler = AdvJSONMemoStyler;
  Info->MemoSource->Lines->Text = S;
  Memo->MemoSource = Info->MemoSource;
  TabControl->TabIndex = Index;
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::MemoIsURL(TObject *Sender, UnicodeString Token, bool &IsUrl)
{
  IsUrl = (Token.Length() >= 5) && (Token[1] == '<') && (Token[Token.Length()] == '>');
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::MemoURLClick(TObject *Sender, UnicodeString URL)
{
  FormMain->Backdoor(GetLiveCodingTargets(), URL.SubString(2, URL.Length() - 2));
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::SetErrorPosition(const String Name, int LineNumber)
{
  for (int I = 0; I < TabControl->Tabs->Count; I++) {
    TInfo *Info = GetInfo(I);
    if (ExtractFileName(Info->Path).CompareIC(Name) == 0) {
      TabControl->TabIndex = I;
      TabControlChange(0);
      String Line = Info->MemoSource->Lines->Strings[LineNumber - 1];
      int Length = Line.Length();
      if (Length <= 0) Length = 10;
      Memo->SetError(LineNumber - 1, 0, 10);
      break;
    }
  }
}
//---------------------------------------------------------------------------
bool __fastcall TFrameLuaEditor::CanUndo()
{
  return Memo->CanUndo();
}
//---------------------------------------------------------------------------
bool __fastcall TFrameLuaEditor::CanRedo()
{
  return Memo->CanRedo();
}
//---------------------------------------------------------------------------
bool __fastcall TFrameLuaEditor::CanCut()
{
  return Memo->CanCut();
}
//---------------------------------------------------------------------------
bool __fastcall TFrameLuaEditor::CanPaste()
{
  return Memo->CanPaste();
}
//---------------------------------------------------------------------------
bool __fastcall TFrameLuaEditor::CanCopy()
{
  return Memo->CanCopy();
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::Undo()
{
  Memo->Undo();
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::Redo()
{
  Memo->Redo();
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::Cut()
{
  Memo->CutToClipBoard();
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::Copy()
{
  Memo->CopyToClipBoard();
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::Paste()
{
  Memo->PasteFromClipBoard();
}
//---------------------------------------------------------------------------
void __fastcall TFrameLuaEditor::SelectAll()
{
  Memo->SelectAll();
}
//---------------------------------------------------------------------------





//---------------------------------------------------------------------------

void __fastcall TFrameLuaEditor::MemoDragOver(TObject *Sender, TObject *Source, int X, int Y, TDragState State, bool &Accept)
{
  Accept = false;
  TTreeView *TreeView = dynamic_cast<TTreeView*>(Source);
  if (TreeView && TreeView->Selected) { 
    TFrameBrowser::TType Type = (TFrameBrowser::TType) TreeView->Selected->Data;
    if (Type >= TFrameBrowser::TYPE_LIBRARY) 
      Accept = true;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFrameLuaEditor::MemoDragDrop(TObject *Sender, TObject *Source, int X, int Y)
{
  TTreeView *TreeView = dynamic_cast<TTreeView*>(Source);
  if (TreeView && TreeView->Selected) { 
    TTreeNode *TreeNode = TreeView->Selected;
    TFrameBrowser::TType Type = (TFrameBrowser::TType) TreeNode->Data;
    switch (Type) {
      case TFrameBrowser::TYPE_OTHER :
        break;
      case TFrameBrowser::TYPE_LIBRARY :
        {
          String S = ReplaceStr(TreeNode->Text, ".*", EmptyStr);
          for (;;) {
            TreeNode = TreeNode->Parent;
            if ((TFrameBrowser::TType) TreeNode->Data != TFrameBrowser::TYPE_LIBRARY) 
              break;
            S = ReplaceStr(TreeNode->Text, ".*", EmptyStr) + "." + S;
          }
          Memo->MouseToCursor(X, Y, X, Y);
          Memo->InsertTextAtXY("\nrequire '" + S + "'", 0, Y);
        }
        break;
      case TFrameBrowser::TYPE_FUNCTION :
        Memo->MouseToCursor(X, Y, X, Y);
        Memo->InsertTextAtXY(TreeNode->Text, X, Y);
        break;
      case TFrameBrowser::TYPE_VALUE :
        break;
      case TFrameBrowser::TYPE_PARAMETER :
        {
          String Parameter = TreeNode->Text;
          String Librarys;
          TreeNode = TreeNode->Parent;
          for (;;) {
            TreeNode = TreeNode->Parent;
            if ((TFrameBrowser::TType) TreeNode->Data != TFrameBrowser::TYPE_LIBRARY) 
              break;
            if (Librarys.Length()) 
              Librarys = "." + Librarys;
            Librarys = ReplaceStr(TreeNode->Text, ".*", EmptyStr) + Librarys;
          }
          String Result;
          int P = Parameter.Pos("=");
          if (P) {
            Result = Parameter.SubString(1, P - 1);
            Parameter.Delete(1, P);
          }
          P = Parameter.Pos(".");
          if (!P) 
            P = Parameter.Pos(":");
          if (P) 
            Parameter.Delete(1, P - 1);
          Memo->MouseToCursor(X, Y, X, Y);
          if (Result.Length())
            Memo->InsertTextAtXY(Result + " = " + Librarys + Parameter, X, Y);
          else
            Memo->InsertTextAtXY(Librarys + Parameter, X, Y);
        }
        break;
    }
  }
}
//---------------------------------------------------------------------------

