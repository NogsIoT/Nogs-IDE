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

#ifndef NodeCommunicationTestDlgH
#define NodeCommunicationTestDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "MainFrm.h"
#include "CommunicationHopFrm.h"
#include <Vcl.Menus.hpp>
//---------------------------------------------------------------------------
class TDialogNodeCommunicationTest : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
  TSplitter *Splitter1;
  TPanel *Panel3;
  TPanel *Panel1;
  TPageControl *PageControl2;
  TTabSheet *TabSheet5;
  TListBox *ListBoxQueryList;
  TTabSheet *TabSheet2;
  TButton *Button17;
  TButton *Button18;
  TButton *Button15;
  TButton *Button2;
  TButton *Button10;
  TButton *Button1;
  TButton *Button16;
  TTabSheet *TabSheet7;
  TButton *Button7;
  TTabSheet *TabSheet6;
  TButton *Button9;
  TPanel *Panel4;
  TPanel *Panel2;
  TPageControl *PageControlHops;
  TTabSheet *TabSheet0;
  TTabSheet *TabSheet3;
  TPanel *Panel5;
  TButton *ButtonSendCON;
  TButton *ButtonSendNON;
  TLabeledEdit *EditSend;
  TFrameCommunicationHop *FrameCommunicationHop0;
  TPopupMenu *PopupMenu1;
  TMenuItem *Appendnewhop1;
  TMenuItem *Deletehop1;
  void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
  void __fastcall ButtonXClick(TObject *Sender);
  void __fastcall ButtonSendCONClick(TObject *Sender);
  void __fastcall ListBoxQueryListClick(TObject *Sender);
  void __fastcall PageControlHopsChange(TObject *Sender);
  void __fastcall Appendnewhop1Click(TObject *Sender);
  void __fastcall Deletehop1Click(TObject *Sender);
  void __fastcall ButtonSendNONClick(TObject *Sender);

private:	// Anwender-Deklarationen
public:		// Anwender-Deklarationen
  __fastcall TDialogNodeCommunicationTest(TComponent* Owner);
};
//---------------------------------------------------------------------------
#endif
