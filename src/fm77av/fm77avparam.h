#ifndef FM77AVPARAM_IS_INCLUDED
#define FM77AVPARAM_IS_INCLUDED
/* { */



#include <vector>
#include <string>
#include <unordered_map>
#include "fm77avdef.h"
#include "fm77avkey.h"

class FM77AVParam
{
public:
	void CleanUp(void);

	enum
	{
		WINDOW_NORMAL,
		WINDOW_MAXIMIZE,
		WINDOW_FULLSCREEN
	};

	enum
	{
		NUM_GAMEPORTS=2,
		NUM_FDDRIVES=4,
		MAX_NUM_SCSI_DEVICES=7,
	};

	enum
	{
		SCALING_DEFAULT=100,
		SCALING_MIN=50,
		SCALING_MAX=1000,
	};

	class VirtualKey
	{
	public:
		std::string fm77avKey;
		int physicalId;
		unsigned int button;
	};

	class HostShortCut
	{
	public:
		std::string hostKey;
		bool ctrl,shift;
		std::string cmdStr;
	};

	unsigned int scaling=100;
	bool autoScaling=false;
	unsigned int windowModeOnStartUp=WINDOW_NORMAL;
	bool windowShift=false;

	std::string startUpStateFName;

	std::string pauseResumeKeyLabel="SCROLLLOCK";

	unsigned int machineType=MACHINETYPE_AUTO;
	std::string ROMPath;
	std::string quickStateSaveFName;

	std::string symTableFName;

	bool pauseOnStart=false;
	unsigned int keyboardMode=FM77AV_KEYBOARD_MODE_DIRECT;

	unsigned int gamePort[NUM_GAMEPORTS]={FM77AV_GAMEPORTEMU_PHYSICAL0,FM77AV_GAMEPORTEMU_PHYSICAL1};
	long long int maxButtonHoldTime[2][2]={{0,0},{0,0}};

	bool noWait=false;

	std::vector <std::string> imgSearchPaths;
	std::vector <HostShortCut> hostShortCutKeys;
	std::vector <std::string> initialCmd;

	std::vector <VirtualKey> virtualKeys;

	std::unordered_map <std::string,std::string> fileNameAlias;

	std::string quickScrnShotDir;
	unsigned int scrnShotX0=0,scrnShotY0=0,scrnShotWid=0,scrnShotHei=0;

	std::string playbackEventLogFName;

	std::string t77Path,t77SavePath;
	bool t77WriteProtect=false;
	std::string fdImgFName[NUM_FDDRIVES];
	bool fdImgWriteProtect[NUM_FDDRIVES]={false,false,false,false};

	bool unitTest=false; // If true, CUI will not be attached.  Implies termination condition.

	bool enableCOM[FM7_MAX_NUM_COMPORTS]={false,false,false,false};

	unsigned int appSpecificSetting=FM77AV_APPSPECIFIC_NONE;

	bool autoLoadTapeFile=false;

	unsigned int autoStopType=0;
	unsigned int autoStopKey=AVKEY_NUM_5;

	bool DOSMode=false;

	uint8_t powerOffAtCPUType=CPU_UNKNOWN;
	uint16_t powerOffAtAddr=0;
	class TestCondMem
	{
	public:
		uint8_t addrType;
		uint32_t addr;
		uint8_t data;
	};
	std::vector <TestCondMem> testCondMem;

	std::unordered_map <std::string,std::string> specialPath;

	// Toward semi-automated 1X RPG map generation.
	std::string mapXYExpression[2];
};



class FM77AVProfile : public FM77AVParam
{
public:
	// MAX_NUM_VIRTUALKEYS is limitation for GUI environment only.
	enum
	{
		MAX_NUM_VIRTUALKEYS=20,
	};


	// autoStart flag is for GUI environment only.
	bool autoStartOnLoad=false;

	std::string errorMsg;

	FM77AVProfile();
	void CleanUp(void);
	std::vector <std::string> Serialize(void) const;
	bool Deserialize(const std::vector <std::string> &text);
};



/* } */
#endif
