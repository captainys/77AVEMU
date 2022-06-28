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

	class Palette
	{
	};
	class CRTC : public Device
	{
	public:
		virtual const char *DeviceName(void) const{return "CRTC";}

		enum
		{
			// VSYNC_CYCLE is 1670000, but it is close enough to 0x1000000(16777216)
			VSYNC_CYCLE=         0x1000000,
			// Measurement taken from actual FM TOWNS II MX hardware tells VSYNC lasts for 60us.
			CRT_VERTICAL_DURATION=0x1000000-60000, // Time CRTC spends for drawing.  VSYNC_CYCLE-CRT_VERTICAL_DURATION gives duration of VSYNC.
			// HSYNC_CYCLE should be 32000, but it is close enough to 0x8000(32768)
			HSYNC_CYCLE=            0x8000, // Not accurate.  Fixed at 31K
			CRT_HORIZONTAL_DURATION= 30000,
		};

		class State
		{
		public:
			Palette palette;
		};
		CRTC(VMBase *vmBase);
		State state;
		Palette &GetPalette(void);
		const Palette &GetPalette(void) const;
		bool InVSYNC(uint64_t fm77avTime) const;
		bool InHSYNC(uint64_t fm77avTime) const;

		/*
		VSYNC Cycle        |<----------------->|
		Vertical Duration  |<------------->|
		VSYNC              000000000000000001111
	                                           |
	                                    NextVSYNCEndTime
		*/
		uint64_t NextVSYNCRisingEdge(uint64_t fm77avTime) const;
		uint64_t NextVSYNCEndTime(uint64_t fm77avTime) const;
	};

	// Devices >>
	PhysicalMemory physMem;

	MainCPUAccess mainMemAcc;
	MainCPU mainCPU;

	SubCPUAccess subMemAcc;
	SubCPU subCPU;

	CRTC crtc;
	// Devices <<

	enum
	{
		FAST_DEVICE_POLLING_INTERVAL=10000,  // Nano-seconds
		DEVICE_POLLING_INTERVAL=   8000000,  // 8ms
	};
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

		bool subSysBusy,subSysHalt;
		bool mainToSubIRQ;

		unsigned int machineType=MACHINETYPE_FM7;
		uint64_t fm77avTime=0;
		uint64_t nextDevicePollingTime=0;
		uint64_t nextRenderingTime=0;

		int timeBalance=0;  // Positive means mainCPU is ahead.  Negative subCPU ahead.
	};
	State state;

	class Variable
	{
	public:
		bool forceQuitOnPowerOff=false;
		bool justLoadedState=false;
		bool monitorIOReadMain[256],monitorIOReadSub[256];
		bool monitorIOWriteMain[256],monitorIOWriteSub[256];
		bool monitorSubSysCmd=false;
		bool monitorBIOSCall=false;
		unsigned int lastDisassemblyCPU=CPU_MAIN;

		bool breakOnUnhaltSubCPU=false;
		uint8_t breakOnSubCmd[FM7_MAX_SUB_CMD];
	};
	Variable var;

	class CPUAddr
	{
	public:
		uint16_t cpu;
		uint16_t addr;
	};

	virtual const char *DeviceName(void) const{return "FM77AV";}

	FM77AV();

	bool SetUp(FM77AVParam &param,class Outside_World *outside_world);

	void IOWrite(uint16_t ioAddr,uint8_t value);
	uint8_t IORead(uint16_t ioAddr);
	uint8_t NonDestructiveIORead(uint16_t ioAddr) const;
	uint8_t NonDestructiveIORead_FM77AVIO_SUBSYS_INTERFACE(void) const;


	bool LoadROMFiles(std::string ROMPath);

	MC6809 &CPU(unsigned int mainOrSub);
	const MC6809 &CPU(unsigned int mainOrSub) const;
	MemoryAccess &MemAccess(unsigned int mainOrSub);

	/*! SubCPU can halt:
	      by HLT instruction,
	      by main CPU I/O $FD05, or
	      in FM-7 mode between VSYNC if VRAM access flag is set.
	*/
	bool SubCPUHalt(void) const;

	/*! Should implement this function when adding disk drive, rs232c etc.
	*/
	bool ExternalDevicePresent(void) const;

	void PowerOn(void);
	void Reset(void);
	unsigned int RunOneInstruction(void);

	void DetectMainCPUBIOSCall(void);

	std::string MachineTypeStr(void) const;
	void PrintSubSystemCommand(void) const;
};

/* } */
#endif
