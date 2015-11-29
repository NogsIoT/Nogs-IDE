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
#ifndef MainFrmH
#define MainFrmH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Imaging.jpeg.hpp>
#include <IdAntiFreezeBase.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdSocketHandle.hpp>
#include <IdUDPBase.hpp>
#include <IdUDPServer.hpp>
#include <Vcl.IdAntiFreeze.hpp>
#include "LogFrm.h"
#include "LuaEditorFrm.h"
#include "ProjectFrm.h"
#include "TargetsFrm.h"
#include "Types.h"
#include "BrowserFrm.h"
#include "ModelessBaseDlg.h"
#include "NogsDesigner.h"
#include "TopologieEditorFrm.h"
#include "Indicator.h"
#include <IdTrivialFTPServer.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.AppEvnts.hpp>
#include <IdIPAddrMon.hpp>
#include "ZeroConfHelper.h"
#include "ZeroConfWizardDlg.h"
#include "NetTypes.h"
//---------------------------------------------------------------------------
class TDialogZeroConf;
class TFormMain : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
  TFrameLuaEditor *FrameLuaEditor;
  TPanel *Panel1;
  TPanel *Panel2;
  TFrameProject *FrameProject;
  TFrameLog *FrameLog;
  TSplitter *Splitter1;
  TMainMenu *MainMenu;
  TMenuItem *File1;
  TMenuItem *MenuItemProjectNew;
  TMenuItem *MenuItemExit;
  TMenuItem *N1;
  TFileOpenDialog *FileOpenDialog;
  TSplitter *Splitter2;
  TSplitter *Splitter3;
  TImage *Image1;
  TIdAntiFreeze *IdAntiFreeze;
  TSplitter *Splitter4;
  TIdUDPServer *IdUDPFFSServer;
  TIdUDPServer *IdUDPBackdoorServer;
  TMenuItem *Help1;
  TMenuItem *MenuItemEdit;
  TMenuItem *MenuItemUndo;
  TMenuItem *MenuItemRedo;
  TMenuItem *MenuItemCut;
  TMenuItem *MenuItemCopy;
  TMenuItem *MenuItemPaste;
  TMenuItem *N4;
  TMenuItem *ools1;
  TMenuItem *MenuItemOptions;
  TMenuItem *N2;
  TMenuItem *MenuItemSelectAll;
  TPanel *Panel3;
  TFrameTargets *FrameTargets;
  TSplitter *Splitter5;
  TFrameBrowser *FrameBrowser;
  TMenuItem *N3;
  TIdUDPServer *IdUDPMemoryTrackerServer;
  TMenuItem *MenuItemMemoryTracker;
  TMenuItem *Nodecommunicationtest1;
  TMenuItem *Programmer1;
  TPageControl *PageControl1;
  TTabSheet *TabSheet1;
  TTabSheet *TabSheet2;
  TFrameDesigner *FrameDesigner1;
  TTabSheet *TabSheet3;
  TFrameTopolgieEditor *FrameTopolgieEditor1;
  TIdTrivialFTPServer *IdTFTPServer;
  TMenuItem *MenuItemNogsWiki;
  TMenuItem *MenuItemNogsHomepage;
  TImageList *ImageListFileTypeSmall;
  TApplicationEvents *ApplicationEvents1;
  TImageList *ImageListNodeTypes;
  TIdIPAddrMon *IdIPAddrMon;
  TMenuItem *MenuItemExistingProject;
  void __fastcall FormCreate(TObject *Sender);
  void __fastcall MenuItemProjectNewClick(TObject *Sender);
  void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
  void __fastcall IdUDPFFSServerStatus(TObject *ASender, const TIdStatus AStatus, const UnicodeString AStatusText);
  void __fastcall MenuItemOptionsClick(TObject *Sender);
  void __fastcall MenuItemExitClick(TObject *Sender);
  void __fastcall MenuItemMemoryTrackerClick(TObject *Sender);
  void __fastcall Nodecommunicationtest1Click(TObject *Sender);
  void __fastcall Programmer1Click(TObject *Sender);
  void __fastcall IdTFTPServerReadFile(TObject *Sender, UnicodeString &FileName, const TPeerInfo &PeerInfo, bool &GrantAccess, TStream *&AStream, bool &FreeStreamOnComplete);
  void __fastcall IdTFTPServerStatus(TObject *ASender, const TIdStatus AStatus, const UnicodeString AStatusText);
  void __fastcall IdTFTPServerTransferComplete(TObject *Sender, const bool Success, const TPeerInfo &PeerInfo, TStream *&AStream, const bool WriteOperation);
  void __fastcall IdTFTPServerUDPException(TIdUDPListenerThread *AThread, TIdSocketHandle *ABinding, const UnicodeString AMessage, const TClass AExceptionClass);
  void __fastcall MenuItemNogsWikiClick(TObject *Sender);
  void __fastcall MenuItemNogsHomepageClick(TObject *Sender);
  void __fastcall MenuItemUndoClick(TObject *Sender);
  void __fastcall MenuItemRedoClick(TObject *Sender);
  void __fastcall MenuItemCutClick(TObject *Sender);
  void __fastcall MenuItemCopyClick(TObject *Sender);
  void __fastcall MenuItemPasteClick(TObject *Sender);
  void __fastcall MenuItemSelectAllClick(TObject *Sender);
  void __fastcall ApplicationEvents1Idle(TObject *Sender, bool &Done);
  void __fastcall IdIPAddrMonStatusChanged(TObject *ASender, int AAdapter, UnicodeString AOldIP, UnicodeString ANewIP);
  void __fastcall MenuItemExistingProjectClick(TObject *Sender);

private:	// Anwender-Deklarationen
  TStringList *FileExtensions;
  bool ZeroConf;

  void __fastcall IdUDPFFSServerOnUDPRead(TIdUDPListenerThread* Thread, DynamicArray<uchar> Data, Idsockethandle::TIdSocketHandle* Binding);
  void __fastcall IdUDPBackdoorServerOnUDPRead(TIdUDPListenerThread* Thread, DynamicArray<uchar> Data, Idsockethandle::TIdSocketHandle* Binding);
  void __fastcall IdUDPMemoryTrackerServerOnUDPRead(TIdUDPListenerThread* Thread, DynamicArray<uchar> Data, Idsockethandle::TIdSocketHandle* Binding);

public:
  netIP4 __fastcall HasZeroConfig();
  void __fastcall ShowZeronConfDialog();
  //void __fastcall CheckZeronConfIPRange();

public:		// Anwender-Deklarationen
  TZeroConfHelper ZeroConfHelper;
  TDialogModelessManager DialogModelessManager;

  __fastcall TFormMain(TComponent* Owner);
  void __fastcall LogClear()
  {
    FrameLog->LogClear();
  }
  TTreeNode* __fastcall Log(int Level, String S)
  {
    return FrameLog->Log(Level, S);
  }
  TTreeNode* __fastcall Log(TTreeNode* TreeNode, int Level, String S)
  {
    return FrameLog->Log(TreeNode, Level, S);
  }

  void __fastcall Backdoor(netIP4 IP, const String Command);
  void __fastcall Backdoor(TStringList *LiveCodingTargets, const String Command);

  class TUniqueId {
    uint32_t Id;
  public:
    String __fastcall Get()
    {
      return Id++;
    }
  } UniqueId;

  int __fastcall FileExtensionToImageIndex(const String FileExtension);

};
//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;
//---------------------------------------------------------------------------
#endif
