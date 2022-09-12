#ifndef PROFILEDLG_IS_INCLUDED
#define PROFILEDLG_IS_INCLUDED
/* { */

#include <vector>

#include "fsgui.h"
#include "fsguifiledialog.h"

#include "fm77avparam.h"

class ProfileDialog : public FsGuiDialog
{
public:
	typedef class ProfileDialog THISCLASS;

	enum
	{
		PATH_LENGTH=1024,
		PATH_SHOW=64,
		MAX_NUM_HOST_SHORTCUT=8,
		MAX_NUM_SHARED_DIR=4,
		MAX_NUM_ALIASES=8,
	};


	class FsGuiMainCanvas *canvasPtr;

	FsGuiTabControl *tab;

	FsGuiStatic *profileFNameTxt;

	FsGuiButton *ROMDirBtn,*TapeImgBtn,*TapeSaveImgBtn,*FDImgBtn[FM77AVProfile::NUM_FDDRIVES];
	FsGuiButton *AutoStartTapeBtn;
	FsGuiButton *FDWriteProtBtn[FM77AVProfile::NUM_FDDRIVES];
	FsGuiTextBox *ROMDirTxt,*TapeImgTxt,*TapeSaveImgTxt,*FDImgTxt[FM77AVProfile::NUM_FDDRIVES];
	FsGuiDropList *gamePortDrp[2]; // None, Pad0, Pad1, Keybord Emulation, Mouse,
	//FsGuiSlider *mouseIntegSpdSlider;
	//FsGuiButton *mouseIntegConsiderVRAMOffsetBtn;
	//FsGuiTextBox *mouseMinXTxt,*mouseMinYTxt,*mouseMaxXTxt,*mouseMaxYTxt;
	FsGuiTextBox *maxButtonHoldTimeTxt[2][2];

	FsGuiTextBox *scrnScaleTxt;
	FsGuiButton *scrnAutoScaleBtn;
	FsGuiDropList *scrnModeDrp;
	FsGuiButton *autoStartBtn;
	FsGuiButton *runBtn;
	FsGuiDropList *machineTypeDrp;
	FsGuiButton *startUpStateFNameBtn;
	FsGuiTextBox *startUpStateFNameTxt;


	FsGuiTextBox *aliasNameTxt[MAX_NUM_ALIASES],*aliasFileTxt[MAX_NUM_ALIASES];
	FsGuiButton *aliasBrowseBtn[MAX_NUM_ALIASES];


	FsGuiDropList *keyboardModeDrp;
	FsGuiDropList *virtualKeyFM77AVKeyDrp[FM77AVProfile::MAX_NUM_VIRTUALKEYS];
	FsGuiDropList *virtualKeyPhysIdDrp[FM77AVProfile::MAX_NUM_VIRTUALKEYS];
	FsGuiDropList *virtualKeyButtonDrp[FM77AVProfile::MAX_NUM_VIRTUALKEYS];
	FsGuiDropList *autoStopTypeDrp;
	FsGuiDropList *autoStopKeyDrp;


	FsGuiTextBox *scrnShotCropTxt[4];
	FsGuiTextBox *mapXYExpressionTxt[2];


	FsGuiDropList *hostShortCutKeyLabelDrp[MAX_NUM_HOST_SHORTCUT];
	FsGuiButton *hostShortCutKeyCtrlBtn[MAX_NUM_HOST_SHORTCUT];
	FsGuiButton *hostShortCutKeyShiftBtn[MAX_NUM_HOST_SHORTCUT];
	FsGuiDropList *hostShortCutFunctionDrp[MAX_NUM_HOST_SHORTCUT];
	FsGuiButton *quickSsDirBtn;
	FsGuiTextBox *quickSsDirTxt;

	FsGuiButton *quickStateSaveFNameBtn;
	FsGuiTextBox *quickStateSaveFNameTxt;

	FsGuiDropList *pauseResumeKeyDrp;

	FsGuiDropList *appSpecificAugDrp;
	FsGuiStatic *appSpecificExplanation;

	FsGuiButton *browseShareDirBtn[MAX_NUM_SHARED_DIR];
	FsGuiTextBox *shareDirTxt[MAX_NUM_SHARED_DIR];

	ProfileDialog(FsGuiMainCanvas *canvasPtr);

	void Make(void);

	void OnSliderPositionChange(FsGuiSlider *slider,const double &prevPos,const double &prevValue);

	virtual void OnDropListSelChange(FsGuiDropList *drp,int prevSel);

	virtual void OnButtonClick(FsGuiButton *btn);
	void OnSelectROMFile(FsGuiDialog *dlg,int returnCode);

	FsGuiTextBox *nowBrowsingTxt=nullptr;
	void Browse(const wchar_t label[],FsGuiTextBox *txt,std::vector <const wchar_t *> extList);
	void OnSelectFile(FsGuiDialog *dlg,int returnCode);

	void BrowseDir(const wchar_t label[],FsGuiTextBox *txt,std::vector <const wchar_t *> extList);
	void OnSelectDir(FsGuiDialog *dlg,int returnCode);

	void BrowseSaveAs(const wchar_t label[],FsGuiTextBox *txt,std::vector <const wchar_t *> extList);
	void OnSelectFileSaveAs(FsGuiDialog *dlg,int returnCode);

	FM77AVProfile GetProfile(void) const;
	void SetProfile(const FM77AVProfile &profile);
};


/* } */
#endif
