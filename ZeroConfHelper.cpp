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
#pragma hdrstop
#include "ZeroConfHelper.h"
#include "IPHelper.h"
//---------------------------------------------------------------------------
__fastcall TZeroConfHelper::TZeroConfHelper()
{
  try {
    IPHelper = new TIPHelper;
  }
  catch (TIPHelper::TError E)
  {
    delete IPHelper, IPHelper = 0;
  }
}
//---------------------------------------------------------------------------
__fastcall TZeroConfHelper::~TZeroConfHelper()
{
  delete IPHelper;
}
//---------------------------------------------------------------------------
TStringList* __fastcall TZeroConfHelper::GetAdapterWithZeroConf()
{
  TStringList *Adapter = new TStringList;

  if (!IPHelper) return Adapter;
  TIPHelper::TIPAdapterInfo *IPAdapterInfo = IPHelper->GetIPAdapterInfo();
  for (int I = 0; I < IPAdapterInfo->GetCount(); I++) {
    TIPHelper::TIPAdapterInfo::TItem *Item = (*IPAdapterInfo)[I];
    netIP4 IP, Mask;
    if (Item->GetFirstIPAndMask(IP, Mask)) {
      do {
        if ((IP[3] == 169) && (IP[2] == 254))
          Adapter->Add(Item->GetDescription());
      } while (Item->GetNextIPAndMask(IP, Mask));
    }
  }
  return Adapter;
}
//---------------------------------------------------------------------------
void __fastcall TZeroConfHelper::AdapterAndIPsToTreeView(TTreeView *TreeView)
{
  try {
    TreeView->Items->BeginUpdate();
    TreeView->Items->Clear();
    TTreeNode *TreeNodeRoot = TreeView->Items->Add(0, "Network");
    try {
      IPHelper = new TIPHelper;
    }
    catch (TIPHelper::TError E)
    {
      delete IPHelper;
    }
    if (IPHelper) {
      TIPHelper::TIPAdapterInfo *IPAdapterInfo = IPHelper->GetIPAdapterInfo();
      for (int I = 0; I < IPAdapterInfo->GetCount(); I++) {
        TIPHelper::TIPAdapterInfo::TItem *Item = (*IPAdapterInfo)[I];
        TTreeNode *TreeNodeAdapter = TreeView->Items->AddChild(TreeNodeRoot, Item->GetDescription());
        netIP4 IP, Mask;
        if (Item->GetFirstIPAndMask(IP, Mask)) {
          do {
            TTreeNode *TreeNodeIP = TreeView->Items->AddChild(TreeNodeAdapter, IP.ToString());
          } while (Item->GetNextIPAndMask(IP, Mask));
        }
      }
    }
  }
  __finally {
    TreeView->Items->EndUpdate();
    TreeView->FullExpand();
  }
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
