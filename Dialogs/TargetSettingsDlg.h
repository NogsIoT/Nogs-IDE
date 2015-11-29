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
#ifndef TargetSettingsDlgH
#define TargetSettingsDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "TargetsFrm.h"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.AppEvnts.hpp>
//---------------------------------------------------------------------------
class TDialogTargetSettings : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
  TPanel *Panel1;
  TButton *ButtonCancel;
  TPanel *Panel2;
  TPageControl *PageControl;
  TTabSheet *TabSheet1;
  TTabSheet *TabSheetUpload;
  TLabel *Label1;
  TLabel *Label2;
  TLabeledEdit *EditName;
  TDateTimePicker *DateTimePickerFirstDate;
  TDateTimePicker *DateTimePickerFirstTime;
  TDateTimePicker *DateTimePickerLastDate;
  TDateTimePicker *DateTimePickerLastTime;
  TGroupBox *GroupBox2;
  TRadioGroup *RadioGroupAutomaticUploadMode;
  TPanel *Panel3;
  TPaintBox *PaintBox;
  TApplicationEvents *ApplicationEvents1;
  TButton *ButtonOk;
  TGroupBox *GroupBox3;
  TComboBox *ComboBoxAutomaticUploadProject;
  TRadioButton *RadioButtonUploadFirst;
  TRadioButton *RadioButtonUploadExplicit;
  void __fastcall ButtonCancelClick(TObject *Sender);
  void __fastcall PaintBoxPaint(TObject *Sender);
  void __fastcall ApplicationEvents1Idle(TObject *Sender, bool &Done);
  void __fastcall ButtonOkClick(TObject *Sender);
  void __fastcall PageControlChange(TObject *Sender);
  void __fastcall ComboBoxAutomaticUploadProjectChange(TObject *Sender);
  void __fastcall RadioButtonUploadFirstClick(TObject *Sender);
  void __fastcall RadioButtonUploadExplicitClick(TObject *Sender);
private:	// Anwender-Deklarationen
  TListItem *ListItem;
public:		// Anwender-Deklarationen
  __fastcall TDialogTargetSettings(TComponent* Owner, TListItem *_ListItem);
};
//---------------------------------------------------------------------------
#endif
