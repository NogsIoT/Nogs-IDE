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
#include "CommunicationTestDlg.h"
#include "UDP.h"
#include "COAP.h"
#include "Error2.h"
#include "Utils.h"
#include "MainFrm.h"
#pragma package(smart_init)
#pragma link "ModelessBaseDlg"
#pragma link "CommunicationHopFrm"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TDialogCommunicationTest::RethinkTabs()
{
  int N = PageControlHops->PageCount - 1;
  for (int I = 0; I < N; I++) {
    String S = PageControlHops->Pages[I]->Caption;
    int P = S.Pos("[");
    S.Delete(1, P - 1);
    P = S.Pos("]");
    S.SetLength(P);
    if (N == 1)
      S = L"(●)―――" + S + L"―――(●)";
    else
      S = (I ? L"――" : L"(●)―――") + S + (I == N - 1 ? L"―――(●)" : L"――");
    PageControlHops->Pages[I]->Caption = S;
  }
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TDialogCommunicationTest::ButtonXClick(TObject *Sender)
{
  TButton *Button = dynamic_cast<TButton*>(Sender);
  EditSend->Text = "[\"" + Button->Caption + "\"]";
  ButtonSendCONClick(0);
}
//---------------------------------------------------------------------------
void __fastcall TDialogCommunicationTest::ListBoxQueryListClick(TObject *Sender)
{
  int Index = ListBoxQueryList->ItemIndex;
  if (Index >= 0)
    EditSend->Text = "[\"" + ListBoxQueryList->Items->Strings[Index] + "\"]";
}
//---------------------------------------------------------------------------
void __fastcall TDialogCommunicationTest::ButtonSendCONClick(TObject *Sender)
{
  TTreeNode *TreeNode = FormMain->Log(0, "CON");
  try {
    TMutator::TPacket *Send = new TMutator::TPacket(EditSend->Text, true);
    FormMain->Log(TreeNode, 0, Send->GetData());
    for (int I = PageControlHops->PageCount - 2; I >= 0; I--) {
      TFrameCommunicationHop *FrameCommunicationHop = (TFrameCommunicationHop*) PageControlHops->Pages[I]->Tag;
      Send = FrameCommunicationHop->Wrap(Send);
    }
    TFrameCommunicationHop *FrameCommunicationHop = (TFrameCommunicationHop*) PageControlHops->Pages[0]->Tag;
    TMutator::TPacket *Recv = FrameCommunicationHop->Transmit(Send);
    for (int I = 0; I < PageControlHops->PageCount - 1; I++) {
      TFrameCommunicationHop *FrameCommunicationHop = (TFrameCommunicationHop*) PageControlHops->Pages[I]->Tag;
      Recv = FrameCommunicationHop->Unwrap(Recv);
    }
    FormMain->Log(TreeNode, 1, Recv->GetData());
    delete Recv;
    TreeNode->Expand(true);
  }
  catch (TError &E) {
    FormMain->Log(TreeNode, 3, E.GetString());
    E.Show();
  }
}
//---------------------------------------------------------------------------
void __fastcall TDialogCommunicationTest::ButtonSendNONClick(TObject *Sender)
{
  TTreeNode *TreeNode = FormMain->Log(0, "NON");
  try {
    TMutator::TPacket *Send = new TMutator::TPacket(EditSend->Text, true);
    FormMain->Log(TreeNode, 0, Send->GetData());
    for (int I = PageControlHops->PageCount - 2; I >= 0; I--) {
      TFrameCommunicationHop *FrameCommunicationHop = (TFrameCommunicationHop*) PageControlHops->Pages[I]->Tag;
      Send = FrameCommunicationHop->Wrap(Send);
    }
    TFrameCommunicationHop *FrameCommunicationHop = (TFrameCommunicationHop*) PageControlHops->Pages[0]->Tag;
    FrameCommunicationHop->Transmit(Send);
    TreeNode->Expand(true);
  }
  catch (TError &E) {
    FormMain->Log(TreeNode, 3, E.GetString());
    E.Show();
  }
}
//---------------------------------------------------------------------------
void __fastcall TDialogCommunicationTest::PageControlHopsChange(TObject *Sender)
{
  if (PageControlHops->ActivePageIndex == PageControlHops->PageCount - 1) {
    {
      static int Unique = 0;
      TTabSheet *TabSheet = PageControlHops->ActivePage;
      TFrameCommunicationHop *FrameCommunicationHop = new TFrameCommunicationHop(this);
      FrameCommunicationHop->Name = "TabSheet" + ++Unique;
      FrameCommunicationHop->Parent = TabSheet;
      FrameCommunicationHop->Align = alClient;
      FrameCommunicationHop->Init(this, TabSheet);
    }
    {
      TTabSheet *TabSheet = new TTabSheet(this);
      TabSheet->PageControl = PageControlHops;
      TabSheet->Caption = "<new hop>";
    }
    PageControlHops->ActivePageIndex == PageControlHops->PageCount - 2;
    RethinkTabs();
  }
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
__fastcall TDialogCommunicationTest::TDialogCommunicationTest(TComponent* Owner)
  : TDialogModeless(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TDialogCommunicationTest::Init()
{
  FrameCommunicationHop0->Init(this, TabSheet0);
  PageControlHops->ActivePageIndex = 0;
}
//---------------------------------------------------------------------------
void __fastcall TDialogCommunicationTest::ChangeTabCaption(TTabSheet *TabSheet, const String Caption)
{
  TabSheet->Caption = "[" + Caption + "]";
  RethinkTabs();
}
//---------------------------------------------------------------------------
void __fastcall TDialogCommunicationTest::Set(const String IP)
{
  FrameCommunicationHop0->Set(IP);
}
//---------------------------------------------------------------------------







