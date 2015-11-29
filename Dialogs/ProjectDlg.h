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
#ifndef ProjectDlgH
#define ProjectDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.AppEvnts.hpp>
//---------------------------------------------------------------------------
class TProject;
class TDialogProject : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
  TPanel *Panel1;
  TPageControl *PageControl1;
  TTabSheet *TabSheet1;
  TApplicationEvents *ApplicationEvents1;
  TButton *ButtonCancel;
private:	// Anwender-Deklarationen
  TProject *Project;
public:		// Anwender-Deklarationen
  __fastcall TDialogProject(TComponent* Owner, TProject *_Project);
};
//---------------------------------------------------------------------------
#endif
