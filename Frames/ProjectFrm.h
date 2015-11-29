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
#ifndef ProjectFrmH
#define ProjectFrmH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.AppEvnts.hpp>
#include "File.h"
//---------------------------------------------------------------------------
class TProject;
class TFrameProject : public TFrame
{
__published:	// Von der IDE verwaltete Komponenten
  TTreeView *TreeView;
  TPanel *Panel5;
  TSpeedButton *ButtonSettings;
  TSpeedButton *ButtonNew;
  TPopupMenu *PopupMenuProject;
  TPopupMenu *PopupMenuFile;
  TMenuItem *MenuItemProjectSettings;
  TMenuItem *N1;
  TPanel *Panel1;
  TMenuItem *MenuItemFileEdit;
  TMenuItem *N2;
  TMenuItem *MenuItemFileRemove;
  TMenuItem *MenuItemFileExclude;
  TMenuItem *Add1;
  TMenuItem *MenuItemProjectExistingFile;
  TMenuItem *MenuItemProjectNewFile;
  TPopupMenu *PopupMenuFolder;
  TMenuItem *Add2;
  TMenuItem *MenuItemDirectoryNewFile;
  TMenuItem *MenuItemDirectoryExistingFile;
  TMenuItem *MenuItemDirectoryNewFolder;
  TMenuItem *MenuItemProjectNewFolder;
  TMenuItem *N5;
  TMenuItem *Add3;
  TMenuItem *MenuItemFileNewFile;
  TMenuItem *MenuItemFileExistingFile;
  TMenuItem *MenuItemFileNewFolder;
  TImageList *ImageList2;
  TOpenDialog *OpenDialog;
  TMenuItem *MenuItemProjectRefresh;
  TApplicationEvents *ApplicationEvents1;
  TPopupMenu *PopupMenuWorkspace;
  TMenuItem *MenuItemProjectRemove;
  TMenuItem *N3;
  TMenuItem *Addnewproject1;
  TMenuItem *MenuItemWorkspaceAddNewProject;
  TMenuItem *MenuItemFolderRemove;
  TMenuItem *N4;
  TMenuItem *MenuItemWorkspaceAddExistingProject;
  void __fastcall TreeViewClick(TObject *Sender);
  void __fastcall MenuItemNewFileClick(TObject *Sender);
  void __fastcall MenuItemExistingFileClick(TObject *Sender);
  void __fastcall MenuItemNewFolderClick(TObject *Sender);
  void __fastcall TreeViewAdvancedCustomDrawItem(TCustomTreeView *Sender, TTreeNode *Node, TCustomDrawState State, TCustomDrawStage Stage, bool &PaintImages, bool &DefaultDraw);
  void __fastcall ButtonSettingsClick(TObject *Sender);
  void __fastcall ButtonNewClick(TObject *Sender);
  void __fastcall ApplicationEvents1Idle(TObject *Sender, bool &Done);
  void __fastcall TreeViewGetImageIndex(TObject *Sender, TTreeNode *Node);
  void __fastcall MenuItemProjectRemoveClick(TObject *Sender);
  void __fastcall MenuItemProjectRefreshClick(TObject *Sender);
  void __fastcall MenuItemFileEditClick(TObject *Sender);
  void __fastcall MenuItemFileRemoveClick(TObject *Sender);
  void __fastcall MenuItemFileExcludeClick(TObject *Sender);
  void __fastcall MenuItemFolderRemoveClick(TObject *Sender);
  void __fastcall MenuItemWorkspaceAddExistingProjectClick(TObject *Sender);
  void __fastcall TreeViewMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall ApplicationEvents1Message(tagMSG &Msg, bool &Handled);

private:	// Anwender-Deklarationen
  String HighlightFromEditorPath;
  String HighlightFromTargetPath;

  TProject* __fastcall GetProject(TTreeNode *TreeNode);
  String __fastcall GetPath(TTreeNode *TreeNode);
public:		// Anwender-Deklarationen
  enum Type {
    tWorkspace,
    tProject,
    tFolder,
    tFile
  };

  __fastcall TFrameProject(TComponent* Owner);
  void __fastcall Init();
  void __fastcall Finish();
  void __fastcall Refresh();
  TStringList* __fastcall GetProjects();

  void __fastcall HighlightFromEditor(const String Path);
  void __fastcall HighlightFromTarget(const String Path);
};
//---------------------------------------------------------------------------
#endif
