#include <algorithm>
#include <iostream>
#include "fm77av.h"
#include "fm77avdef.h"



FM77AV::FM77AV() :
	Device(this),
	physMem(this),
	mainMemAcc(this,&physMem),
	mainCPU(this),
	subMemAcc(this,&physMem),
	subCPU(this)
{
}

bool FM77AV::SetUp(FM77AVParam &param)
{
	state.machineType=param.machineType;
	if(true!=LoadROMFiles(param.ROMPath))
	{
		std::cout << "Failed to load ROM files." << std::endl;
		return false;
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
}
bool FM77AV::SubCPUHalt(void) const
{
	return true==subCPU.state.halt ||
	       true==state.subSysHalt;
	       // || (machine type==FM-7, VRAM-access is set, VSYNC=false);
}
bool FM77AV::ExternalDevicePresent(void) const
{
	return false; // Tentatively no external devices.
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
			int64_t mainCPUClk=mainCPU.RunOneInstruction(mainMemAcc);
			int64_t subCPUClk=subCPU.RunOneInstruction(subMemAcc);

			int64_t mainCPUTime=(mainCPUClk*SCALE_NANO)/mainCPU.state.freq;
			int64_t subCPUTime=(subCPUClk*SCALE_NANO)/subCPU.state.freq;

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
		case FM7_SUBCMD_PUT_BLOCK1://            0x1C,
			break;
		case FM7_SUBCMD_GET_BLOCK2://            0x1D,
			break;
		case FM7_SUBCMD_PUT_BLOCK2://            0x1E,
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
