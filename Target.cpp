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
#include "Target.h"
#include "MainFrm.h"
#include "FatFsImageBuilder.h"
//---------------------------------------------------------------------------
#define FFS_SERVER_ERROR_NOT_ALLOWED -101
#define FFS_SERVER_ERROR_NO_PROCECT  -102
//---------------------------------------------------------------------------
static double AToF(AnsiString S)
{
  int P = S.Pos(",");
  if (P) S[P] = '.';
  return atof(S.c_str());
}
//---------------------------------------------------------------------------
// TTarget
//---------------------------------------------------------------------------
__fastcall TTarget::TTarget(TListItem *_ListItem, const String _Startup, const String _Type):
  ListItem(_ListItem), Startup(_Startup), Type(_Type), Indicator(0), AutomaticUploadMode(TTarget::aumFFS), FatFsImage(0)
{
  FirstTime = LastTime = Now();
}
//---------------------------------------------------------------------------
__fastcall TTarget::TTarget(TListItem *_ListItem, TIniFile *Ini, const String Section):
  ListItem(_ListItem), Indicator(0), FatFsImage(0)
{
  Startup = Ini->ReadString(Section, "Startup", EmptyStr);
  Type = Ini->ReadString(Section, "Type", EmptyStr);
  FirstTime = AToF(Ini->ReadString(Section, "FirstTime", Now()));
  LastTime = AToF(Ini->ReadString(Section, "LastTime", FirstTime));
  AutomaticUploadMode = Ini->ReadInteger(Section, "AutomaticUploadMode", TTarget::aumFFS);
  ProjectPath = Ini->ReadString(Section, "ProjectPath", EmptyStr);
}
//---------------------------------------------------------------------------
__fastcall TTarget::~TTarget()
{
  delete Indicator;
  delete FatFsImage;
}
//---------------------------------------------------------------------------
void __fastcall TTarget::Save(TIniFile *Ini, const String Section)
{
  Ini->WriteString(Section, "Startup", Startup);
  Ini->WriteString(Section, "Type", Type);
  Ini->WriteFloat(Section, "FirstTime", FirstTime);
  Ini->WriteFloat(Section, "LastTime", LastTime);
  Ini->WriteInteger(Section, "AutomaticUploadMode", AutomaticUploadMode);
  Ini->WriteString(Section, "ProjectPath", ProjectPath);
}
//---------------------------------------------------------------------------
TTarget* __fastcall TTarget::Get(TListItem *ListItem)
{
  return (TTarget*) ListItem->Data;
}
//---------------------------------------------------------------------------
TIndicator* __fastcall TTarget::GetIndicator()
{
  return Indicator;
}
//---------------------------------------------------------------------------
void __fastcall TTarget::SetIndicator(TIndicator *_Indicator)
{
  Indicator = _Indicator;
}
//---------------------------------------------------------------------------
TTarget::TNodeType __fastcall TTarget::GetNodeType()
{
  if (Type == "PN") return PN;
  if (Type == "CN") return CN;
  if (Type == "SN") return SN;
  return 0;
}
//---------------------------------------------------------------------------
String __fastcall TTarget::GetStartup()
{
  return Startup;
}
//---------------------------------------------------------------------------
void __fastcall TTarget::SetLastUsage()
{
  LastTime = Now();
}
//---------------------------------------------------------------------------
void __fastcall TTarget::SetLastUsage(TDateTime Time)
{
  LastTime = Time;
}
//---------------------------------------------------------------------------
TDateTime __fastcall TTarget::GetFirstTime()
{
  return FirstTime;
}
//---------------------------------------------------------------------------
TDateTime __fastcall TTarget::GetLastTime()
{
  return LastTime;
}
//---------------------------------------------------------------------------
TTarget::TAutomaticUploadMode __fastcall TTarget::GetAutomaticUploadMode()
{
  return AutomaticUploadMode;
}
//---------------------------------------------------------------------------
void __fastcall TTarget::SetAutomaticUploadMode(TTarget::TAutomaticUploadMode _AutomaticUploadMode)
{
  AutomaticUploadMode = _AutomaticUploadMode;
}
//---------------------------------------------------------------------------
String __fastcall TTarget::GetProjectPath(bool FallBack)
{
  TStringList *ProjectPathes = FormMain->FrameProject->GetProjects();
  int Index = ProjectPathes->IndexOf(ProjectPath);
  if (ProjectPath.IsEmpty() || (Index < 0)) {
    return FallBack && ProjectPathes->Count ? ProjectPathes->Strings[0] : EmptyStr;
  }
  return ProjectPath;
}
//---------------------------------------------------------------------------
void __fastcall TTarget::SetProjectPath(const String Path)
{
  ProjectPath = Path;
}
//---------------------------------------------------------------------------
int __fastcall TTarget::HandleFFS(TFFSPacket *FFSPacket)
{
  switch (FFSPacket->Command) {

    case ffscCreate :
      {
        String ProjectPath = GetProjectPath(true);
        if (AutomaticUploadMode != aumFFS) {
          FFSPacket->Parameter = FFS_SERVER_ERROR_NOT_ALLOWED;
          return 8;
        }
        if (!DirectoryExists(ProjectPath)) {
          FFSPacket->Parameter = FFS_SERVER_ERROR_NO_PROCECT;
          return 8;
        }
        TFatFsImageBuilder *FatFsImageBuilder = TFatFsImageBuilder::Acquire();
        if (!FatFsImageBuilder)
          return 0;
        SetIndicator(new TIndicatorFadeOut("FFS Create image", clBlue));
        int Blocks = FatFsImageBuilder->BuildFromDirectory(ProjectPath);
        FormMain->Log(0, "FFS: Create FAT image with " + String(Blocks) + " blocks from " + ProjectPath);
        FatFsImage = FatFsImageBuilder->Clone();
        FatFsImageBuilder->Release();
        FFSPacket->Parameter = Blocks;
        return 8;
      }

    case ffscRead :
      {
        //FormMain->Log(0, "FFS: Read " + String(FFSPacket.Parameter));
        SetIndicator(new TIndicatorFadeOut("FFS Read", (TColor) RGB(128, 255, 0)));
        BYTE *Sector = FatFsImage->GetSectorAddress(FFSPacket->Parameter);
        if (!Sector)
          return 0;
        memcpy(FFSPacket->Data, Sector, 512);
        return 8 + 512;
      }

    case ffscWrite :
      {
        //FormMain->Log(0, "FFS: Write " + String(FFSPacket.Parameter));
        SetIndicator(new TIndicatorFadeOut("FFS Write", (TColor) RGB(255, 128, 0)));
        BYTE *Sector = FatFsImage->GetSectorAddress(FFSPacket->Parameter);
        if (!Sector)
          return 0;
        memcpy(Sector, FFSPacket->Data, 512);
        return 8;
      }
      break;

  }
  return 0;
}
//---------------------------------------------------------------------------
TStream* __fastcall TTarget::HandleTFTP(const String Path)
{
  if (AutomaticUploadMode != aumTFTP) 
    return 0;

  TFatFsImageBuilder *FatFsImageBuilder = TFatFsImageBuilder::Acquire();
  if (!FatFsImageBuilder)
    return 0;
  String ProjectPath = GetProjectPath(true);
  SetIndicator(new TIndicatorFadeOut("TFTP Create image", clBlue));
  TStream *Stream = FatFsImageBuilder->BuildFromDirectory(ProjectPath, 0);
  TreeNode = FormMain->Log(0, "TFTP: Create FAT image with " + String(FatFsImageBuilder->UsedSectors()) + " blocks from " + ProjectPath);
  FatFsImageBuilder->Release(); 
  SetIndicator(new TIndicatorStatic("TFTP Upload", (TColor) RGB(255, 255, 0)));
  return Stream;
}
//---------------------------------------------------------------------------
void __fastcall TTarget::HandleTFTP(TStream *Stream, bool Success)
{
  FormMain->Log(TreeNode, Success ? 1 : 3, Success ? "TFTP: Transfer of " + String(Stream->Size) + " bytes complete" : String("TFTP: Transfer error!"));
  TreeNode->Expand(true);
  SetIndicator(0);
}
//---------------------------------------------------------------------------
#pragma package(smart_init)

