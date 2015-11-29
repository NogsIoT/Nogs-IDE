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
#ifndef ICMPDLLH
#define ICMPDLLH
#include <System.Classes.hpp>
#include <windows.h>
#include <Ipexport.h>
//---------------------------------------------------------------------------
class TICMPDLL
{
  HMODULE DLL;
  bool DLLOwner;

  typedef HANDLE (__stdcall *TIcmpCreateFile)(void);
  TIcmpCreateFile _IcmpCreateFile;

  typedef BOOL (__stdcall *TIcmpCloseHandle)(HANDLE IcmpHandle);
  TIcmpCloseHandle _IcmpCloseHandle;

  typedef DWORD (__stdcall *TIcmpSendEcho)(
    HANDLE IcmpHandle,
    IPAddr DestinationAddress,
    LPVOID RequestData,
    WORD RequestSize,
    PIP_OPTION_INFORMATION RequestOptions,
    LPVOID ReplyBuffer,
    DWORD ReplySize,
    DWORD Timeout
  );
  TIcmpSendEcho _IcmpSendEcho;

public:
  enum TError {
    CantGetDLL,
    CantFindIcmpCreateFile,
    CantFindIcmpCloseHandle,
    CantFindIcmpSendEcho
  };

private:
  __fastcall TICMPDLL();
public:
  __fastcall ~TICMPDLL();

  static TICMPDLL* __fastcall Get();

  bool __fastcall Ping(const String IP, int Timeout = 1000);
  uint32_t __fastcall Ping(const String IP, uint32_t MessageId, int Timeout = 1000);
};
//---------------------------------------------------------------------------
#endif
