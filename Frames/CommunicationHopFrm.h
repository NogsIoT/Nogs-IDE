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
#ifndef CommunicationHopFrmH
#define CommunicationHopFrmH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "Mutator.h"
//---------------------------------------------------------------------------
class TDialogCommunicationTest;
class TFrameCommunicationHop : public TFrame
{
__published:	// Von der IDE verwaltete Komponenten
  TPageControl *PageControl;
  TTabSheet *TabSheet1;
  TTabSheet *TabSheet3;
  TTabSheet *TabSheet4;
  TLabeledEdit *EditCOAPPort;
  TLabeledEdit *EditCOAPIP;
  TButton *ButtonCOAPPing;
  TTabSheet *TabSheet2;
  TPanel *PanelNumber;
  TSplitter *Splitter1;
  TLabeledEdit *EditUDPIP;
  TLabeledEdit *EditUDPPort;
  TLabeledEdit *EditTCPIP;
  TLabeledEdit *EditTCPPort;
  TListBox *ListBoxSerial;
  void __fastcall PageControlChange(TObject *Sender);
  void __fastcall ButtonCOAPPingClick(TObject *Sender);
private:	// Anwender-Deklarationen
  TDialogCommunicationTest *DialogCommunicationTest;
  TTabSheet *TabSheet;
public:		// Anwender-Deklarationen
  __fastcall TFrameCommunicationHop(TComponent* Owner);
  void __fastcall Init(TDialogCommunicationTest *_DialogCommunicationTest, TTabSheet *_TabSheet);

  TMutator::TPacket* __fastcall Wrap(TMutator::TPacket *Packet);
  TMutator::TPacket* __fastcall Unwrap(TMutator::TPacket *Packet);

  void __fastcall Set(const String IP);

  TMutator::TPacket* __fastcall Transmit(TMutator::TPacket *Packet);
};
//---------------------------------------------------------------------------
#endif
