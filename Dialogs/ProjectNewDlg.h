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

#ifndef ProjectNewDlgH
#define ProjectNewDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.FileCtrl.hpp>
#include <Vcl.ImgList.hpp>
#include <SHDocVw.hpp>
#include <Vcl.OleCtrls.hpp>
#include <Vcl.AppEvnts.hpp>
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------
class TDialogProjectNew : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
  TPanel *Panel1;
  TButton *ButtonCancel;
  TPageControl *PageControl;
  TTabSheet *TabSheet1;
  TTabSheet *TabSheet2;
  TListView *ListView;
  TTreeView *TreeView;
  TButton *ButtonCreate;
  TSplitter *Splitter1;
  TRichEdit *RichEdit;
  TGroupBox *GroupBox1;
  TApplicationEvents *ApplicationEvents1;
  TLabel *Label;
  TPanel *Panel2;
  TEdit *Edit;
  TSpeedButton *ButtonOpen;
  void __fastcall ButtonCancelClick(TObject *Sender);
  void __fastcall ButtonCreateClick(TObject *Sender);
  void __fastcall ListViewClick(TObject *Sender);
  void __fastcall TreeViewGetImageIndex(TObject *Sender, TTreeNode *Node);
  void __fastcall ApplicationEvents1Idle(TObject *Sender, bool &Done);
  void __fastcall ButtonOpenClick(TObject *Sender);
private:	// Anwender-Deklarationen
  String TemplatePath;
  void __fastcall BuildTreeView(TTreeView *TreeView, TTreeNode *TreeNode, const String Path);
public:		// Anwender-Deklarationen
  __fastcall TDialogProjectNew(TComponent* Owner);
  String __fastcall GetProjectPath();
};
//---------------------------------------------------------------------------
#endif
