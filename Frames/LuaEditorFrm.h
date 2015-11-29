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
#ifndef LuaEditorFrmH
#define LuaEditorFrmH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "AdvMemo.hpp"
#include "AdvmCSHS.hpp"
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.AppEvnts.hpp>
#include <Vcl.Dialogs.hpp>
#include "MessageBoxDlg.h"
#include <Vcl.Menus.hpp>
#include <Vcl.ImgList.hpp>
//---------------------------------------------------------------------------
class TFrameLuaEditor : public TFrame
{
__published:	// Von der IDE verwaltete Komponenten
  TPanel *Panel5;
  TSpeedButton *ButtonOpenb;
  TSpeedButton *ButtonSave;
  TSpeedButton *ButtonSaveAs;
  TSpeedButton *ButtonNew;
  TTabControl *TabControl;
  TAdvMemo *Memo;
  TApplicationEvents *ApplicationEvents1;
  TSaveDialog *SaveDialog;
  TOpenDialog *OpenDialog;
  TSpeedButton *ButtonReboot;
  TAdvCSharpMemoStyler *AdvLuaMemoStyler;
  TSpeedButton *ButtonRestart;
  TPopupMenu *PopupMenuMemo;
  TMenuItem *MenuItemUndo;
  TMenuItem *MenuItemRedo;
  TMenuItem *N1;
  TMenuItem *MenuItemCut;
  TMenuItem *MenuItemCopy;
  TMenuItem *MenuItemPaste;
  TMenuItem *N2;
  TMenuItem *MenuItemSelectAll;
  TMenuItem *N3;
  TMenuItem *MenuItemToggleCommend;
  TMenuItem *N4;
  TMenuItem *MenuItemLoad;
  TMenuItem *MenuItemToggleBookmarkX;
  TMenuItem *MenuItemToggleBookmark1;
  TMenuItem *MenuItemToggleBookmark2;
  TMenuItem *MenuItemToggleBookmark3;
  TMenuItem *MenuItemToggleBookmark4;
  TMenuItem *MenuItemToggleBookmark6;
  TMenuItem *MenuItemToggleBookmark7;
  TMenuItem *MenuItemToggleBookmark8;
  TMenuItem *MenuItemToggleBookmark9;
  TMenuItem *MenuItemToggleBookmark5;
  TMenuItem *MenuItemEdit;
  TMenuItem *Gotobookmark1;
  TMenuItem *N11;
  TMenuItem *N21;
  TMenuItem *N31;
  TMenuItem *N41;
  TMenuItem *N51;
  TMenuItem *N61;
  TMenuItem *N71;
  TMenuItem *N81;
  TMenuItem *N91;
  TMenuItem *N5;
  TPopupMenu *PopupMenuTabControl;
  TMenuItem *MenuItemCloseTab;
  TAdvCSharpMemoStyler *AdvJSONMemoStyler;
  TSpeedButton *ButtonSaveAll;
  TMenuItem *N6;
  TMenuItem *CopyHTML1;
  TPanel *Panel1;
  TAdvCSharpMemoStyler *AdvCSharpMemoStyler1;
  TMenuItem *Special1;
  TMenuItem *MenuItemPasteEVECoprocessorList;
  void __fastcall ButtonNewClick(TObject *Sender);
  void __fastcall ButtonOpenbClick(TObject *Sender);
  void __fastcall ButtonSaveClick(TObject *Sender);
  void __fastcall TabControlChange(TObject *Sender);
  void __fastcall ApplicationEvents1Idle(TObject *Sender, bool &Done);
  void __fastcall ButtonSaveAsClick(TObject *Sender);
  void __fastcall ButtonRebootClick(TObject *Sender);
  void __fastcall ButtonRestartClick(TObject *Sender);
  void __fastcall MenuItemUndoClick(TObject *Sender);
  void __fastcall MenuItemRedoClick(TObject *Sender);
  void __fastcall MenuItemCutClick(TObject *Sender);
  void __fastcall MenuItemCopyClick(TObject *Sender);
  void __fastcall MenuItemPasteClick(TObject *Sender);
  void __fastcall MenuItemSelectAllClick(TObject *Sender);
  void __fastcall MenuItemToggleCommendClick(TObject *Sender);
  void __fastcall PopupMenuMemoPopup(TObject *Sender);
  void __fastcall MenuItemLoadClick(TObject *Sender);
  void __fastcall MemoLineBkColor(TObject *Sender, int LineNo, TColor &BkColor);
  void __fastcall MemoMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall MenuItemToggleBookmarkXClick(TObject *Sender);
  void __fastcall TabControlChanging(TObject *Sender, bool &AllowChange);
  void __fastcall MenuItemCloseTabClick(TObject *Sender);
  void __fastcall ButtonSaveAllClick(TObject *Sender);
  void __fastcall MemoIsURL(TObject *Sender, UnicodeString AToken, bool &IsUrl);
  void __fastcall MemoURLClick(TObject *Sender, UnicodeString URL);
  void __fastcall CopyHTML1Click(TObject *Sender);
  void __fastcall TabControlGetImageIndex(TObject *Sender, int TabIndex, int &ImageIndex);
  void __fastcall ButtonRebootMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall MenuItemSetLiveCodingTargetsClick(TObject *Sender);
  void __fastcall MenuItemPasteEVECoprocessorListClick(TObject *Sender);
  void __fastcall MemoDragOver(TObject *Sender, TObject *Source, int X, int Y, TDragState State, bool &Accept);
  void __fastcall MemoDragDrop(TObject *Sender, TObject *Source, int X, int Y);



private:	// Anwender-Deklarationen
  class TInfo {
    TStringList *LiveCodingTargets;
  public:
    int Index;
    TAdvMemoSource *MemoSource;
   	//int CurX, CurY, LeftCol, TopLine, SelStartX, SelStartY,SelEndX, SelEndY;
    String Path;
    __fastcall TInfo():
      LiveCodingTargets(0)
    {}
    __fastcall ~TInfo()
    {
      SetLiveCodingTargets(0);
    }
    void __fastcall SetLiveCodingTargets(TStringList* _LiveCodingTargets)
    {
      if ((int) LiveCodingTargets >= 0xFF) 
        delete LiveCodingTargets;
      LiveCodingTargets = _LiveCodingTargets;
    }
    void __fastcall SetLiveCodingTargets(int _LiveCodingTargets)
    {
      SetLiveCodingTargets((TStringList*) _LiveCodingTargets);
    }
    TStringList* __fastcall GetLiveCodingTargets()
    {
      return LiveCodingTargets;
    }
  };

  struct TLoadContext {
    int LineStartNo;
    int LineEndNo;
    String Source;
    String Error;
    __fastcall TLoadContext()
    {
      Clear();
    }
    __fastcall Clear()
    {
      LineStartNo = LineEndNo = -1;
      Source = EmptyStr;
    }
  } LoadContext;

  TInfo* __fastcall GetInfo()
  {
    int Index = TabControl->TabIndex;
    if (Index < 0)
      return 0;
    TInfo *Info = (TInfo*) TabControl->Tabs->Objects[Index];
    Info->Index = Index;
    return Info;
  }

  TInfo* __fastcall GetInfo(int Index)
  {
    if ((Index < 0) || (Index >= TabControl->Tabs->Count)) return 0;
    TInfo *Info = (TInfo*) TabControl->Tabs->Objects[Index];
    Info->Index = Index;
    return Info;
  }

  String __fastcall ShortPath(const String Path);

  TStringList* __fastcall GetLiveCodingTargets();

public:		// Anwender-Deklarationen
  __fastcall TFrameLuaEditor(TComponent* Owner);
  void __fastcall Init();
  void __fastcall Finish();
  void __fastcall New();
  void __fastcall Load(const String Path);

  void __fastcall NewJSON(const String S);

  void __fastcall SetErrorPosition(const String Name, int LineNumber);

  bool __fastcall CanUndo();
  bool __fastcall CanRedo();
  bool __fastcall CanCut();
  bool __fastcall CanCopy();
  bool __fastcall CanPaste();

  void __fastcall Undo();
  void __fastcall Redo();
  void __fastcall Cut();
  void __fastcall Copy();
  void __fastcall Paste();
  void __fastcall SelectAll();
};
//---------------------------------------------------------------------------
#endif
