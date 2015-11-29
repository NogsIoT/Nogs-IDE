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

#ifndef SysLogH
#define SysLogH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdSocketHandle.hpp>
#include <IdSysLogServer.hpp>
#include <IdUDPBase.hpp>
#include <IdUDPServer.hpp>
#include <Vcl.ComCtrls.hpp>
#include <System.IniFiles.hpp>
#include <Vcl.Menus.hpp>
#include "cgauges.h"
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFacility;
class TFrameSysLog : public TFrame
{
__published:	// Von der IDE verwaltete Komponenten
  TListView *ListView;
  TPopupMenu *PopupMenu;
  TMenuItem *MenuItemSettings;
  TMenuItem *MenuItemClear;
  TMenuItem *N1;
  TPanel *PanelGauge;
  TCGauge *CGauge;
  void __fastcall ListViewCustomDrawItem(TCustomListView *Sender, TListItem *ListItem, TCustomDrawState State, bool &DefaultDraw);
  void __fastcall ListViewCustomDrawSubItem(TCustomListView *Sender, TListItem *ListItem, int SubItem, TCustomDrawState State, bool &DefaultDraw);
  void __fastcall MenuItemSettingsClick(TObject *Sender);
  void __fastcall MenuItemClearClick(TObject *Sender);
private:	// Anwender-Deklarationen
  TFacility *Facility;
  String ClearOnText;
public:		// Anwender-Deklarationen
  __fastcall TFrameSysLog(TComponent* Owner);
  void __fastcall Init();
  void __fastcall Syslog(TIdSysLogMessage *SysLogMessage,  TIdSocketHandle *Binding);
};
//---------------------------------------------------------------------------
#endif
