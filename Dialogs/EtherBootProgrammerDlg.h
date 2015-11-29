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

#ifndef EtherBootProgrammerDlgH
#define EtherBootProgrammerDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "ModelessBaseDlg.h"
#include <Vcl.ExtCtrls.hpp>
#include "EtherBootFrm.h"
//---------------------------------------------------------------------------
class TDialogEtherBootProgrammer : public TDialogModeless
{
__published:	// Von der IDE verwaltete Komponenten
  TFrameEtherBoot *FrameEtherBoot1;
private:	// Anwender-Deklarationen
public:		// Anwender-Deklarationen
  __fastcall TDialogEtherBootProgrammer(TComponent* Owner);
};
//---------------------------------------------------------------------------
#endif
