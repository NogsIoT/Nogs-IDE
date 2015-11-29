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
#ifndef TargetH
#define TargetH
#include <System.IniFiles.hpp>
//---------------------------------------------------------------------------
#pragma pack(push, 1)
struct TFFSPacket {
  uint32_t Command;     // See enum TFFSCommand
  int32_t Parameter;    // Parameter for the command, mostly the sector number
  uint8_t Data[512];    // Size of one sector
};
#pragma pack(pop)
//---------------------------------------------------------------------------
class TIndicator;
class TFatFsImage;
class TTarget
{
public:
  enum TNodeType {
    PN = 1,
    CN = 2,
    SN = 3
  };
  enum TAutomaticUploadMode {
    aumNone,
    aumFFS,
    aumTFTP
  };
  enum TFFSCommand {
    ffscCreate = 1000,
    ffscRead = 1001,
    ffscWrite = 1002
  };
private:
  TListItem *ListItem;
  TIndicator *Indicator;
  String Startup;
  String Type;
  TDateTime FirstTime;
  TDateTime LastTime;
  TAutomaticUploadMode AutomaticUploadMode;
  String ProjectPath;
  TFatFsImage *FatFsImage;
  TTreeNode *TreeNode;
public:
  __fastcall TTarget(TListItem *_ListItem, const String _Startup, const String _Type);
  __fastcall TTarget(TListItem *_ListItem, TIniFile *Ini, const String Section);
  __fastcall ~TTarget();

  void __fastcall Save(TIniFile *Ini, const String Section);

  static TTarget* __fastcall Get(TListItem *ListItem);

  TNodeType __fastcall GetNodeType();

  TIndicator* __fastcall GetIndicator();
  void __fastcall SetIndicator(TIndicator *_Indicator);

  String __fastcall GetStartup();

  void __fastcall SetLastUsage();
  void __fastcall SetLastUsage(TDateTime Time);
  TDateTime __fastcall GetFirstTime();
  TDateTime __fastcall GetLastTime();

  TAutomaticUploadMode __fastcall GetAutomaticUploadMode();
  void __fastcall SetAutomaticUploadMode(TAutomaticUploadMode _AutomaticUploadMode);

  String __fastcall GetProjectPath(bool FallBack);
  void __fastcall SetProjectPath(const String Path);

  int __fastcall HandleFFS(TFFSPacket *FFSPacket);
  TStream* __fastcall HandleTFTP(const String Path);
  void __fastcall HandleTFTP(TStream *Stream, bool Success);
};
//---------------------------------------------------------------------------
#endif
