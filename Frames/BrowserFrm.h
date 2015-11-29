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
#ifndef BrowserFrmH
#define BrowserFrmH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.AppEvnts.hpp>
#include <Vcl.ImgList.hpp>
#include "BrowserFrm.h"
#include "Target.h"
#include "NetTypes.h"
#include <Vcl.Menus.hpp>
//---------------------------------------------------------------------------
class TJSONArray;
class TFrameBrowser : public TFrame
{
__published:	// Von der IDE verwaltete Komponenten
  TPanel *Panel5;
  TSpeedButton *ButtonBrowse;
  TTreeView *TreeView;
  TPopupMenu *PopupMenu1;
  TMenuItem *MenuItemCopyAsHTML;
  TMenuItem *CopyforWiki1;
  TMenuItem *CopyforWikilinks1;
  TMenuItem *Wiki1;
  void __fastcall TreeViewCustomDrawItem(TCustomTreeView *Sender, TTreeNode *Node, TCustomDrawState State, bool &DefaultDraw);
  void __fastcall ButtonBrowseClick(TObject *Sender);
  void __fastcall MenuItemCopyAsHTMLClick(TObject *Sender);
  void __fastcall TreeViewGetSelectedIndex(TObject *Sender, TTreeNode *Node);
  void __fastcall CopyforWiki1Click(TObject *Sender);
  void __fastcall CopyforWikilinks1Click(TObject *Sender);
  void __fastcall TreeViewMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
private:	// Anwender-Deklarationen
  netIP4 IP;
  TTarget::TNodeType Type;

  TStringList *ExpandTrigger; 

  bool __fastcall Expand(TTreeNode *TreeNode);

  int MouseX, MouseY;
  

public:		// Anwender-Deklarationen
  enum TType {
    TYPE_OTHER     = 0, // clWhite
    TYPE_LIBRARY   = 1, // clSkyBlue
    TYPE_FUNCTION  = 2, // clMoneyGreen
    TYPE_VALUE     = 3, // clYellow
    TYPE_PARAMETER = 4, // RGB(255, 128, 0), orange
  };

  __fastcall TFrameBrowser(TComponent* Owner);
  void __fastcall Init();
  void __fastcall Finish();
  void __fastcall BrowsePN(const String Library, const String Function, uint32_t TransactionsId = 0);
  void __fastcall BrowseCN(const String Library, const String Function, uint32_t TransactionsId = 0);
  void __fastcall BrowseSN(const String Library, const String Function, uint32_t TransactionsId = 0);
  void __fastcall Set(netIP4 _IP, TTarget::TNodeType _Type);
  void __fastcall Recv(TJSONArray *Header, TJSONArray *Response);
};
//---------------------------------------------------------------------------
#endif
