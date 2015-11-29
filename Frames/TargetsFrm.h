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
#ifndef TargetsFrmH
#define TargetsFrmH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.AppEvnts.hpp>
#include <System.IniFiles.hpp>
#include <Vcl.Menus.hpp>
#include "NetTypes.h"
//---------------------------------------------------------------------------
class TPinger;
class TUDP;
class TJSONArray;
class TIndicator;
class TTarget;
class TFrameTargets : public TFrame
{
__published:
  TListView *ListView;
  TPanel *Panel5;
  TSpeedButton *ButtonDelete;
  TTimer *Timer1;
  TPanel *Panel1;
  TApplicationEvents *ApplicationEvents1;
  TSpeedButton *ButtonSettings;
  TPopupMenu *PopupMenu1;
  TMenuItem *MenuItemNodeCommunicationTest;
  TMenuItem *MenuItemPing;
  TImageList *ImageListStates;
  TTimer *Timer2;
  TMenuItem *MenuItemSettings;
  TMenuItem *MenuItemDelete;
  TMenuItem *N1;
  TMenuItem *N2;
  TPopupMenu *PopupMenu2;
  TMenuItem *Showbroadcastandmulticastaddresses1;
  TCheckBox *CheckBoxPing;
  void __fastcall ListViewCustomDrawItem(TCustomListView *Sender, TListItem *Item, TCustomDrawState State, bool &DefaultDraw);
  void __fastcall ListViewCustomDrawSubItem(TCustomListView *Sender, TListItem *Item, int SubItem, TCustomDrawState State, bool &DefaultDraw);
  void __fastcall Timer1Timer(TObject *Sender);
  void __fastcall ButtonDeleteClick(TObject *Sender);
  void __fastcall ApplicationEvents1Idle(TObject *Sender, bool &Done);
  void __fastcall ButtonSettingsClick(TObject *Sender);
  void __fastcall MenuItemNodeCommunicationTestClick(TObject *Sender);
  void __fastcall MenuItemPingClick(TObject *Sender);
  void __fastcall Timer2Timer(TObject *Sender);
  void __fastcall Showbroadcastandmulticastaddresses1Click(TObject *Sender);
  void __fastcall CheckBoxPingClick(TObject *Sender);
  void __fastcall ListViewClick(TObject *Sender);

private:
  bool NeedRefresh;
  TPinger *Pinger;
  TStringList *BroadcastIPs;
  TUDP *UDP;

  struct TJoin {
    netMAC MAC;
    netIP4 IP;
    netIP4 Mask;
  };
  DynamicArray<TJoin> Joins;


public: ///////////
  TUDP *UnitTestUDP;

public:
  __fastcall TFrameTargets(TComponent* Owner);

  void __fastcall Init();
  void __fastcall Finish();
  void __fastcall Set(const String Startup, const String Type, netIP4 IP, netIP4 Mask, netMAC MAC);
  void __fastcall Set(TJSONArray *JSONArray);

  void __fastcall SetIndicator(const String IP, TIndicator *Indicator);
  TTarget* __fastcall GetTarget(const String IP);
  String __fastcall GetSelectedTargetIP();
};
//---------------------------------------------------------------------------
#endif
