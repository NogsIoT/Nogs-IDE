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

#ifndef SetupAllDlgH
#define SetupAllDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFrameSetupBase;
class TDialogSetupAll : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
  TPanel *Panel1;
  TButton *ButtonClose;
  TPanel *Panel2;
  TListBox *ListBox;
  TSplitter *Splitter1;
  TPanel *PanelFrames;
  void __fastcall ButtonCloseClick(TObject *Sender);
  void __fastcall ListBoxClick(TObject *Sender);
private:	// Anwender-Deklarationen
  TFrameSetupBase *ActiveFameSetupBase;

  void __fastcall Add(const String Name, TFrameSetupBase *FameSetupBase);
public:		// Anwender-Deklarationen
  __fastcall TDialogSetupAll(TComponent* Owner);
};
//---------------------------------------------------------------------------
#endif
