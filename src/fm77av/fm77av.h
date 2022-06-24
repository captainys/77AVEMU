#ifndef FM77AV_IS_INCLUDED
#define FM77AV_IS_INCLUDED
/* { */

#include <string>
#include <stdint.h>
#include "vmbase.h"
#include "mc6809.h"
#include "fm77avmemory.h"
#include "fm77avdef.h"
#include "fm77avparam.h"

class FM77AV : public VMBase, public Device
{
public:
	class MainCPU : public MC6809
	{
	public:
		virtual const char *DeviceName(void) const{return "mainCPU";}
		MainCPU(FM77AV *vmBase) : MC6809(vmBase){};
	};
	class SubCPU : public MC6809
	{
	public:
		virtual const char *DeviceName(void) const{return "subCPU";}
		SubCPU(FM77AV *vmBase) : MC6809(vmBase){};
	};

	// Devices >>
	PhysicalMemory physMem;

	MainCPUAccess mainMemAcc;
	MainCPU mainCPU;

	SubCPUAccess subMemAcc;
	SubCPU subCPU;
	// Devices <<


	enum
	{
		IRQ_KEY=0x01,
		IRQ_PRINTER=0x02,
		IRQ_TIMER=0x04,
		IRQ_EXT=0x08,    // RS232C
		IRQ_YM2203C=0x20,
	};
	enum
	{
		FIRQ_SUBSYS_ATTENTION=0x1,
		FIRQ_BREAK_KEY=0x02,
	};


	class SystemState
	{
	public:
		uint16_t irq=0,firq=0;
	};
	class State
	{
	public:
		SystemState main,sub;

		unsigned int machineType=MACHINETYPE_FM7;
		uint64_t fm77avTime=0;
		int clockBalance=0;  // Positive means mainCPU is ahead.  Negative subCPU ahead.
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

	bool SetUp(FM77AVParam &param);

	void IOWrite(uint16_t ioAddr,uint8_t value);
	uint8_t IORead(uint16_t ioAddr);
	uint8_t NonDestructiveIORead(uint16_t ioAddr) const;
	uint8_t NonDestructiveIORead_FM77AVIO_SUBSYS_INTERFACE(void) const;


	bool LoadROMFiles(std::string ROMPath);

	void PowerOn(void);
	void Reset(void);
	unsigned int RunOneInstruction(void);

	std::string MachineTypeStr(void) const;
};

/* } */
#endif
