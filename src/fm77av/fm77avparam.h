#ifndef FM77AVPARAM_IS_INCLUDED
#define FM77AVPARAM_IS_INCLUDED
/* { */



#include <vector>
#include <string>
#include "fm77avdef.h"

class FM77AVParam
{
public:
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

	unsigned int machineType=MACHINETYPE_AUTO;
	std::string ROMPath;

	bool pauseOnStart=false;
	unsigned int keyboardMode=FM77AV_KEYBOARD_MODE_DIRECT;

	unsigned int gamePort[NUM_GAMEPORTS]={FM77AV_GAMEPORTEMU_PHYSICAL0,FM77AV_GAMEPORTEMU_MOUSE};
	long long int maxButtonHoldTime[2][2]={{0,0},{0,0}};

	bool noWait=false;

	std::vector <std::string> imgSearchPaths;

	std::string t77Path;
	std::string fdImgFName[NUM_FDDRIVES];
	bool fdImgWriteProtect[NUM_FDDRIVES]={false,false,false,false};
};


/* } */
#endif
