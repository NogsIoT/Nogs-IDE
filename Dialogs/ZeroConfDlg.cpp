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
#include "ZeroConfDlg.h"
#include "IPHelper.h"
#include "Busy.h"
#include "MainFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TDialogZeroConf::IdIPAddrMonStatusChanged(TObject *Sender, int Adapter, UnicodeString OldIP, UnicodeString NewIP)
{
  ListBox->Items->Assign(GStack->LocalAddresses);
  netIP4 IP2 = FormMain->HasZeroConfig();
  if (IP2 != IP) {
    IP = IP2;
    PaintBox->Invalidate();
    Panel0->Visible = !IP;
    Panel1->Visible = IP;
  }
}
//---------------------------------------------------------------------------
void __fastcall TDialogZeroConf::PaintBoxPaint(TObject *Sender)
{
  ImageList->Draw(PaintBox->Canvas, 0, 0, IP != 0);
}
//---------------------------------------------------------------------------
void __fastcall TDialogZeroConf::PaintBoxIPPaint(TObject *Sender)
{
  String S = IP.ToString();
  TCanvas *C = PaintBoxIP->Canvas;
  C->Brush->Color = RGB(51, 51, 51);
  C->FillRect(Bounds(0, 0, 32767, 32767));
  C->Font->Color = clLime;
  C->Font->Size = 40;
  C->TextOut(PaintBoxIP->Width / 2 - C->TextWidth(S) / 2, 5, S);
}
//---------------------------------------------------------------------------
void __fastcall TDialogZeroConf::Label1Click(TObject *Sender)
{
  ShellExecute(0, L"OPEN", L"http://en.wiki.nogs.cc/index.php/Zeroconf_auf_Windows_einrichten", L"", L"", SW_SHOWNORMAL);  
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
__fastcall TDialogZeroConf::TDialogZeroConf(TComponent* Owner)
  : TDialogModeless(Owner)
{
  IP = FormMain->HasZeroConfig();
  Panel0->Align = alClient;
  Panel0->Visible = !IP;
  Panel1->Align = alClient;
  Panel1->Visible = IP;
  Label2->Caption = 
    "This computer has a valid Zeroconf address. "
    "Be shure the Nogs Board is connected to "
    "this network segment.";
  Label3->Caption = 
    "This computer has no fixed "
    "IP address in the Zeroconf "
    "address area!\n"
    "Change or add an IP "
    "address in  the range from "
    "169.254.1.1 to "
    "169.254.255.255.";

}
//---------------------------------------------------------------------------
/*



*/                                                   


