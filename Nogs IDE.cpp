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
#include <tchar.h>
//---------------------------------------------------------------------------
#include <Vcl.Styles.hpp>
#include <Vcl.Themes.hpp>














































USEFORM("SetupAllDlg.cpp", DialogSetupAll);
USEFORM("Wizards\ZeroConf\ZeroConfWizardDlg.cpp", DialogZeroConfWizard);
USEFORM("Wizards\ZeroConf\ZeroConfWizardAdaperFrm.cpp", FrameZeroConfWizardAdapter); /* TFrame: File Type */
USEFORM("SetupOneDlg.cpp", DialogSetupOne);
USEFORM("MainFrm.cpp", FormMain);
USEFORM("Dialogs\LiveCodingTargetsDlg.cpp", DialogLiveCodingTargets);
USEFORM("Dialogs\MemoryTrackerDlg.cpp", DialogMemoryTracker);
USEFORM("Dialogs\EtherBootProgrammerDlg.cpp", DialogEtherBootProgrammer);
USEFORM("Dialogs\CommunicationTestDlg.cpp", DialogCommunicationTest);
USEFORM("Dialogs\ModelessBaseDlg.cpp", DialogModeless);
USEFORM("Dialogs\ZeroConfDlg.cpp", DialogZeroConf);
USEFORM("Dialogs\TargetSettingsDlg.cpp", DialogTargetSettings);
USEFORM("Dialogs\ProjectDlg.cpp", DialogProject);
USEFORM("Dialogs\ProjectNewDlg.cpp", DialogProjectNew);
USEFORM("..\..\Librarys\CBuilderUC\MessageBoxDlg\MessageBoxDlg.cpp", DialogMessageBox);
USEFORM("Frames\TargetsFrm.cpp", FrameTargets); /* TFrame: File Type */
USEFORM("Frames\TopologieEditorFrm.cpp", FrameTopolgieEditor); /* TFrame: File Type */
USEFORM("Frames\SysLog.cpp", FrameSysLog); /* TFrame: File Type */
USEFORM("Frames\Setup\SetupBaseFrm.cpp", FrameSetupBase); /* TFrame: File Type */
USEFORM("Frames\Setup\SetupSysLogFrm.cpp", FrameSetupSysLog); /* TFrame: File Type */
USEFORM("Frames\ProjectFrm.cpp", FrameProject); /* TFrame: File Type */
USEFORM("EtherBootFrm.cpp", FrameEtherBoot); /* TFrame: File Type */
USEFORM("Frames\LuaEditorFrm.cpp", FrameLuaEditor); /* TFrame: File Type */
USEFORM("Frames\NogsDesigner.cpp", FrameDesigner); /* TFrame: File Type */
USEFORM("Frames\LogFrm.cpp", FrameLog); /* TFrame: File Type */
USEFORM("Frames\BrowserFrm.cpp", FrameBrowser); /* TFrame: File Type */
USEFORM("Frames\CommunicationHopFrm.cpp", FrameCommunicationHop); /* TFrame: File Type */
//---------------------------------------------------------------------------
WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
  try
  {
     Application->Initialize();
     Application->MainFormOnTaskBar = true;
     TStyleManager::TrySetStyle("Carbon");
     Application->CreateForm(__classid(TFormMain), &FormMain);
     Application->Run();
  }
  catch (Exception &exception)
  {
     Application->ShowException(&exception);
  }
  catch (...)
  {
     try
     {
       throw Exception("");
     }
     catch (Exception &exception)
     {
       Application->ShowException(&exception);
     }
  }
  return 0;
}
//---------------------------------------------------------------------------
