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
#pragma hdrstop
#include "ICMPDLL.h"
#include <stdint.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
TICMPDLL *ICMPDLL = 0;
//---------------------------------------------------------------------------
__fastcall TICMPDLL::TICMPDLL():
  DLLOwner(false), _IcmpCreateFile(0), _IcmpCloseHandle(0), _IcmpSendEcho(0)
{
  DLL = GetModuleHandle(L"ICMP.DLL");
  if (!DLL) {
    DLL = LoadLibrary(L"ICMP.DLL");
    if (!DLL) throw CantGetDLL;
    DLLOwner = true;
  }
  _IcmpCreateFile = (TIcmpCreateFile) GetProcAddress(DLL, "IcmpCreateFile");
  if (!_IcmpCreateFile) throw CantFindIcmpCreateFile;
  _IcmpCloseHandle = (TIcmpCloseHandle) GetProcAddress(DLL, "IcmpCloseHandle");
  if (!_IcmpCloseHandle) throw CantFindIcmpCloseHandle;
  _IcmpSendEcho = (TIcmpSendEcho) GetProcAddress(DLL, "IcmpSendEcho");
  if (!_IcmpSendEcho) throw CantFindIcmpSendEcho;
}
//---------------------------------------------------------------------------
__fastcall TICMPDLL::~TICMPDLL()
{
  if (DLLOwner) FreeLibrary(DLL);
}
//---------------------------------------------------------------------------
TICMPDLL* __fastcall TICMPDLL::Get()
{
  if (!ICMPDLL) ICMPDLL = new TICMPDLL;
  return ICMPDLL;
}
//---------------------------------------------------------------------------
bool __fastcall TICMPDLL::Ping(const String IP, int Timeout)
{
  HANDLE Handle = _IcmpCreateFile();
  IPAddr DestinationAddress = inet_addr(((AnsiString) IP).c_str());
  uint8_t Recv[128];
  memset(Recv, 0, sizeof(Recv));
  uint8_t Send[32] = "Nogs Ping";
  DWORD Result = _IcmpSendEcho(Handle, DestinationAddress, Send, sizeof(Send), 0, Recv, sizeof(Recv), Timeout);
  _IcmpCloseHandle(Handle);
  return Result > 0;
}
//---------------------------------------------------------------------------
uint32_t __fastcall TICMPDLL::Ping(const String IP, uint32_t MessageId, int Timeout)
{
  HANDLE Handle = _IcmpCreateFile();
  IPAddr DestinationAddress = inet_addr(((AnsiString) IP).c_str());
  uint8_t Recv[128];
  memset(Recv, 0, sizeof(Recv));
  uint8_t Send[32];
  memcpy(Send, &MessageId, 4);
  DWORD Result = _IcmpSendEcho(Handle, DestinationAddress, Send, sizeof(Send), 0, Recv, sizeof(Recv), Timeout);
  _IcmpCloseHandle(Handle);
  if (!Result) return 0;
  memcpy(&MessageId, &Recv[28], 4);
  return MessageId;
}
//---------------------------------------------------------------------------



