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
#pragma hdrstop
#include "Project.h"
#include <System.IniFiles.hpp>
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
String __fastcall TProject::GetPathFromTreeNode(TTreeNode *TreeNode)
{
  String Path;
  for (;;) {
    Path = TreeNode->Text + (Path.Length() ? "\\" + Path : EmptyStr);
    if (!TreeNode->Level) break;
    TreeNode = TreeNode->Parent;
  }
  return Path;
}
//---------------------------------------------------------------------------
void __fastcall TProject::BuildTreeView(TTreeView *TreeView, TTreeNode *TreeNode, const String Path)
{
  TSearchRec SearchRec;
  if (FindFirst(Path + "\\*.*", faAnyFile, SearchRec) == 0) {
    do {
      if (SearchRec.Name[1] != '.') {
        if (SearchRec.Attr & faDirectory) {
          TTreeNode *TreeNode2 = TreeView->Items->AddChild(TreeNode, SearchRec.Name);
          TreeNode2->ImageIndex = 2;
          TreeNode2->SelectedIndex = 2;
          BuildTreeView(TreeView, TreeNode2, Path + "\\" + SearchRec.Name);
        }
      }
    } while (FindNext(SearchRec) == 0);
    FindClose(SearchRec);
  }
  if (FindFirst(Path + "\\*.*", faAnyFile, SearchRec) == 0) {
    do {
      if (SearchRec.Name[1] != '.') {
        if (!(SearchRec.Attr & faDirectory))
          TTreeNode *TreeNode2 = TreeView->Items->AddChild(TreeNode, SearchRec.Name);
      }
    } while (FindNext(SearchRec) == 0);
    FindClose(SearchRec);
  }
}
//---------------------------------------------------------------------------
__fastcall TProject::TProject(TTreeNode *_TreeNode):
  StartupFile("Startup.lua")
{
  TreeNode = _TreeNode;
  TreeNode->ImageIndex = 1;
  TreeNode->SelectedIndex = 1;
}
//---------------------------------------------------------------------------
__fastcall TProject::~TProject()
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TProject::Open(const String Path)
{
  SetPath(Path);
  TIniFile *Ini = new TIniFile(Path + "\\.Settings.cfg");
  StartupFile = Ini->ReadString("Settings", "StartupFile", "Startup.lua");
  delete Ini;
  Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TProject::Close()
{
  TIniFile *Ini = new TIniFile(GetPath() + "\\.Settings.cfg");
  Ini->WriteString("Settings", "StartupFile", StartupFile);
  delete Ini;
}
//---------------------------------------------------------------------------
void __fastcall TProject::Refresh()
{
  String Path = GetPath();
  if (DirectoryExists(Path)) {
    TreeNode->DeleteChildren();
    BuildTreeView(dynamic_cast<TTreeView*>(TreeNode->TreeView), TreeNode, Path);
  }
}
//---------------------------------------------------------------------------
void __fastcall TProject::SetPath(const String Path)
{
  TreeNode->Text = Path;
}
//---------------------------------------------------------------------------
String __fastcall TProject::GetPath()
{
  return TreeNode->Text;
}
//---------------------------------------------------------------------------
String __fastcall TProject::GetStartupFile()
{
  return StartupFile;
}
//---------------------------------------------------------------------------
TColor __fastcall TProject::GetTreeNodeTextColor(TTreeNode *TreeNode)
{
  if ((TreeNode->Level == 2) && (TreeNode->Text == StartupFile))
    return clLime;
  return clWhite;
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
