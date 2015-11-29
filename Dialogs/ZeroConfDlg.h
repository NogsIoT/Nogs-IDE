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
#ifndef ZeroConfDlgH
#define ZeroConfDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <IdBaseComponent.hpp>
#include <IdNetworkCalculator.hpp>
#include <IdComponent.hpp>
#include <IdIPAddrMon.hpp>
#include <IdIPWatch.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include "NetTypes.h"
#include "ModelessBaseDlg.h"
//---------------------------------------------------------------------------
class TIPHelper;
class TDialogZeroConf : public TDialogModeless
{
__published:	// Von der IDE verwaltete Komponenten
  TIdIPAddrMon *IdIPAddrMon;
  TPanel *Panel2;
  TPanel *Panel3;
  TImageList *ImageList;
  TPanel *Panel4;
  TPanel *Panel5;
  TShape *Shape1;
  TPaintBox *PaintBox;
  TTimer *Timer1;
  TPanel *Panel6;
  TPanel *Panel7;
  TPanel *Panel0;
  TListBox *ListBox;
  TPanel *Panel1;
  TPaintBox *PaintBoxIP;
  TLabel *Label1;
  TLabel *Label2;
  TPanel *Panel8;
  TLabel *Label3;
  void __fastcall PaintBoxPaint(TObject *Sender);
  void __fastcall IdIPAddrMonStatusChanged(TObject *ASender, int AAdapter, UnicodeString AOldIP, UnicodeString ANewIP);
  void __fastcall PaintBoxIPPaint(TObject *Sender);
  void __fastcall Label1Click(TObject *Sender);
private:	// Anwender-Deklarationen
  TIPHelper *IPHelper;
  netIP4 IP;
public:		// Anwender-Deklarationen
  __fastcall TDialogZeroConf(TComponent* Owner);
};
//---------------------------------------------------------------------------
#endif
