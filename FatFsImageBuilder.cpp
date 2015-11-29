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
#include <System.Classes.hpp>
#include "FatFsImageBuilder.h"
#include <stdio.h>
#include <wchar.h>

#define MIN_FAT16 4086U   /* Minimum number of clusters for FAT16 */
#define MIN_FAT32 65526U  /* Minimum number of clusters for FAT32 */

#define BPB_NumFATs			16		/* Number of FAT copies (1) */
#define BPB_RootEntCnt	17		/* Number of root directory entries for FAT12/16 (2) */
#define BPB_TotSec16		19		/* Volume size [sector] (2) */
#define BPB_FATSz16			22		/* FAT size [sector] (2) */
#define BPB_TotSec32		32		/* Volume size [sector] (4) */
#define BPB_FATSz32			36		/* FAT size [sector] (4) */
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
class TMemoryStreamWithCGauge : public TMemoryStream
{
  TCGauge *CGauge;
public:
  __fastcall TMemoryStreamWithCGauge(TCGauge *_CGauge):
    CGauge(_CGauge), TMemoryStream()
  {}
	virtual int __fastcall Read(void *Buffer, int Count)
  {
    int Result = TMemoryStream::Read(Buffer, Count);
    if (CGauge) {
      CGauge->MaxValue = Size;
      CGauge->Progress = Position;
    }
    return Result;
  }
	virtual int __fastcall Write(const void *Buffer, int Count)
  {
    int Result = TMemoryStream::Write(Buffer, Count);
    if (CGauge) {
      CGauge->MaxValue = Size;
      CGauge->Progress = Position;
    }
    return Result;
  }
};
//---------------------------------------------------------------------------
// TFatFsImage
//---------------------------------------------------------------------------
__fastcall TFatFsImage::TFatFsImage():
  Sectors(0), SectorsCount(0)
{}
//---------------------------------------------------------------------------
__fastcall TFatFsImage::~TFatFsImage()
{
  delete [] Sectors;
}
//---------------------------------------------------------------------------
void __fastcall TFatFsImage::Clear()
{
  memset(Sectors, 0, SectorsCount * GetSectorSize());
}
//---------------------------------------------------------------------------
void __fastcall TFatFsImage::Init(int _SectorsCount)
{
  delete Sectors;
  SectorsCount = _SectorsCount;
  Sectors = new TSector[SectorsCount];
  Clear();
}
//---------------------------------------------------------------------------
int __fastcall TFatFsImage::GetSectorSize()
{
  return sizeof(TSector);
}
//---------------------------------------------------------------------------
int __fastcall TFatFsImage::GetSectorsCount()
{
  return SectorsCount;
}
//---------------------------------------------------------------------------
BYTE* __fastcall TFatFsImage::GetSectorAddress(int Sector)
{
  return (Sector >= 0) && (Sector < SectorsCount) ? Sectors[Sector] : 0;
}
//---------------------------------------------------------------------------
bool __fastcall TFatFsImage::IsEmptySector(int Sector)
{
  BYTE *Byte = GetSectorAddress(Sector);
  for (int I = 0; I < GetSectorSize();I++) {
    if (*Byte++) return false;
  }
  return true;
}
//---------------------------------------------------------------------------
int __fastcall TFatFsImage::UsedSectors()
{
  int Used = 0;
  for (int I = 0; I < SectorsCount; I++)
    if (!IsEmptySector(I))
      Used++;
  return Used;
}
//---------------------------------------------------------------------------
// TFatFsImageBuilder
//---------------------------------------------------------------------------
TFatFsImageBuilder *FatFsImageBuilder = 0;
//---------------------------------------------------------------------------
extern "C" int GetSectorCount()
{
  return FatFsImageBuilder->GetSectorsCount();
}
//---------------------------------------------------------------------------
extern "C" BYTE* GetSectorAddress(int Sector)
{
  return FatFsImageBuilder->GetSectorAddress(Sector);
}
//---------------------------------------------------------------------------
void __fastcall TFatFsImageBuilder::Clear()
{
  TFatFsImage::Clear();
  f_mount(&FatFs, "", 0);
  int Result = f_mkfs("", 1, 512);
  if (Result)
    throw Result;
  f_setlabel("image");
}
//---------------------------------------------------------------------------
__fastcall TFatFsImageBuilder::TFatFsImageBuilder():
  TFatFsImage(), Acquired(false)
{}
//---------------------------------------------------------------------------
TFatFsImageBuilder* __fastcall TFatFsImageBuilder::Acquire()
{
  if (!FatFsImageBuilder)
    FatFsImageBuilder = new TFatFsImageBuilder;
  if (FatFsImageBuilder->Acquired)
    return 0;
  FatFsImageBuilder->Acquired = true;
  return FatFsImageBuilder;
}
//---------------------------------------------------------------------------
void __fastcall TFatFsImageBuilder::Release()
{
  if (FatFsImageBuilder) FatFsImageBuilder->Acquired = false;
}
//---------------------------------------------------------------------------
void __fastcall TFatFsImageBuilder::ShowDirectory(const AnsiString Path, TTreeView *TreeView, TTreeNode *TreeNode)
{
  if (!TreeNode) {
    char S[12];
    f_getlabel("", S, 0);
    TreeNode = TreeView->Items->Add(0, S[0] ? AnsiString(S) : Path);
  }
  FILINFO fno;
  char lfn[_MAX_LFN + 1];
  fno.lfname = lfn;
  fno.lfsize = sizeof lfn;
  DIR dir;
  if (f_opendir(&dir, Path.c_str()) == FR_OK) {
    for (;;) {
      if (f_readdir(&dir, &fno) != FR_OK || fno.fname[0] == 0) break;
      TTreeNode *TreeNode2 = TreeView->Items->AddChild(TreeNode, fno.lfname[0] ? fno.lfname : fno.fname);
      if (fno.fattrib & AM_DIR)
        ShowDirectory(Path + "/" + fno.fname, TreeView, TreeNode2);
    }
    f_closedir(&dir);
  }
}
//---------------------------------------------------------------------------
int __fastcall TFatFsImageBuilder::CopyToImage(const String SourecePath, const AnsiString DestinationPath)
{
  int X = UsedSectors();
  if (SourecePath.IsEmpty()) 
    return 0;
  TSearchRec SearchRec;
  if (FindFirst(SourecePath + "\\*.*", faAnyFile, SearchRec) == 0) {
    do {
      String SourecePath2 = SourecePath + "\\" + SearchRec.Name;
      AnsiString DetinationPath2 = DestinationPath + "\\" + AnsiString(SearchRec.Name);
      if (SearchRec.Attr & faDirectory) {

        AnsiString DirectoryName = SearchRec.Name;
        if ((DirectoryName[1] != '.') && (f_mkdir(DetinationPath2.c_str()) == FR_OK))
          CopyToImage(SourecePath + "\\" + SearchRec.Name, DestinationPath + "/" + DirectoryName);

      } else {

        HANDLE SrcFile = CreateFile(SourecePath2.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
        if (SrcFile != INVALID_HANDLE_VALUE) {
          FIL DstFile;
          int Result = f_open(&DstFile, DetinationPath2.c_str(), FA_CREATE_ALWAYS | FA_WRITE);
          if (Result == FR_OK) {
            for (;;) {
              char Buffer[4096];
              DWORD ReadCount;
              ReadFile(SrcFile, Buffer, sizeof(Buffer), &ReadCount, 0);
              if (!ReadCount) break;
              UINT WriteCount;
              f_write(&DstFile, Buffer, (UINT) ReadCount, &WriteCount);
            }
            f_close(&DstFile);
            CloseHandle(SrcFile);
          }
        }

      }
    } while (FindNext(SearchRec) == 0);
    FindClose(SearchRec);
  }
  return UsedSectors();
}
//---------------------------------------------------------------------------
void __fastcall TFatFsImageBuilder::Crunch()
{
  DWORD szf, szfp, edf, edfp;
  DWORD szd, szdp, edd, eddp;

  DWORD szvol = LD_WORD(Sectors[0] + BPB_TotSec16);
  if (!szvol) szvol = LD_DWORD(Sectors[0] + BPB_TotSec32);

  /* Truncate unused root directory entries */
  if (FatFs.fs_type != FS_FAT32) {
    // Count the number of root directory entrys
    DWORD DirectoryEntrys = 0;
    for (int I = 0; I < FatFs.n_rootdir; I++) {
      //if (RamDisk[FatFs.dirbase * 512 + I * 32]) DirectoryEntrys = I + 1;
      if (Sectors[FatFs.dirbase + I / 16][(I % 16) * 32]) DirectoryEntrys = I + 1;
    }
    szd = (DirectoryEntrys + 15) / 16;
    szdp = FatFs.n_rootdir / 16;
    if (szd < szdp) {
      edd = FatFs.dirbase + szd;
      eddp = FatFs.database;
      //MoveMemory(RamDisk + (edd * 512), RamDisk + (eddp * 512), (szvol - eddp) * 512);
      for (int I = 0; I < szvol - eddp; I++)
        memcpy(Sectors[edd + I], Sectors[eddp + I], 512);
      szvol -= szdp - szd;
      FatFs.database -= szdp - szd;
      ST_WORD(Sectors[0] + BPB_RootEntCnt, szd * 16);
    }
  }
}
//---------------------------------------------------------------------------
int __fastcall TFatFsImageBuilder::BuildFromDirectory(const String SourcePath)
{
  Clear();
  return CopyToImage(SourcePath, "0:");
}
//---------------------------------------------------------------------------
TStream* __fastcall TFatFsImageBuilder::BuildFromDirectory(const String SourcePath, TCGauge *CGauge)
{
  Clear();
  int N = CopyToImage(SourcePath, "0:");
  int Count = SectorsCount;
  for (int I = SectorsCount - 1;I >= 0;I--) {
    if (!IsEmptySector(I)) {
      Count = I + 1;
      break;
    }
  }
  TMemoryStreamWithCGauge *Stream = new TMemoryStreamWithCGauge(CGauge);
  Stream->Write(Sectors, Count * GetSectorSize());
  Stream->Position = 0;
  return Stream;
}
//---------------------------------------------------------------------------
TFatFsImage* __fastcall TFatFsImageBuilder::Clone()
{
  TFatFsImage *FatFsImage = new TFatFsImage;
  FatFsImage->Init(SectorsCount);
  memcpy(FatFsImage->GetSectorAddress(0), Sectors, SectorsCount * GetSectorSize());
  return FatFsImage;
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
