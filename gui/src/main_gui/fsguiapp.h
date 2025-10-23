/* ////////////////////////////////////////////////////////////

File Name: fsguiapp.h
Copyright (c) 2017 Soji Yamakawa.  All rights reserved.
http://www.ysflight.com

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS 
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//////////////////////////////////////////////////////////// */

#ifndef FSGUI_APP_IS_INCLUDED
#define FSGUI_APP_IS_INCLUDED
/* { */

#include <fsgui.h>
#include "profiledlg.h"
#include "runvm.h"

class FsGuiMainCanvas : public FsGuiCanvas
{
public:
	class ResumeVMDialog : public FsGuiDialog
	{
	public:
		FsGuiButton *btn;
		FsGuiMainCanvas *appPtr;
		void Make(FsGuiMainCanvas *appPtr);
		virtual void OnButtonClick(FsGuiButton *) override;
	};

	/*! Main menu.  MainMenu is created in MakeMainMenu function, which is called 
	    from the constructor.
	*/
	FsGuiPopUpMenu *mainMenu;
	FsGuiPopUpMenuItem *tapeImage,*tapeSaveImage,*fd0Image,*fd1Image;

	/*! Low-level interface, FsLazyWindow framework, checks for this value to see
	    if the application run-loop should be terminated.
	*/
	YSBOOL appMustTerminate;

	YSBOOL VMMustResume=YSFALSE;

private:
	/*! For convenience, you can use THISCLASS instead of FsGuiMainCanvas 
	    in the member functions.
	*/
	typedef FsGuiMainCanvas THISCLASS;

public:
	// [Core data structure]
	struct HumanReadable
	{
		unsigned int code;
		std::string label;
	};

	class GUIOptions
	{
	public:
		std::string profileDirSysENC;

		bool RecognizeArguments(int ac,char *av[]);
	};


	std::vector <std::string> VMLog;

	std::string profileDirSysENC; // In system encoding.  If empty, ~/Documents/Mutsu_FM77AV will be used.

	mutable YsWString lastSelectedProfileFName;
	mutable YsWString lastSelectedFDFName;
	mutable YsWString lastSelectedHDFName;
	mutable YsWString lastSelectedTapeFName;
	mutable YsWString lastScreenShotFName;
	mutable YsWString lastVGMFName;
	mutable YsWString lastEventFName;

	// separateProcess flag may only be changed when starting the VM.
	bool separateProcess=false;
	FM77AVVM VM;

	// pauseResumeKey is updated in ReallyRun function from TownsProfile.
	unsigned int pauseResumeKey=FSKEY_SCROLLLOCK;

	// [Modeless dialogs]
	//   (1) Add a pointer in the following chunk.
	//   (2) Add an initialization in the constructor of the aplication.
	//   (3) Add deletion in the destructor of the application.
	//   (4) Add RemoveDialog in Edit_ClearUIIMode
	ProfileDialog *profileDlg=nullptr;
	ResumeVMDialog *resumeVMDlg=nullptr;

	// [Modal dialogs]


	/*! Constructor is called after OpenGL context is created.
	    It is safe to make OpenGL function calls inside.
	*/
	FsGuiMainCanvas();

	/*! */
	~FsGuiMainCanvas();

	/*! This function is called from the low-level interface to get an
	    application pointer.
	*/
	static FsGuiMainCanvas *GetMainCanvas();

	/*! This funciion is called from the low-level interface for
	    deleting the application.
	*/
	static void DeleteMainCanvas(void);

	/*! Customize this function for adding menus.
	*/
	void MakeMainMenu(void);
	void DeleteMainMenu(void);

public:
	/*! In this function, shared GLSL renderers are created,
	    View-target is set to (-5,-5,-5) to (5,5,5),
	    and view distance is set to 7.5 by default.
	*/
	void Initialize(int argc,char *argv[]);

	/*! This function is called regularly from the low-level interface.
	*/
	void OnInterval(void);

	/*! This function is called from the low-level interface when the
	    window needs to be re-drawn.
	*/
	void Draw(void);


	/*! Start Mutsu_CUI with the profile set in the profileDlg.
	*/
	void Run(void);
	void SetUpImageMenu(const FM77AVParam &param);
	void ResetImageMenu(void);


	/*!
	*/
	bool IsVMRunning(void) const;

	/*!
	*/
	void SendVMCommand(std::string cmd);

	/*!
	*/
	void ResumeVMIfSameProc(void);

private:
	bool ReallyRun(bool usePipe=true);
	std::vector <YsWString> CheckMissingROMFiles(void) const;

	YsWString GetDefaultNewDiskImageFileName(void) const;
	YsWString GetDefaultOpenDiskImageFileName(void) const;
	YsWString GetDefaultNewHardDiskImageFileName(void) const;
	YsWString GetDefaultNewEventLogFileName(void) const;

public:
	static std::string GamePortEmulationTypeToHumanReadable(unsigned int emuType);
	static unsigned int HumanReadableToGamePortEmulationType(std::string);
	static std::vector <std::string> GetSelectableGamePortEmulationTypeHumanReadable(void);

private:
	// [Menu pointers for check marks]
	FsGuiPopUpMenu *fileRecentProfile;
	FsGuiRecentFiles recentProfile;
	void AddRecentlyUsedFile(const wchar_t wfn[]);
	void RefreshRecentlyUsedFileList(void);
	static void File_Recent(void *appPtr,FsGuiPopUpMenu *,FsGuiPopUpMenuItem *itm);
	YsWString GetRecentFileListFileName(void) const;

	// [Port][Button][Speed]
	enum
	{
		MAX_NUM_BUTTONS=8,
		NUM_AUTOSHOT_SPEED=4
	};
	FsGuiPopUpMenuItem *padAutoShot[2][MAX_NUM_BUTTONS][NUM_AUTOSHOT_SPEED];


	// [Menu call-backs]
	/*! Sample call-back functions.
	*/
	void VM_Not_Running_Error(void);
	void VM_Already_Running_Error(void);

	void File_SaveDefaultProfile(FsGuiPopUpMenuItem *);
	void File_SaveDefaultConfirm(FsGuiDialog *dlg,int returnCode);
	void File_ReloadDefaultProfile(FsGuiPopUpMenuItem *);
	YsWString GetDefaultProfileFileName(void) const;
	YsWString GetMutsuProfileDir(void) const;

	void SaveProfile(YsWString fName) const;
	bool LoadProfile(YsWString fName);

	void File_SaveProfile(FsGuiPopUpMenuItem *);
	void File_OpenProfile(FsGuiPopUpMenuItem *);
	void File_OpenProfile_FileSelected(FsGuiDialog *dlg,int returnCode);
	void File_SaveProfileAs(FsGuiPopUpMenuItem *);
	void File_SaveProfileAs_FileSelected(FsGuiDialog *dlg,int returnCode);
	void File_SaveProfileAs_OverwriteConfirm(FsGuiDialog *dlg,int returnCode);

	void File_Exit(FsGuiPopUpMenuItem *);
	void File_Exit_ConfirmExitCallBack(FsGuiDialog *,int);
	void File_Exit_ReallyExit(void);



	void View_OpenProfileDialog(FsGuiPopUpMenuItem *);


	YsWString lastStateFName;
	YsWString selectedStateFName;
	bool loadStateThenPause=false;
	void State_SaveState(FsGuiPopUpMenuItem *);
	void State_SaveState_FileSelected(FsGuiDialog *dlg,int returnCode);
	void State_SaveState_Confirm(FsGuiDialog *dlg,int returnCode);
	void State_SaveState_Save(YsWString fName);
	void State_LoadState(FsGuiPopUpMenuItem *);
	void State_LoadStateAndPause(FsGuiPopUpMenuItem *);
	void State_LoadState_FileSelected(FsGuiDialog *dlg,int returnCode);
	void State_LoadLastState(FsGuiPopUpMenuItem *);
	void State_LoadLastStateAndPause(FsGuiPopUpMenuItem *);
	void State_SaveStateMem0(FsGuiPopUpMenuItem *);
	void State_SaveStateMem1(FsGuiPopUpMenuItem *);
	void State_SaveStateMem2(FsGuiPopUpMenuItem *);
	void State_SaveStateMem3(FsGuiPopUpMenuItem *);
	void State_SaveStateMem4(FsGuiPopUpMenuItem *);
	void State_SaveStateMem5(FsGuiPopUpMenuItem *);
	void State_SaveStateMem6(FsGuiPopUpMenuItem *);
	void State_SaveStateMem7(FsGuiPopUpMenuItem *);
	void State_SaveStateMem8(FsGuiPopUpMenuItem *);
	void State_SaveStateMem9(FsGuiPopUpMenuItem *);
	void State_SaveStateMem(int slot);
	void State_LoadStateMem0(FsGuiPopUpMenuItem *);
	void State_LoadStateMem1(FsGuiPopUpMenuItem *);
	void State_LoadStateMem2(FsGuiPopUpMenuItem *);
	void State_LoadStateMem3(FsGuiPopUpMenuItem *);
	void State_LoadStateMem4(FsGuiPopUpMenuItem *);
	void State_LoadStateMem5(FsGuiPopUpMenuItem *);
	void State_LoadStateMem6(FsGuiPopUpMenuItem *);
	void State_LoadStateMem7(FsGuiPopUpMenuItem *);
	void State_LoadStateMem8(FsGuiPopUpMenuItem *);
	void State_LoadStateMem9(FsGuiPopUpMenuItem *);
	void State_LoadStateMem(int slot);


	bool genFloppyDisk=true;
	unsigned int genDiskSize=0;
	void File_New_320KB(FsGuiPopUpMenuItem *);
	void File_New_640KB(FsGuiPopUpMenuItem *);

	void File_New_FileSelected(FsGuiDialog *dlg,int returnCode);


	void VM_Start(FsGuiPopUpMenuItem *);
	void VM_StartAndCloseGUI(FsGuiPopUpMenuItem *);
	void VM_Reset(FsGuiPopUpMenuItem *);
	void VM_PowerOff(FsGuiPopUpMenuItem *);
	void VM_PowerOffConfirm(FsGuiDialog *dlg,int returnCode);
	void VM_Pause(FsGuiPopUpMenuItem *);
	void VM_Resume(FsGuiPopUpMenuItem *);

	void VM_FM8Speed(FsGuiPopUpMenuItem *);
	void VM_FM7Speed(FsGuiPopUpMenuItem *);
	void VM_FM77Speed(FsGuiPopUpMenuItem *);

	void VM_Keyboard_Direct(FsGuiPopUpMenuItem *);
	void VM_Keyboard_Translation1(FsGuiPopUpMenuItem *);
	void VM_Keyboard_Translation2(FsGuiPopUpMenuItem *);
	void VM_Keyboard_Translation3(FsGuiPopUpMenuItem *);

	void VM_Keyboard_AutoStop(FsGuiPopUpMenuItem *);
	void VM_Keyboard_AutoStop_Num5(FsGuiPopUpMenuItem *);
	void VM_Keyboard_AutoStop_ESC(FsGuiPopUpMenuItem *);
	void VM_Keyboard_AutoStop_BackSpace(FsGuiPopUpMenuItem *);

	void VM_SaveScreenshot(FsGuiPopUpMenuItem *);
	void VM_SaveScreenshot_FileSelected(FsGuiDialog *dlg,int returnCode);

	void Audio_Start_VGMCapture(FsGuiPopUpMenuItem *);
	void Audio_Stop_VGMCapture(FsGuiPopUpMenuItem *);
	void Audio_Save_VGMCapture(FsGuiPopUpMenuItem *);
	void Audio_Save_VGMCapture_FileSelected(FsGuiDialog *dlg,int returnCode);
	void Audio_Save_VGMCapture_Confirm(FsGuiDialog *dlgIn,int returnCode);
	void Audio_Save_VGMCapture_Save(YsWString fName);

	void Tape_SelectAlias(FsGuiPopUpMenuItem *item);
	void Tape_SelectSaveAlias(FsGuiPopUpMenuItem *item);
	void FD0_SelectAlias(FsGuiPopUpMenuItem *item);
	void FD1_SelectAlias(FsGuiPopUpMenuItem *item);


	bool selectSaveTape=false;
	void Tape_SelectImageFile(FsGuiPopUpMenuItem *);
	void Tape_SelectSaveImageFile(FsGuiPopUpMenuItem *);
	void Tape_ImageFileSelected(FsGuiDialog *dlg,int returnCode);
	void Tape_Eject(FsGuiPopUpMenuItem *);

	FsGuiPopUpMenuItem *FD0_writeProtectMenu,*FD0_writeUnprotectMenu;
	void FD0_SelectImageFile(FsGuiPopUpMenuItem *);
	void FD0_ImageFileSelected(FsGuiDialog *dlg,int returnCode);
	void FD0_WriteProtect(FsGuiPopUpMenuItem *);
	void FD0_WriteUnprotect(FsGuiPopUpMenuItem *);
	void FD0_Eject(FsGuiPopUpMenuItem *);

	FsGuiPopUpMenuItem *FD1_writeProtectMenu,*FD1_writeUnprotectMenu;
	void FD1_SelectImageFile(FsGuiPopUpMenuItem *);
	void FD1_ImageFileSelected(FsGuiDialog *dlg,int returnCode);
	void FD1_WriteProtect(FsGuiPopUpMenuItem *);
	void FD1_WriteUnprotect(FsGuiPopUpMenuItem *);
	void FD1_Eject(FsGuiPopUpMenuItem *);

	class GamePortDialog;
	void Device_GamePort0(FsGuiPopUpMenuItem *);
	void Device_GamePort1(FsGuiPopUpMenuItem *);
	void Device_GamePort_DeviceSelected(FsGuiDialog *dlg,int returnCode);
	void Device_AutoShot(FsGuiPopUpMenuItem *);

	void EventLog_StartRecording(FsGuiPopUpMenuItem *);
	void EventLog_EndRecording(FsGuiPopUpMenuItem *);
	void EventLog_MakeRepeat(FsGuiPopUpMenuItem *);
	void EventLog_Replay(FsGuiPopUpMenuItem *);
	void EventLog_Stop(FsGuiPopUpMenuItem *);
	void EventLog_Open(FsGuiPopUpMenuItem *);
	void EventLog_Open_FileSelected(FsGuiDialog *dlg,int returnCode);
	void EventLog_Save(FsGuiPopUpMenuItem *);
	void EventLog_Save_FileSelected(FsGuiDialog *dlg,int returnCode);
	void EventLog_Save_Confirm(FsGuiDialog *dlg,int returnCode);
	void EventLog_Save_Save(YsWString fName);



	void Help_About(FsGuiPopUpMenuItem *);
	void Help_Help(FsGuiPopUpMenuItem *);



	/* 1. Currently-open profile
	   2. Any known directory of the profile dialog, if the profile dialog is open + "*.Mutsu"
	   3. Any known directory of the file used by the VM + "*.Mutsu"
	   4. User's home directory + "*.Mutsu"
	*/
	YsWString GetDefaultOpenProfileFileName(void) const;

	/* 1. Last selected tape file name.
	   2. Any known directory + "*.T77"
	*/
	YsWString GetDefaultTapeImageFileName(void) const;

	/*! 1. lastStateFName
	    3. Any known directory of the file used by the VM + "*.7State"
	    4. User's home directory + "*.7State"
	*/
	YsWString GetDefaultVMStateFileName(void) const;

	/*! Any known directory of the file used by the VM + "*.png"
	*/
	YsWString GetDefaultScreenShotFileName(void) const;

	/*! Any known directory of the file used by the VM + "*.vgm"
	*/
	YsWString GetDefaultVGMFileName(void) const;

	/*!
	*/
	YsWString GetAnyKnownDirectory(void) const;
};

/* } */
#endif
