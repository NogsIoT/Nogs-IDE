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

#ifndef ModelessBaseDlgH
#define ModelessBaseDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TDialogModeless : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
  TPanel *PanelBaseBar;
  TCheckBox *CheckBoxStayOnTop;
  TButton *ButtonClose;
  TBevel *Bevel1;
  void __fastcall CheckBoxStayOnTopClick(TObject *Sender);
  void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
  void __fastcall ButtonCloseClick(TObject *Sender);
private:	// Anwender-Deklarationen
public:		// Anwender-Deklarationen
  __fastcall TDialogModeless(TComponent* Owner);
  virtual void __fastcall Init()
  {}
  virtual void __fastcall Finish()
  {}
};
//---------------------------------------------------------------------------
class TDialogModelessManager
{
  TStringList *List;
public:
  __fastcall TDialogModelessManager()
  {
    List = new TStringList;
  }
  __fastcall ~TDialogModelessManager()
  {
    for (int I = 0; I < List->Count; I++) {
      TDialogModeless *DialogModeless = (TDialogModeless*) (TDialogModeless*) List->Objects[I];
      delete DialogModeless;
    }
    delete List;
  }
  void __fastcall Register(const String Name, TDialogModeless *DialogModeless)
  {
    int Index = List->Add(Name);
    List->Objects[Index] = DialogModeless;
    DialogModeless->Init();
    DialogModeless->Show();
  }
  void __fastcall Unregister(TDialogModeless *DialogModeless)
  {
    DialogModeless->Finish();
    for (int I = List->Count - 1; I >= 0; I--)
      if (List->Objects[I] == DialogModeless) {
        List->Delete(I);
      }
  }
  bool __fastcall NeedCreation(const String Name)
  {
    int Index = List->IndexOf(Name);
    if (Index < 0) return true;
    TDialogModeless *DialogModeless = (TDialogModeless*) List->Objects[Index];
    DialogModeless->BringToFront();
    return false;
  }
  TDialogModeless* __fastcall Get(const String Name)
  {
    int Index = List->IndexOf(Name);
    if (Index < 0) return 0;
    return (TDialogModeless*) List->Objects[Index];
  }
};
//---------------------------------------------------------------------------
#endif
