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
#include "ProjectFrm.h"
#include "Usul.h"
#include "Error2.h"
#include "MainFrm.h"
#include <System.IOUtils.hpp>
#include "LuaEditorFrm.h"
#include "ProjectDlg.h"
#include "Project.h"
#include "ProjectNewDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
TProject* __fastcall TFrameProject::GetProject(TTreeNode *TreeNode)
{
  while (TreeNode->Level > 1) TreeNode = TreeNode->Parent;
  return (TProject*) TreeNode->Data;
}
//---------------------------------------------------------------------------
String __fastcall TFrameProject::GetPath(TTreeNode *TreeNode)
{
  String Path = TreeNode->Text;
  while (TreeNode->Level >= 2) {
    TreeNode = TreeNode->Parent;
    Path = TreeNode->Text + "\\" + Path;
  }
  return Path;
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TFrameProject::ApplicationEvents1Idle(TObject *Sender, bool &Done)
{
  TTreeNode *TreeNode = TreeView->Selected;
  ButtonSettings->Enabled = TreeNode && (TreeNode->Level >= 1);
}
//---------------------------------------------------------------------------
void __fastcall TFrameProject::ButtonNewClick(TObject *Sender)
{
  TDialogProjectNew *DialogProjectNew = 0;
  try {
    DialogProjectNew = new TDialogProjectNew(this);
    if (DialogProjectNew->ShowModal() == mrOk) {
      String Path = DialogProjectNew->GetProjectPath();
      TTreeNode *TreeNode = TreeView->Items->AddChild(TreeView->Items->Item[0], Path);
      TProject *Project = new TProject(TreeNode);
      TreeNode->Data = Project;
      Project->Open(Path);
      TreeNode->Expand(false);
    }
  }
  __finally {
    delete DialogProjectNew;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameProject::MenuItemWorkspaceAddExistingProjectClick(TObject *Sender)
{
  try {
    String Path = Usul()->Path;
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
      if (!SelectDirectory("Select Project Directory", Path, Path))
        return;
    }
    {
      TTreeNode *TreeNode = TreeView->Items->Item[0];
      for (int I = 0; I < TreeNode->Count; I++) {
        TProject *Project = (TProject*) TreeNode->Item[I]->Data;
        if (Project->GetPath().CompareIC(Path) == 0)
          throw Error0("Project '" + Path + "' already exists in workspace");
      }
    }
    TTreeNode *TreeNode = TreeView->Items->AddChild(TreeView->Items->Item[0], Path);
    TProject *Project = new TProject(TreeNode);
    TreeNode->Data = Project;
    Project->Open(Path);
    TreeNode->Expand(false);
  }
  catch (TError &E) {
    E.Show();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameProject::ButtonSettingsClick(TObject *Sender)
{
  TTreeNode *TreeNode = TreeView->Selected;
  TDialogProject *DialogProject = 0;
  try {
    DialogProject = new TDialogProject(this, GetProject(TreeNode));
    DialogProject->ShowModal();
  }
  __finally {
    delete DialogProject;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameProject::MenuItemProjectRemoveClick(TObject *Sender)
{
  TTreeNode *TreeNode = TreeView->Selected;
  int Result = MessageBox("Remove the project '" + TreeNode->Text + "' from the workspace?","Remove project",MB_ICONQUESTION | MB_YESNOCANCEL);
  if (Result == IDYES) 
    TreeNode->Delete();
}
//---------------------------------------------------------------------------
void __fastcall TFrameProject::MenuItemProjectRefreshClick(TObject *Sender)
{
  Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TFrameProject::MenuItemNewFileClick(TObject *Sender)
{
  try {
    String Path = GetPath(TreeView->Selected);
    String Name;
    if (InputQuery("New file", "Name", Name)) {
      Path += "\\" + Name;
      if (FileExists(Path))
        throw Error0("File already exists");
      ::TFile F;
      F.Open(Path, "wb");
    }
  }
  catch (TError &E) {
    E.Show();
  }
  Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TFrameProject::MenuItemExistingFileClick(TObject *Sender)
{
  try {
    String Path = GetPath(TreeView->Selected);
    OpenDialog->InitialDir = Path;
    if (OpenDialog->Execute()) {
      String SourcePath = OpenDialog->FileName;
      String TargetPath = Path + "\\" + ExtractFileName(OpenDialog->FileName);
      if (FileExists(TargetPath)) {
        int Result = MessageBox("File '" + ExtractFileName(OpenDialog->FileName) + "' already exists. Overwrite the existing file?", "Add file to project", MB_ICONQUESTION | MB_YESNOCANCEL);
        if (Result != IDYES)
          return;
      }
      if (!CopyFileTo(SourcePath, TargetPath))
        throw Error0("Can't copy file");
    }
  }
  catch (TError &E) {
    E.Show();
  }
  Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TFrameProject::MenuItemNewFolderClick(TObject *Sender)
{
  try {
    String Path = GetPath(TreeView->Selected);
    String Name;
    if (InputQuery("New folder", "Name", Name)) {
      Path += "\\" + Name;
      if (DirectoryExists(Path))
        throw Error0("Folder already exists");
      if (!CreateDir(Path))
        throw Error0("Can't create folder directory");
    }
  }
  catch (TError &E) {
    E.Show();
  }
  Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TFrameProject::MenuItemFolderRemoveClick(TObject *Sender)
{
  TTreeNode *TreeNode = TreeView->Selected;
  int Result = MessageBox("Remove folder '" + TreeNode->Text + "' from the project?", "Remove project", MB_ICONQUESTION | MB_YESNOCANCEL);
  if (Result == IDYES) {
    TreeNode->Delete();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameProject::MenuItemFileEditClick(TObject *Sender)
{
  TTreeNode *TreeNode = TreeView->Selected;
  if (TreeNode->ImageIndex >= tFile) {
    String Path = GetPath(TreeNode);
    FormMain->FrameLuaEditor->Load(Path);
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameProject::MenuItemFileRemoveClick(TObject *Sender)
{
  TTreeNode *TreeNode = TreeView->Selected;
  int Result = MessageBox("Remove file '" + TreeNode->Text + "' from the project?", "Remove project", MB_ICONQUESTION | MB_YESNOCANCEL);
  if (Result == IDYES) 
    TreeNode->Delete();
}
//---------------------------------------------------------------------------
void __fastcall TFrameProject::MenuItemFileExcludeClick(TObject *Sender)
{
//  
}
//---------------------------------------------------------------------------
void __fastcall TFrameProject::TreeViewAdvancedCustomDrawItem(TCustomTreeView *Sender, TTreeNode *TreeNode, TCustomDrawState State, TCustomDrawStage Stage, bool &PaintImages, bool &DefaultDraw)
{
  TProject *Project = GetProject(TreeNode);
  if (TreeNode->Level == 1) {
    TreeView->Canvas->Font->Color = HighlightFromEditorPath.Length() && (TreeNode->Text + "\\" == HighlightFromEditorPath.SubString(1, TreeNode->Text.Length() + 1)) ? clYellow : clWhite;
    //TreeView->Canvas->Font->Color = HighlightFromEditorPath.Length() && (TreeNode->Text + "\\" == HighlightFromEditorPath.SubString(1, TreeNode->Text.Length() + 1)) ? clYellow : clWhite;
  } else
    TreeView->Canvas->Font->Color = Project->GetTreeNodeTextColor(TreeNode);
  TreeView->Canvas->Brush->Color = State.Contains(cdsFocused) ? RGB(64, 64, 64) : RGB(24, 24, 24);
}
//---------------------------------------------------------------------------
void __fastcall TFrameProject::TreeViewClick(TObject *Sender)
{
  TTreeNode *TreeNode = TreeView->Selected;                                                      
  if (TreeNode) {
    switch (TreeNode->ImageIndex) {
      case tWorkspace  : TreeView->PopupMenu = PopupMenuWorkspace; break;
      case tProject    : TreeView->PopupMenu = PopupMenuProject; break;
      case tFolder     : TreeView->PopupMenu = PopupMenuFolder; break;
      default          : TreeView->PopupMenu = PopupMenuFile; break;
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameProject::TreeViewGetImageIndex(TObject *Sender, TTreeNode *TreeNode)
{
  if (TreeNode->Level >= 2) {
    int Index = FormMain->FileExtensionToImageIndex(ExtractFileExt(TreeNode->Text));
    if (Index >= 0) {
      TreeNode->ImageIndex = Index;
      TreeNode->SelectedIndex = Index;
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameProject::TreeViewMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if (Button == mbRight) {
    TreeView->Selected = TreeView->GetNodeAt(X, Y);
    TreeViewClick(TreeView);
  }
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
__fastcall TFrameProject::TFrameProject(TComponent* Owner)
  : TFrame(Owner)
{}
//---------------------------------------------------------------------------
void __fastcall TFrameProject::Init()
{
  TreeView->Items->BeginUpdate();
  TTreeNode *TreeNodeProjects = TreeView->Items->Add(0, "Workspace");
  int Count = Usul()->Ini->ReadInteger("Projects", "Count", 0);
  for (int I = 0;I < Count; I++) {
    String Path = Usul()->Ini->ReadString("Projects", "Path" + String(I), EmptyStr);
    if (DirectoryExists(Path)) {
      TTreeNode *TreeNode = TreeView->Items->AddChild(TreeNodeProjects, EmptyStr);
      TProject *Project = new TProject(TreeNode);
      TreeNode->Data = Project;
      Project->Open(Path);
    }
  }
  TreeNodeProjects->Expand(false);
  if (TreeNodeProjects->Count == 1)
    TreeNodeProjects->Item[0]->Expand(false);
  TreeView->Items->EndUpdate();
  ///////////////////////////DragAcceptFiles(TreeView->Handle, true);
}
//---------------------------------------------------------------------------
void __fastcall TFrameProject::Finish()
{
  TTreeNode *TreeNodeProjects = TreeView->Items->Item[0];
  int Count = TreeNodeProjects->Count;
  Usul()->Ini->WriteInteger("Projects", "Count", Count);
  for (int I = 0;I < Count; I++) {
    TTreeNode *TreeNode = TreeNodeProjects->Item[I];
    TProject *Project = (TProject*) TreeNode->Data;
    Usul()->Ini->WriteString("Projects", "Path" + String(I), Project->GetPath());
    Project->Close();
    delete Project;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameProject::Refresh()
{
  TreeView->Items->BeginUpdate();
  TTreeNode *TreeNodeProjects = TreeView->Items->Item[0];
  int Count = TreeNodeProjects->Count;
  Usul()->Ini->WriteInteger("Projects", "Count", Count);
  for (int I = 0; I < Count; I++) {
    TTreeNode *TreeNode = TreeNodeProjects->Item[I];
    TProject *Project = (TProject*) TreeNode->Data;
    Project->Refresh();
  }
  TreeView->Items->EndUpdate();
}
//---------------------------------------------------------------------------
TStringList* __fastcall TFrameProject::GetProjects()
{
  TStringList *StringList = new TStringList;
  TTreeNode *TreeNodeProjects = TreeView->Items->Item[0];
  int Count = TreeNodeProjects->Count;
  for (int I = 0;I < Count; I++) {
    TTreeNode *TreeNode = TreeNodeProjects->Item[I];
    StringList->Add(TreeNode->Text);
  }
  return StringList;
}
//---------------------------------------------------------------------------
void __fastcall TFrameProject::HighlightFromEditor(const String Path)
{
  HighlightFromEditorPath = Path;
  TreeView->Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TFrameProject::HighlightFromTarget(const String Path)
{
  HighlightFromTargetPath = Path;
  TreeView->Invalidate();
}
//---------------------------------------------------------------------------




























void __fastcall TFrameProject::ApplicationEvents1Message(tagMSG &Msg, bool &Handled)
{
  if (Msg.message == WM_DROPFILES) {
    int N = DragQueryFile((HDROP) Msg.wParam, -1, 0, 0);
    for (int I = 0; I < N; I++) {
      wchar_t Buffer[MAX_PATH];
      DragQueryFile((HDROP) Msg.wParam, I, Buffer, sizeof(Buffer));
      String Name(Buffer);
      /*TreeView->Selected*/ TTreeNode *TreeNode = TreeView->GetNodeAt(Msg.pt.x, Msg.pt.y);
      if (TreeNode)
        FormMain->Log(0, TreeNode->Text);



    }
    DragFinish((HDROP) Msg.wParam);
    Handled = true;
  }
}
//---------------------------------------------------------------------------


