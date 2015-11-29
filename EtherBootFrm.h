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
#ifndef EtherBootFrmH
#define EtherBootFrmH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.AppEvnts.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Dialogs.hpp>
//---------------------------------------------------------------------------
class TFrameEtherBoot : public TFrame
{
__published:	// Von der IDE verwaltete Komponenten
  TGroupBox *GroupBox1;
  TPanel *Panel1;
  TGroupBox *GroupBoxFlash;
  TLabel *Label1;
  TSpeedButton *ButtonOpen;
  TEdit *EditFlashPath;
  TEdit *EditCRC;
  TButton *ButtonEraseFlash;
  TButton *ButtonSendFlash;
  TGroupBox *GroupBoxConfig;
  TPanel *Panel3;
  TButton *ButtonGetConfig;
  TButton *ButtonSendConfig;
  TButton *ButtonDefaultIni;
  TPanel *Panel5;
  TMemo *MemoConfig;
  TButton *ButtonTryToEnterBootloader;
  TSplitter *Splitter1;
  TApplicationEvents *ApplicationEvents;
  TOpenDialog *OpenDialog;
  TButton *ButtonStartCommand;
  TProgressBar *ProgressBar;
  TCheckBox *CheckBoxAutomaticSendFlash;
  TTimer *Timer;
  TPanel *Panel2;
  TListBox *ListBoxLog;
  TPanel *PanelStatus;
  TButton *ButtonSetIPAddress;
  void __fastcall ButtonTryToEnterBootloaderClick(TObject *Sender);
  void __fastcall ButtonSendFlashClick(TObject *Sender);
  void __fastcall ButtonEraseFlashClick(TObject *Sender);
  void __fastcall ButtonGetConfigClick(TObject *Sender);
  void __fastcall ButtonSendConfigClick(TObject *Sender);
  void __fastcall ButtonDefaultIniClick(TObject *Sender);
  void __fastcall ButtonOpenClick(TObject *Sender);
  void __fastcall ApplicationEventsIdle(TObject *Sender, bool &Done);
  void __fastcall ButtonStartCommandClick(TObject *Sender);
  void __fastcall CheckBoxAutomaticSendFlashClick(TObject *Sender);
  void __fastcall TimerTimer(TObject *Sender);
  void __fastcall ButtonSetIPAddressClick(TObject *Sender);
private:	// Anwender-Deklarationen
  String IP;
  TDateTime FlashFileTimeStamp;

  struct TProcess {
    TFrameEtherBoot *FrameEtherBoot;
    __fastcall TProcess(TFrameEtherBoot *_FrameEtherBoot, int Max):
      FrameEtherBoot(_FrameEtherBoot)
    {
      FrameEtherBoot->ProgressBar->Max = Max;
    }
    __fastcall ~TProcess()
    {
      FrameEtherBoot->ProgressBar->Position = 0;
    }
    void __fastcall Set(int Position, int Max = 0)
    {
      if (Max) FrameEtherBoot->ProgressBar->Max = Max;
      FrameEtherBoot->ProgressBar->Position = Position;
    }
  };

  void __fastcall Log(const String S);

  bool __fastcall TryToEnterBootloader();
  void __fastcall SendFlash();
  void __fastcall EraseFlash();
  void __fastcall GetConfig();
  void __fastcall SendConfig();

public:		// Anwender-Deklarationen
  __fastcall TFrameEtherBoot(TComponent* Owner);
  void __fastcall SetIP(String _IP);
};
//---------------------------------------------------------------------------
#endif
