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

#ifndef ToolUDPSenderDlgH
#define ToolUDPSenderDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdSocketHandle.hpp>
#include <IdUDPBase.hpp>
#include <IdUDPServer.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "Utils.h"
#include <IdUDPClient.hpp>
#include "UDP.h"
#include "Error2.h"
//---------------------------------------------------------------------------
class TDialogToolUDPSender : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
  TButton *ButtonSend;
  TLabeledEdit *EditPort;
  TLabeledEdit *EditContens;
  TLabeledEdit *EditIP;
  TIdUDPClient *IdUDPClient;
  void __fastcall ButtonSendClick(TObject *Sender);
private:	// Anwender-Deklarationen
public:		// Anwender-Deklarationen
  __fastcall TDialogToolUDPSender(TComponent* Owner);
};
//---------------------------------------------------------------------------
#endif
