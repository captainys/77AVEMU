#ifndef FM77AV_IS_INCLUDED
#define FM77AV_IS_INCLUDED
/* { */

#include <string>
#include <stdint.h>
#include "vmbase.h"
#include "mc6809.h"
#include "memory.h"
#include "fm77avdef.h"

class FM77AV : public VMBase, public Device
{
public:
	// Devices >>
	PhysicalMemory physMem;

	MainCPUAccess mainMemAcc;
	MC6809 mainCPU;

	SubCPUAccess subMemAcc;
	MC6809 subCPU;
	// Devices <<



	class State
	{
	public:
		unsigned int machineType=MACHINETYPE_FM7;
		uint64_t fm77avTime=0;
		unsigned int clockBalance=0;  // Positive means mainCPU is ahead.  Negative subCPU ahead.
	};
	State state;

	class Variable
	{
	public:
		bool forceQuitOnPowerOff=false;
		bool justLoadedState=false;
	};
	Variable var;

	virtual const char *DeviceName(void) const{return "FM77AV";}

	FM77AV();

	bool LoadROMFiles(std::string ROMPath);

	void PowerOn(void);
	void Reset(void);
	unsigned int RunOneInstruction(void);

	std::string MachineTypeStr(void) const;
};

/* } */
#endif
