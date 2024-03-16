#ifndef FM77AV_IS_INCLUDED
#define FM77AV_IS_INCLUDED
/* { */

#include <string>
#include <stdint.h>
#include "lineparser.h"
#include "vmbase.h"
#include "mc6809.h"
#include "fm77avmemory.h"
#include "fm77avdef.h"
#include "fm77avparam.h"
#include "fm77avcrtc.h"
#include "fm77avfdc.h"
#include "fm77avkeyboard.h"
#include "fm77avtape.h"
#include "fm77avsound.h"
#include "fm77avgameport.h"
#include "fm77aveventlog.h"
#include "fm77avserialport.h"
#include "fm77av40dmac.h"
#include "outside_world.h"

#include "../exas_compiler/exas_compiler.h"

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

	std::string symTableFName;

	FM77AVCRTC crtc;
	FM77AVFDC fdc;
	FM77AVKeyboard keyboard;
	FM77AVDataRecorder dataRecorder;
	FM77AVSound sound;
	FM77AVGamePort gameport;
	FM77AVSerialPort serialport;
	FM77AV40DMAC dmac;
#ifdef ENABLE_EXAS_COMPILER
	ExasCompiler exasCompiler;
#endif
	// Devices <<

	enum
	{
		ADDR_MAIN,  // Keep it zero, same as CPU_MAIN
		ADDR_SUB,   // Keep it one, same as CPU_AUB
		ADDR_PHYS,
		ADDR_VRAM,  // VRAM address 
		ADDR_VRAM_RAW,  // If RAW, VRAM offset should not apply.

		ADDR_NONE=CPU_UNKNOWN, // Keep it same as CPU_UNKNOWN
	};
	static unsigned int StrToAddressType(std::string str);
	class Address
	{
	public:
		unsigned int type;
		unsigned int addr;
	};

	enum
	{
		FM8_FREQ=1000000,
		FM7_FREQ=MC6809::DEFAULT_FREQ,
		FM77_FREQ=MC6809::DEFAULT_FREQ,

		FAST_DEVICE_POLLING_INTERVAL=10000,  // Nano-seconds
		DEVICE_POLLING_INTERVAL=   8000000,  // 8ms
		CATCHUP_DEFICIT_CUTOFF=2000000, // If the deficit is above this threshold, just don't increase.
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
			MAIN_IRQ_ENABLE_TXRDY=0x20,    // Looks like IRQ mask is stored in M9 74LS175.  Maybe I should make RS232C responsible for remembering it.
			MAIN_IRQ_ENABLE_RXRDY=0x40,
			MAIN_IRQ_ENABLE_SYNDET=0x80,

			MAIN_IRQ_ENABLE_YM2203C=0x100, // Looks like there is no I/O for masking it.  YM2203C card schematic does not have anything to remember mask status.

			MAIN_IRQ_ENABLE_ALWAYS_ON=0xFF00, // YM2203C etc.  Never mask by $FD02.
		};
		enum
		{
			// Read $FD03
			MAIN_IRQ_SOURCE_KEY=0x01,
			MAIN_IRQ_SOURCE_PRINTER=0x02,
			MAIN_IRQ_SOURCE_TIMER=0x04,
			MAIN_IRQ_SOURCE_EXT=0x08,   // FDC, RS232C  Combine FDC and RS232C int into this bit when memory read.

			MAIN_IRQ_SOURCE_FDC=0x10,

			// Read $FD17
			MAIN_IRQ_SOURCE_YM2203C=0x100, // Bit 3, Active-Low
			MAIN_IRQ_SOURCE_DMA=0x200,
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
		unsigned int machineType=MACHINETYPE_FM7;
		unsigned int keyboardIRQHandler=CPU_SUB; // Controlled by $FD02 bit 0.  FM-Techknow pp.151.
		uint64_t fm77avTime=0;
		uint64_t nextDevicePollingTime=0;
		uint64_t nextFastDevicePollingTime=0;
		uint64_t nextRenderingTime=0;
		uint64_t next20msTimer=FM77AVTIME_MILLISEC*20;
		uint64_t next2msTimer=FM77AVTIME_MILLISEC*2;
		uint64_t nextSecondInfm77avTime=0;

		int timeBalance=0;  // Positive means mainCPU is ahead.  Negative subCPU ahead.

		/*! Nanoseconds VM is lagging behind the real time.
		*/
		long long int timeDeficit=0;

		bool subSysBusy,subSysHalt,subSysHaltSoon;
		bool subNMIMask=false;

		bool CRTCHaltsSubCPU=false;  // True in FM-7/8 speed mode.

		SystemState main,sub;
	};
	State state;


	// Toward semi-automated map generation.
	// Some games (like Dragon Slayer) stores the location as a pointer,
	// in which case, X and Y can be calculated as:
	//    X=(addr-constant1)%constant2
	//    Y=(addr-constant1)/constant2
	class MemoryEvaluation : public LineParser
	{
	private:
		FM77AV *fm77avPtr=nullptr;
	public:
		MemoryEvaluation(FM77AV *fm77avPtr);

		bool ready=false;

		bool Decode(std::string str);

		std::string MatchCustomKeyword(std::string str) const override;
		bool IsCustomUnaryOperator(std::string str) const override;
		long long int EvaluateCustomUnaryOperator(const Term *t,long long int operand) const override;

		// Eg.  Write formula like the following.  PHYS:, MAIN:, SUB: can be a short form P:, M:, S:, respectively.
		//   BYTE_PHYS:$3600A
		//   (WORD_PHYS:0x3600A-0x4E00)/0x50
		//   (WORD_MAIN:0x600A-0x4E00)%0x50
		// Main/Sub won't be decoded at run time.  MAIN: will be mapped to physical 0x30000 to 0x3FFF
		long long int EvaluateRawNumber(const std::string &str) const override;

		unsigned int EvaluateMemoryReference(const std::string &str,unsigned int addr,unsigned int nBytes) const;

		unsigned int DecodePhysicalAddressBase(std::string term) const;
	};

	class Variable
	{
	public:
		bool noWait=false;
		bool catchUpRealTime=true;

		bool forceQuitOnPowerOff=false;
		bool justLoadedState=false;
		bool monitorIOReadMain[256],monitorIOReadSub[256];
		bool monitorIOWriteMain[256],monitorIOWriteSub[256];
		bool monitorSubSysCmd=false;
		bool monitorBIOSCall=false;
		unsigned int lastDisassemblyCPU=CPU_MAIN;

		bool noWaitWhileTapeMotorOn=true;

		bool autoLoadTapeFile=false;

		bool breakOnUnhaltSubCPU=false;
		uint8_t breakOnSubCmd[FM7_MAX_SUB_CMD];

		enum
		{
			TIME_ADJUSTMENT_LOG_LEN=4096
		};
		unsigned int timeAdjustLogPtr=0;
		int64_t timeAdjustLog[TIME_ADJUSTMENT_LOG_LEN];
		int64_t timeDeficitLog[TIME_ADJUSTMENT_LOG_LEN];

		std::vector <std::string> imgSearchPaths;

		std::string quickScrnShotDir;
		// scrnShotWid==0 or scrnShotHei==0 mean no cropping.
		unsigned int scrnShotX0=0,scrnShotY0=0,scrnShotWid=0,scrnShotHei=0;

		std::vector <std::string> initialCmd;

		std::unordered_map <std::string,std::string> fileNameAlias;

		Address powerOffAt;
		std::vector <FM77AVParam::TestCondMem> testCondMem;

		// bool MAGUSAware=true;
	};
	Variable var;
	MemoryEvaluation mapXY[2];

	FM77AVEventLog eventLog;

	class CPUAddr
	{
	public:
		uint16_t cpu;
		uint16_t addr;
	};

	virtual const char *DeviceName(void) const{return "FM77AV";}

	FM77AV();

	bool SetUp(const FM77AVParam &param,class Outside_World *outside_world,Outside_World::WindowInterface *window);

	void IOWrite(const CanAccessMemory *accessFrom,uint16_t ioAddr,uint8_t value);
	uint8_t IORead(const CanAccessMemory *accessFrom,uint16_t ioAddr);
	uint8_t NonDestructiveIORead(uint16_t ioAddr) const;

	/*! Returns aliased file name if input matches an alias label.
	    If not, returns input.
	*/
	std::string FileNameAlias(std::string input) const;
	static std::string FileNameAlias(std::string input,const std::unordered_map<std::string,std::string> &aliases);

	/*! Returns a file name found after applying alias and if relative path searching inside
	    the imgSearchPaths.  If nothing is found, returns either alias or the input. */
	std::string FindFile(std::string fName) const;
	static std::string FindFile(std::string fName,const std::unordered_map<std::string,std::string> &aliases,const std::vector <std::string> &imgSearchPaths);

	bool NoWait(void) const;

	bool IsMapXAvailable(void) const;
	int GetMapX(void) const;
	bool IsMapYAvailable(void) const;
	int GetMapY(void) const;
private:
	bool IsMemoryEvaluationAvailable(const MemoryEvaluation &mapLoc) const;
	int GetMemoryEvaluation(const MemoryEvaluation &mapLoc) const;

private:
	void RunFastDevicePollingInternal(void);
public:
	/*! Check nextFastDevicePollingTime and call RunScheduledTask function of the devices in fastDevices.
	*/
	inline void RunFastDevicePolling(void)
	{
		if(state.nextFastDevicePollingTime<state.fm77avTime)
		{
			RunFastDevicePollingInternal();
		}
	}

	inline void ProcessSound(class Outside_World::Sound *soundPtr)
	{
		sound.ProcessSound(soundPtr);
	}
	inline void ProcessInterrupts(void)
	{
		if(state.next20msTimer<=state.fm77avTime && true!=subCPU.state.middleInst)
		{
			state.next20msTimer=state.fm77avTime+FM77AVTIME_MILLISEC*20;
			if(true!=SubCPUHalt() && true!=state.subNMIMask)
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
		// else
		// {
		// 	// Question: Is it edge-sensitive?  Or, does it latch when 2ms passes?
		// 	// Looks like it should latch.  If not latch, Polar Star III map moves
		// 	// extremely slow.  Most likely IRQs were dropping while accessing sub-CPU.
		// 	state.main.irqSource&=~SystemState::MAIN_IRQ_SOURCE_TIMER;
		// }
		if(0!=(state.main.irqSource&state.main.irqEnableBits))
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


	static std::vector <std::string> CheckMissingROMFiles(const FM77AVParam &param);
	unsigned int LoadROMFilesAndIdentifyMachineType(std::string ROMPath);
	bool LoadROMFiles(std::string ROMPath);

	MC6809 &CPU(unsigned int mainOrSub);
	const MC6809 &CPU(unsigned int mainOrSub) const;
	MemoryAccess &MemAccess(unsigned int mainOrSub);

	MC6809 *ToCPU(const CanAccessMemory *accessFrom);
	const MC6809 *ToCPU(const CanAccessMemory *accessFrom) const;

	FM77AV40DMAC *ToDMAC(const CanAccessMemory *accessFrom);
	const FM77AV40DMAC *ToDMAC(const CanAccessMemory *accessFrom) const;

	/*! SubCPU can halt:
	      by HLT instruction,
	      by main CPU I/O $FD05, or
	      in FM-7 mode between VSYNC if VRAM access flag is set.
	*/
	bool SubCPUHalt(void) const;

	/*! Set Game-Pad State.  Called from Outside_World.
	*/
	void SetGamePadState(int port,bool Abutton,bool Bbutton,bool left,bool right,bool up,bool down,bool run,bool pause);

	/*!
	*/
	void SetMouseMotion(int portId,int dx,int dy);

	/*!
	*/
	void SetMouseButtonState(bool lButton,bool rButton);

	/*! Should implement this function when adding disk drive, rs232c etc.
	*/
	bool ExternalDevicePresent(void) const;

	void SetFM8Speed(void);
	void SetFM7Speed(void);
	void SetFM77Speed(void);

	void PowerOn(void);
	void Reset(void);
	unsigned int RunOneInstruction(void);

	void TypeCommandForStartingTapeProgram(void);

	void RenderQuiet(class FM77AVRender &render);

	void DetectMainCPUBIOSCall(void);

	std::string MachineTypeStr(void) const;
	void PrintSubSystemCommand(void) const;

	bool SaveState(std::string fName) const;
	bool LoadState(std::string fName,class Outside_World &outsideWorld);
	/* virtual */ uint32_t SerializeVersion(void) const;
	/* virtual */ void SpecificSerialize(std::vector <unsigned char> &data,std::string stateFName) const;
	/* virtual */ bool SpecificDeserialize(const unsigned char *&data,std::string stateFName,uint32_t version);

	std::vector <std::string> GetIRQStatusText(void) const;

	bool LoadSymbolTable(std::string fName);
	bool AutoSaveSymbolTable(void) const;

	int TestSuccess(void) const;
};

/* } */
#endif
