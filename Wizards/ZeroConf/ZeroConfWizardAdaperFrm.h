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

#ifndef ZeroConfWizardAdaperFrmH
#define ZeroConfWizardAdaperFrmH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "IPHelper.h"
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFrameZeroConfWizardAdapter : public TFrame
{
__published:	// Von der IDE verwaltete Komponenten
  TPanel *Panel1;
  TTreeView *TreeView;
  TShape *Shape;
  void __fastcall TreeViewCustomDrawItem(TCustomTreeView *Sender, TTreeNode *Node, TCustomDrawState State, bool &DefaultDraw);
private:	// Benutzer-Deklarationen
  TIPHelper::TIPAdapterInfo::TItem *Item;
  int Index;
  bool Init;
  void __fastcall Text(int Level, const String S)
  {                  
    TTreeNode *TreeNode = TreeView->Items->Add(0, S);
    TreeNode->Data = (void*) Level;
  }
public:		// Benutzer-Deklarationen
  __fastcall TFrameZeroConfWizardAdapter(TComponent* Owner, TIPHelper::TIPAdapterInfo::TItem *_Item, int _Index);
  void __fastcall Refresh();
};
//---------------------------------------------------------------------------
#endif
