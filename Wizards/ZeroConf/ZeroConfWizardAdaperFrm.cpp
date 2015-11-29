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
#include "ZeroConfWizardAdaperFrm.h"
#include "NetTypes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
#define BOLD      (1 << 8)
#define UNDERLINE (1 << 9)
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TFrameZeroConfWizardAdapter::TreeViewCustomDrawItem(TCustomTreeView *Sender, TTreeNode *TreeNode, TCustomDrawState State, bool &DefaultDraw)
{
  //TColor Colors[] = {clWhite, clSkyBlue, clMoneyGreen, clYellow, (TColor) RGB(255, 128, 0)};
  TColor Colors[] = {clWhite, clLime, clYellow, (TColor) RGB(255, 128, 64), clAqua, clFuchsia};
  TCanvas *C = TreeView->Canvas;
  C->Brush->Color = (TColor) RGB(25, 25, 25);
  int Data = (int) TreeNode->Data;
  C->Font->Color = Colors[Data & 0xFF];
  if (Data & BOLD)
    C->Font->Style = C->Font->Style << fsBold;
  if (Data & UNDERLINE)
    C->Font->Style = C->Font->Style << fsUnderline;
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
__fastcall TFrameZeroConfWizardAdapter::TFrameZeroConfWizardAdapter(TComponent* Owner, TIPHelper::TIPAdapterInfo::TItem *_Item, int _Index)
  : TFrame(Owner), Init(true), Item(_Item), Index(_Index)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrameZeroConfWizardAdapter::Refresh()
{
  if (Init) {
    Init = false;
    Text(0 + BOLD + UNDERLINE, Item->GetDescription());

    netIP4 PrimaryIP;
    {
      hostent *remoteHost = gethostbyname(0);
      if (remoteHost) 
        PrimaryIP(SWAP32(*((uint32_t*) remoteHost->h_addr_list[0])));
    }
    bool PrimaryAdapter = false;
    netIP4 IP, Mask;
    int Count = 0;
    if (Item->GetFirstIPAndMask(IP, Mask)) {
      do {
        if (IP != netIP4::Null()) {
          Count++;
          //TIPHelper::TIPNetTable::TItem::TType Type = Item->GetType();
          if ((IP[3] == 169) && (IP[2] == 254)) {
            Text(1, "  " + IP.ToString() + " (ZeroConf)");
            Shape->Brush->Color = clLime;
          } else 
            Text(0, "  " + IP.ToString());
          if (IP == PrimaryIP) 
            PrimaryAdapter = true;
        }
      } while (Item->GetNextIPAndMask(IP, Mask));
    } 
    if (!Count) 
      Text(2, "  No assigned IP address.");
    if (PrimaryAdapter)
      Text(0, "This is the primary network adapter.");
    if (Item->HasDHCP()) {
      Text(3 + BOLD, "DHCP is enabled.");
      Text(3, "  Under Windows it is not possible to mix static IP addresses with DHCP. So, this adaper is not usable for ZeroConf.");
      Shape->Brush->Color = clRed;
    }
  }
}
//---------------------------------------------------------------------------


