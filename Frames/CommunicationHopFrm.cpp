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
#include "CommunicationHopFrm.h"
#include "Error2.h"
#include "Utils.h"
#include "MutatorCOAPJSON.h"
#include "UDP.h"
#include "MainFrm.h"
#include "COAP.h"
#include "CommunicationTestDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TFrameCommunicationHop::PageControlChange(TObject *Sender)
{
  DialogCommunicationTest->ChangeTabCaption(TabSheet, PageControl->ActivePage->Caption);
}
//---------------------------------------------------------------------------
TMutator::TPacket* __fastcall TFrameCommunicationHop::Wrap(TMutator::TPacket *Packet)
{
  switch (PageControl->ActivePageIndex) {
    case 3 : return TMutatorCOAPJSON::Wrap(Packet);
  }
  throw Error0("Not implemented wrap");
}
//---------------------------------------------------------------------------
TMutator::TPacket* __fastcall TFrameCommunicationHop::Unwrap(TMutator::TPacket *Packet)
{
  switch (PageControl->ActivePageIndex) {
    case 3 : return TMutatorCOAPJSON::Unwrap(Packet);
  }
  throw Error0("Not implemented unwrap");
}
//---------------------------------------------------------------------------
TMutator::TPacket* __fastcall TFrameCommunicationHop::Transmit(TMutator::TPacket *Packet)
{
  TUDP UDP;
  if (!UDP.Connect(EditCOAPIP->Text, TUtils::AToI(EditCOAPPort->Text), TUDP::Client))
    throw Error0("Can't connect");
  UDP.Send(Packet->GetData());
  if (Packet->GetConfirm()) {
    Packet->SetData(UDP.Recv(500));
    if (Packet->GetData().IsEmpty())
      throw Error0("Timeout");
    return Packet;
  }
  delete Packet;
  return 0;
}
//---------------------------------------------------------------------------
void __fastcall TFrameCommunicationHop::ButtonCOAPPingClick(TObject *Sender)
{
  TTreeNode *TreeNode = FormMain->Log(0, "COAP.Ping");
  try {
    TUDP UDP;
    if (!UDP.Connect(EditCOAPIP->Text, TUtils::AToI(EditCOAPPort->Text), TUDP::Client))
      throw Error0("UDP connect fails");
    TCOAP COAP;
    TCOAP::TPingResult Result = COAP.Ping(&UDP, 1000);
    if ((int) Result >= 1)
      FormMain->Log(TreeNode, 1, "Ok, CoAP version " + String((int) Result));
    else
      switch (Result) {
        case TCOAP::NoAnswer :
          FormMain->Log(TreeNode, 3, "No answer");
          break;
        case TCOAP::WrongAnswer :
          FormMain->Log(TreeNode, 3, "Wrong answer");
          break;
        default :
          FormMain->Log(TreeNode, 3, "Unknown error");
      }
    TreeNode->Expand(true);
  }
  catch (TError &E) {
    FormMain->Log(TreeNode, 3, E.GetString());
    E.Show();
  }
}
/*
  TUDP UDP;
  UDP.Connect(TUDP::StringToIP("255.255.255.255"),COAP_PORT,TUDP::Client);
  TCOAP COAP(UDP);
  AnsiString Recv = COAP.SendJSON("[\"sys.info()\"]",TCOAP::CON);
  for (;;) {
    String S = COAP.RecvJSON();
    if (S.IsEmpty()) break;
    TNogsNodeEthernetCOAP *NogsNodeEthernetCOAP = new TNogsNodeEthernetCOAP(this,EmptyStr,new TJSONArray(S));
    Add(NogsNodeEthernetCOAP);
    NogsNodeEthernetCOAP->ToListItem(ListView);
  }
*/
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
__fastcall TFrameCommunicationHop::TFrameCommunicationHop(TComponent* Owner)
  : TFrame(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrameCommunicationHop::Set(const String IP)
{
  EditUDPIP->Text = IP;
  EditTCPIP->Text = IP;
  EditCOAPIP->Text = IP;
}
//---------------------------------------------------------------------------
void __fastcall TFrameCommunicationHop::Init(TDialogCommunicationTest *_DialogCommunicationTest, TTabSheet *_TabSheet)
{
  DialogCommunicationTest = _DialogCommunicationTest;
  TabSheet = _TabSheet;
  TabSheet->Tag = (int) this;
  PageControlChange(0);
  PanelNumber->Caption = TabSheet->PageIndex;
  bool Self = TabSheet->PageIndex == 0;
  ButtonCOAPPing->Visible = Self;
}
//---------------------------------------------------------------------------

