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
#ifndef CommunicationTestDlgH
#define CommunicationTestDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Menus.hpp>
#include "ModelessBaseDlg.h"
#include "CommunicationHopFrm.h"
//---------------------------------------------------------------------------
class TDialogCommunicationTest : public TDialogModeless
{
__published:	// Von der IDE verwaltete Komponenten
  TPanel *Panel3;
  TPanel *Panel2;
  TPageControl *PageControl2;
  TTabSheet *TabSheet5;
  TListBox *ListBoxQueryList;
  TTabSheet *TabSheet2;
  TButton *Button17;
  TButton *Button18;
  TButton *Button15;
  TButton *Button2;
  TButton *Button10;
  TButton *Button3;
  TButton *Button16;
  TTabSheet *TabSheet7;
  TButton *Button7;
  TTabSheet *TabSheet6;
  TButton *Button9;
  TPanel *Panel5;
  TButton *ButtonSendCON;
  TButton *ButtonSendNON;
  TLabeledEdit *EditSend;
  TPanel *Panel4;
  TPanel *Panel6;
  TPageControl *PageControlHops;
  TTabSheet *TabSheet0;
  TFrameCommunicationHop *FrameCommunicationHop0;
  TTabSheet *TabSheet3;
  TPopupMenu *PopupMenu1;
  TMenuItem *Appendnewhop1;
  TMenuItem *Deletehop1;
  TSplitter *Splitter1;
  void __fastcall ButtonXClick(TObject *Sender);
  void __fastcall ListBoxQueryListClick(TObject *Sender);
  void __fastcall ButtonSendCONClick(TObject *Sender);
  void __fastcall ButtonSendNONClick(TObject *Sender);
  void __fastcall PageControlHopsChange(TObject *Sender);


private:	// Anwender-Deklarationen
  void __fastcall RethinkTabs();
public:		// Anwender-Deklarationen
  __fastcall TDialogCommunicationTest(TComponent* Owner);
  virtual void __fastcall Init();

  void __fastcall ChangeTabCaption(TTabSheet *TabSheet, const String Caption);
  void __fastcall Set(const String IP);
};
//---------------------------------------------------------------------------
#endif
