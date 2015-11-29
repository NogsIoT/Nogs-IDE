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

#ifndef LogFrmH
#define LogFrmH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include "SysLog.h"
#include <Vcl.ExtCtrls.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdSocketHandle.hpp>
#include <IdSysLogServer.hpp>
#include <IdUDPBase.hpp>
#include <IdUDPServer.hpp>
#include <Vcl.Menus.hpp>
//---------------------------------------------------------------------------
class TFrameLog : public TFrame
{
__published:	// Von der IDE verwaltete Komponenten
  TPageControl *PageControlLog;
  TTabSheet *TabSheet2;
  TTreeView *TreeViewLog;
  TSplitter *Splitter1;
  TIdSyslogServer *IdSyslogServer;
  TTabControl *TabControl;
  TPopupMenu *PopupMenu;
  TMenuItem *MenuItemClear;
  TMenuItem *MenuItemOpen;
  void __fastcall TreeViewLogCustomDrawItem(TCustomTreeView *Sender, TTreeNode *Node, TCustomDrawState State, bool &DefaultDraw);
  void __fastcall IdSyslogServerSyslog(TObject *Sender, TIdSysLogMessage *ASysLogMessage, TIdSocketHandle *ABinding);
  void __fastcall MenuItemClearClick(TObject *Sender);
  void __fastcall MenuItemOpenClick(TObject *Sender);
  void __fastcall PopupMenuPopup(TObject *Sender);
private:	// Anwender-Deklarationen
public:		// Anwender-Deklarationen
  __fastcall TFrameLog(TComponent* Owner);
  void __fastcall Init();
  void __fastcall LogClear();
  TTreeNode* __fastcall Log(int Level, String S);
  TTreeNode* __fastcall Log(TTreeNode* TreeNode, int Level, String S);
};
//---------------------------------------------------------------------------
#endif
