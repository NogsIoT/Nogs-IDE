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
#include "EtherBootFrm.h"
#include "Error2.h"
#include "File.h"
#include "NetTypes.h"
#include "Types.h"
#include "UDP.h"
#include "MainFrm.h"
#include "Busy.h"
#include "Usul.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
uint32 CRC32(uint8 *Buffer, uint32 Size)
{
  uint32 CRC = 0;
  while (Size--) CRC += *Buffer++;
  return CRC;
}
//---------------------------------------------------------------------------
// TFTP
//---------------------------------------------------------------------------
#define TFTP_OPCODE_RRQ             1
#define TFTP_OPCODE_WRQ             2
#define TFTP_OPCODE_DATA            3
#define TFTP_OPCODE_ACK             4
#define TFTP_OPCODE_ERROR           5
#define TFTP_OPCODE_COMMAND_START 201

#define TFTP_PORT1         69
#define TFTP_PORT2         1765

#pragma pack(push, 1)
struct TTFTPHeader {
  net16 OpCode;
  union {
    struct { // RRQ, WRQ
      char FileNameAndMode[512];
    } RWRQ;
    struct { // DATA
      net16 No;
      uint8 Bytes[512];
    } Data;
    struct { // ACK
      net16 No;
    } Ack;
    struct { // ERROR
      net16 Code;
      char Message[80];
    } Error;
  };
  int __fastcall SetRRQ(const AnsiString Name, const AnsiString Mode)
  {
     OpCode(TFTP_OPCODE_RRQ);
     char *Ptr = RWRQ.FileNameAndMode;
     strcpy(Ptr, Name.c_str());
     Ptr += Name.Length() + 1;
     strcpy(Ptr, Mode.c_str());
     return sizeof(OpCode) + Name.Length() + 1 + Mode.Length() + 1;
  }
  int __fastcall SetWRQ(const AnsiString Name, const AnsiString Mode)
  {
     OpCode(TFTP_OPCODE_WRQ);
     char *Ptr = RWRQ.FileNameAndMode;
     strcpy(Ptr, Name.c_str());
     Ptr += Name.Length() + 1;
     strcpy(Ptr, Mode.c_str());
     return sizeof(OpCode) + Name.Length() + 1 + Mode.Length() + 1;
  }
  int __fastcall SetStart()
  {
     OpCode(TFTP_OPCODE_COMMAND_START);
     char *Ptr = RWRQ.FileNameAndMode;
     *Ptr++ = 0;
     *Ptr++ = 0;
     return sizeof(OpCode) + 1 + 1;
  }
  AnsiString __fastcall GetDataAsString()
  {
    return OpCode == TFTP_OPCODE_DATA ? AnsiString((char*) Data.Bytes) : AnsiString(EmptyStr);
  }
  int __fastcall SetData(const AnsiString S, int No)
  {
    OpCode(TFTP_OPCODE_DATA);
    Data.No(No);
    strncpy(Data.Bytes, S.c_str(), sizeof(Data.Bytes));
    return sizeof(OpCode) + sizeof(Data.No) + S.Length();
  }
  int __fastcall SetData(uint8 *Buffer, int BytesToSend, int No)
  {
    if (BytesToSend > 512) BytesToSend = 512;
    OpCode(TFTP_OPCODE_DATA);
    Data.No(No);
    memcpy(Data.Bytes, Buffer, BytesToSend);
    return sizeof(OpCode) + sizeof(Data.No) + BytesToSend;
  }
  void __fastcall CheckForError()
  {
    if (OpCode == TFTP_OPCODE_ERROR) throw AnsiString(Error.Message) + " (" + AnsiString(Error.Code) * ")";
    if (OpCode != TFTP_OPCODE_ACK) throw AnsiString("Missing ACK");
  }
  void __fastcall CheckForError(int No)
  {
    CheckForError();
    if (Ack.No != No) throw AnsiString("Wrong ACK no: expected ") + AnsiString(No) + ", got " + AnsiString(Ack.No);
  }
};
#pragma pack(pop)
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TFrameEtherBoot::Log(const String S)
{
  ListBoxLog->Items->Add(S);
  ListBoxLog->TopIndex = ListBoxLog->Items->Count - 1;
}
//---------------------------------------------------------------------------
bool __fastcall TFrameEtherBoot::TryToEnterBootloader()
{
  TProcess Process(this, 9);
  try {
    TUDP UDP;
    UDP.Connect(IP, TFTP_PORT1, TUDP::Client);
    ProgressBar->Max = 9;
    int Try = 0;
    TTFTPHeader TFTPHeader;           //  GetAsyncKeyState
    for (;;) {
      UDP.Send((char*) &TFTPHeader, TFTPHeader.SetRRQ("config.ini", "netascii"));
      Process.Set(++Try % 10);
      int UDPLength = UDP.Recv((char*) &TFTPHeader, sizeof(TFTPHeader), 100);
      if (UDPLength) break;
      if (Try % 50 == 0) Log("Cycle board power!");
      if (Try > 210) return false;
      int Key;
      //GetAsyncKeyState(&Key);
      //if (Key ==
    }
    Log("Bootloader or backdoor active!");
    return true;
  }
  catch (const AnsiString S) {
    Log(S);
    return false;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameEtherBoot::SendFlash()
{
  TBusy Busy;
  uint8 *Buffer = 0;
  try {
    try {
      if (!TryToEnterBootloader()) throw AnsiString("Can't enter bootloader");

      TFile F(EditFlashPath->Text, "rb");
      if (!F.Ok()) throw AnsiString("Can't open file");
      fseek(F, 0, SEEK_END);
      int Size = ftell(F);
      fseek(F, 0, SEEK_SET);
      Buffer = new uint8[Size];
      fread(Buffer, 1, Size, F);

      if ((Buffer[0] != 0x0C) || (Buffer[1] != 0x94)) throw AnsiString("Wrong file type");
      //Buffer[0] = 0;

      EditCRC->Text = AnsiString::IntToHex(CRC32(Buffer, Size), 8);

      TUDP UDP1, UDP2;
      UDP1.Connect(IP, TFTP_PORT1, TUDP::Client);
      UDP2.Connect(IP, TFTP_PORT2, TUDP::Client);

      ProgressBar->Max = Size / 512;

      TTFTPHeader TFTPHeader;
      UDP1.Send((char*) &TFTPHeader, TFTPHeader.SetWRQ("flash.bin", "octet"));

      int UDPLength = UDP1.Recv((char*) &TFTPHeader, sizeof(TFTPHeader), 1500);
      if (!UDPLength) throw AnsiString("No answer to WRQ");
      TFTPHeader.CheckForError();

      for (int I = 0;I <= Size / 512;I++) {
        int BytesToSend = Size - I * 512;
        ProgressBar->Position = I;
        for (int Try = 1;Try <= 3;Try++) {
          UDP2.Send((char*) &TFTPHeader, TFTPHeader.SetData(Buffer + 512 * I, BytesToSend, I + 1));
          UDPLength = UDP2.Recv((char*) &TFTPHeader, sizeof(TFTPHeader), 500);
          if (UDPLength) break;
        }
        if (!UDPLength) throw AnsiString("No answer to WRQ");
        TFTPHeader.CheckForError(I + 1);
      }
      {
        String MAC = MemoConfig->Lines->Values["MAC"];
        if (MAC.Length())
          Usul()->Ini->WriteString("EtherBoot@" + IP, "MAC", MAC);
      }
      Log("Send succesfully");
    }
    catch (const AnsiString S) {
      Log(S);
    }
  }
  __finally {
    delete [] Buffer;
    ProgressBar->Position = 0;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameEtherBoot::EraseFlash()
{
  try {
    TUDP UDP1, UDP2;
    UDP1.Connect(IP, TFTP_PORT1, TUDP::Client);
    UDP2.Connect(IP, TFTP_PORT2, TUDP::Client);

    TTFTPHeader TFTPHeader;
    UDP1.Send((char*) &TFTPHeader, TFTPHeader.SetWRQ("flash.bin", "octet"));

    int UDPLength = UDP1.Recv((char*) &TFTPHeader, sizeof(TFTPHeader), 1500);
    if (!UDPLength) throw AnsiString("No answer to WRQ");
    TFTPHeader.CheckForError();

    uint8 Buffer[512];
    memset(Buffer, 0xFF, sizeof(Buffer));

    UDP2.Send((char*) &TFTPHeader, TFTPHeader.SetData(Buffer, 512, 1));
    UDPLength = UDP2.Recv((char*) &TFTPHeader, sizeof(TFTPHeader), 1500);
    if (!UDPLength) throw AnsiString("No answer to WRQ");
    TFTPHeader.CheckForError(1);
    Log("Send succesfully");
  }
  catch (const AnsiString S) {
    Log(S);
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameEtherBoot::GetConfig()
{
  TProcess Process(this, 5);
  try {
    MemoConfig->Lines->Clear();
    MemoConfig->Refresh();
    ::Sleep(150);

    TUDP UDP;
    UDP.Connect(IP, TFTP_PORT1, TUDP::Client);

    TTFTPHeader TFTPHeader;
    int UDPLength;
    for (int I = 0;I <= 4;I++) {
      Process.Set(I + 1);
      UDP.Send((char*) &TFTPHeader, TFTPHeader.SetRRQ("config.ini", "netascii"));
      UDPLength = UDP.Recv((char*) &TFTPHeader, sizeof(TFTPHeader), 1500);
      if (UDPLength) break;
    }
    if (!UDPLength) throw AnsiString("No answer to RRQ");

    //int No = TFTPHeader.Data.No;
    MemoConfig->Lines->Text = TFTPHeader.GetDataAsString();
    Log("Recv succesfully");
  }
  catch (const AnsiString S) {
    Log(S);
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameEtherBoot::SendConfig()
{
  try {
    TUDP UDP1, UDP2;
    UDP1.Connect(IP, TFTP_PORT1, TUDP::Client);
    UDP2.Connect(IP, TFTP_PORT2, TUDP::Client);

    TTFTPHeader TFTPHeader;
    int UDPLength;
    for (int I = 0;I <= 4;I++) {
      UDP1.Send((char*) &TFTPHeader, TFTPHeader.SetWRQ("config.ini", "netascii"));
      UDPLength = UDP1.Recv((char*) &TFTPHeader, sizeof(TFTPHeader), 1500);
      if (UDPLength) break;
    }
    if (!UDPLength) throw AnsiString("No answer to WRQ");
    TFTPHeader.CheckForError();

    UDP2.Send((char*) &TFTPHeader, TFTPHeader.SetData(MemoConfig->Lines->Text, 1));

    UDPLength = UDP2.Recv((char*) &TFTPHeader, sizeof(TFTPHeader), 1500);
    if (!UDPLength) throw AnsiString("No answer to WRQ");
    TFTPHeader.CheckForError(1);
    Log("Send succesfully");
  }
  catch (const AnsiString S) {
    Log(S);
  }
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TFrameEtherBoot::ApplicationEventsIdle(TObject *Sender, bool &Done)
{
  if (IP.Length()) {
    ButtonStartCommand->Enabled = true;
    ButtonOpen->Enabled = true;
    ButtonSendFlash->Enabled = FileExists(EditFlashPath->Text);
    ButtonEraseFlash->Enabled = true;
    CheckBoxAutomaticSendFlash->Enabled = ButtonSendFlash->Enabled;
    ButtonGetConfig->Enabled = true;
    if (!MemoConfig->Lines->Count) {
      ButtonSendConfig->Enabled = false;
    } else if (MemoConfig->Lines->Values["MAC"] == "FABE02181208") {
      ButtonSendConfig->Enabled = false;
      PanelStatus->Caption = "MAC is the default address! Change to board lable address! Use 'arp -d ...' to refresh ARP table!" ;
      PanelStatus->Hint = PanelStatus->Caption;
      PanelStatus->Font->Color = clRed;
    } else {
      ButtonSendConfig->Enabled = true;
      PanelStatus->Caption = EmptyStr;
    }
    ButtonDefaultIni->Enabled = true;
  } else {
    ButtonStartCommand->Enabled = false;
    ButtonOpen->Enabled = false;
    ButtonSendFlash->Enabled = false;
    ButtonEraseFlash->Enabled = false;
    CheckBoxAutomaticSendFlash->Enabled = false;
    ButtonGetConfig->Enabled = false;
    ButtonSendConfig->Enabled = false;
    ButtonDefaultIni->Enabled = false;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameEtherBoot::TimerTimer(TObject *Sender)
{
  TDateTime FlashFileTimeStamp2;
  if (FileAge(EditFlashPath->Text, FlashFileTimeStamp2)) {
    if (FlashFileTimeStamp != FlashFileTimeStamp2) {
      FlashFileTimeStamp = FlashFileTimeStamp2;
      TBusy Busy;
      TProcess Process(this, 3);
      try {
        Log("New flash file found: " + FlashFileTimeStamp.DateTimeString());
        if (!TryToEnterBootloader()) throw AnsiString("Can't enter bootloader");
        Log("Sending new flash contens");
        SendFlash();
      }
      catch (const AnsiString S) {
        Log(S);
      }
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameEtherBoot::CheckBoxAutomaticSendFlashClick(TObject *Sender)
{
  Timer->Enabled = CheckBoxAutomaticSendFlash->Checked && FileAge(EditFlashPath->Text, FlashFileTimeStamp);
}
//---------------------------------------------------------------------------
void __fastcall TFrameEtherBoot::ButtonSetIPAddressClick(TObject *Sender)
{
  String IP = this->IP;
  if (IP.IsEmpty())
    IP = Usul()->Ini->ReadString("EtherBoot@Settings", "IP", "192.168.1.200");
  if (InputQuery("Set IP address", "IP", IP)) {
    SetIP(IP);
    Usul()->Ini->WriteString("EtherBoot@Settings", "IP", IP);
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameEtherBoot::ButtonTryToEnterBootloaderClick(TObject *Sender)
{
  TBusy Busy;
  if (IP.IsEmpty() && InputQuery("Try to enter bootloader", "IP", IP)) SetIP(IP);
  if (IP.Length()) TryToEnterBootloader();
}
//---------------------------------------------------------------------------
void __fastcall TFrameEtherBoot::ButtonStartCommandClick(TObject *Sender)
{
  TBusy Busy;
  TProcess Process(this, 20);
  try {
    TUDP UDP1, UDP2;
    UDP1.Connect(IP, TFTP_PORT1, TUDP::Client);
    TTFTPHeader TFTPHeader;
    UDP1.Send((char*) &TFTPHeader, TFTPHeader.SetStart());
    Log("Send start command");
    for (int I = 1;I <= 20;I++) {
      Process.Set(I);
      Sleep(250);
    }
  }
  catch (const AnsiString S) {
    Log(S);
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameEtherBoot::ButtonOpenClick(TObject *Sender)
{
  if (OpenDialog->Execute()) {
    EditFlashPath->Text = OpenDialog->FileName;
    Usul()->Ini->WriteString(IP, "FlashPath", EditFlashPath->Text);
  }
}
//---------------------------------------------------------------------------
void __fastcall TFrameEtherBoot::ButtonSendFlashClick(TObject *Sender)
{
  TBusy Busy;
  SendFlash();
}
//---------------------------------------------------------------------------
void __fastcall TFrameEtherBoot::ButtonEraseFlashClick(TObject *Sender)
{
  TBusy Busy;
  EraseFlash();
}
//---------------------------------------------------------------------------
void __fastcall TFrameEtherBoot::ButtonGetConfigClick(TObject *Sender)
{
  TBusy Busy;


  GetConfig();
}
//---------------------------------------------------------------------------
void __fastcall TFrameEtherBoot::ButtonSendConfigClick(TObject *Sender)
{
  TBusy Busy;
  SendConfig();
}
//---------------------------------------------------------------------------
void __fastcall TFrameEtherBoot::ButtonDefaultIniClick(TObject *Sender)
{
  MemoConfig->Lines->Clear();
  MemoConfig->Lines->Add("MAC=" + Usul()->Ini->ReadString(IP, "MAC", "FABE02181208"));
  MemoConfig->Lines->Add("IP=" + IP);
  MemoConfig->Lines->Add("Mask=255.255.255.0");
  MemoConfig->Lines->Add("AutoStart=1");
  Log("Create default config.ini. Change MAC address to board lable address!");
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
__fastcall TFrameEtherBoot::TFrameEtherBoot(TComponent* Owner)
  : TFrame(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrameEtherBoot::SetIP(String _IP)
{
  IP = _IP;
  GroupBoxFlash->Caption = "\\\\" + IP + "\\flash.bin";
  GroupBoxConfig->Caption = "\\\\" + IP + "\\config.ini";
  MemoConfig->Lines->Clear();
  EditFlashPath->Text = Usul()->Ini->ReadString(IP, "FlashPath", EmptyStr);
  OpenDialog->FileName = EditFlashPath->Text;
  Log("Set IP address to " + IP);
}
//---------------------------------------------------------------------------










