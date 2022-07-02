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
#include "fm77avcrtc.h"
#include "fm77avkeyboard.h"
#include "fm77avtape.h"

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

	FM77AVCRTC crtc;
	FM77AVKeyboard keyboard;
	FM77AVDataRecorder dataRecorder;
	// Devices <<

	enum
	{
		FAST_DEVICE_POLLING_INTERVAL=10000,  // Nano-seconds
		DEVICE_POLLING_INTERVAL=   8000000,  // 8ms
	};

	class SystemState
	{
	public:
		enum
		{
			// Write to $FD02
			MAIN_IRQ_ENABLE_KEY=0x01,
			MAIN_IRQ_ENABLE_PRINTER=0x02,
			MAIN_IRQ_ENABLE_TIMER=0x04,
			MAIN_IRQ_ENABLE_FDC=0x10,
			MAIN_IRQ_ENABLE_TXRDY=0x20,
			MAIN_IRQ_ENABLE_RXRDY=0x40,
			MAIN_IRQ_ENABLE_SYNDET=0x80,
		};
		enum
		{
			// Read $FD03
			MAIN_IRQ_SOURCE_KEY=0x01,
			MAIN_IRQ_SOURCE_PRINTER=0x02,
			MAIN_IRQ_SOURCE_TIMER=0x04,
			MAIN_IRQ_SOURCE_EXT=0x08,   // FDC, RS232C
		};
		enum
		{
			// Read $FD04
			MAIN_FIRQ_SOURCE_ATTENTION=0x01,
			MAIN_FIRQ_SOURCE_BREAK_KEY=0x02,
		};
		enum
		{
			SUB_IRQ_SOURCE_CANCEL_REQ=0x01,
			SUB_FIRQ_SOURCE_KEY=0x01,
		};
		uint16_t irqEnableBits=0;
		uint16_t irqSource=0,firqSource=0;
	};
	class State
	{
	public:
		SystemState main,sub;

		bool subSysBusy,subSysHalt;
		bool mainToSubIRQ;

		unsigned int machineType=MACHINETYPE_FM7;
		unsigned int keyboardIRQHandler=CPU_SUB; // Controlled by $FD02 bit 0.  FM-Techknow pp.151.
		uint64_t fm77avTime=0;
		uint64_t nextDevicePollingTime=0;
		uint64_t nextRenderingTime=0;
		uint64_t next20msTimer=FM77AVTIME_MILLISEC*20;
		uint64_t next2msTimer=FM77AVTIME_MILLISEC*2;

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

	inline void ProcessInterrupts(void)
	{
		if(state.next20msTimer<=state.fm77avTime)
		{
			state.next20msTimer=state.fm77avTime+FM77AVTIME_MILLISEC*20;
			if(true!=SubCPUHalt())
			{
				// Don't fire NMI when sub-cpu is halt.
				// Greater than 40ms halt will double-fire the NMI, which is not good.
				subCPU.NMI(subMemAcc);
			}
		}
		if(state.next2msTimer<=state.fm77avTime)
		{
			state.next2msTimer=state.fm77avTime+FM77AVTIME_MILLISEC*2;
			if(0!=(state.main.irqEnableBits&SystemState::MAIN_IRQ_ENABLE_TIMER))
			{
				state.main.irqSource|=SystemState::MAIN_IRQ_SOURCE_TIMER;
			}
		}
		else
		{
			// Question: Is it edge-sensitive?  Or, does it latch when 2ms passes?
			state.main.irqSource&=~SystemState::MAIN_IRQ_SOURCE_TIMER;
		}
		if(0!=state.main.irqSource)
		{
			mainCPU.IRQ(mainMemAcc);
		}
		if(0!=state.sub.irqSource)
		{
			subCPU.IRQ(subMemAcc);
		}
		if(0!=state.main.firqSource)
		{
			mainCPU.FIRQ(mainMemAcc);
		}
		if(0!=state.sub.firqSource)
		{
			subCPU.FIRQ(subMemAcc);
		}
	}
	bool KeyIRQFlagSet(void) const;
	void SetKeyIRQFlag(void);
	void ClearKeyIRQFlag(void);
	void SetBreakKeyFIRQFlag(void);
	void ClearBreakKeyFIRQFlag(void);

	void SetSubToMainFIRQFlag(void);
	void ClearSubToMainFIRQFlag(void);

	void ClearMainToSubIRQFlag(void);

	void EnableShadowRAM(void);
	void DisableShadowRAM(void);


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
