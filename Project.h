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
#ifndef ProjectH
#define ProjectH
//---------------------------------------------------------------------------
class TDialogProject;
class TProject
{
  friend TDialogProject;
private:
  TTreeNode *TreeNode;
  String StartupFile;

  String __fastcall GetPathFromTreeNode(TTreeNode *TreeNode);
  void __fastcall BuildTreeView(TTreeView *TreeView, TTreeNode *TreeNode, const String Path);
public:
  __fastcall TProject(TTreeNode *_TreeNode);
  __fastcall ~TProject();
  
  void __fastcall Open(const String Path);
  void __fastcall Close();
  void __fastcall Refresh();

  void __fastcall SetPath(const String Path);
  String __fastcall GetPath();
  String __fastcall GetStartupFile();
  TColor __fastcall GetTreeNodeTextColor(TTreeNode *TreeNode);
};
//---------------------------------------------------------------------------
#endif
