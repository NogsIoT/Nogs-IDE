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

#ifndef SetupSysLogFrmH
#define SetupSysLogFrmH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "SetupBaseFrm.h"
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFacility;
class TFrameSetupSysLog : public TFrameSetupBase
{
__published:	// Von der IDE verwaltete Komponenten
  TGroupBox *GroupBox4;
  TListView *ListViewSysLogColors;
  TPanel *Panel3;
  TPanel *Panel5;
  TLabel *Label2;
  TLabel *Label3;
  TColorListBox *ColorListBoxSysLogColorsBackColor;
  TColorListBox *ColorListBoxSysLogColorsFontColor;
  TButton *ButtonSysLogColorsResetToDefault;
  TPageControl *PageControl1;
  TTabSheet *TabSheet1;
  TTabSheet *TabSheet2;
  TLabeledEdit *EditClearOn;
  TListView *ListView;
  TPanel *Panel1;
  TPanel *Panel2;
  TLabel *Label1;
  TLabel *Label4;
  TCheckBox *CheckBoxVisible;
  TColorListBox *ColorListBoxBackColor;
  TColorListBox *ColorListBoxFontColor;
  TButton *Button1;
  TButton *Button2;
  void __fastcall ListViewClick(TObject *Sender);
  void __fastcall ListViewCustomDrawItem(TCustomListView *Sender, TListItem *Item, TCustomDrawState State, bool &DefaultDraw);
  void __fastcall ListViewCustomDrawSubItem(TCustomListView *Sender, TListItem *Item, int SubItem, TCustomDrawState State, bool &DefaultDraw);
  void __fastcall ColorListBoxFontColorClick(TObject *Sender);
  void __fastcall ColorListBoxBackColorClick(TObject *Sender);
  void __fastcall Button2Click(TObject *Sender);
  void __fastcall CheckBoxVisibleClick(TObject *Sender);
private:	// Anwender-Deklarationen
  TFacility *Facility;
public:		// Anwender-Deklarationen
  __fastcall TFrameSetupSysLog(TComponent* Owner);
  void __fastcall Init();
};
//---------------------------------------------------------------------------
#endif
