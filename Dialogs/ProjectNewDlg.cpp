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
#include "ProjectNewDlg.h"
#include "Project.h"
#include "Busy.h"
#include "Usul.h"
#include <System.StrUtils.hpp>
#include <System.SysUtils.hpp>
#include "MainFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TDialogProjectNew::BuildTreeView(TTreeView *TreeView, TTreeNode *TreeNode, const String Path)
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
        if (!(SearchRec.Attr & faDirectory)) {
          TTreeNode *TreeNode2 = TreeView->Items->AddChild(TreeNode, SearchRec.Name);
          TreeNode2->ImageIndex = 1;
          TreeNode2->SelectedIndex = 1;
        }
      }
    } while (FindNext(SearchRec) == 0);
    FindClose(SearchRec);
  }
}
//---------------------------------------------------------------------------
void __fastcall TDialogProjectNew::ApplicationEvents1Idle(TObject *Sender, bool &Done)
{
  bool IsDirectoryExists = DirectoryExists(Edit->Text);
  bool IsFileExists = FileExists(Edit->Text);
  if (IsDirectoryExists) 
    Label->Caption = "Project aready exists! Please use a different name.";
  else if (IsFileExists) 
    Label->Caption = "There is a file with this project name! Please use a different name.";
  else
    Label->Caption = EmptyStr;
  ButtonCreate->Enabled = ListView->Selected && !IsDirectoryExists && !IsFileExists;
}
//---------------------------------------------------------------------------
void __fastcall TDialogProjectNew::ListViewClick(TObject *Sender)
{
  if (ListView->Selected) {
    TBusy Busy;
    {
      String Path = Usul()->Path + "\\Templates\\Projects\\." + ListView->Selected->Caption + ".rtf";
      RichEdit->Visible = FileExists(Path);
      if (RichEdit->Visible)
        RichEdit->Lines->LoadFromFile(Path);
    }
    TreeView->Items->BeginUpdate();
    TreeView->Items->Clear();
    String Path = TemplatePath + "\\" + ListView->Selected->Caption;
    TTreeNode *TreeNode = TreeView->Items->Add(0, Path);
    TreeNode->ImageIndex = 1;
    TreeNode->SelectedIndex = 1;
    BuildTreeView(TreeView, TreeNode, Path);
    TreeNode->Expand(true);
    TreeView->Items->EndUpdate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TDialogProjectNew::TreeViewGetImageIndex(TObject *Sender, TTreeNode *TreeNode)
{
  int Index = FormMain->FileExtensionToImageIndex(ExtractFileExt(TreeNode->Text));
  if ((Index >= 0) && (TreeNode->Level >= 1)) {
    TreeNode->ImageIndex = Index;
    TreeNode->SelectedIndex = Index;
  }
}
//---------------------------------------------------------------------------
void __fastcall TDialogProjectNew::ButtonCancelClick(TObject *Sender)
{
  ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TDialogProjectNew::ButtonCreateClick(TObject *Sender)
{
  TTreeNode *TreeNodeLog = FormMain->Log(0, "Create new project '" + Edit->Text + "'");
  FormMain->Log(TreeNodeLog, CreateDir(Edit->Text) ? 1 : 3, "Create directory '" + Edit->Text + "'");
  for (int I = 1; I < TreeView->Items->Count; I++) {
    TTreeNode *TreeNode = TreeView->Items->Item[I];
    bool IsDirectory = TreeNode->ImageIndex == 2;
    String SourcePath = TreeNode->Text;
    while (TreeNode->Level >= 2) {
      TreeNode = TreeNode->Parent;
      SourcePath = TreeNode->Text + "\\" + SourcePath;
    }
    String TargetPath = Edit->Text + "\\" + SourcePath;
    SourcePath = TreeView->Items->Item[0]->Text + "\\" + SourcePath;
    if (IsDirectory) {
      bool Success = CreateDir(TargetPath);
      FormMain->Log(TreeNodeLog, Success ? 1 : 3, "Create directory '" + TargetPath + "'");
    } else {
      bool Success = CopyFileTo(SourcePath, TargetPath);
      FormMain->Log(TreeNodeLog, Success ? 1 : 3, "Copy file '" + SourcePath + "' --> '" + TargetPath + "'");
    }
  }
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TDialogProjectNew::ButtonOpenClick(TObject *Sender)
{
  String Path = Usul()->PathDocuments;
  if (Win32MajorVersion >= 6) {
    TFileOpenDialog *FileOpenDialog = new TFileOpenDialog(0);
    FileOpenDialog->Title = "Select Project Directory";
    FileOpenDialog->Options = TFileDialogOptions() << fdoPickFolders << fdoPathMustExist << fdoForceFileSystem;
    FileOpenDialog->OkButtonLabel = "Select";
    FileOpenDialog->DefaultFolder = Path;
    FileOpenDialog->FileName = Path;
    if (!FileOpenDialog->Execute()) {
      delete FileOpenDialog;
      return;
    }
    Path = FileOpenDialog->FileName;
    delete FileOpenDialog;
  } else {
    //if (!SelectDirectory(Path, TSelectDirOpts(), 0))
    if (!SelectDirectory("Select Project Directory", Path, Path))
      return;
  }
  Edit->Text = Path;
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
__fastcall TDialogProjectNew::TDialogProjectNew(TComponent* Owner)
  : TForm(Owner)
{
  TemplatePath = Usul()->Ini->ReadString("Settings", "PathTemplatesProjects", TemplatePath = Usul()->Path + "\\Templates\\Projects");
  PageControl->ActivePageIndex = 0;
  TSearchRec SearchRec;
  if (FindFirst(TemplatePath + "\\*", faDirectory, SearchRec) == 0) {
    do {
      if (SearchRec.Name[1] != '.') {
        TListItem *ListItem = ListView->Items->Add();
        ListItem->Caption = SearchRec.Name;
        ListItem->ImageIndex = 1;
      }
    } while (FindNext(SearchRec) == 0);
  }
  FindClose(SearchRec);

  if (Usul()->HasWriteAccessToExeDirectrory())
    Edit->Text = Usul()->Path + "\\New project";
  else
    Edit->Text = Usul()->PathDocuments + "\\New project";
  for (int I = 2;;I++) {
    bool IsDirectoryExists = DirectoryExists(Edit->Text);
    bool IsFileExists = FileExists(Edit->Text);
    if (!IsDirectoryExists && !IsFileExists) break;
    Edit->Text = Usul()->Path + "\\New project" + String(I);
  }
}
//---------------------------------------------------------------------------
String __fastcall TDialogProjectNew::GetProjectPath()
{
  return Edit->Text;
}
//---------------------------------------------------------------------------







