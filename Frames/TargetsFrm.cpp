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
#include "TargetsFrm.h"
#include "Usul.h"
#include "MainFrm.h"
#include "TargetSettingsDlg.h"
#include "CommunicationTestDlg.h"
#include "ICMPDLL.h"
#include "UDP.h"
#include "COAP.h"
#include "Error2.h"
#include "JSON.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//#include <System.Classes.hpp>
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
class TPinger : public TThread
{
  TStringList *IPs;
  void __fastcall TPinger::Execute()
  {
    for (int I = 0; I < IPs->Count; I++) {
      if (Terminated) break;
      uint32_t MessagedId = 0xBEEF0000 + I;
      IPs->Objects[I] = (TObject*) ((TICMPDLL::Get()->Ping(IPs->Strings[I], MessagedId, 100) == MessagedId) ? 1 : 3);
    }
  }
public:
  __fastcall TPinger::TPinger(TStringList *_IPs):
    TThread(false), IPs(_IPs)
  {
  }
  TStringList* __fastcall GetIPs()
  {
    return IPs;
  }
};
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TFrameTargets::Timer1Timer(TObject *Sender)
{
  for (int I = 0; I < ListView->Items->Count; I++) {
    TListItem *ListItem = ListView->Items->Item[I];
    if (TTarget::Get(ListItem)->GetIndicator()) {
      NeedRefresh = true;
      break;
    }
  }
  if (NeedRefresh) {
    for (int I = 0; I < ListView->Items->Count; I++) {
      TListItem *ListItem = ListView->Items->Item[I];
      TIndicator *Indicator = TTarget::Get(ListItem)->GetIndicator();
      if (Indicator) {
        Indicator->Step();
        if (Indicator->IsFinished()) {
          delete Indicator;
          TTarget::Get(ListItem)->SetIndicator(0);
        }
      }
    }
    ListView->Invalidate();
    NeedRefresh = false;
  }
  try {
    if (UDP->IsConnected()) {
      AnsiString Recv = UDP->Recv(0);
      if (Recv.Length()) {
        try {
          AnsiString RecvJSON = TCOAP::JSONUnwrap(Recv);
          if (RecvJSON != "[\"sys.info()\"]") {
            String Startup, IP, Mask, MAC;
            TJSONArray JSONArray(RecvJSON);
            Set(&JSONArray);
          }
        } catch (TJSONScanner::TError) {}
      }
    }
    if (UnitTestUDP && UnitTestUDP->IsConnected()) {
      AnsiString Recv = UnitTestUDP->Recv(0);
      if (Recv.Length()) {
        try {
          AnsiString RecvJSON = TCOAP::JSONUnwrap(Recv);
          if (RecvJSON != "[\"sys.info()\"]") {
            String Startup, IP, Mask, MAC;
            TJSONArray JSONArray(RecvJSON);
            Set(&JSONArray);
          }
        } catch (TJSONScanner::TError) {}
      }
    }
  } catch (...) {
    ShowMessage(2);
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameTargets::Timer2Timer(TObject *Sender)
{
  if (Pinger) {
    if (Pinger->Finished) {
      TStringList *IPs = Pinger->GetIPs();
      ListView->Items->BeginUpdate();
      for (int I = 0; I < ListView->Items->Count; I++)
        ListView->Items->Item[I]->StateIndex = -1;
      for (int I = 0; I < ListView->Items->Count; I++) {
        if (I >= IPs->Count) break;
        TListItem *ListItem = ListView->Items->Item[I];
        if (ListItem->SubItems->Strings[1] == IPs->Strings[I])
          ListItem->StateIndex = (int) IPs->Objects[I];
      }
      ListView->Items->EndUpdate();
      delete IPs;
      delete Pinger, Pinger = 0;
    }
  } else {
    TStringList *IPs = new TStringList;
    for (int I = 0; I < ListView->Items->Count; I++) {
      TListItem *ListItem = ListView->Items->Item[I];
      IPs->Add(ListItem->SubItems->Strings[1]);
    }
    Pinger = new TPinger(IPs);
  }

  if (UnitTestUDP) {
    if (UnitTestUDP->IsConnected())
      UnitTestUDP->Send(TCOAP::JSONWrap("[\"sys.info()\"]", true));
    else
      UnitTestUDP->Connect("127.0.0.1", TCOAP::Port + 50000, TUDP::Client);
  }


  try {
   static int I = -1;
    if (++I >= BroadcastIPs->Count) I = 0;
    String IP = BroadcastIPs->Strings[I];
    if (UDP->Connect(IP, TCOAP::Port, TUDP::Client))
      UDP->Send(TCOAP::JSONWrap("[\"sys.info()\"]", true));
////    if (UDP->IsConnected())
////      UDP->Send(TCOAP::JSONWrap("[\"sys.info()\"]", true));
//    for (int I = 0; I < BroadcastIPs->Count; I++) {
//      if (UDP->Connect(BroadcastIPs->Strings[I], TCOAP::Port, TUDP::Client))
//        UDP->Send(TCOAP::JSONWrap("[\"sys.info()\"]", true));
//    }
//
////    if (!UDP->IsConnected())
////      UDP->Connect(-1, TCOAP::Port, TUDP::Client);
////    if (UDP->IsConnected())
////      UDP->Send(TCOAP::JSONWrap("[\"sys.info()\"]", true));
  } catch (...) {
    ShowMessage(1);
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameTargets::ApplicationEvents1Idle(TObject *Sender, bool &Done)
{
  ButtonDelete->Enabled = ListView->Selected;
  ButtonSettings->Enabled = ListView->Selected;
  MenuItemSettings->Enabled = ListView->Selected;
  MenuItemNodeCommunicationTest->Enabled = ListView->Selected;
  MenuItemPing->Enabled = ListView->Selected;
  MenuItemDelete->Enabled = ListView->Selected;
}
//---------------------------------------------------------------------------
void __fastcall TFrameTargets::ButtonDeleteClick(TObject *Sender)
{
  ListView->Items->Delete(ListView->Selected->Index);
}
//---------------------------------------------------------------------------
void __fastcall TFrameTargets::ButtonSettingsClick(TObject *Sender)
{
  TDialogTargetSettings *DialogTargetSettings = 0;
  try {
    TListItem *ListItem = ListView->Selected;
    DialogTargetSettings = new TDialogTargetSettings(this, ListItem);
    DialogTargetSettings->ShowModal();
    String Section = "Target" + String(ListItem->Index);
    TTarget::Get(ListItem)->Save(Usul()->Ini, Section);
  }
  __finally {
    delete DialogTargetSettings;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameTargets::CheckBoxPingClick(TObject *Sender)
{
  Timer2->Enabled = CheckBoxPing->Checked;
  if (!Timer2->Enabled) 
    for (int I = 0; I < ListView->Items->Count; I++)
      ListView->Items->Item[I]->StateIndex = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFrameTargets::ListViewClick(TObject *Sender)
{
  TListItem *ListItem = ListView->Selected;
  if (ListItem) {
    netIP4 IP;
    IP(ListItem->SubItems->Strings[1]);
    FormMain->FrameBrowser->Set(IP, TTarget::Get(ListItem)->GetNodeType());
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameTargets::ListViewCustomDrawItem(TCustomListView *Sender, TListItem *ListItem, TCustomDrawState State, bool &DefaultDraw)
{
  //TRect Rect = ListItem->DisplayRect(drBounds); //type TDisplayCode = (drBounds, drIcon, drLabel, drSelectBounds)
  TListView *ListView = dynamic_cast<TListView*>(Sender);
  TIndicator *Indicator = TTarget::Get(ListItem)->GetIndicator();
  if (Indicator) {
    ListItem->Caption = Indicator->GetText();
    ListView->Canvas->Font->Color = Indicator->GetFgColor();
    ListView->Canvas->Brush->Color = Indicator->GetBgColor();
  } else {
    ListItem->Caption = EmptyStr;
    ListView->Canvas->Font->Color = clWhite;
    ListView->Canvas->Brush->Color = RGB(25, 25, 25);
  }
  DefaultDraw = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrameTargets::ListViewCustomDrawSubItem(TCustomListView *Sender, TListItem *ListItem, int SubItem, TCustomDrawState State, bool &DefaultDraw)
{
  TListView *ListView = dynamic_cast<TListView*>(Sender);
  ListView->Canvas->Brush->Color = RGB(25, 25, 25);
  ListView->Canvas->Font->Color = clWhite;
  DefaultDraw = true;
}
//---------------------------------------------------------------------------
void __fastcall TFrameTargets::MenuItemNodeCommunicationTestClick(TObject *Sender)
{
  if (FormMain->DialogModelessManager.NeedCreation("TDialogCommunicationTest"))
    FormMain->DialogModelessManager.Register("TDialogCommunicationTest", new TDialogCommunicationTest(this));
  TDialogCommunicationTest *DialogCommunicationTest = (TDialogCommunicationTest*) FormMain->DialogModelessManager.Get("TDialogCommunicationTest");
  DialogCommunicationTest->Set(ListView->Selected->SubItems->Strings[1]);
}
//---------------------------------------------------------------------------
void __fastcall TFrameTargets::MenuItemPingClick(TObject *Sender)
{
  TListItem *ListItem = ListView->Selected;
  String IP = ListItem->SubItems->Strings[1];
  TTreeNode *TreeNode = FormMain->Log(0, "Pinging " + IP);
  if (TICMPDLL::Get()->Ping(IP)) {
    FormMain->Log(TreeNode, 1, "Success");
    ListItem->StateIndex = 1;
  } else {
    FormMain->Log(TreeNode, 3, "Fail");
    ListItem->StateIndex = 3;
  }
  TreeNode->Expand(false);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
__fastcall TFrameTargets::TFrameTargets(TComponent* Owner)
  : TFrame(Owner), Pinger(0), UDP(0), UnitTestUDP(0)
{
  BroadcastIPs = new TStringList;
  BroadcastIPs->Sorted = true;
  BroadcastIPs->Duplicates = System::Classes::dupIgnore;
//  char namebuf[256];
//  gethostname(namebuf,sizeof(namebuf));
//  PHostEnt varPHostEnt = gethostbyname(namebuf);
//  for (int I = 0; I < varPHostEnt->h_length; I++) {
//    uint32_t IP = *((uint32_t*) varPHostEnt->h_addr_list[I]);
//    int A = IP & 0xFF;
//    // Klasse A: 255.0.0.0 1-127
//    // Klasse B: 255.255.0.0 128-191
//    // Klasse C: 255.255.255.0 192-224
//    if ((A >= 1) && (A <= 127))
//      BroadcastIPs->Add("255.255.255.0");
//    else if ((A >= 128) && (A <= 191))
//      BroadcastIPs->Add("255.255.0.0");
//    else if ((A >= 192) && (A <= 224))
//      BroadcastIPs->Add("255.0.0.0");
//  }
  BroadcastIPs->Add("255.255.255.255");
  //BroadcastIPs->Add("127.0.0.1");
  //BroadcastIPs->Add("169.254.0.0");

  UnitTestUDP = new TUDP;
}
//---------------------------------------------------------------------------
void __fastcall TFrameTargets::Init()
{
  try {
    ListView->Items->BeginUpdate();
    TIniFile *Ini = Usul()->Ini;
    int N = Ini->ReadInteger("Target", "Count", 0);
    for (int I = 0; I < N; I++) {
      String Section = "Target" + String(I);
      TListItem *ListItem = ListView->Items->Add();
      //ListItem->Caption = "Test";
      ListItem->SubItems->Add(Ini->ReadString(Section, "MAC", EmptyStr));
      ListItem->SubItems->Add(Ini->ReadString(Section, "IP", EmptyStr));
      ListItem->SubItems->Add(Ini->ReadString(Section, "Mask", EmptyStr));
      ListItem->SubItems->Add(Ini->ReadString(Section, "Name", EmptyStr));
      ListItem->Data = new TTarget(ListItem, Ini, Section);
      ListItem->ImageIndex = TTarget::Get(ListItem)->GetNodeType();
    }
    if (ListView->Items->Count) {
      TListItem *ListItem = ListView->Items->Item[0];
      netIP4 IP;
      IP(ListItem->SubItems->Strings[1]);
      FormMain->FrameBrowser->Set(IP, TTarget::Get(ListItem)->GetNodeType());
    }
    UDP = new TUDP;
  }
  __finally {
    ListView->Items->EndUpdate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameTargets::Finish()
{
  TIniFile *Ini = Usul()->Ini;
  int N = ListView->Items->Count;
  Ini->WriteInteger("Target", "Count", N);
  for (int I = 0; I < N; I++) {
    String Section = "Target" + String(I);
    TListItem *ListItem = ListView->Items->Item[I];
    Ini->WriteString(Section, "MAC", ListItem->SubItems->Strings[0]);
    Ini->WriteString(Section, "IP", ListItem->SubItems->Strings[1]);
    Ini->WriteString(Section, "Mask", ListItem->SubItems->Strings[2]);
    Ini->WriteString(Section, "Name", ListItem->SubItems->Strings[3]);
    TTarget::Get(ListItem)->Save(Ini, Section);
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameTargets::Set(const String Startup, const String Type, netIP4 IP, netIP4 Mask, netMAC MAC)
{
  try {
    ListView->Items->BeginUpdate();
    for (int I = 0; I < ListView->Items->Count; I++) {
      TListItem *ListItem = ListView->Items->Item[I];
      if (ListItem->SubItems->Strings[0] == MAC.ToString()) {
        ListItem->SubItems->Strings[1] = IP.ToString();
        ListItem->SubItems->Strings[2] = Mask.ToString();
        ListItem->SubItems->Strings[3] = Startup;
        TTarget::Get(ListItem)->SetLastUsage();
        ListItem->ImageIndex = TTarget::Get(ListItem)->GetNodeType();
        return;
      }
    }
    TListItem *ListItem = ListView->Items->Add();
    ListItem->SubItems->Add(MAC.ToString());
    ListItem->SubItems->Add(IP.ToString());
    ListItem->SubItems->Add(Mask.ToString());
    ListItem->SubItems->Add(Startup);
    ListItem->Data = new TTarget(ListItem, Startup, Type);
    ListItem->ImageIndex = TTarget::Get(ListItem)->GetNodeType();
  }
  __finally {
    ListView->Items->EndUpdate();
    NeedRefresh = true;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameTargets::Set(TJSONArray *JSONArray)
{
  Joins.Length = 0;
  try {
    String Startup, Type;
    netIP4 IP, Mask;
    netMAC MAC;
    for (int I = 0;I < JSONArray->Length();I++) {
      TJSONObject *JSONObject = JSONArray->GetJSONObject(I);
      String Name = JSONObject->GetName(0).LowerCase();
      if ((Name == "startup") || (Name == "name"))      // TODO name entfernen
        Startup = JSONObject->GetString(0);
      else if (Name == "type")
        Type = JSONObject->GetString(0);
      else if (Name == "ip")
        IP(JSONObject->GetString(0));
      else if (Name == "mask")
        Mask(JSONObject->GetString(0));
      else if (Name == "mac")
        MAC(JSONObject->GetString(0));
    }
    if (Type.IsEmpty()) {
      if (Startup.Pos("PN")) Type = "PN";
      if (Startup.Pos("CN")) Type = "CN";
      if (Startup.Pos("SN")) Type = "SN";
    }
    if (!Joins.Length)
      Set(Startup, Type, IP, Mask, MAC);
    TJoin Join;
    Join.MAC = MAC;
    Join.IP = IP;
    Join.Mask = Mask;
    Joins[Joins.Length++] = Join;
    if (MAC.ToString()[1] != 'F')
      FormMain->Log(0, "--> " + MAC.ToString() + " " + IP.ToString() + " " + Mask.ToString());
  }
  catch (TJSONScanner::TError &E) {}
}
//---------------------------------------------------------------------------
void __fastcall TFrameTargets::SetIndicator(const String IP, TIndicator *Indicator)
{
  for (int I = 0; I < ListView->Items->Count; I++) {
    TListItem *ListItem = ListView->Items->Item[I];
    if (ListItem->SubItems->Strings[1] == IP) {
      TIndicator *Indicator2 = (TIndicator*) ListView->Tag;
      if (Indicator2) delete Indicator2;
      TTarget::Get(ListItem)->SetIndicator(Indicator);
      break;
    }
  }
}
//---------------------------------------------------------------------------
TTarget* __fastcall TFrameTargets::GetTarget(const String IP)
{
  for (int I = 0; I < ListView->Items->Count; I++) {
    TListItem *ListItem = ListView->Items->Item[I];
    if (ListItem->SubItems->Strings[1] == IP)
      return (TTarget*) ListItem->Data;
  }
  return 0;
}
//---------------------------------------------------------------------------
String __fastcall TFrameTargets::GetSelectedTargetIP()
{
  TListItem *ListItem = ListView->Selected;
  return ListItem ? ListItem->SubItems->Strings[1] : EmptyStr;
}
//---------------------------------------------------------------------------
void __fastcall TFrameTargets::Showbroadcastandmulticastaddresses1Click(TObject *Sender)
{
  TTreeNode *TreeNode = FormMain->Log(0, "List of broadcast and multicast addresses:");
  for (int I = 0; I < BroadcastIPs->Count; I++)
    FormMain->Log(TreeNode, 0, BroadcastIPs->Strings[I]);
  TreeNode->Expand(false);
}
//---------------------------------------------------------------------------



