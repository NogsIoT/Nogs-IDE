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
/* Bugs

   Große Files >= 338k beschädigen das FFS -> 2:/Startup.lua nicht mehr lesbar. Warum?




















*/
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "MainFrm.h"
#include "Usul.h"
#include "Format.h"
#include <Vcl.FileCtrl.hpp>
#include "Error2.h"
#include "FatFsImageBuilder.h"
#include "SetupAllDlg.h"
#include "JSON.h"
#include "EtherBootFrm.h"
#include "EtherBootProgrammerDlg.h"
#include "CommunicationTestDlg.h"
#include "MemoryTrackerDlg.h"
#include "Target.h"
#include "ZeroConfDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "LuaEditorFrm"
#pragma link "ProjectFrm"
#pragma link "ProjectFrm"
#pragma link "TargetsFrm"
#pragma link "BrowserFrm"
#pragma link "NogsDesigner"
#pragma link "TopologieEditorFrm"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
TFormMain *FormMain;
//---------------------------------------------------------------------------
void __fastcall GetFileVersion(const AnsiString Path,int &A,int &B,int &C,int &D)
{
  A = B = C = D = 0;
  DWORD Dummy;
  DWORD FVISize = GetFileVersionInfoSize(String(Path).c_str(),&Dummy);
  LPBYTE VersionInfo = new BYTE[FVISize];
  GetFileVersionInfo(String(Path).c_str(),0,FVISize,VersionInfo);
  UINT Len;
  VS_FIXEDFILEINFO *Ffi;
  if (VerQueryValue(VersionInfo,L"\\",(void**) &Ffi,&Len)) {
    DWORD FileVersionMS = Ffi->dwFileVersionMS;
    DWORD FileVersionLS = Ffi->dwFileVersionLS;
    delete [] VersionInfo;
    A = HIWORD(FileVersionMS);
    B = LOWORD(FileVersionMS);
    C = HIWORD(FileVersionLS);
    D = LOWORD(FileVersionLS);
  }
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TFormMain::MenuItemProjectNewClick(TObject *Sender)
{
  FrameProject->ButtonNewClick(0);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MenuItemExistingProjectClick(TObject *Sender)
{
  FrameProject->MenuItemWorkspaceAddExistingProjectClick(0);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::ApplicationEvents1Idle(TObject *Sender, bool &Done)
{
  static bool Init = true;
  if (Init) {
    Init = false;
    ShowZeronConfDialog();
  }
  MenuItemUndo->Enabled = FrameLuaEditor->CanUndo();
  MenuItemRedo->Enabled = FrameLuaEditor->CanRedo();
  MenuItemCut->Enabled = FrameLuaEditor->CanCut();
  MenuItemCopy->Enabled = FrameLuaEditor->CanCopy();
  MenuItemPaste->Enabled = FrameLuaEditor->CanPaste();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MenuItemExitClick(TObject *Sender)
{
  Application->Terminate();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MenuItemUndoClick(TObject *Sender)
{
  FrameLuaEditor->Undo();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MenuItemRedoClick(TObject *Sender)
{
  FrameLuaEditor->Redo();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MenuItemCutClick(TObject *Sender)
{
  FrameLuaEditor->Cut();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MenuItemCopyClick(TObject *Sender)
{
  FrameLuaEditor->Copy();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MenuItemPasteClick(TObject *Sender)
{
  FrameLuaEditor->Paste();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MenuItemSelectAllClick(TObject *Sender)
{
  FrameLuaEditor->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MenuItemOptionsClick(TObject *Sender)
{
  TDialogSetupAll *DialogSetupAll = 0;
  try {
    DialogSetupAll = new TDialogSetupAll(this);
    DialogSetupAll->ShowModal();
  }
  __finally {
    delete DialogSetupAll;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::Nodecommunicationtest1Click(TObject *Sender)
{
  if (DialogModelessManager.NeedCreation("TDialogCommunicationTest"))
    DialogModelessManager.Register("TDialogCommunicationTest", new TDialogCommunicationTest(this));
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MenuItemMemoryTrackerClick(TObject *Sender)
{
  if (DialogModelessManager.NeedCreation("TDialogMemoryTracker"))
    DialogModelessManager.Register("TDialogMemoryTracker", new TDialogMemoryTracker(this));
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::Programmer1Click(TObject *Sender)
{
  TDialogEtherBootProgrammer *DialogEtherBootProgrammer = 0;
  try {
    DialogEtherBootProgrammer = new TDialogEtherBootProgrammer(this);
    DialogEtherBootProgrammer->ShowModal();
  }
  __finally {
    delete DialogEtherBootProgrammer;
  }
}
void __fastcall TFormMain::MenuItemNogsWikiClick(TObject *Sender)
{
  ShellExecute(0, L"OPEN", L"http://en.wiki.nogs.cc", L"", L"", SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MenuItemNogsHomepageClick(TObject *Sender)
{
  ShellExecute(0, L"OPEN", L"http://www.nogs.cc", L"", L"", SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TFormMain::IdIPAddrMonStatusChanged(TObject *Sender, int Adapter, UnicodeString _OldIP, UnicodeString _NewIP)
{
  //netIP4 IP = HasZeroConfig();
  netIP4 NewIP, OldIP;
  NewIP(_NewIP);
  OldIP(_OldIP);
  Log(1, "IP Monitor: IP change on adapter " + String(Adapter) + " from " + _OldIP + " to " + _NewIP);
  bool IsZeroConfOld = (OldIP[3] == 169) && (OldIP[2] == 254);
  bool IsZeroConfNew = (NewIP[3] == 169) && (NewIP[2] == 254);
  if (IsZeroConfOld && !IsZeroConfNew)
    ShowZeronConfDialog();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::IdUDPFFSServerOnUDPRead(TIdUDPListenerThread* Thread, DynamicArray<uchar> Data, Idsockethandle::TIdSocketHandle* Binding)
{
  TFFSPacket FFSPacket;
  memset(&FFSPacket, 0, sizeof(FFSPacket));
  int Length = Data.Length;
  if (Length > sizeof(FFSPacket))
    Length = sizeof(FFSPacket);
  {
    uint8 *Ptr = (uint8*) &FFSPacket;
    for (int I = 0; I < Length; I++) 
      *Ptr++= Data[I];
  }
  TTarget *Target = FrameTargets->GetTarget(Binding->PeerIP);
  if (Target) {
    Data.Length = Target->HandleFFS(&FFSPacket);
    if (Data.Length > 0) {
      uint8 *Ptr = (uint8*) &FFSPacket;
      for (int I = 0; I < Data.Length; I++) Data[I] = *Ptr++;
      IdUDPFFSServer->SendBuffer(Binding->PeerIP, 51121, Data);
    }
  }
  //Log(0, String(FFSPacket.Command) + " " + String(FFSPacket.Parameter));
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::IdUDPFFSServerStatus(TObject *ASender, const TIdStatus Status, const UnicodeString StatusText)
{
  Log(0, "FFS: Status " + StatusText);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::IdTFTPServerReadFile(TObject *Sender, UnicodeString &FileName, const TPeerInfo &PeerInfo, bool &GrantAccess, TStream *&Stream, bool &FreeStreamOnComplete)
{
  TTarget *Target = FrameTargets->GetTarget(PeerInfo.PeerIP);
  if (Target) {
    Stream = Target->HandleTFTP(FileName);
    GrantAccess = Stream;
    FreeStreamOnComplete = false;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::IdTFTPServerStatus(TObject *Sender, const TIdStatus Status, const UnicodeString StatusText)
{
  Log(0, "TFTP: Status " + StatusText);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::IdTFTPServerTransferComplete(TObject *Sender, const bool Success, const TPeerInfo &PeerInfo, TStream *&Stream, const bool WriteOperation)
{
  TTarget *Target = FrameTargets->GetTarget(PeerInfo.PeerIP);
  if (Target)
    Target->HandleTFTP(Stream, Success);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::IdTFTPServerUDPException(TIdUDPListenerThread *Thread, TIdSocketHandle *Binding, const UnicodeString Message, const TClass AExceptionClass)
{
  Log(3, "TFTP: UDP exception " + Message);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::IdUDPBackdoorServerOnUDPRead(TIdUDPListenerThread* Thread, DynamicArray<uchar> Data, Idsockethandle::TIdSocketHandle* Binding)
{
  static bool Lock = false;
  try {
    if (!Lock) {
      Lock = true;
      String Recv;
      for (int I = 0; I < Data.Length; I++) 
        Recv += (char) Data[I];
      Log(0, 0, Recv);
      try {
        TJSONArray JSONArray(Recv);
        TJSONObject *JSONObject = JSONArray.GetJSONObject(0);
        if (JSONObject) {
          String Result = JSONObject->GetName(0).LowerCase();
          if (Result == "success") {
            // Success. Try to print result
            String S = JSONObject->GetString(0);
            TTreeNode *TreeNode = Log(1, S);
            try {
              TJSONArray JSONArray(S);

              TJSONArray *Header = JSONArray.GetJSONArray(0);
              TJSONArray *Response = JSONArray.GetJSONArray(1);
              if (Header && Response) {
                FrameBrowser->Recv(Header, Response);
                return;
              }

              TStringList *StringList = new TStringList;
              TJSONFormatter JSONFormatter;
              StringList->Text = JSONArray.ToString(JSONFormatter);
              for (int I = 0; I < StringList->Count; I++) {
                Log(TreeNode, 1, StringList->Strings[I]);
                if (I >= 10) {
                  Log(TreeNode, 1, "...");
                  break;
                }
              }
              delete StringList;
            }
            catch (TJSONScanner::TError &Error) {}
            return;
          } else if (Result == "error") {
            String S = JSONObject->GetString(0);
            Log(3, "Error " + S);
            return;
          }
        }
        // Fallback: JSON formatted print
        TTreeNode *TreeNode = Log(2, Recv);
        TJSONFormatter JSONFormatter;
        TStringList *StringList = new TStringList;
        StringList->Text = JSONArray.ToString(JSONFormatter);
        for (int I = 0; I < StringList->Count; I++)
          Log(TreeNode, 2, StringList->Strings[I]);
        delete StringList;
        return;
      }
      catch (TJSONScanner::TError &E) {
        Log(3, "JSON error: " + TJSONScanner::ErrorToString(E));
      }
      // Fallback: Print message as is
      //Log(0, Recv);
    }
  }
  __finally {
    Lock = false;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::IdUDPMemoryTrackerServerOnUDPRead(TIdUDPListenerThread* Thread, DynamicArray<uchar> Data, Idsockethandle::TIdSocketHandle* Binding)
{
  TDialogMemoryTracker *DialogMemoryTracker = (TDialogMemoryTracker*) DialogModelessManager.Get("TDialogModelessManager");
  if (DialogMemoryTracker) {
    TDialogMemoryTracker::TBuffer Buffer;
    uint8_t *Ptr = (uint8_t*) &Buffer;
    for (int I = 0; I < Data.Length; I++) *Ptr++ = Data[I];
    DialogMemoryTracker->Add(&Buffer);
  }
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TFormMain::FormCloseQuery(TObject *Sender, bool &CanClose)
{
  FrameProject->Finish();
  FrameLuaEditor->Finish();
  FrameTargets->Finish();
  FrameBrowser->Finish();
  Usul()->Ini->WriteInteger("MainWindow", "Left", Left);
  Usul()->Ini->WriteInteger("MainWindow", "Top", Top);
  Usul()->Ini->WriteInteger("MainWindow", "Width", Width);
  Usul()->Ini->WriteInteger("MainWindow", "Height", Height);
  Usul()->Ini->WriteInteger("MainWindow", "Maximized", WindowState == wsMaximized);
  CanClose = true;
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::FormCreate(TObject *Sender)
{
  Log(0, "Start");
  {
    int A, B, C, D;
    GetFileVersion(Application->ExeName, A, B, C, D);
    Caption = Format("Nogs-IDE V%d.%d%c " __DATE__ " " __TIME__, A, B, 'a' + C);
  }

  FileExtensions = new TStringList;

  if (GetKeyState(VK_LSHIFT) & 0x8000) {
    Left = 0;
    Top = 0;
    Width = 1024;
    Height = 768;
    WindowState = wsNormal;
  } else {
    Left = Usul()->Ini->ReadInteger("MainWindow", "Left", Left);
    Top = Usul()->Ini->ReadInteger("MainWindow", "Top", Top);
    if (Usul()->Ini->ReadInteger("MainWindow", "Maximized", 0))
      WindowState = wsMaximized;
    else {
      Width = Usul()->Ini->ReadInteger("MainWindow", "Width", Width);
      Height = Usul()->Ini->ReadInteger("MainWindow", "Height", Height);
    }
  }

  Log(0, "Document path: " + (Usul()->HasWriteAccessToExeDirectrory() ? Usul()->Path : Usul()->PathDocuments));

  FrameProject->Init();
  FrameLuaEditor->Init();
  FrameTargets->Init();
  FrameBrowser->Init();
  FrameLog->Init();

  IdUDPFFSServer->OnUDPRead = IdUDPFFSServerOnUDPRead; // Workaround
  IdUDPBackdoorServer->OnUDPRead = IdUDPBackdoorServerOnUDPRead; // Workaround
  IdUDPMemoryTrackerServer->OnUDPRead = IdUDPMemoryTrackerServerOnUDPRead; // Workaround

  //Log(0, "SysLog Server port: " + String(IdSyslogServer->DefaultPort));
  Log(0, "TFTP port: " + String(IdTFTPServer->DefaultPort));

  IdUDPFFSServer->Active = true;
  IdTFTPServer->Active = true;
  IdUDPBackdoorServer->Active = true;
  IdUDPMemoryTrackerServer->Active = true;
  //IdSyslogServer->Active = true;

  {
    TTreeNode *TreeNode = Log(0, "Local IP addresses:");
    bool HasZeroConf = false;
    for (int I = 0; I < GStack->LocalAddresses->Count; I++) {
      String IP = GStack->LocalAddresses->Strings[I];
      bool IsZeroConf = IP.SubString(1, 7) == "169.254";
      if (IsZeroConf) {
        Log(TreeNode, 1, IP + " (ZeroConf address)");
        HasZeroConf = true;
      } else
        Log(TreeNode, 0, IP);
    }
    if (!HasZeroConf)
      Log(TreeNode, 3, "No ZeroConf address found. Config one adapter to an address in the range from 169.254.1.1 to 169.264.254.254!");
  }
  {
    TFatFsImageBuilder *FatFsImageBuilder = TFatFsImageBuilder::Acquire();
    FatFsImageBuilder->Init(512 * 1024 / FatFsImageBuilder->GetSectorSize());
    FatFsImageBuilder->Release();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::ShowZeronConfDialog()
{
  if (DialogModelessManager.NeedCreation("TDialogZeroConf"))
    DialogModelessManager.Register("TDialogZeroConf", new TDialogZeroConf(this));
}
//---------------------------------------------------------------------------
netIP4 __fastcall TFormMain::HasZeroConfig()
{
  TIpProperty *IpProperty = 0;
  try {
    IpProperty = new TIpProperty;
    int ZeroConfRangeCount = 0;
    for (int I = 0; I < GStack->LocalAddresses->Count; I++) {
      netIP4 IP;
      IP(GStack->LocalAddresses->Strings[I]);
      if ((IP[3] == 169) && (IP[2] == 254))
        return IP;
    }
    return netIP4::Null();
  } __finally {
    delete IpProperty;
  }
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
__fastcall TFormMain::TFormMain(TComponent* Owner)
  : TForm(Owner)
{}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void __fastcall TFormMain::Backdoor(netIP4 IP, const String Command)
{
  if (Command.Pos("sys.stop():in")) {
    int I = 0;
  }
  Log(4, "NOGS-IDE: Send '" + Command + "' to backdoor of " + IP.ToString());
  DynamicArray<uchar> Data;
  Data.Length = Command.Length();
  for (int I = 0; I < Data.Length; I++) Data[I] = Command[I + 1];
  IdUDPBackdoorServer->SendBuffer(IP.ToString(), 11111, Data);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::Backdoor(TStringList *LiveCodingTargets, const String Command)
{
  switch ((int) LiveCodingTargets) {
    case 1:
      {
        netIP4 IP;
        IP("169.254.255.255");
        Backdoor(IP, Command);
      }
      break;
    case 0:
    case 2:
      Backdoor(netIP4::Broadcast(), Command);
      break;
    default :
      {
        for (int I = 0;I < LiveCodingTargets->Count; I++) {
          netIP4 IP;
          IP(LiveCodingTargets->Strings[I]);
          Backdoor(IP, Command);
        }
      }
  }
}    
//---------------------------------------------------------------------------
int __fastcall TFormMain::FileExtensionToImageIndex(const String FileExtension)
{
  if (FileExtension.IsEmpty())
    return -1;
  int Index = FileExtensions->IndexOf(FileExtension);
  if (Index >= 0)
    return (int) FileExtensions->Objects[Index];
  SHFILEINFO FileInfo;
  if (!SHGetFileInfo((L"*" + FileExtension).c_str(), FILE_ATTRIBUTE_NORMAL, &FileInfo, sizeof(FileInfo), SHGFI_ICON | SHGFI_SMALLICON | SHGFI_SYSICONINDEX | SHGFI_USEFILEATTRIBUTES))
    return -1;
  TIcon *Icon = new TIcon;
  Icon->Handle = FileInfo.hIcon;
  Index = ImageListFileTypeSmall->AddIcon(Icon);
  delete Icon;
  FileExtensions->Objects[FileExtensions->Add(FileExtension)] = (TObject*) Index;
  return Index;
}
//---------------------------------------------------------------------------
































