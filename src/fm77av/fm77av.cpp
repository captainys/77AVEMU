#include <algorithm>
#include <iostream>
#include <fstream>
#include <memory>
#include "fm77av.h"
#include "fm77avdef.h"
#include "fm77avrender.h"
#include "outside_world.h"

#include "../exas_compiler/exas_compiler.h"

FM77AV::FM77AV() :
	Device(this),
	physMem(this),
	mainMemAcc(this,&physMem),
	mainCPU(this),
	subMemAcc(this,&physMem),
	subCPU(this),
	crtc(this),
	keyboard(this),
	fdc(this),
	dataRecorder(this),
	sound(this),
	gameport(this),
	serialport(this),
	dmac(this)
{
	allDevices.push_back(&mainCPU);
	allDevices.push_back(&subCPU);
	allDevices.push_back(&physMem);
	allDevices.push_back(&mainMemAcc);
	allDevices.push_back(&subMemAcc);
	allDevices.push_back(&crtc);
	allDevices.push_back(&keyboard);
	allDevices.push_back(&fdc);
	allDevices.push_back(&dataRecorder);
	allDevices.push_back(&sound);
	allDevices.push_back(&gameport);
	allDevices.push_back(&serialport);
	VMBase::CacheDeviceIndex();

	for(auto &b : var.breakOnSubCmd)
	{
		b=0;
	}

	var.powerOffAt.type=ADDR_NONE;
}
/* static */ unsigned int FM77AV::StrToAddressType(std::string str)
{
	cpputil::Capitalize(str);
	if("M"==str || "MAIN"==str)
	{
		return ADDR_MAIN;
	}
	else if("S"==str || "SUB"==str)
	{
		return ADDR_SUB;
	}
	else if("P"==str || "PHYS"==str)
	{
		return ADDR_PHYS;
	}
	else if("V"==str || "VRAM"==str)
	{
		return ADDR_VRAM;
	}
	else if("VR"==str || "VRAMR"==str || "VRAMRAW"==str)
	{
		return ADDR_VRAM_RAW;
	}
	else ADDR_NONE;
}

bool FM77AV::SetUp(const FM77AVParam &param,Outside_World *outside_world)
{
	var.fileNameAlias=param.fileNameAlias;  // Do it before loading tape and disk images.
	var.imgSearchPaths=param.imgSearchPaths;  // Do it before loading tape and disk images.

	std::string ROMPath,FB30ROM;
	cpputil::SeparatePathFile(ROMPath,FB30ROM,FindFile(cpputil::MakeFullPathName(param.ROMPath,"FBASIC30.ROM")));

#ifdef ENABLE_EXAS_COMPILER
	exasCompiler.LoadROMFiles(ROMPath);
#endif

	state.machineType=param.machineType;
	if(MACHINETYPE_AUTO==state.machineType)
	{
		state.machineType=LoadROMFilesAndIdentifyMachineType(ROMPath);
		if(MACHINETYPE_UNKNOWN==state.machineType)
		{
			std::cout << "Insufficient ROM files." << std::endl;
			return false;
		}
	}
	else
	{
		if(true!=LoadROMFiles(ROMPath))
		{
			std::cout << "Failed to load ROM files." << std::endl;
			return false;
		}
	}

	var.noWait=param.noWait;

	if(""!=param.t77Path)
	{
		if(true==dataRecorder.LoadT77(FindFile(param.t77Path)))
		{
			std::cout << "Loaded Cassette: " << param.t77Path << std::endl;
		}
		else
		{
			std::cout << "Failed to load: " << param.t77Path << std::endl;
			return false;
		}
	}
	if(""!=param.t77SavePath)
	{
		dataRecorder.LoadAutoSaveT77(FindFile(param.t77SavePath));
		dataRecorder.state.toSave.Dniwer();
		dataRecorder.state.toSave.t77.fName=param.t77SavePath; // If not exist, force set file name.
	}

	fdc.searchPaths=param.imgSearchPaths;
	for(int drv=0; drv<4; ++drv)
	{
		if(""!=param.fdImgFName[drv])
		{
			fdc.LoadD77orRAW(drv,FindFile(param.fdImgFName[drv]).c_str());
			if(true==param.fdImgWriteProtect[drv])
			{
				// D77 image may have write-protect switch.
				// Enable write protect only when specified by the parameter.
				fdc.SetWriteProtect(drv,true);
			}
		}
	}


	for(int i=0; i<FM77AV_NUM_GAMEPORTS; ++i)
	{
		outside_world->gamePort[i]=param.gamePort[i];
		gameport.state.ports[i].device=FM77AVGamePort::EmulationTypeToDeviceType(param.gamePort[i]);
		gameport.state.ports[i].maxButtonHoldTime[0]=param.maxButtonHoldTime[i][0];
		gameport.state.ports[i].maxButtonHoldTime[1]=param.maxButtonHoldTime[i][1];
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

	if(CPU_UNKNOWN!=param.powerOffAtCPUType)
	{
		var.powerOffAt.type=param.powerOffAtCPUType;
		var.powerOffAt.addr=param.powerOffAtAddr;
		CPU(param.powerOffAtCPUType).debugger.SetBreakPoint(param.powerOffAtAddr,param.powerOffAtAddr);
	}

	keyboard.var.autoStopAfterThis=param.autoStopType;
	keyboard.var.autoStopKey=param.autoStopKey;

	var.quickScrnShotDir=param.quickScrnShotDir;
	var.initialCmd=param.initialCmd;

	if(0<param.playbackEventLogFName.size())
	{
		if(true==eventLog.LoadEventLog(param.playbackEventLogFName))
		{
			eventLog.BeginPlayback(state.fm77avTime);
		}
	}

	var.autoLoadTapeFile=param.autoLoadTapeFile;

	for(int i=0; i<FM7_MAX_NUM_COMPORTS; ++i)
	{
		serialport.state.enabled[i]=param.enableCOM[i];
	}

	for(auto hsc : param.hostShortCutKeys)
	{
		outside_world->RegisterHostShortCut(hsc.hostKey,hsc.ctrl,hsc.shift,hsc.cmdStr);
	}
	outside_world->scaling=param.scaling;
	outside_world->windowShift=param.windowShift;
	outside_world->autoScaling=param.autoScaling;
	outside_world->windowModeOnStartUp=param.windowModeOnStartUp;

	outside_world->virtualKeys.clear();
	for(auto vk : param.virtualKeys)
	{
		auto fm77avKey=FM77AVKeyLabelToKeyCode(vk.fm77avKey);
		if(0!=fm77avKey)
		{
			Outside_World::VirtualKey ovk;
			ovk.fm77avKey=fm77avKey;
			ovk.physicalId=vk.physicalId;
			ovk.button=vk.button;
			outside_world->virtualKeys.push_back(ovk);
		}
	}

	outside_world->CacheGamePadIndicesThatNeedUpdates();
	outside_world->keyboardMode=param.keyboardMode;

	if(""!=param.symTableFName)
	{
		LoadSymbolTable(param.symTableFName);
	}

	Reset();
	return true;
}

bool FM77AV::LoadROMFiles(std::string ROMPath)
{
	return physMem.LoadROMFiles(ROMPath);
}

unsigned int FM77AV::LoadROMFilesAndIdentifyMachineType(std::string ROMPath)
{
	state.machineType=MACHINETYPE_FM77AV40;
	if(true==physMem.LoadROMFiles(ROMPath))
	{
		return MACHINETYPE_FM77AV40;
	}
	state.machineType=MACHINETYPE_FM77AV;
	if(true==physMem.LoadROMFiles(ROMPath))
	{
		return MACHINETYPE_FM77AV;
	}
	state.machineType=MACHINETYPE_FM7;
	if(true==physMem.LoadROMFiles(ROMPath))
	{
		return MACHINETYPE_FM7;
	}
	return MACHINETYPE_UNKNOWN;
}

/*static*/ std::vector <std::string> FM77AV::CheckMissingROMFiles(const FM77AVParam &param)
{
	auto machineType=param.machineType;
	if(MACHINETYPE_AUTO==machineType)
	{
		machineType=MACHINETYPE_FM7; // Minimum FM-7 ROMs are required.
	}

	std::vector <std::string> missing;

	std::unique_ptr <HasROMImages> physMem(new HasROMImages);
	for(auto toLoad : physMem->GetROMToLoad(machineType))
	{
		auto fName=cpputil::MakeFullPathName(param.ROMPath,toLoad.fName);
		auto fNameAlt=cpputil::MakeFullPathName(param.ROMPath,toLoad.fNameAlt);
		fName=FindFile(fName,param.fileNameAlias,param.imgSearchPaths);
		fNameAlt=FindFile(fNameAlt,param.fileNameAlias,param.imgSearchPaths);
		std::ifstream ifp(fName,std::ios::binary);
		std::ifstream ifpAlt(fNameAlt,std::ios::binary);
		if(true!=ifp.is_open() && true!=ifpAlt.is_open())
		{
			missing.push_back(toLoad.fName);
			if(""!=toLoad.fNameAlt)
			{
				missing.back().push_back('(');
				missing.back()+=toLoad.fNameAlt;
				missing.back().push_back(')');
			}
		}
	}

	return missing;
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

void FM77AV::SetFM8Speed(void)
{
	mainCPU.state.freq=FM8_FREQ;
	subCPU.state.freq=FM8_FREQ;
	state.CRTCHaltsSubCPU=true;
}
void FM77AV::SetFM7Speed(void)
{
	mainCPU.state.freq=FM7_FREQ;
	subCPU.state.freq=FM7_FREQ;
	state.CRTCHaltsSubCPU=true;
}
void FM77AV::SetFM77Speed(void)
{
	mainCPU.state.freq=FM77_FREQ;
	subCPU.state.freq=FM77_FREQ;
	state.CRTCHaltsSubCPU=false;
}

void FM77AV::PowerOn(void)
{
	Reset();
}
void FM77AV::Reset(void)
{
	Device::Reset();

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
	state.subNMIMask=false;
	state.nextDevicePollingTime=0;
	state.nextRenderingTime=0;
	state.next20msTimer=state.fm77avTime+FM77AVTIME_MILLISEC*20;
	state.next2msTimer=state.fm77avTime+FM77AVTIME_MILLISEC*2;

	state.timeBalance=0;  // Positive means mainCPU is ahead.  Negative subCPU ahead.
	state.timeDeficit=0;


	state.main.irqEnableBits=SystemState::MAIN_IRQ_SOURCE_YM2203C;
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
	       true==state.subSysHalt ||
	       (true==state.CRTCHaltsSubCPU && true==crtc.state.VRAMAccessFlag && true!=crtc.InBlank(state.fm77avTime))
	       ;
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
	bool subCPUHalt=SubCPUHalt();
	if(true!=mainCPU.state.halt && true!=subCPUHalt)
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
	else if(true!=mainCPU.state.halt)
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
	else if(true!=subCPUHalt)
	{
		clocksPassed=subCPU.RunOneInstruction(subMemAcc);

		nanosec=clocksPassed;
		nanosec*=SCALE_NANO;
		nanosec/=mainCPU.state.freq;
	}
	else // if(true==mainCPU.state.halt && true==subCPUHalt)
	{
		clocksPassed=2; // 2 is the minimum clocks, which is the minimum time resolution.
		nanosec=FM77AVTIME_MICROSEC;
		state.timeBalance=0;
	}

	state.fm77avTime+=nanosec;

	if(0!=state.subCPUTemporaryReadyTimeDelta)
	{
		state.subCPUTemporaryReadyTime=state.fm77avTime+state.subCPUTemporaryReadyTimeDelta;
		state.subCPUTemporaryReadyTimeDelta=0;
	}

	return nanosec;
}

void FM77AV::TypeCommandForStartingTapeProgram(void)
{
	auto files=dataRecorder.state.primary.t77.Files();
	if(0<files.size())
	{
		std::string cmd;
		switch(files[0].fType)
		{
		case FM7File::FTYPE_BASIC_BINARY:
		case FM7File::FTYPE_BASIC_ASCII:
			cmd="RUN\"\"";
			break;
		case FM7File::FTYPE_BINARY:
			cmd="LOADM\"\",,R";
			break;
		default:
			cmd="PRINT \"Not a F-BASIC file.\"";
			break;
		}
		keyboard.var.autoType.push(0x0D);
		for(auto c : cmd)
		{
			keyboard.var.autoType.push(c);
		}
		keyboard.Type(0x0D); // Trigger auto-type.
	}
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

std::string FM77AV::FileNameAlias(std::string input) const
{
	return FileNameAlias(input,var.fileNameAlias);
}

/* static */ std::string FM77AV::FileNameAlias(std::string input,const std::unordered_map<std::string,std::string> &aliases)
{
	auto INPUT=input;
	cpputil::Capitalize(INPUT);
	auto found=aliases.find(INPUT);
	if(found!=aliases.end())
	{
		return found->second;
	}
	return input;
}

std::string FM77AV::FindFile(std::string fName) const
{
	return FindFile(fName,var.fileNameAlias,var.imgSearchPaths);
}

/*static*/ std::string FM77AV::FindFile(std::string fName,const std::unordered_map<std::string,std::string> &aliases,const std::vector <std::string> &imgSearchPaths)
{
	fName=FileNameAlias(fName,aliases);
#ifdef _WIN32
	if(2<=fName.size() && ':'==fName[1])
	{
		// Must be starting with the drive letter.
		return fName;
	}
#endif
	if('/'==fName[0] || '\\'==fName[0])
	{
		// Must be a full-path name.
		return fName;
	}

	// Then must be a relative path.
	// If it finds from the CWD, return it.
	std::ifstream ifp(fName,std::ios::binary);
	if(true==ifp.is_open())
	{
		return fName;
	}

	for(auto dir : imgSearchPaths)
	{
		auto ful=cpputil::MakeFullPathName(dir,fName);
		std::ifstream ifp(ful,std::ios::binary);
		if(true==ifp.is_open())
		{
			return ful;
		}
	}

	return fName;
}

bool FM77AV::NoWait(void) const
{
	if(true==var.noWaitWhileTapeMotorOn && true==dataRecorder.state.motor)
	{
		return true;
	}
	return var.noWait;
}

void FM77AV::RunFastDevicePollingInternal(void)
{
	sound.SoundPolling(state.fm77avTime);
	state.nextFastDevicePollingTime=state.fm77avTime+FAST_DEVICE_POLLING_INTERVAL;
}

void FM77AV::ForceRender(class FM77AVRender &render,class Outside_World *outside_world)
{
	render.Prepare(*this);
	render.BuildImage(crtc.GetPalette());
	if(true==outside_world->ImageNeedsFlip())
	{
		render.FlipUpsideDown();
	}
	outside_world->Render(render.GetImage(),*this);
	outside_world->UpdateStatusBitmap(*this);
}

void FM77AV::RenderQuiet(class FM77AVRender &render)
{
	render.Prepare(*this);
	render.BuildImage(crtc.GetPalette());
}

void FM77AV::SetGamePadState(int port,bool Abutton,bool Bbutton,bool left,bool right,bool up,bool down,bool run,bool pause)
{
	auto &p=gameport.state.ports[port&1];
	p.SetGamePadState(Abutton,Bbutton,left,right,up,down,run,pause,state.fm77avTime);
}

std::vector <std::string> FM77AV::GetIRQStatusText(void) const
{
	std::vector <std::string> text;
	text.push_back("Main CPU");

	text.push_back("Pending IRQ:");
	if(0==state.main.irqSource)
	{
		text.back()+="None";
	}
	else
	{
		if(0!=(SystemState::MAIN_IRQ_SOURCE_KEY&state.main.irqSource))
		{
			text.back()+="KEY ";
		}
		if(0!=(SystemState::MAIN_IRQ_SOURCE_PRINTER&state.main.irqSource))
		{
			text.back()+="PRINTER ";
		}
		if(0!=(SystemState::MAIN_IRQ_SOURCE_TIMER&state.main.irqSource))
		{
			text.back()+="TIMER ";
		}
		if(0!=(SystemState::MAIN_IRQ_SOURCE_FDC&state.main.irqSource))
		{
			text.back()+="FDC ";
		}
		if(0!=(SystemState::MAIN_IRQ_SOURCE_EXT&state.main.irqSource))
		{
			text.back()+="EXT ";
		}
		if(0!=(SystemState::MAIN_IRQ_SOURCE_YM2203C&state.main.irqSource))
		{
			text.back()+="YM2203C ";
		}
		if(0!=(SystemState::MAIN_IRQ_SOURCE_DMA&state.main.irqSource))
		{
			text.back()+="DMA ";
		}
	}
	text.push_back("Pending FIRQ:");
	if(0==state.main.firqSource)
	{
		text.back()+="None";
	}
	else
	{
		if(0!=(SystemState::MAIN_FIRQ_SOURCE_ATTENTION&state.main.firqSource))
		{
			text.back()+="ATTENTION ";
		}
		if(0!=(SystemState::MAIN_FIRQ_SOURCE_BREAK_KEY&state.main.firqSource))
		{
			text.back()+="BREAKKEY";
		}
	}

	text.push_back("Sub CPU");

	text.push_back("Pending IRQ:");
	if(0==state.sub.irqSource)
	{
		text.back()+="None";
	}
	else
	{
		if(0!=(SystemState::SUB_IRQ_SOURCE_CANCEL_REQ&state.sub.irqSource))
		{
			text.back()+="CANCEL ";
		}
	}
	text.push_back("Pending FIRQ:");
	if(0==state.sub.firqSource)
	{
		text.back()+="None";
	}
	else
	{
		if(0!=(SystemState::SUB_FIRQ_SOURCE_KEY&state.sub.firqSource))
		{
			text.back()+="KEY ";
		}
	}
	return text;
}

bool FM77AV::LoadSymbolTable(std::string fName)
{
	std::ifstream ifp(fName);
	symTableFName=fName; // Maybe new.  Store it regardless of existing or not existing.
	if(true==ifp.is_open())
	{
		std::vector <std::string> mainSym,subSym;
		bool nowMain=true;
		while(true!=ifp.eof())
		{
			std::string str;
			std::getline(ifp,str);
			if("/main"==str)
			{
				nowMain=true;
			}
			else if("/sub"==str)
			{
				nowMain=false;
			}
			else
			{
				if(true==nowMain)
				{
					mainSym.push_back(str);
				}
				else
				{
					subSym.push_back(str);
				}
			}
		}
		mainCPU.debugger.symTable.Load(mainSym);
		subCPU.debugger.symTable.Load(subSym);

		return true;
	}
	return false;
}
bool FM77AV::AutoSaveSymbolTable(void) const
{
	if(""!=symTableFName)
	{
		std::ofstream ofp(symTableFName);
		if(true==ofp.is_open())
		{
			auto mainSym=mainCPU.debugger.symTable.Save();
			auto subSym=subCPU.debugger.symTable.Save();

			if(0<mainSym.size())
			{
				ofp << "/main" << std::endl;
				for(auto txt : mainSym)
				{
					ofp << txt << std::endl;
				}
			}
			if(0<subSym.size())
			{
				ofp << "/sub" << std::endl;
				for(auto txt : subSym)
				{
					ofp << txt << std::endl;
				}
			}

			return true;
		}
		std::cout << "Cannot Open Symbol File." << std::endl;
		return false;
	}
	else
	{
		std::cout << "Symbol File Name Not Set." << std::endl;
		std::cout << "Cannot Auto-Save." << std::endl;
		return false;
	}
}
