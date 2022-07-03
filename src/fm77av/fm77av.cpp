#include <algorithm>
#include <iostream>
#include "fm77av.h"
#include "fm77avdef.h"
#include "outside_world.h"



FM77AV::FM77AV() :
	Device(this),
	physMem(this),
	mainMemAcc(this,&physMem),
	mainCPU(this),
	subMemAcc(this,&physMem),
	subCPU(this),
	crtc(this),
	keyboard(this),
	fdc(this)
{
	allDevices.push_back(&mainCPU);
	allDevices.push_back(&subCPU);
	allDevices.push_back(&physMem);
	allDevices.push_back(&mainMemAcc);
	allDevices.push_back(&subMemAcc);
	allDevices.push_back(&crtc);
	allDevices.push_back(&keyboard);
	allDevices.push_back(&fdc);
	VMBase::CacheDeviceIndex();

	for(auto &b : var.breakOnSubCmd)
	{
		b=0;
	}
}

bool FM77AV::SetUp(FM77AVParam &param,Outside_World *outside_world)
{
	state.machineType=param.machineType;
	if(true!=LoadROMFiles(param.ROMPath))
	{
		std::cout << "Failed to load ROM files." << std::endl;
		return false;
	}

	var.noWait=param.noWait;

	if(""!=param.t77Path)
	{
		if(true==dataRecorder.LoadT77(param.t77Path))
		{
			std::cout << "Loaded Cassette: " << param.t77Path << std::endl;
		}
		else
		{
			std::cout << "Failed to load: " << param.t77Path << std::endl;
			return false;
		}
	}


	fdc.searchPaths=param.imgSearchPaths;
	for(int drv=0; drv<4; ++drv)
	{
		if(""!=param.fdImgFName[drv])
		{
			fdc.LoadD77orRAW(drv,param.fdImgFName[drv].c_str());
			if(true==param.fdImgWriteProtect[drv])
			{
				// D77 image may have write-protect switch.
				// Enable write protect only when specified by the parameter.
				fdc.SetWriteProtect(drv,true);
			}
		}
	}


	for(int i=0; i<32; ++i)
	{
		//	MAINSYS_BOOT_ROM_END=       0x3FFE0,
		physMem.state.data[PhysicalMemory::MAINSYS_INT_VECTOR+i]=physMem.ROM_BOOT_BASIC[0x1E0+i];
	}

	physMem.state.data[PhysicalMemory::SUBSYS_RESET_VECTOR  ]=0xE0;
	physMem.state.data[PhysicalMemory::SUBSYS_RESET_VECTOR+1]=0x00;

	physMem.state.data[PhysicalMemory::MAINSYS_RESET_VECTOR  ]=0xFE;
	physMem.state.data[PhysicalMemory::MAINSYS_RESET_VECTOR+1]=0x00;

	outside_world->CacheGamePadIndicesThatNeedUpdates();

	Reset();
	return true;
}

bool FM77AV::LoadROMFiles(std::string ROMPath)
{
	return physMem.LoadROMFiles(ROMPath);
}

MC6809 &FM77AV::CPU(unsigned int mainOrSub)
{
	if(CPU_SUB==mainOrSub)
	{
		return subCPU;
	}
	return mainCPU;
}
const MC6809 &FM77AV::CPU(unsigned int mainOrSub) const
{
	if(CPU_SUB==mainOrSub)
	{
		return subCPU;
	}
	return mainCPU;
}
MemoryAccess &FM77AV::MemAccess(unsigned int mainOrSub)
{
	if(CPU_SUB==mainOrSub)
	{
		return subMemAcc;
	}
	return mainMemAcc;
}

void FM77AV::PowerOn(void)
{
	Reset();
}
void FM77AV::Reset(void)
{
	// Sub-CPU reset vector is based on:
	//   F-BASIC Analysis Manual Phase III Sub-System, Fig. 2.2.1, pp. 31, Shuwa System Trading, 
	switch(state.machineType)
	{
	case MACHINETYPE_FM7:
		mainCPU.state.PC=0xFE00;
		subCPU.state.PC=0xE000;
		break;
	case MACHINETYPE_FM77AV:
	case MACHINETYPE_FM77AV40:
		mainCPU.state.PC=0x6000;
		subCPU.state.PC=0xE000;
		break;
	}
	state.subSysBusy=true; // Busy on reset.
	state.subSysHalt=false;
	state.mainToSubIRQ=false;
	state.nextDevicePollingTime=0;
	state.nextRenderingTime=0;
	state.next20msTimer=state.fm77avTime+FM77AVTIME_MILLISEC*20;
	state.next2msTimer=state.fm77avTime+FM77AVTIME_MILLISEC*2;

	state.timeBalance=0;  // Positive means mainCPU is ahead.  Negative subCPU ahead.
	state.timeDeficit=0;


	state.main.irqEnableBits=0;
	state.main.irqSource=0;
	state.main.firqSource=0;

	state.sub.irqEnableBits=~0;
	state.sub.irqSource=0;
	state.sub.firqSource=0;

	physMem.Reset();
	mainMemAcc.Reset();
	mainCPU.Reset();
	subMemAcc.Reset();
	subCPU.Reset();
	crtc.Reset();
	fdc.Reset();
	keyboard.Reset();
	dataRecorder.Reset();
}
bool FM77AV::SubCPUHalt(void) const
{
	return true==subCPU.state.halt ||
	       true==state.subSysHalt;
	       // || (machine type==FM-7, VRAM-access is set, VSYNC=false);
}
bool FM77AV::ExternalDevicePresent(void) const
{
	return true; // Has FDD.
}
unsigned int FM77AV::RunOneInstruction(void)
{
	unsigned int clocksPassed=0;
	uint64_t nanosec=0;
	if(true==mainCPU.state.halt && true==SubCPUHalt())
	{
		clocksPassed=2; // 2 is the minimum clocks, which is the minimum time resolution.
		nanosec=FM77AVTIME_MICROSEC;
		state.timeBalance=0;
	}
	else if(true==SubCPUHalt())
	{
		clocksPassed=mainCPU.RunOneInstruction(mainMemAcc);
		if(FM7_BIOS_ENTRY_ADDR==mainCPU.state.PC)
		{
			DetectMainCPUBIOSCall();
		}

		nanosec=clocksPassed;
		nanosec*=SCALE_NANO;
		nanosec/=mainCPU.state.freq;
	}
	else if(true==mainCPU.state.halt)
	{
		clocksPassed=subCPU.RunOneInstruction(subMemAcc);

		nanosec=clocksPassed;
		nanosec*=SCALE_NANO;
		nanosec/=mainCPU.state.freq;
	}
	else
	{
		unsigned int clocksPassed=0;

		if(0==state.timeBalance)
		{
			// Run Sub-CPU first.
			//   Main-CPU may halt Sub-CPU.
			//   Sub-CPU may never halt MainCPU.
			//   What happened in Cosmo Crash was:
			//     1. Main-CPU halted the Sub-CPU.
			//     2. Sub-CPU Busy Flag automatically set HI.
			//     3. Although Sub-CPU is not supposed to run at this time, it went ahead and executed one instruction.
			//     4. The one instruction exactly cleared the busy flag.
			//     5. Main-CPU waits for the sub-CPU busy flag, but never came.
			//     6. NMI accumurated on sub-CPU, destroyed stack.
			//     7. Stack eventually reached $D40A, which set the busy flag.
			//     8. Too late.
			int64_t subCPUClk=subCPU.RunOneInstruction(subMemAcc);
			int64_t subCPUTime=(subCPUClk*SCALE_NANO)/subCPU.state.freq;

			int64_t mainCPUClk=mainCPU.RunOneInstruction(mainMemAcc);
			int64_t mainCPUTime=(mainCPUClk*SCALE_NANO)/mainCPU.state.freq;

			state.timeBalance=mainCPUTime-subCPUTime;
			nanosec=std::max(mainCPUTime,subCPUTime);

			if(FM7_BIOS_ENTRY_ADDR==mainCPU.state.PC)
			{
				DetectMainCPUBIOSCall();
			}
		}
		else if(0<state.timeBalance) // Main CPU ahead
		{
			int64_t subCPUClk=subCPU.RunOneInstruction(subMemAcc);
			int64_t subCPUTime=(subCPUClk*SCALE_NANO)/subCPU.state.freq;
			state.timeBalance-=subCPUTime;
			nanosec=(0<state.timeBalance ? 0 : -state.timeBalance);
		}
		else // if(state.timeBalance<0) // Sub CPU ahead
		{
			int64_t mainCPUClk=mainCPU.RunOneInstruction(mainMemAcc);
			int64_t mainCPUTime=(mainCPUClk*SCALE_NANO)/mainCPU.state.freq;
			state.timeBalance+=mainCPUTime;
			nanosec=(state.timeBalance<0 ? 0 : state.timeBalance);

			if(FM7_BIOS_ENTRY_ADDR==mainCPU.state.PC)
			{
				DetectMainCPUBIOSCall();
			}
		}
	}

	state.fm77avTime+=nanosec;

	return nanosec;
}

std::string FM77AV::MachineTypeStr(void) const
{
	return ::MachineTypeToStr(state.machineType);
}

void FM77AV::PrintSubSystemCommand(void) const
{
	std::cout << "SubSysCmd($FC80):";
	for(int i=0; i<8; ++i)
	{
		std::cout << cpputil::Ubtox(physMem.state.data[PhysicalMemory::SUBSYS_SHARED_RAM_BEGIN+i]) << " ";
	}
	if(0x80!=physMem.state.data[PhysicalMemory::SUBSYS_SHARED_RAM_BEGIN])
	{
		auto cmd=physMem.state.data[PhysicalMemory::SUBSYS_SHARED_RAM_BEGIN+2];
		std::cout << SubCmdToStr(cmd) << " ";
		switch(cmd)
		{
		case FM7_SUBCMD_INKEY://                 0x29,
			break;
		case FM7_SUBCMD_DEFINE_PFKEY://          0x2A,
			break;
		case FM7_SUBCMD_GET_PFKEY_STR://         0x2B,
			break;
		case FM7_SUBCMD_INT_CTRL://              0x2C,
			break;
		case FM7_SUBCMD_SET_TIMER://             0x3D,
			break;
		case FM7_SUBCMD_READ_TIMER://            0x3E,
			break;
		case FM7_SUBCMD_TEST://                  0x3F,
			break;
		case FM7_SUBCMD_INIT://                  0x01,
			break;
		case FM7_SUBCMD_ERASE://                 0x02,
			break;
		case FM7_SUBCMD_PUT://                   0x03,
			{
				auto n=physMem.state.data[PhysicalMemory::SUBSYS_SHARED_RAM_BEGIN+3];
				for(int i=0; i<n; ++i)
				{
					auto chr=physMem.state.data[PhysicalMemory::SUBSYS_SHARED_RAM_BEGIN+4+i];
					if(0x20<=chr && chr<0x80)
					{
						std::cout << '\'' << chr << '\'';
					}
					else
					{
						std::cout << "($" << cpputil::Ubtox(chr) << ")";
					}
				}
				std::cout << std::endl;
			}
			break;
		case FM7_SUBCMD_GET://                   0x04,
			break;
		case FM7_SUBCMD_GETC://                  0x05,
			break;
		case FM7_SUBCMD_GET_CHAR_BLOCK1://       0x06,
			break;
		case FM7_SUBCMD_PUT_CHAR_BLOCK1://       0x07,
			break;
		case FM7_SUBCMD_GET_CHAR_BLOCK2://       0x08,
			break;
		case FM7_SUBCMD_PUT_CHAR_BLOCK2://       0x09,
			break;
		case FM7_SUBCMD_GET_BUFFER_ADDR://       0x0A,
			break;
		case FM7_SUBCMD_TABSET://                0x0B,
			break;
		case FM7_SUBCMD_CONSOLE_CONTROL://       0x0C,
			break;
		case FM7_SUBCMD_ERASE2://                0x0D,
			break;
		case FM7_SUBCMD_LINE://                  0x15,
			break;
		case FM7_SUBCMD_CHAIN://                 0x16,
			break;
		case FM7_SUBCMD_POINT://                 0x17,
			break;
		case FM7_SUBCMD_PAINT://                 0x18,
			break;
		case FM7_SUBCMD_SYMBOL://                0x19,
			break;
		case FM7_SUBCMD_CHANGE_COLOR://          0x1A,
			break;
		case FM7_SUBCMD_GET_BLOCK1://            0x1B,
			break;
		case FM7_SUBCMD_GET_BLOCK2://            0x1D,
			break;
		case FM7_SUBCMD_PUT_BLOCK1://            0x1C,
		case FM7_SUBCMD_PUT_BLOCK2://            0x1E,
			{
				uint32_t addr=PhysicalMemory::SUBSYS_SHARED_RAM_BEGIN;
				auto x1=physMem.NonDestructiveFetchWord(addr+3);
				auto y1=physMem.NonDestructiveFetchWord(addr+5);
				auto x2=physMem.NonDestructiveFetchWord(addr+7);
				auto y2=physMem.NonDestructiveFetchWord(addr+9);
				int CL=physMem.NonDestructiveFetchByte(addr+11);
				int FN=physMem.NonDestructiveFetchByte(addr+12);
				int len=physMem.NonDestructiveFetchByte(addr+13);
				std::cout << "(" << x1 << "," << y1 << ")-(" << x2 << "," << y2 << ") " << CL << " " << FN << " " << len << std::endl;
			}
			break;
		case FM7_SUBCMD_GCURSOR://               0x1F,
			break;
		case FM7_SUBCMD_CHAR_LINE://             0x20,
			break;
		}
	}
	std::cout << std::endl;
}

void FM77AV::DetectMainCPUBIOSCall(void)
{
	if(true==var.monitorBIOSCall)
	{
		std::cout << "BIOS Call:";
		for(uint16_t i=0; i<10; ++i)
		{
			std::cout << " " << cpputil::Ubtox(mainMemAcc.FetchByte(mainCPU.state.X+i));
		}
		std::cout << " " << BIOSCmdToStr(mainMemAcc.FetchByte(mainCPU.state.X)) << std::endl;
	}
}

bool FM77AV::NoWait(void) const
{
	return var.noWait;
}
