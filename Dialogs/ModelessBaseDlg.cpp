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
#include "ModelessBaseDlg.h"
#include "MainFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
void __fastcall TDialogModeless::CheckBoxStayOnTopClick(TObject *Sender)
{
  FormStyle = CheckBoxStayOnTop->Checked ? fsStayOnTop : fsNormal;
}
//---------------------------------------------------------------------------
__fastcall TDialogModeless::TDialogModeless(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TDialogModeless::FormCloseQuery(TObject *Sender, bool &CanClose)
{
  FormMain->DialogModelessManager.Unregister(this);
}
//---------------------------------------------------------------------------
void __fastcall TDialogModeless::ButtonCloseClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------

