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

#include <vcl.h>
#pragma hdrstop

#include "ToolUDPSenderDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"



//---------------------------------------------------------------------------
__fastcall TDialogToolUDPSender::TDialogToolUDPSender(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TDialogToolUDPSender::ButtonSendClick(TObject *Sender)
{
/* */
  try {
    TUDP UDP;
    if (!UDP.Connect(EditIP->Text, TUtils::AToI(EditPort->Text), TUDP::Server))
      throw TError("Can't do UDP.Connect");
    UDP.Send(EditContens->Text);
  }
  catch (TError &E)
  {
    E.Show();
  }
/**/
/**/
  DynamicArray<unsigned char> Data;
  Data.Length = EditContens->Text.Length();
  for (int I = 0; I < Data.Length; I++) Data[I] = EditContens->Text[I + 1];
  IdUDPClient->Active = true;
  //IdUDPClient->SendBuffer(EditIP->Text, TUtils::AToI(EditPort->Text), Data);
  IdUDPClient->Host = EditIP->Text;
  IdUDPClient->Port = TUtils::AToI(EditPort->Text);
  IdUDPClient->SendBuffer(Data);
  //IdUDPClient->Active = false;
/**/
}
//---------------------------------------------------------------------------
