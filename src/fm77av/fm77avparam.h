#ifndef FM77AVPARAM_IS_INCLUDED
#define FM77AVPARAM_IS_INCLUDED
/* { */



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

	unsigned int machineType=MACHINETYPE_FM7;
	std::string ROMPath;
};


/* } */
#endif
