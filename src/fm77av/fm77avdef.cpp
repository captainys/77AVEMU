#include "fm77avdef.h"



std::string MachineTypeToStr(unsigned int machineType)
{
	switch(machineType)
	{
	case MACHINETYPE_FM7:
		return "FM-7";
	case MACHINETYPE_FM77AV:
		return "FM77AV";
	case MACHINETYPE_FM77AV40:
		return "FM77AV40";
	}
	return "UNKNOWN";
}
unsigned int StrToMachineType(std::string str)
{
	if("FM7"==str || "FM-7"==str)
	{
		return MACHINETYPE_FM7;
	}
	if("FM77AV"==str)
	{
		return MACHINETYPE_FM77AV;
	}
	if("FM77AV40"==str)
	{
		return MACHINETYPE_FM77AV40;
	}
	return MACHINETYPE_UNKNOWN;
}
