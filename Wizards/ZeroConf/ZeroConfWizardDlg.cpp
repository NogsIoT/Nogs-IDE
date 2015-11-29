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
#include "ZeroConfWizardDlg.h"
#include "ZeroConfWizardAdaperFrm.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TDialogZeroConfWizard::TDialogZeroConfWizard(TComponent* Owner)
  : TForm(Owner)
{
  WSADATA WSAData;
  WSAStartup(MAKEWORD(2, 2), &WSAData);

  TIPHelper *IPHelper;
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
      TFrameZeroConfWizardAdapter *FrameZeroConfWizardAdapter = new TFrameZeroConfWizardAdapter(this, Item, I);
      FrameZeroConfWizardAdapter->Name = "FrameZeroConfWizardAdapter" + String(I);
      FrameZeroConfWizardAdapter->Align = alTop;
      FrameZeroConfWizardAdapter->Parent = ScrollBox;
      FrameZeroConfWizardAdapter->Refresh();
    }
  }
}
//---------------------------------------------------------------------------
