#include <iostream>
#include <string>
#include <set>
#include "profiledlg.h"
#include "fsguiapp.h"

#include "ym2612.h"



static std::string hotKeyAssignable[]=
{
	"NULL",
	"SPACE",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"ESC",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
	"PRINTSCRN",
	"CAPSLOCK",
	"SCROLLLOCK",
	"PAUSEBREAK",
	"BS",
	"TAB",
	"ENTER",
	"SHIFT",
	"CTRL",
	"ALT",
	"INS",
	"DEL",
	"HOME",
	"END",
	"PAGEUP",
	"PAGEDOWN",
	"UP",
	"DOWN",
	"LEFT",
	"RIGHT",
	"NUMLOCK",
	"TILDA",
	"MINUS",
	"PLUS",
	"LBRACKET",
	"RBRACKET",
	"BACKSLASH",
	"SEMICOLON",
	"SINGLEQUOTE",
	"COMMA",
	"DOT",
	"SLASH",
};

static std::string hotKeyFunc[][2]=
{
	{"None",""},
	{"Quick Screenshot","QSS"},
	{"Quick State Save","QSAVE"},
	{"Quick State Load","QLOAD"},
	{"No Wait","NOWAIT"},
	{"Yes Wait","YESWAIT"},
};




ProfileDialog::ProfileDialog(FsGuiMainCanvas *canvasPtr)
{
	this->canvasPtr=canvasPtr;
}

void ProfileDialog::Make(void)
{
	const int nShowPath=PATH_SHOW;

	profileFNameTxt=AddStaticText(0,FSKEY_NULL,L"PROFILE",YSFALSE);

	tab=AddTabControl(0,FSKEY_NULL,YSTRUE);

	int mainTabId=0;
	{
		auto tabId=AddTab(tab,"Main");
		mainTabId=tabId;
		BeginAddTabItem(tab,tabId);

		ROMDirBtn=AddTextButton(0,FSKEY_NULL,FSGUI_PUSHBUTTON,"ROM dir:",YSTRUE);
		ROMDirTxt=AddTextBox(0,FSKEY_NULL,FsGuiTextBox::HORIZONTAL,"",nShowPath,YSFALSE);
		ROMDirTxt->SetLengthLimit(PATH_LENGTH);
		AddStaticText(0,FSKEY_NULL,"(When browsing for the ROM dir, please select one of the ROM files.)",YSTRUE);

		TapeImgBtn=AddTextButton(0,FSKEY_NULL,FSGUI_PUSHBUTTON,"Tape Image:",YSTRUE);
		TapeImgTxt=AddTextBox(0,FSKEY_NULL,FsGuiTextBox::HORIZONTAL,"",nShowPath,YSFALSE);
		TapeImgTxt->SetLengthLimit(PATH_LENGTH);

		TapeSaveImgBtn=AddTextButton(0,FSKEY_NULL,FSGUI_PUSHBUTTON,"Tape Image (Save):",YSTRUE);
		TapeSaveImgTxt=AddTextBox(0,FSKEY_NULL,FsGuiTextBox::HORIZONTAL,"",nShowPath,YSFALSE);
		TapeSaveImgTxt->SetLengthLimit(PATH_LENGTH);

		AutoStartTapeBtn=AddTextButton(0,FSKEY_NULL,FSGUI_CHECKBOX,"Auto Start Tape Program",YSTRUE);

		FDImgBtn[0]=AddTextButton(0,FSKEY_NULL,FSGUI_PUSHBUTTON,"FD0:",YSTRUE);
		FDWriteProtBtn[0]=AddTextButton(0,FSKEY_NULL,FSGUI_CHECKBOX,"Write Protect",YSFALSE);
		FDImgTxt[0]=AddTextBox(0,FSKEY_NULL,FsGuiTextBox::HORIZONTAL,"",nShowPath-24,YSFALSE);
		FDImgTxt[0]->SetLengthLimit(PATH_LENGTH);

		FDImgBtn[1]=AddTextButton(0,FSKEY_NULL,FSGUI_PUSHBUTTON,"FD1:",YSTRUE);
		FDWriteProtBtn[1]=AddTextButton(0,FSKEY_NULL,FSGUI_CHECKBOX,"Write Protect",YSFALSE);
		FDImgTxt[1]=AddTextBox(0,FSKEY_NULL,FsGuiTextBox::HORIZONTAL,"",nShowPath-24,YSFALSE);
		FDImgTxt[1]->SetLengthLimit(PATH_LENGTH);

		scrnScaleTxt=AddTextBox(0,FSKEY_NULL,FsGuiTextBox::HORIZONTAL,L"Scaling(%):",4,YSTRUE);
		scrnScaleTxt->SetLengthLimit(PATH_LENGTH);
		scrnScaleTxt->SetInteger(100);

		scrnAutoScaleBtn=AddTextButton(0,FSKEY_NULL,FSGUI_CHECKBOX,"Auto Scaling (Fit to Window Size)",YSTRUE);
		scrnModeDrp=AddEmptyDropList(0,FSKEY_NULL,"",4,20,20,YSFALSE);
		scrnModeDrp->AddString("NORMAL WINDOW",YSTRUE);
		scrnModeDrp->AddString("MAXIMIZE",YSFALSE);
		scrnModeDrp->AddString("FULL SCREEN",YSFALSE);

		AddStaticText(0,FSKEY_NULL,"Model:",YSTRUE);

		machineTypeDrp=AddEmptyDropList(0,FSKEY_NULL,"",20,20,20,YSFALSE);
		machineTypeDrp->AddString("Unspecified",YSTRUE);
		machineTypeDrp->AddString("AUTO",YSFALSE);
		machineTypeDrp->AddString(MachineTypeToStr(MACHINETYPE_FM7).c_str(),YSFALSE);
		machineTypeDrp->AddString(MachineTypeToStr(MACHINETYPE_FM77AV).c_str(),YSFALSE);
		machineTypeDrp->AddString(MachineTypeToStr(MACHINETYPE_FM77AV40).c_str(),YSFALSE);


		startUpStateFNameBtn=AddTextButton(0,FSKEY_NULL,FSGUI_PUSHBUTTON,"Load VM State",YSTRUE);
		startUpStateFNameTxt=AddTextBox(0,FSKEY_NULL,FsGuiTextBox::HORIZONTAL,"",nShowPath,YSFALSE);

		EndAddTabItem();
	}

	{
		auto tabId=AddTab(tab,"GamePort");
		BeginAddTabItem(tab,tabId);

		for(int i=0; i<2; ++i)
		{
			std::string label="Game Port ";
			label.push_back('0'+i);
			label.push_back(':');

			AddStaticText(0,FSKEY_NULL,label.c_str(),YSTRUE);
			gamePortDrp[i]=AddEmptyDropList(0,FSKEY_NULL,"",16,40,40,YSTRUE);
			for(auto label : FsGuiMainCanvas::GetSelectableGamePortEmulationTypeHumanReadable())
			{
				gamePortDrp[i]->AddString(label.c_str(),YSFALSE);
			}

			AddStaticText(0,FSKEY_NULL,"Max Button-Hold Time(ms) ",YSTRUE);
			maxButtonHoldTimeTxt[i][0]=AddTextBox(0,FSKEY_NULL,FsGuiTextBox::HORIZONTAL,L"Btn0:",4,YSFALSE);
			maxButtonHoldTimeTxt[i][1]=AddTextBox(0,FSKEY_NULL,FsGuiTextBox::HORIZONTAL,L"Btn1:",4,YSFALSE);
		}

		AddStaticText(0,FSKEY_NULL,
			"Max Button-Hold Time will help you make a single click.  Some games are virtually unplayable\n"
			"because you end up making unwanted menu selection unless you really release the button quickly\n"
			"after pressing. By setting non-zero value in Max Button-Hold Time, Mutsu virtually releases\n"
			"the mouse/pad button after the specified time."
			,
			YSTRUE);

		gamePortDrp[0]->SelectByString(FsGuiMainCanvas::GamePortEmulationTypeToHumanReadable(FM77AV_GAMEPORTEMU_PHYSICAL0).c_str());
		gamePortDrp[1]->SelectByString(FsGuiMainCanvas::GamePortEmulationTypeToHumanReadable(FM77AV_GAMEPORTEMU_MOUSE).c_str());

		EndAddTabItem();
	}

	{
		auto tabId=AddTab(tab,"Keyboard1");
		BeginAddTabItem(tab,tabId);

		AddStaticText(0,FSKEY_NULL,"Auto Stop",YSTRUE);
		autoStopTypeDrp=AddEmptyDropList(0,FSKEY_NULL,"",8,32,32,YSTRUE);
		autoStopTypeDrp->AddString(FM77AVKeyboard::AutoStopToStr(FM77AVKeyboard::AUTOSTOP_NONE).c_str(),YSTRUE);
		autoStopTypeDrp->AddString(FM77AVKeyboard::AutoStopToStr(FM77AVKeyboard::AUTOSTOP_AFTER_NUM_RELEASE).c_str(),YSFALSE);
		autoStopTypeDrp->AddString(FM77AVKeyboard::AutoStopToStr(FM77AVKeyboard::AUTOSTOP_AFTER_ARROW_RELEASE).c_str(),YSFALSE);
		autoStopTypeDrp->AddString(FM77AVKeyboard::AutoStopToStr(FM77AVKeyboard::AUTOSTOP_AFTER_NUM_RELEASE_AND_RETYPE).c_str(),YSFALSE);
		autoStopTypeDrp->AddString(FM77AVKeyboard::AutoStopToStr(FM77AVKeyboard::AUTOSTOP_AFTER_ARROW_RELEASE_AND_RETYPE).c_str(),YSFALSE);
		autoStopTypeDrp->AddString(FM77AVKeyboard::AutoStopToStr(FM77AVKeyboard::AUTOSTOP_AFTER_ANY_KEY_RELEASE).c_str(),YSFALSE);

		AddStaticText(0,FSKEY_NULL,"Virtually Press This Key to Auto Stop",YSTRUE);
		autoStopKeyDrp=AddEmptyDropList(0,FSKEY_NULL,"",20,20,20,YSTRUE);
		for(int FM77AVKey=0; FM77AVKey<256; ++FM77AVKey)
		{
			auto str=FM77AVKeyCodeToKeyLabel(FM77AVKey);
			auto reverse=FM77AVKeyLabelToKeyCode(str);
			if(AVKEY_NULL!=reverse)
			{
				autoStopKeyDrp->AddString(str.c_str(),YSFALSE);
			}
		}
		autoStopKeyDrp->SelectByString("NUM_5");

		AddStaticText(0,FSKEY_NULL,"FM-7 seriew before FM77AV could not detect key-release.  Therefore, FM-7 users",YSTRUE);
		AddStaticText(0,FSKEY_NULL,"could start moving a character by pressing a key, but could not stop by releasing",YSTRUE);
		AddStaticText(0,FSKEY_NULL,"the key.  We typically pressed Num-5 to stop.  The emulator can solve this problem",YSTRUE);
		AddStaticText(0,FSKEY_NULL,"by virtually pressing a different key when num-keys or arrow-keys are released.",YSTRUE);

		EndAddTabItem();
	}

	{
		auto tabId=AddTab(tab,"Keyboard2");
		BeginAddTabItem(tab,tabId);

		AddStaticText(0,FSKEY_NULL,"Keyboard Mode (TRANS1:ESC->ESC+BREAK, TRANS2:ESC->ESC, TRANS3:ESC->BREAK):",YSTRUE);
		keyboardModeDrp=AddEmptyDropList(0,FSKEY_NULL,"",20,20,20,YSFALSE);
		for(int i=0; i<FM77AV_KEYBOARD_MODE_NUM_MODES; ++i)
		{
			keyboardModeDrp->AddString(KeyboardModeToStr(i).c_str(),YSFALSE);
		}

		AddStaticText(0,FSKEY_NULL,"Virtual Keys:",YSTRUE);
		for(int row=0; row<10; ++row)
		{
			virtualKeyFM77AVKeyDrp[row]=AddEmptyDropList(0,FSKEY_NULL,"",20,20,20,YSTRUE);
			virtualKeyFM77AVKeyDrp[row]->AddString("(None)",YSTRUE);
			for(int FM77AVKey=0; FM77AVKey<256; ++FM77AVKey)
			{
				auto str=FM77AVKeyCodeToKeyLabel(FM77AVKey);
				auto reverse=FM77AVKeyLabelToKeyCode(str);
				if(AVKEY_NULL!=reverse)
				{
					virtualKeyFM77AVKeyDrp[row]->AddString(str.c_str(),YSFALSE);
				}
			}

			AddStaticText(0,FSKEY_NULL,"JoystickID:",YSFALSE);
			virtualKeyPhysIdDrp[row]=AddEmptyDropList(0,FSKEY_NULL,"",8,4,4,YSFALSE);
			virtualKeyPhysIdDrp[row]->AddString("0",YSTRUE);
			virtualKeyPhysIdDrp[row]->AddString("1",YSFALSE);
			virtualKeyPhysIdDrp[row]->AddString("2",YSFALSE);
			virtualKeyPhysIdDrp[row]->AddString("3",YSFALSE);
			virtualKeyPhysIdDrp[row]->AddString("4",YSFALSE);
			virtualKeyPhysIdDrp[row]->AddString("5",YSFALSE);
			virtualKeyPhysIdDrp[row]->AddString("6",YSFALSE);
			virtualKeyPhysIdDrp[row]->AddString("7",YSFALSE);

			AddStaticText(0,FSKEY_NULL,"Button:",YSFALSE);
			virtualKeyButtonDrp[row]=AddEmptyDropList(0,FSKEY_NULL,"",8,4,4,YSFALSE);
			virtualKeyButtonDrp[row]->AddString("0",YSTRUE);
			virtualKeyButtonDrp[row]->AddString("1",YSFALSE);
			virtualKeyButtonDrp[row]->AddString("2",YSFALSE);
			virtualKeyButtonDrp[row]->AddString("3",YSFALSE);
			virtualKeyButtonDrp[row]->AddString("4",YSFALSE);
			virtualKeyButtonDrp[row]->AddString("5",YSFALSE);
			virtualKeyButtonDrp[row]->AddString("6",YSFALSE);
			virtualKeyButtonDrp[row]->AddString("7",YSFALSE);
			virtualKeyButtonDrp[row]->AddString("8",YSTRUE);
			virtualKeyButtonDrp[row]->AddString("9",YSFALSE);
			virtualKeyButtonDrp[row]->AddString("10",YSFALSE);
			virtualKeyButtonDrp[row]->AddString("11",YSFALSE);
			virtualKeyButtonDrp[row]->AddString("12",YSFALSE);
			virtualKeyButtonDrp[row]->AddString("13",YSFALSE);
			virtualKeyButtonDrp[row]->AddString("14",YSFALSE);
			virtualKeyButtonDrp[row]->AddString("15",YSFALSE);
		}

		EndAddTabItem();
	}
	
	{
		auto tabId=AddTab(tab,"Keyboard3");
		BeginAddTabItem(tab,tabId);
		
		AddStaticText(0,FSKEY_NULL,"Virtual Keys:",YSTRUE);
		for(int row=10; row<FM77AVProfile::MAX_NUM_VIRTUALKEYS; ++row)
		{
			virtualKeyFM77AVKeyDrp[row]=AddEmptyDropList(0,FSKEY_NULL,"",20,20,20,YSTRUE);
			virtualKeyFM77AVKeyDrp[row]->AddString("(None)",YSTRUE);
			for(int FM77AVKey=0; FM77AVKey<256; ++FM77AVKey)
			{
				auto str=FM77AVKeyCodeToKeyLabel(FM77AVKey);
				auto reverse=FM77AVKeyLabelToKeyCode(str);
				if(AVKEY_NULL!=reverse)
				{
					virtualKeyFM77AVKeyDrp[row]->AddString(str.c_str(),YSFALSE);
				}
			}

			AddStaticText(0,FSKEY_NULL,"JoystickID:",YSFALSE);
			virtualKeyPhysIdDrp[row]=AddEmptyDropList(0,FSKEY_NULL,"",8,4,4,YSFALSE);
			virtualKeyPhysIdDrp[row]->AddString("0",YSTRUE);
			virtualKeyPhysIdDrp[row]->AddString("1",YSFALSE);
			virtualKeyPhysIdDrp[row]->AddString("2",YSFALSE);
			virtualKeyPhysIdDrp[row]->AddString("3",YSFALSE);
			virtualKeyPhysIdDrp[row]->AddString("4",YSFALSE);
			virtualKeyPhysIdDrp[row]->AddString("5",YSFALSE);
			virtualKeyPhysIdDrp[row]->AddString("6",YSFALSE);
			virtualKeyPhysIdDrp[row]->AddString("7",YSFALSE);

			AddStaticText(0,FSKEY_NULL,"Button:",YSFALSE);
			virtualKeyButtonDrp[row]=AddEmptyDropList(0,FSKEY_NULL,"",8,4,4,YSFALSE);
			virtualKeyButtonDrp[row]->AddString("0",YSTRUE);
			virtualKeyButtonDrp[row]->AddString("1",YSFALSE);
			virtualKeyButtonDrp[row]->AddString("2",YSFALSE);
			virtualKeyButtonDrp[row]->AddString("3",YSFALSE);
			virtualKeyButtonDrp[row]->AddString("4",YSFALSE);
			virtualKeyButtonDrp[row]->AddString("5",YSFALSE);
			virtualKeyButtonDrp[row]->AddString("6",YSFALSE);
			virtualKeyButtonDrp[row]->AddString("7",YSFALSE);
			virtualKeyButtonDrp[row]->AddString("8",YSTRUE);
			virtualKeyButtonDrp[row]->AddString("9",YSFALSE);
			virtualKeyButtonDrp[row]->AddString("10",YSFALSE);
			virtualKeyButtonDrp[row]->AddString("11",YSFALSE);
			virtualKeyButtonDrp[row]->AddString("12",YSFALSE);
			virtualKeyButtonDrp[row]->AddString("13",YSFALSE);
			virtualKeyButtonDrp[row]->AddString("14",YSFALSE);
			virtualKeyButtonDrp[row]->AddString("15",YSFALSE);
		}

		EndAddTabItem();
	}

	{
		auto tabId=AddTab(tab,"Application");
		BeginAddTabItem(tab,tabId);

		AddStaticText(0,FSKEY_NULL,L"Application-Specific Augmentation",YSTRUE);
		appSpecificAugDrp=AddEmptyDropList(0,FSKEY_NULL,"",10,40,40,YSTRUE);
		appSpecificAugDrp->AddString(FM77AVAppToStr(FM77AV_APPSPECIFIC_NONE).c_str(),YSTRUE);

		std::set <std::string> apps;
		for(unsigned int i=0; i<FM77AV_NUM_APPSPECIFIC; ++i)
		{
			if(FM77AV_APPSPECIFIC_NONE!=i)
			{
				apps.insert(FM77AVAppToStr(i));
			}
		}
		for(auto s : apps)
		{
			appSpecificAugDrp->AddString(s.c_str(),YSFALSE);
		}

		appSpecificExplanation=AddStaticText(0,FSKEY_NULL,L"",YSTRUE);

		EndAddTabItem();
	}

	{
		auto tabId=AddTab(tab,"Hot Keys");
		BeginAddTabItem(tab,tabId);

		AddStaticText(0,FSKEY_NULL,"Pause/Resume(MENU on/off):",YSTRUE);
		pauseResumeKeyDrp=AddEmptyDropList(0,FSKEY_NULL,"",8,12,12,YSFALSE);
		for(auto str : hotKeyAssignable)
		{
			pauseResumeKeyDrp->AddString(str.c_str(),YSFALSE);
		}

		for(int i=0; i<MAX_NUM_HOST_SHORTCUT; ++i)
		{
			hostShortCutKeyLabelDrp[i]=AddEmptyDropList(0,FSKEY_NULL,"",8,6,8,(0==i%2 ? YSTRUE : YSFALSE));
			for(auto str : hotKeyAssignable)
			{
				hostShortCutKeyLabelDrp[i]->AddString(str.c_str(),YSFALSE);
			}
			hostShortCutKeyLabelDrp[i]->Select(0);
			hostShortCutKeyCtrlBtn[i]=AddTextButton(0,FSKEY_NULL,FSGUI_CHECKBOX,"CTRL",YSFALSE);
			hostShortCutKeyShiftBtn[i]=AddTextButton(0,FSKEY_NULL,FSGUI_CHECKBOX,"SHIFT",YSFALSE);
			hostShortCutFunctionDrp[i]=AddEmptyDropList(0,FSKEY_NULL,"",8,14,16,YSFALSE);
			for(auto pair : hotKeyFunc)
			{
				hostShortCutFunctionDrp[i]->AddString(pair[0].c_str(),YSFALSE);
			}
			hostShortCutFunctionDrp[i]->Select(0);
		}

		AddStaticText(0,FSKEY_NULL,"Quick Screen Shot Dir:",YSTRUE);
		quickSsDirBtn=AddTextButton(0,FSKEY_NULL,FSGUI_PUSHBUTTON,"Browse",YSTRUE);
		quickSsDirTxt=AddTextBox(0,FSKEY_NULL,FsGuiTextBox::HORIZONTAL,"",nShowPath,YSFALSE);;

		AddStaticText(0,FSKEY_NULL,"Quick State-Save File Name:",YSTRUE);
		quickStateSaveFNameBtn=AddTextButton(0,FSKEY_NULL,FSGUI_PUSHBUTTON,"Browse",YSTRUE);
		quickStateSaveFNameTxt=AddTextBox(0,FSKEY_NULL,FsGuiTextBox::HORIZONTAL,"",nShowPath,YSFALSE);

		EndAddTabItem();
	}

	tab->SelectCurrentTab(mainTabId);

	runBtn=AddTextButton(0,FSKEY_NULL,FSGUI_PUSHBUTTON,"START",YSTRUE);
	autoStartBtn=AddTextButton(0,FSKEY_NULL,FSGUI_CHECKBOX,"Auto Start (Start VM as soon as the profile is loaded)",YSFALSE);

// macOS doen't send SCROLL_LOCK key code to the application.  WTF.
#ifndef __APPLE__
	AddStaticText(0,FSKEY_NULL,
		"Press SCROLL_LOCK or Click on the lower-right [MENU] icon\n"
		"to come back to the GUI from the VM.",
		YSTRUE);
#else
	AddStaticText(0,FSKEY_NULL,
		"Click on the lower-right [MENU] icon\n"
		"to come back to the GUI from the VM.",
		YSTRUE);
#endif

	Fit();
	SetArrangeType(FSDIALOG_ARRANGE_TOP_LEFT);


	FM77AVProfile defaultProfile;
	SetProfile(defaultProfile);
}

/* virtual */ void ProfileDialog::OnDropListSelChange(FsGuiDropList *drp,int prevSel)
{
	if(appSpecificAugDrp==drp)
	{
		auto str=appSpecificAugDrp->GetSelectedString();
		auto app=FM77AVStrToApp(str.c_str());
		switch(app)
		{
		default:
		case FM77AV_APPSPECIFIC_NONE:
			appSpecificExplanation->SetText("");
			break;
		}
	}
	else if(scrnModeDrp==drp)
	{
		if(0!=scrnModeDrp->GetSelection())
		{
			scrnAutoScaleBtn->SetCheck(YSTRUE);
		}
	}
}

void ProfileDialog::OnSliderPositionChange(FsGuiSlider *slider,const double &prevPos,const double &prevValue)
{
}

/* virtual */ void ProfileDialog::OnButtonClick(FsGuiButton *btn)
{
	if(ROMDirBtn==btn)
	{
		YsString def;
		def.MakeFullPathName(ROMDirTxt->GetString(),"FBASIC30.ROM");

		auto fdlg=FsGuiDialog::CreateSelfDestructiveDialog<FsGuiFileDialog>();
		fdlg->Initialize();
		fdlg->mode=FsGuiFileDialog::MODE_OPEN;
		fdlg->multiSelect=YSFALSE;
		fdlg->title.Set(L"Select a ROM file (like FBASIC30.ROM)");
		fdlg->fileExtensionArray.Append(L".ROM");
		fdlg->defaultFileName.SetUTF8String(def);
		fdlg->BindCloseModalCallBack(&THISCLASS::OnSelectROMFile,this);
		AttachModalDialog(fdlg);
	}
	if(TapeImgBtn==btn)
	{
		std::vector <const wchar_t *> extList={L".T77"};
		Browse(L"Tape Image",TapeImgTxt,extList);
	}
	if(TapeSaveImgBtn==btn)
	{
		std::vector <const wchar_t *> extList={L".T77"};
		Browse(L"Tape Image",TapeSaveImgTxt,extList);
	}
	if(startUpStateFNameBtn==btn)
	{
		std::vector <const wchar_t *> extList={L".FState"};
		Browse(L"Start-Up VM State",startUpStateFNameTxt,extList);
	}
	for(int i=0; i<2; ++i)
	{
		if(FDImgBtn[i]==btn)
		{
			YsWString label(L"Floppy Drive ");
			label.push_back('0'+i);
			std::vector <const wchar_t *> extList={L".BIN",L".D77",L".D88"};
			Browse(label,FDImgTxt[i],extList);
		}
	}
	if(runBtn==btn)
	{
		canvasPtr->Run();
	}

	if(quickSsDirBtn==btn)
	{
		std::vector <const wchar_t *> extList={L".png"};
		BrowseDir(L"Select A Quick Screenshot File (Can be dummy or new png)",quickSsDirTxt,extList);
	}
	if(quickStateSaveFNameBtn==btn)
	{
		std::vector <const wchar_t *> extList={L".FState"};
		BrowseSaveAs(L"Select A Quick State-Save File",quickStateSaveFNameTxt,extList);
	}
}

void ProfileDialog::OnSelectROMFile(FsGuiDialog *dlg,int returnCode)
{
	auto fdlg=dynamic_cast <FsGuiFileDialog *>(dlg);
	if((int)YSOK==returnCode && nullptr!=fdlg)
	{
		YsWString ful(fdlg->selectedFileArray[0]);
		YsWString pth,fil;
		ful.SeparatePathFile(pth,fil);
		ROMDirTxt->SetText(pth);
	}
}
void ProfileDialog::Browse(const wchar_t label[],FsGuiTextBox *txt,std::vector <const wchar_t *> extList)
{
	nowBrowsingTxt=txt;

	YsString def=ROMDirTxt->GetString();

	auto fdlg=FsGuiDialog::CreateSelfDestructiveDialog<FsGuiFileDialog>();
	fdlg->Initialize();
	fdlg->mode=FsGuiFileDialog::MODE_OPEN;
	fdlg->multiSelect=YSFALSE;
	fdlg->title.Set(label);
	for(auto ext : extList)
	{
		fdlg->fileExtensionArray.Append(ext);
	}
	fdlg->defaultFileName.SetUTF8String(def);
	fdlg->BindCloseModalCallBack(&THISCLASS::OnSelectFile,this);
	AttachModalDialog(fdlg);
}
void ProfileDialog::OnSelectFile(FsGuiDialog *dlg,int returnCode)
{
	auto fdlg=dynamic_cast <FsGuiFileDialog *>(dlg);
	if((int)YSOK==returnCode && nullptr!=fdlg)
	{
		nowBrowsingTxt->SetText(fdlg->selectedFileArray[0]);
		if(TapeImgTxt==nowBrowsingTxt)
		{
			autoStartBtn->SetCheck(YSTRUE);
		}
	}
}

void ProfileDialog::BrowseSaveAs(const wchar_t label[],FsGuiTextBox *txt,std::vector <const wchar_t *> extList)
{
	nowBrowsingTxt=txt;

	YsString def=ROMDirTxt->GetString();

	auto fdlg=FsGuiDialog::CreateSelfDestructiveDialog<FsGuiFileDialog>();
	fdlg->Initialize();
	fdlg->mode=FsGuiFileDialog::MODE_SAVE;
	fdlg->multiSelect=YSFALSE;
	fdlg->title.Set(label);
	for(auto ext : extList)
	{
		fdlg->fileExtensionArray.Append(ext);
	}
	fdlg->defaultFileName.SetUTF8String(def);
	fdlg->BindCloseModalCallBack(&THISCLASS::OnSelectFile,this);
	AttachModalDialog(fdlg);
}
void ProfileDialog::OnSelectFileSaveAs(FsGuiDialog *dlg,int returnCode)
{
	auto fdlg=dynamic_cast <FsGuiFileDialog *>(dlg);
	if((int)YSOK==returnCode && nullptr!=fdlg)
	{
		nowBrowsingTxt->SetText(fdlg->selectedFileArray[0]);
	}
}

void ProfileDialog::BrowseDir(const wchar_t label[],FsGuiTextBox *txt,std::vector <const wchar_t *> extList)
{
	nowBrowsingTxt=txt;

	YsString def=ROMDirTxt->GetString();

	auto fdlg=FsGuiDialog::CreateSelfDestructiveDialog<FsGuiFileDialog>();
	fdlg->Initialize();
	fdlg->mode=FsGuiFileDialog::MODE_SAVE;
	fdlg->multiSelect=YSFALSE;
	fdlg->title.Set(label);
	for(auto ext : extList)
	{
		fdlg->fileExtensionArray.Append(ext);
	}
	fdlg->defaultFileName.SetUTF8String(def);
	fdlg->BindCloseModalCallBack(&THISCLASS::OnSelectDir,this);
	AttachModalDialog(fdlg);
}
void ProfileDialog::OnSelectDir(FsGuiDialog *dlg,int returnCode)
{
	auto fdlg=dynamic_cast <FsGuiFileDialog *>(dlg);
	if((int)YSOK==returnCode && nullptr!=fdlg)
	{
		auto ful=fdlg->selectedFileArray[0];
		YsWString dir,nam;
		ful.SeparatePathFile(dir,nam);
		nowBrowsingTxt->SetText(dir);
	}
}

FM77AVProfile ProfileDialog::GetProfile(void) const
{
	FM77AVProfile profile;

	profile.ROMPath=ROMDirTxt->GetString().data();
	profile.t77Path=TapeImgTxt->GetString().data();
	profile.t77SavePath=TapeSaveImgTxt->GetString().data();
	profile.autoLoadTapeFile=(AutoStartTapeBtn->GetCheck()==YSTRUE);
	profile.fdImgFName[0]=FDImgTxt[0]->GetString().data();
	profile.fdImgWriteProtect[0]=(YSTRUE==FDWriteProtBtn[0]->GetCheck());
	profile.fdImgFName[1]=FDImgTxt[1]->GetString().data();
	profile.fdImgWriteProtect[1]=(YSTRUE==FDWriteProtBtn[1]->GetCheck());

	for(int gameport=0; gameport<2; ++gameport)
	{
		auto sel=gamePortDrp[gameport]->GetSelectedString();
		profile.gamePort[gameport]=FsGuiMainCanvas::HumanReadableToGamePortEmulationType(sel.c_str());

		profile.maxButtonHoldTime[gameport][0]=maxButtonHoldTimeTxt[gameport][0]->GetInteger();
		profile.maxButtonHoldTime[gameport][1]=maxButtonHoldTimeTxt[gameport][1]->GetInteger();
		profile.maxButtonHoldTime[gameport][0]*=1000000;
		profile.maxButtonHoldTime[gameport][1]*=1000000;
	}

	profile.machineType=StrToMachineType(machineTypeDrp->GetSelectedString().c_str());

	profile.autoStart=(YSTRUE==autoStartBtn->GetCheck());

	profile.scaling=scrnScaleTxt->GetInteger();
	profile.autoScaling=(YSTRUE==scrnAutoScaleBtn->GetCheck());
	switch(scrnModeDrp->GetSelection())
	{
	case 0:
		profile.windowModeOnStartUp=FM77AVParam::WINDOW_NORMAL;
		break;
	case 1:
		profile.windowModeOnStartUp=FM77AVParam::WINDOW_MAXIMIZE;
		break;
	case 2:
		profile.windowModeOnStartUp=FM77AVParam::WINDOW_FULLSCREEN;
		break;
	}

	profile.appSpecificSetting=FM77AVStrToApp(appSpecificAugDrp->GetSelectedString().c_str());


	profile.keyboardMode=StrToKeyboardMode(keyboardModeDrp->GetSelectedString().c_str());
	for(int row=0; row<FM77AVProfile::MAX_NUM_VIRTUALKEYS; ++row)
	{
		auto FM77AVKeyStr=virtualKeyFM77AVKeyDrp[row]->GetSelectedString();
		profile.virtualKeys[row].fm77avKey=FM77AVKeyStr.c_str();
		profile.virtualKeys[row].physicalId=virtualKeyPhysIdDrp[row]->GetSelection();
		profile.virtualKeys[row].button=virtualKeyButtonDrp[row]->GetSelection();
	}


	profile.startUpStateFName=startUpStateFNameTxt->GetString().data();

	profile.quickScrnShotDir=quickSsDirTxt->GetString().data();
	for(int i=0; i<MAX_NUM_HOST_SHORTCUT; ++i)
	{
		auto selHostKey=hostShortCutKeyLabelDrp[i]->GetSelection();
		auto selFunc=hostShortCutFunctionDrp[i]->GetSelection();
		if(0!=selHostKey && 0!=selFunc)
		{
			FM77AVParam::HostShortCut hsc;
			hsc.hostKey=hostShortCutKeyLabelDrp[i]->GetSelectedString().c_str();
			for(auto pair : hotKeyFunc)
			{
				std::string label=hostShortCutFunctionDrp[i]->GetSelectedString().c_str();
				if(pair[0]==label)
				{
					hsc.cmdStr=pair[1];
					break;
				}
			}
			hsc.ctrl=(YSTRUE==hostShortCutKeyCtrlBtn[i]->GetCheck());
			hsc.shift=(YSTRUE==hostShortCutKeyShiftBtn[i]->GetCheck());
			profile.hostShortCutKeys.push_back(hsc);
		}
	}

	profile.quickStateSaveFName=quickStateSaveFNameTxt->GetString().data();

	profile.autoStopType=FM77AVKeyboard::StrToAutoStop(autoStopTypeDrp->GetSelectedString().c_str());
	profile.autoStopKey=FM77AVKeyLabelToKeyCode(autoStopKeyDrp->GetSelectedString().c_str());

	profile.pauseResumeKeyLabel=pauseResumeKeyDrp->GetSelectedString().data();

	return profile;
}
void ProfileDialog::SetProfile(const FM77AVProfile &profile)
{
	YsWString str;

	str.SetUTF8String(profile.ROMPath.data());
	ROMDirTxt->SetText(str);

	str.SetUTF8String(profile.t77Path.data());
	TapeImgTxt->SetText(str);

	str.SetUTF8String(profile.t77SavePath.data());
	TapeSaveImgTxt->SetText(str);

	AutoStartTapeBtn->SetCheck(true==profile.autoLoadTapeFile ? YSTRUE : YSFALSE);

	str.SetUTF8String(profile.fdImgFName[0].data());
	FDImgTxt[0]->SetText(str);
	FDWriteProtBtn[0]->SetCheck(true==profile.fdImgWriteProtect[0] ? YSTRUE : YSFALSE);

	str.SetUTF8String(profile.fdImgFName[1].data());
	FDImgTxt[1]->SetText(str);
	FDWriteProtBtn[1]->SetCheck(true==profile.fdImgWriteProtect[1] ? YSTRUE : YSFALSE);

	for(int gameport=0; gameport<2; ++gameport)
	{
		gamePortDrp[gameport]->SelectByString(FsGuiMainCanvas::GamePortEmulationTypeToHumanReadable(profile.gamePort[gameport]).c_str());
		maxButtonHoldTimeTxt[gameport][0]->SetInteger(profile.maxButtonHoldTime[gameport][0]/1000000);
		maxButtonHoldTimeTxt[gameport][1]->SetInteger(profile.maxButtonHoldTime[gameport][1]/1000000);
	}

	if(MACHINETYPE_UNKNOWN==profile.machineType)
	{
		machineTypeDrp->Select(0);
	}
	else
	{
		machineTypeDrp->SelectByString(MachineTypeToStr(profile.machineType).c_str());
	}

	if(true==profile.autoStart)
	{
		autoStartBtn->SetCheck(YSTRUE);
	}
	else
	{
		autoStartBtn->SetCheck(YSFALSE);
	}

	scrnAutoScaleBtn->SetCheck(true==profile.autoScaling ? YSTRUE : YSFALSE);
	switch(profile.windowModeOnStartUp)
	{
	case FM77AVParam::WINDOW_NORMAL:
		scrnModeDrp->Select(0);
		break;
	case FM77AVParam::WINDOW_MAXIMIZE:
		scrnModeDrp->Select(1);
		break;
	case FM77AVParam::WINDOW_FULLSCREEN:
		scrnModeDrp->Select(2);
		break;
	}
	scrnScaleTxt->SetInteger(profile.scaling);

	appSpecificAugDrp->SelectByString(FM77AVAppToStr(profile.appSpecificSetting).c_str(),YSTRUE);

	keyboardModeDrp->SelectByString(KeyboardModeToStr(profile.keyboardMode).c_str());
	for(int row=0; row<FM77AVProfile::MAX_NUM_VIRTUALKEYS; ++row)
	{
		auto keyStr=profile.virtualKeys[row].fm77avKey;
		virtualKeyFM77AVKeyDrp[row]->Select(0);
		virtualKeyFM77AVKeyDrp[row]->SelectByString(keyStr.c_str(),YSFALSE);

		virtualKeyPhysIdDrp[row]->Select(profile.virtualKeys[row].physicalId);
		virtualKeyButtonDrp[row]->Select(profile.virtualKeys[row].button);
	}

	str.SetUTF8String(profile.startUpStateFName.data());
	startUpStateFNameTxt->SetText(str);



	str.SetUTF8String(profile.quickScrnShotDir.data());
	quickSsDirTxt->SetText(str);

	for(int i=0; i<MAX_NUM_HOST_SHORTCUT && i<profile.hostShortCutKeys.size(); ++i)
	{
		hostShortCutKeyLabelDrp[i]->Select(0);
		hostShortCutFunctionDrp[i]->Select(0);
	}
	for(int i=0; i<MAX_NUM_HOST_SHORTCUT && i<profile.hostShortCutKeys.size(); ++i)
	{
		auto hsc=profile.hostShortCutKeys[i];

		hostShortCutKeyLabelDrp[i]->SelectByString(hsc.hostKey.c_str(),YSFALSE);
		hostShortCutKeyCtrlBtn[i]->SetCheck(hsc.ctrl ? YSTRUE : YSFALSE);
		hostShortCutKeyShiftBtn[i]->SetCheck(hsc.shift ? YSTRUE : YSFALSE);

		for(auto pair : hotKeyFunc)
		{
			if(pair[1]==hsc.cmdStr)
			{
				hostShortCutFunctionDrp[i]->SelectByString(pair[0].c_str(),YSFALSE);
				break;
			}
		}
	}

	str.SetUTF8String(profile.quickStateSaveFName.data());
	quickStateSaveFNameTxt->SetText(str);

	autoStopTypeDrp->Select(profile.autoStopType);
	autoStopKeyDrp->SelectByString(FM77AVKeyCodeToKeyLabel(profile.autoStopKey).c_str());

	pauseResumeKeyDrp->SelectByString(profile.pauseResumeKeyLabel.c_str(),YSFALSE);
}
