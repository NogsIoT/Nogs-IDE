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
#ifndef FatFsImageBuilderH
#define FatFsImageBuilderH
extern "C" {
  #include "FatFs-0-10-c/integer.h"
  #include "FatFs-0-10-c/ff.h"
}
#include "Vcl.ComCtrls.hpp"
#include "cgauges.h"
//---------------------------------------------------------------------------
class TFatFsImage
{
public:
  typedef BYTE TSector[512];

protected:
  TSector *Sectors;
  int SectorsCount;

public:
  __fastcall TFatFsImage();
  virtual __fastcall ~TFatFsImage();
  void __fastcall Clear();
  void __fastcall Init(int _SectorsCount);

  int __fastcall GetSectorSize();
  int __fastcall GetSectorsCount();

  BYTE* __fastcall GetSectorAddress(int Sector);

  bool __fastcall IsEmptySector(int Sector);
  int __fastcall UsedSectors();
};
//---------------------------------------------------------------------------
class TFatFsImageBuilder : public TFatFsImage
{
  FATFS FatFs;
  bool Acquired;
  __fastcall TFatFsImageBuilder();
public:
  static TFatFsImageBuilder* __fastcall Acquire();
  static void __fastcall Release();

  void __fastcall Clear();

  void __fastcall ShowDirectory(const AnsiString Path, TTreeView *TreeView, TTreeNode *TreeNode);
  int __fastcall CopyToImage(const String SourecePath, const AnsiString DestinationPath);
  void __fastcall Crunch();

  int __fastcall BuildFromDirectory(const String SourcePath);
  TStream* __fastcall BuildFromDirectory(const String SourcePath, TCGauge *CGauge);

  TFatFsImage* __fastcall Clone();
};
//---------------------------------------------------------------------------
extern "C" {
  int GetSectorCount();
  BYTE* GetSectorAddress(int Sector);
}
//---------------------------------------------------------------------------
#endif
