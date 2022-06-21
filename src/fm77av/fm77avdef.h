#ifndef FM77AVDEF_IS_INCLUDED
#define FM77AVDEF_IS_INCLUDED
/* { */



#include <string>

enum
{
	MACHINETYPE_UNKNOWN,
	MACHINETYPE_FM7,
	MACHINETYPE_FM77AV,
	MACHINETYPE_FM77AV40
};
std::string MachineTypeToStr(unsigned int machineType);
unsigned int StrToMachineType(std::string str);


/* } */
#endif
