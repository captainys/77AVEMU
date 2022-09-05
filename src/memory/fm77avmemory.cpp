#include <iostream>
#include <fstream>
#include <string>
#include "memory.h"
#include "mc6809util.h"
#include "fm77av.h"
#include "cpputil.h"
#include "fm77avmemory.h"
#include "fm77avdef.h"

std::vector <HasROMImages::ROMToLoad> HasROMImages::GetROMToLoad(unsigned int machineType)
{
	// FM-7
	//   BOOT_BAS.ROM
	//   BOOT_DOS.ROM
	//   FBASIC30.ROM
	//   SUBSYS_C.ROM or SUBSYS.ROM
	if(machineType<MACHINETYPE_FM77AV)
	{
		std::vector <ROMToLoad> lst=
		{
			{true, ROM_BOOT_BASIC,BOOT_ROM_SIZE,                              "BOOT_BAS.ROM",""},
			{true, ROM_BOOT_DOS,  BOOT_ROM_SIZE,                              "BOOT_DOS.ROM",""},
			{true, ROM_FBASIC,    FBASIC_ROM_SIZE,                            "FBASIC30.ROM",""},
			{true, ROM_SUBSYS_C,  SUBSYS_MONITOR_ROM_SIZE+ASCII_FONT_ROM_SIZE,"SUBSYS_C.ROM","SUBSYS.ROM"},
			{false,ROM_KANJI,     KANJI_ROM_SIZE,                             "KANJI1.ROM","KANJI.ROM"},
		};
		return lst;
	}
	// FM77AV
	//   BOOT_BAS.ROM
	//   BOOT_DOS.ROM
	//   FBASIC30.ROM
	//   INITIATE.ROM
	//   KANJI.ROM
	//   SUBSYSCG.ROM
	//   SUBSYS_A.ROM
	//   SUBSYS_B.ROM
	//   SUBSYS_C.ROM
	else if(MACHINETYPE_FM77AV==machineType)
	{
		std::vector <ROMToLoad> lst=
		{
			{true, ROM_BOOT_BASIC,BOOT_ROM_SIZE,                              "BOOT_BAS.ROM",""},
			{true, ROM_BOOT_DOS,  BOOT_ROM_SIZE,                              "BOOT_DOS.ROM",""},
			{true, ROM_FBASIC,    FBASIC_ROM_SIZE,                            "FBASIC30.ROM",""},
			{true, ROM_INITIATOR, INITIATOR_ROM_SIZE,                         "INITIATE.ROM",""},
			{true ,ROM_KANJI,     KANJI_ROM_SIZE,                             "KANJI1.ROM","KANJI.ROM"},
			{true, ROM_ASCII_FONT,4*ASCII_FONT_ROM_SIZE,                      "SUBSYSCG.ROM",""},
			{true, ROM_SUBSYS_A,  SUBSYS_MONITOR_ROM_SIZE,                    "SUBSYS_A.ROM",""},
			{true, ROM_SUBSYS_B,  SUBSYS_MONITOR_ROM_SIZE,                    "SUBSYS_B.ROM",""},
			{true, ROM_SUBSYS_C,  SUBSYS_MONITOR_ROM_SIZE+ASCII_FONT_ROM_SIZE,"SUBSYS_C.ROM","SUBSYS.ROM"},
		};
		return lst;
	}
	// FM77AV40
	//   BOOT_BAS.ROM
	//   BOOT_DOS.ROM
	//   FBASIC30.ROM
	//   INITIATE.ROM
	//   KANJI1.ROM or KANJI.ROM
	//   KANJI2.ROM
	//   SUBSYSCG.ROM
	//   SUBSYS_A.ROM
	//   SUBSYS_B.ROM
	//   SUBSYS_C.ROM
	//   DICROM.ROM
	else if(MACHINETYPE_FM77AV40==machineType)
	{
		std::vector <ROMToLoad> lst=
		{
			{true, ROM_BOOT_BASIC,BOOT_ROM_SIZE,                              "BOOT_BAS.ROM",""},
			{true, ROM_BOOT_DOS,  BOOT_ROM_SIZE,                              "BOOT_DOS.ROM",""},
			{true, ROM_FBASIC,    FBASIC_ROM_SIZE,                            "FBASIC30.ROM",""},
			{true, ROM_INITIATOR, INITIATOR_ROM_SIZE,                         "INITIATE.ROM",""},
			{true ,ROM_KANJI,     KANJI_ROM_SIZE,                             "KANJI1.ROM","KANJI.ROM"},
			{false,ROM_KANJI2,    KANJI_ROM_SIZE,                             "KANJI2.ROM",""},
			{true, ROM_ASCII_FONT,4*ASCII_FONT_ROM_SIZE,                      "SUBSYSCG.ROM",""},
			{true, ROM_SUBSYS_A,  SUBSYS_MONITOR_ROM_SIZE,                    "SUBSYS_A.ROM",""},
			{true, ROM_SUBSYS_B,  SUBSYS_MONITOR_ROM_SIZE,                    "SUBSYS_B.ROM",""},
			{true, ROM_SUBSYS_C,  SUBSYS_MONITOR_ROM_SIZE+ASCII_FONT_ROM_SIZE,"SUBSYS_C.ROM","SUBSYS.ROM"},
			{false,ROM_DIC,       DIC_ROM_SIZE,                               "DICROM.ROM",""},
		};
		return lst;
	}

	// If unknown, return the longest list.
	std::vector <ROMToLoad> lst=
	{
		{true, ROM_BOOT_BASIC,BOOT_ROM_SIZE,                              "BOOT_BAS.ROM",""},
		{true, ROM_BOOT_DOS,  BOOT_ROM_SIZE,                              "BOOT_DOS.ROM",""},
		{true, ROM_FBASIC,    FBASIC_ROM_SIZE,                            "FBASIC30.ROM",""},
		{true, ROM_INITIATOR, INITIATOR_ROM_SIZE,                         "INITIATE.ROM",""},
		{true ,ROM_KANJI,     KANJI_ROM_SIZE,                             "KANJI1.ROM","KANJI.ROM"},
		{false,ROM_KANJI2,    KANJI_ROM_SIZE,                             "KANJI2.ROM",""},
		{true, ROM_ASCII_FONT,4*ASCII_FONT_ROM_SIZE,                      "SUBSYSCG.ROM",""},
		{true, ROM_SUBSYS_A,  SUBSYS_MONITOR_ROM_SIZE,                    "SUBSYS_A.ROM",""},
		{true, ROM_SUBSYS_B,  SUBSYS_MONITOR_ROM_SIZE,                    "SUBSYS_B.ROM",""},
		{true, ROM_SUBSYS_C,  SUBSYS_MONITOR_ROM_SIZE+ASCII_FONT_ROM_SIZE,"SUBSYS_C.ROM","SUBSYS.ROM"},
		{false,ROM_DIC,       DIC_ROM_SIZE,                               "DICROM.ROM",""},
	};
	return lst;
}

bool HasROMImages::LoadROMFiles(std::string ROMPath,unsigned int machineType)
{
	auto toLoad=GetROMToLoad(machineType);

	for(auto rom : toLoad)
	{
		auto fullPath0=cpputil::MakeFullPathName(ROMPath,rom.fName);
		std::ifstream ifp(fullPath0,std::ios::binary);
		if(true!=ifp.is_open())
		{
			auto fullPath1=cpputil::MakeFullPathName(ROMPath,rom.fNameAlt);
			ifp.open(fullPath1,std::ios::binary);
		}
		if(true!=ifp.is_open())
		{
			if(true==rom.mandatory)
			{
				std::cout << "Failed to load " << fullPath0 << std::endl;
				std::cout << "Which is mandatory for emulating " << MachineTypeToStr(machineType) << std::endl;
				return false;
			}
			else
			{
				continue;
			}
		}
		ifp.read((char *)rom.ptr,rom.length);
	}

	return true;
}

////////////////////////////////////////////////////////////

PhysicalMemory::PhysicalMemory(VMBase *vmBase) : Device(vmBase)
{
	for(auto &d : state.data)
	{
		d=0;
	}
	for(auto &d : state.extVRAM)
	{
		d=0;
	}
	for(auto &d : state.av40DicRAM)
	{
		d=0;
	}
	for(auto &d : ROM_BOOT_DOS)
	{
		d=0xFF;
	}
	for(auto &d : ROM_BOOT_BASIC)
	{
		d=0xFF;
	}
	for(auto &d : ROM_FBASIC)
	{
		d=0xFF;
	}
	for(auto &d : ROM_INITIATOR)
	{
		d=0xFF;
	}
	for(auto &d : ROM_KANJI)
	{
		d=0xFF;
	}
	for(auto &d : ROM_KANJI2)
	{
		d=0xFF;
	}
	for(auto &d : ROM_DIC)
	{
		d=0xFF;
	}
	for(auto &d : ROM_ASCII_FONT)
	{
		d=0xFF;
	}
	for(auto &d : ROM_SUBSYS_A)
	{
		d=0xFF;
	}
	for(auto &d : ROM_SUBSYS_B)
	{
		d=0xFF;
	}
	for(auto &d : ROM_SUBSYS_C)
	{
		d=0xFF;
	}

	for(auto &t : memType)
	{
		t=MEMTYPE_RAM;
	}
	for(unsigned int addr=SUBSYS_VRAM_BEGIN; addr<SUBSYS_VRAM_END; ++addr)
	{
		memType[addr]=MEMTYPE_SUBSYS_VRAM;
	}
	for(unsigned int addr=SUBSYS_RAM_C000_BEGIN; addr<SUBSYS_RAM_C000_END; ++addr)
	{
		memType[addr]=MEMTYPE_SUBSYS_RAM_C000;
	}
	for(unsigned int addr=SUBSYS_SHARED_RAM_BEGIN; addr<SUBSYS_SHARED_RAM_END; ++addr)
	{
		memType[addr]=MEMTYPE_SUBSYS_SHARED_RAM;
	}
	for(unsigned int addr=SUBSYS_IO_BEGIN; addr<SUBSYS_IO_END; ++addr)
	{
		memType[addr]=MEMTYPE_SUBSYS_IO;
	}
	for(unsigned int addr=SUBSYS_FONT_ROM_BEGIN; addr<SUBSYS_FONT_ROM_END; ++addr)
	{
		memType[addr]=MEMTYPE_SUBSYS_FONT_ROM;
	}
	for(unsigned int addr=SUBSYS_MONITOR_ROM_BEGIN; addr<SUBSYS_MONITOR_ROM_END; ++addr)
	{
		memType[addr]=MEMTYPE_SUBSYS_MONITOR_ROM;
	}

	for(unsigned int addr=AV40_DICRAM_BEGIN; addr<AV40_DICRAM_END; ++addr)
	{
		memType[addr]=MEMTYPE_AV40_DICRAM;
	}
	for(unsigned int addr=AV40_DICROM_BEGIN; addr<AV40_DICROM_END; ++addr)
	{
		memType[addr]=MEMTYPE_AV40_DICROM;
	}

	for(unsigned int addr=MAINSYS_INITIATOR_ROM_BEGIN; addr<MAINSYS_INITIATOR_ROM_END; ++addr)
	{
		memType[addr]=MEMTYPE_MAINSYS_INITIATOR_ROM;
	}
	for(unsigned int addr=MAINSYS_FBASIC_ROM_BEGIN; addr<MAINSYS_FBASIC_ROM_END; ++addr)
	{
		memType[addr]=MEMTYPE_MAINSYS_FBASIC_ROM;
	}
	for(unsigned int addr=MAINSYS_SHARED_RAM_BEGIN; addr<MAINSYS_SHARED_RAM_END; ++addr)
	{
		memType[addr]=MEMTYPE_MAINSYS_SHARED_RAM;
	}
	for(unsigned int addr=MAINSYS_IO_BEGIN; addr<MAINSYS_IO_END; ++addr)
	{
		memType[addr]=MEMTYPE_MAINSYS_IO;
	}
	for(unsigned int addr=MAINSYS_BOOT_ROM_BEGIN; addr<MAINSYS_BOOT_ROM_END; ++addr)
	{
		memType[addr]=MEMTYPE_MAINSYS_BOOT_ROM;
	}
	memType[0x3FFFE]=MEMTYPE_MAIN_RESET_VECTOR;
	memType[0x3FFFF]=MEMTYPE_MAIN_RESET_VECTOR;
	for(unsigned int addr=PHYSMEM_SIZE; addr<PHYSMEM_ADDR_SIZE; ++addr)
	{
		memType[addr]=MEMTYPE_NOT_EXIST;
	}

	// Experimented on actual FM77AV.
	// Default memory contents for $00000 to $0FFFF are $FF.
	for(uint32_t addr=0; addr<65536; ++addr)
	{
		state.data[addr]=0xFF;
	}
}

bool PhysicalMemory::LoadROMFiles(std::string ROMPath)
{
	auto fm77avPtr=(FM77AV *)vmPtr;
	return HasROMImages::LoadROMFiles(ROMPath,fm77avPtr->state.machineType);
}

void PhysicalMemory::WriteFD04(uint8_t data)
{
	auto fm77avPtr=(FM77AV *)vmPtr;
	if(MACHINETYPE_FM77AV40<=fm77avPtr->state.machineType)
	{
		state.av40SubRAMBWriteProtect=(0==(data&4));
	}
}
void PhysicalMemory::WriteFD10(uint8_t data)
{
	state.avBootROM=(0==(data&2));
}
void PhysicalMemory::WriteFD20(uint8_t data)
{
	uint16_t data16=data;
	state.kanjiAddr&=0xFF;
	state.kanjiAddr|=(data16<<8);
}
void PhysicalMemory::WriteFD21(uint8_t data)
{
	state.kanjiAddr&=0xFF00;
	state.kanjiAddr|=data;
}
uint8_t PhysicalMemory::ReadFD22(void) const
{
	uint32_t ptr=state.kanjiAddr;
	return ROM_KANJI[ptr<<1];
}
uint8_t PhysicalMemory::ReadFD23(void) const
{
	uint32_t ptr=state.kanjiAddr;
	return ROM_KANJI[(ptr<<1)+1];
}

void PhysicalMemory::WriteFD13(uint8_t data)
{
	auto fm77avPtr=(FM77AV *)vmPtr;
	if(MACHINETYPE_FM77AV<=fm77avPtr->state.machineType)
	{
		auto prev=state.subMonType;
		if(MACHINETYPE_FM77AV40<=fm77avPtr->state.machineType)
		{
			data&=7;
		}
		else
		{
			data&=3;
		}
		switch(data)
		{
		case 0:
			state.subMonType=SUBMON_C;
			break;
		case 1:
			state.subMonType=SUBMON_A;
			break;
		case 2:
			state.subMonType=SUBMON_B;
			break;
		case 4:
			if(MACHINETYPE_FM77AV40<=fm77avPtr->state.machineType)
			{
				state.subMonType=SUBMON_RAM;
			}
			break;
		}
		if(prev!=state.subMonType)
		{
			state.subROMSwitch=true;
		}
	}
}
void PhysicalMemory::WriteFD2E(uint8_t data)
{
	auto fm77avPtr=(FM77AV *)vmPtr;
	if(MACHINETYPE_FM77AV40<=fm77avPtr->state.machineType)
	{
		state.av40DicRAMEnabled=(0!=(data&0x80));
		state.av40DicROMEnabled=(0!=(data&0x40));
		state.av40DicROMBank=(data&0x3F);
	}
}
void PhysicalMemory::WriteD42E(uint8_t data)
{
	state.av40SubRAMABank=((data>>3)&3);
	state.av40SubRAMBBank=(data&7);
	state.av40SubKanjiLevel=(data>>7);
}
void PhysicalMemory::WriteD42F(uint8_t data)
{
	auto fm77avPtr=(FM77AV *)vmPtr;
	if(SCRNMODE_320X200_260KCOL==fm77avPtr->crtc.state.scrnMode ||
	   SCRNMODE_640X400==fm77avPtr->crtc.state.scrnMode)
	{
		state.VRAMBank=(data&3);
		if(3==state.VRAMBank)
		{
			state.VRAMBank=0;
		}
	}
}
void PhysicalMemory::WriteD430(uint8_t data)
{
	auto fm77avPtr=(FM77AV *)vmPtr;
	if(SCRNMODE_640X200==fm77avPtr->crtc.state.scrnMode ||
	   SCRNMODE_320X200_4096COL==fm77avPtr->crtc.state.scrnMode)
	{
		state.VRAMBank=((data>>5)&1);
	}

	auto prev=state.subFontType;
	state.subFontType=data&3;
}

void PhysicalMemory::WriteD406(uint8_t data)
{
	uint16_t data16=data;
	state.kanjiAddr&=0xFF;
	state.kanjiAddr|=(data16<<8);
}
void PhysicalMemory::WriteD407(uint8_t data)
{
	state.kanjiAddr&=0xFF00;
	state.kanjiAddr|=data;
}
uint8_t PhysicalMemory::NonDestructiveReadD406(void) const
{
	uint32_t ptr=state.kanjiAddr;
	return ROM_KANJI[ptr<<1];
}
uint8_t PhysicalMemory::NonDestructiveReadD407(void) const
{
	uint32_t ptr=state.kanjiAddr;
	return ROM_KANJI[(ptr<<1)+1];
}

void PhysicalMemory::Reset(void)
{
	Device::Reset();
	state.VRAMAccessMask=0;
	state.FE00ROMMode=true;
	state.shadowRAMEnabled=false;
	state.avBootROM=false;
	state.av40DicRAMEnabled=false;
	state.av40DicROMEnabled=false;
	state.av40DicROMBank=0;
	state.av40SubRAMBWriteProtect=false;
	state.av40SubRAMABank=0;
	state.av40SubRAMBBank=0;
	state.av40SubKanjiLevel=0;
	state.subROMSwitch=false;
	state.VRAMBank=0;

	auto fm77avPtr=(FM77AV *)vmPtr;
	if(MACHINETYPE_FM77AV<=fm77avPtr->state.machineType)
	{
		state.avBootROM=true;
	}

	for(unsigned int addr=MAINSYS_BOOT_ROM_BEGIN; addr<MAINSYS_BOOT_ROM_END; ++addr)
	{
		if(true==state.DOSMode)
		{
			state.data[addr]=ROM_BOOT_DOS[addr-MAINSYS_BOOT_ROM_BEGIN];
		}
		else
		{
			state.data[addr]=ROM_BOOT_BASIC[addr-MAINSYS_BOOT_ROM_BEGIN];
		}
	}
}

const uint8_t *PhysicalMemory::GetVRAMBank(int bank) const
{
	switch(bank)
	{
	case 0:
		return state.data+SUBSYS_VRAM_BEGIN;
	case 1:
		return state.extVRAM;
	case 2:
		return state.extVRAM+FM77AV_VRAM_BANK_SIZE;
	}
	std::cout << "Bank not supported." << std::endl;
	return nullptr;
}
uint8_t *PhysicalMemory::GetVRAMBank(int bank)
{
	switch(bank)
	{
	case 0:
		return state.data+SUBSYS_VRAM_BEGIN;
	case 1:
		return state.extVRAM;
	case 2:
		return state.extVRAM+FM77AV_VRAM_BANK_SIZE;
	}
	std::cout << "Bank not supported." << std::endl;
	return nullptr;
}
uint32_t PhysicalMemory::GetVRAMBankSize(int bank) const
{
	return FM77AV_VRAM_BANK_SIZE;
}
uint8_t *PhysicalMemory::GetCurrentVRAMBank(void)
{
	auto fm77avPtr=(const FM77AV *)vmPtr;
	if(0==state.VRAMBank)
	{
		return state.data+SUBSYS_VRAM_BEGIN;
	}
	return state.extVRAM+(state.VRAMBank-1)*FM77AV_VRAM_BANK_SIZE;
}

void PhysicalMemory::CLR(uint32_t addr)
{
	if(SubCPUAccess::SUBCPU_ADDR_BASE+FM77AVIO_SUBCPU_BUSY==addr)
	{
		// It creates 2 clocks of sub-CPU ready, which corresponds to 1000nanosec.
		// This 2 clocks is before CLR finishes.
		// This CLR function is called before fm77avTime is updated, therefore
		// FM77AV::RunOneInstruction is responsible for adding delta and clearing it.
		auto fm77avPtr=(FM77AV *)vmPtr;
		fm77avPtr->state.subCPUTemporaryReadyTimeDelta=1000;
	}
}
uint8_t PhysicalMemory::FetchByteConst(uint32_t addr) const
{
	auto fm77avPtr=(const FM77AV *)vmPtr;
	switch(memType[addr])
	{
	case MEMTYPE_RAM:
	case MEMTYPE_SUBSYS_SHARED_RAM:
		return state.data[addr];
	case MEMTYPE_NOT_EXIST:
		return 0xFF;
	case MEMTYPE_SUBSYS_VRAM:
		addr=fm77avPtr->crtc.TransformVRAMAddress(addr);
		// Question: Is the VRAM Access Mask valid in non-640x200-singlePage mode?
		{
			uint16_t addr16=addr;
			if(addr16<0x4000)
			{
				if(0!=(state.VRAMAccessMask&1))
				{
					return 0xFF;
				}
			}
			else if(addr16<0x8000)
			{
				if(0!=(state.VRAMAccessMask&2))
				{
					return 0xFF;
				}
			}
			else
			{
				if(0!=(state.VRAMAccessMask&4))
				{
					return 0xFF;
				}
			}
		}
		if(0==state.VRAMBank)
		{
			return state.data[addr];
		}
		addr&=0xFFFF;
		addr%=0xC000;
		return state.extVRAM[(state.VRAMBank-1)*0xC000+addr];

	case MEMTYPE_SUBSYS_RAM_C000:
		// Question is if RAM-A Bank is only enable in the all-RAM mode.  If so, this needs an additional check.
		if(0!=state.av40SubRAMABank)
		{
			return state.av40SubRAMA[0x1000*(state.av40SubRAMABank-1)+addr-SUBSYS_RAM_C000_BEGIN];
		}
		return state.data[addr];

	case MEMTYPE_SUBSYS_FONT_ROM:
		if(SUBMON_RAM==state.subMonType)
		{
			if(0!=state.av40SubRAMBBank)
			{
				return state.av40SubRAMB[0x800*(state.av40SubRAMBBank-1)+addr-SUBSYS_FONT_ROM_BEGIN];
			}
			return state.data[addr];
		}
		else if(SUBMON_C==state.subMonType)
		{
			return ROM_SUBSYS_C[addr-SUBSYS_FONT_ROM_BEGIN];
		}
		else // if(SUBMON_A==state. || SUBMON_B==state.)
		{
			return ROM_ASCII_FONT[state.subFontType*ASCII_FONT_ROM_SIZE+(addr-SUBSYS_FONT_ROM_BEGIN)];
		}
		break;
	case MEMTYPE_SUBSYS_MONITOR_ROM:
		if(SUBMON_C==state.subMonType)
		{
			return ROM_SUBSYS_C[addr-SUBSYS_FONT_ROM_BEGIN];
		}
		else if(SUBMON_A==state.subMonType)
		{
			return ROM_SUBSYS_A[addr-SUBSYS_MONITOR_ROM_BEGIN];
		}
		else if(SUBMON_B==state.subMonType)
		{
			return ROM_SUBSYS_B[addr-SUBSYS_MONITOR_ROM_BEGIN];
		}
		else if(SUBMON_RAM==state.subMonType)
		{
			return state.data[addr];
		}
		return ROM_SUBSYS_C[addr-SUBSYS_FONT_ROM_BEGIN];
	case MEMTYPE_MAINSYS_INITIATOR_ROM: // Can be TWR
		if(true==state.avBootROM)
		{
			return ROM_INITIATOR[addr&(INITIATOR_ROM_SIZE-1)];
		}
		return state.data[addr];
	case MEMTYPE_MAINSYS_FBASIC_ROM:    // Can be Shadow RAM
		if(true==state.shadowRAMEnabled)
		{
			return state.data[addr];
		}
		return ROM_FBASIC[addr&0x7FFF];
	case MEMTYPE_MAINSYS_SHARED_RAM:
		if(true==fm77avPtr->state.subSysHalt)
		{
			return state.data[SUBSYS_SHARED_RAM_BEGIN+(addr&0x7F)];
		}
		return 0xFF;
	case MEMTYPE_MAINSYS_BOOT_ROM:
		return state.data[addr];
	case MEMTYPE_MAIN_RESET_VECTOR:
		if(true!=state.FE00ROMMode)
		{
			return state.data[addr];
		}
		if(0==(addr&1))
		{
			if(true==state.avBootROM)
			{
				return 0x60; // 0x6000
			}
			else
			{
				return 0xFE; // 0xFE00
			}
		}
		return 0;


	case MEMTYPE_AV40_DICRAM:
		if(true==state.av40DicRAMEnabled)
		{
			return state.av40DicRAM[addr-AV40_DICRAM_BEGIN];
		}
		return state.data[addr];
	case MEMTYPE_AV40_DICROM:
		if(true==state.av40DicROMEnabled)
		{
			return ROM_DIC[(0x1000*state.av40DicROMBank)+addr-AV40_DICROM_BEGIN];
		}
		return state.data[addr];


	case MEMTYPE_SUBSYS_IO:
	case MEMTYPE_MAINSYS_IO:
		Abort("Not supposed to come here. (1)");
		return 0;
	}
	Abort("Not supposed to come here. (2)");
	return 0;
}

uint8_t PhysicalMemory::FetchByte(uint32_t addr)
{
	auto fm77avPtr=(FM77AV *)vmPtr;

	// Memo to self:  Never ever overwrite addr before FetchByteConst.
	//                I wasted 12 hours to debug this.
	//                If address-transformation is needed (I hope no more though)
	//                do it in a separate variable.

	if(true==var.memAttr[addr].brkOnRead)
	{
		auto data=FetchByteConst(addr);
		if(var.memAttr[addr].brkOnReadMinMax[0]<=data && data<=var.memAttr[addr].brkOnReadMinMax[1])
		{
			std::cout << "Memory Read " << cpputil::Uitox(addr) << " Value=" << cpputil::Ubtox(data) << std::endl;
			if(true!=var.memAttr[addr].justMonitorDontBreakOnRead)
			{
				fm77avPtr->mainCPU.debugger.stop=true;
			}
		}
	}

	switch(memType[addr])
	{
	case MEMTYPE_SUBSYS_VRAM:
		if(MACHINETYPE_FM77AV<=fm77avPtr->state.machineType)
		{
			fm77avPtr->crtc.VRAMDummyRead(fm77avPtr->crtc.TransformVRAMAddress(addr));
		}
		break;
	case MEMTYPE_SUBSYS_IO:
	case MEMTYPE_MAINSYS_IO:
		return fm77avPtr->IORead(addr&0xFFFF);
	}

	// Memo to self:  Never ever overwrite addr before FetchByteConst.
	//                I wasted 12 hours to debug this.
	//                If address-transformation is needed (I hope no more though)
	//                do it in a separate variable.

	return FetchByteConst(addr);
}
uint16_t PhysicalMemory::FetchWord(uint32_t addr0,uint32_t addr1)
{
	uint8_t hiByte = FetchByte(addr0);
	uint8_t loByte = FetchByte(addr1); 
	return mc6809util::FetchWord(hiByte, loByte);
}
void PhysicalMemory::StoreByte(uint32_t addr,uint8_t d)
{
	auto fm77avPtr=(FM77AV *)vmPtr;

	if(true==var.memAttr[addr].brkOnWrite)
	{
		if(var.memAttr[addr].brkOnWriteMinMax[0]<=d && d<=var.memAttr[addr].brkOnWriteMinMax[1])
		{
			std::cout << "Memory Write " << cpputil::Uitox(addr) << " Value=" << cpputil::Ubtox(d) << std::endl;
			if(true!=var.memAttr[addr].justMonitorDontBreakOnWrite)
			{
				fm77avPtr->mainCPU.debugger.stop=true;
			}
		}
	}

	switch(memType[addr])
	{
	case MEMTYPE_RAM:
	case MEMTYPE_SUBSYS_SHARED_RAM:
		state.data[addr]=d;
		return;
	case MEMTYPE_NOT_EXIST:
		return;
	case MEMTYPE_SUBSYS_VRAM:
		if(true==fm77avPtr->crtc.state.hardDraw.enabled)
		{
			// Hardware drawing is supposed to be dummy-READ a VRAM byte.
			// However, Pro Baseball Fan (Telenet) is dummy-writing to a VRAM byte to use hardware drawing.
			// So, apparently reading and writing both work.
			fm77avPtr->crtc.VRAMDummyRead(fm77avPtr->crtc.TransformVRAMAddress(addr));
			return;
		}

		addr=fm77avPtr->crtc.TransformVRAMAddress(addr);
		{
			uint16_t addr16=addr;
			if(addr16<0x4000)
			{
				if(0!=(state.VRAMAccessMask&1))
				{
					return;
				}
			}
			else if(addr16<0x8000)
			{
				if(0!=(state.VRAMAccessMask&2))
				{
					return;
				}
			}
			else
			{
				if(0!=(state.VRAMAccessMask&4))
				{
					return;
				}
			}
		}
		if(0==state.VRAMBank)
		{
			state.data[addr]=d;
		}
		else
		{
			addr&=0xFFFF;
			addr%=0xC000;
			state.extVRAM[(state.VRAMBank-1)*0xC000+addr]=d;
		}
		return;

	case MEMTYPE_SUBSYS_RAM_C000:
		// Question is if RAM-A Bank is only enable in the all-RAM mode.  If so, this needs an additional check.
		if(0!=state.av40SubRAMABank)
		{
			state.av40SubRAMA[0x1000*(state.av40SubRAMABank-1)+addr-SUBSYS_RAM_C000_BEGIN]=d;
			return;
		}
		state.data[addr]=d;
		return;

	case MEMTYPE_SUBSYS_IO:
	case MEMTYPE_MAINSYS_IO:
		fm77avPtr->IOWrite(addr,d);
		return;

	case MEMTYPE_SUBSYS_FONT_ROM:
		if(SUBMON_RAM==state.subMonType && true!=state.av40SubRAMBWriteProtect)
		{
			if(0!=state.av40SubRAMBBank)
			{
				state.av40SubRAMB[0x800*(state.av40SubRAMBBank-1)+addr-SUBSYS_FONT_ROM_BEGIN]=d;
			}
			state.data[addr]=d;
		}
		return;
	case MEMTYPE_SUBSYS_MONITOR_ROM:
		if(SUBMON_RAM==state.subMonType && true!=state.av40SubRAMBWriteProtect)
		{
			state.data[addr]=d;
		}
		return;
	case MEMTYPE_MAINSYS_INITIATOR_ROM: // Can be TWR
		if(true!=state.avBootROM)
		{
			state.data[addr]=d;
		}
		return;
	case MEMTYPE_MAINSYS_FBASIC_ROM:    // Can be Shadow RAM
		if(true==state.shadowRAMEnabled)
		{
			state.data[addr]=d;
		}
		return;
	case MEMTYPE_MAINSYS_SHARED_RAM:
		if(true==fm77avPtr->state.subSysHalt)
		{
			state.data[SUBSYS_SHARED_RAM_BEGIN+(addr&0x7F)]=d;
		}
		return;
	case MEMTYPE_MAINSYS_BOOT_ROM:
	case MEMTYPE_MAIN_RESET_VECTOR:
		if(true!=state.FE00ROMMode)
		{
			state.data[addr]=d;
		}
		return;
	case MEMTYPE_AV40_DICRAM:
		if(true!=state.av40DicRAMEnabled)
		{
			state.data[addr]=d;
		}
		else
		{
			state.av40DicRAM[addr-AV40_DICRAM_BEGIN]=d;
		}
		return;
	case MEMTYPE_AV40_DICROM:
		// F-BASIC V3.4 L20 writes to this area while Dic ROM is enabled.
		// There is a possibility that Dic ROM Enable flag only influence READ, and
		// always writable.
		// if(true!=state.av40DicROMEnabled)
		{
			state.data[addr]=d;
		}
		return;
	}
}
void PhysicalMemory::StoreWord(uint32_t addr0,uint32_t addr1,uint16_t d)
{
	StoreByte(addr0,(d>>8));
	StoreByte(addr1,(d&0xFF));
}
uint8_t PhysicalMemory::NonDestructiveFetchByte(uint32_t addr) const
{
	auto fm77avPtr=(FM77AV *)vmPtr;
	switch(memType[addr])
	{
	case MEMTYPE_SUBSYS_IO:
	case MEMTYPE_MAINSYS_IO:
		return fm77avPtr->NonDestructiveIORead(addr&0xFFFF);
	}
	return FetchByteConst(addr);
}
uint16_t PhysicalMemory::NonDestructiveFetchWord(uint32_t addr) const
{
	uint16_t hiByte=NonDestructiveFetchByte(addr);
	uint16_t loByte=NonDestructiveFetchByte(addr+1);
	return (hiByte<<8)|loByte;
}

std::vector <std::string> PhysicalMemory::GetStatusText(void) const
{
	std::vector <std::string> text;
	if(true==state.DOSMode)
	{
		text.push_back("DOSMode ");
	}
	else
	{
		text.push_back("BASICMode ");
	}

	text.back()+="FE00:";
	if(true==state.FE00ROMMode)
	{
		text.back()+="BootROM ";
	}
	else
	{
		text.back()+="RAM ";
	}

	text.back()+="8000:";
	if(true==state.shadowRAMEnabled)
	{
		text.back()+="UraRAM ";
	}
	else
	{
		text.back()+="ROM ";
	}

	text.back()+="6000:";
	if(true==state.avBootROM)
	{
		text.back()+="InitiatorROM ";
	}
	else
	{
		text.back()+="RAM ";
	}
	text.back()+="VRAM Access Mask:";
	text.back()+=cpputil::Ubtox(state.VRAMAccessMask);

	text.push_back("");
	text.back()+="SubSysROM:";
	switch(state.subMonType)
	{
	case SUBMON_A:
		text.back()+="TypeA ";
		break;
	case SUBMON_B:
		text.back()+="TypeB ";
		break;
	case SUBMON_C:
		text.back()+="TypeC ";
		break;
	case SUBMON_RAM:
		text.back()+="RAM ";
		break;
	}
	text.back()+="FontROM:";
	switch(state.subFontType)
	{
	case 0:
		text.back()+="Katakana ";
		break;
	case 1:
		text.back()+="Hiragana ";
		break;
	case 2:
		text.back()+="ROM1 ";
		break;
	case 3:
		text.back()+="ROM2 ";
		break;
	case 4:
		text.back()+="Corrupt ";
		break;
	}

	text.back()+=" RAMABank:";
	text.back()+=cpputil::Ubtox(state.av40SubRAMABank);
	text.back()+=" RAMBBank:";
	text.back()+=cpputil::Ubtox(state.av40SubRAMBBank);
	text.back()+=" RAMWriteProtect:";
	text.back()+=cpputil::Ubtox(state.av40SubRAMBWriteProtect);

	text.push_back("DicROM:");
	if(true==state.av40DicROMEnabled)
	{
		text.back()+="Enabled";
	}
	else
	{
		text.back()+="Disabled";
	}
	text.back()+="  DicRAM:";
	if(true==state.av40DicRAMEnabled)
	{
		text.back()+="Enabled";
	}
	else
	{
		text.back()+="Disabled";
	}


	return text;
}

/* virtual */ uint32_t PhysicalMemory::SerializeVersion(void) const
{
	return 0;
}
/* virtual */ void PhysicalMemory::SpecificSerialize(std::vector <unsigned char> &data,std::string stateFName) const
{
	PushBool(data,state.DOSMode);

	PushBool(data,state.FE00ROMMode);
	PushUint16(data,state.VRAMAccessMask);
	PushBool(data,state.shadowRAMEnabled);
	PushUint16(data,state.VRAMBank);
	PushBool(data,state.avBootROM);
	PushBool(data,state.av40DicRAMEnabled);
	PushBool(data,state.av40DicROMEnabled);
	PushUint32(data,state.av40DicROMBank);

	PushUcharArray(data,PHYSMEM_SIZE,state.data);
	PushUcharArray(data,EXTVRAM_SIZE,state.extVRAM);
	PushUcharArray(data,AV40_DICRAM_SIZE,state.av40DicRAM);
	PushUcharArray(data,AV40_SUBRAMA_SIZE,state.av40SubRAMA);
	PushUcharArray(data,AV40_SUBRAMB_SIZE,state.av40SubRAMB);

	PushBool(data,state.av40SubRAMBWriteProtect);
	PushUint32(data,state.av40SubRAMABank);
	PushUint32(data,state.av40SubRAMBBank);
	PushUint16(data,state.av40SubKanjiLevel);

	PushUint16(data,state.subMonType);
	PushUint16(data,state.subFontType);
	PushBool(data,state.subROMSwitch);

	PushUint32(data,state.kanjiAddr);
}
/* virtual */ bool PhysicalMemory::SpecificDeserialize(const unsigned char *&data,std::string stateFName,uint32_t version)
{
	state.DOSMode=ReadBool(data);

	state.FE00ROMMode=ReadBool(data);
	state.VRAMAccessMask=ReadUint16(data);
	state.shadowRAMEnabled=ReadBool(data);
	state.VRAMBank=ReadUint16(data);
	state.avBootROM=ReadBool(data);
	state.av40DicRAMEnabled=ReadBool(data);
	state.av40DicROMEnabled=ReadBool(data);
	state.av40DicROMBank=ReadUint32(data);

	ReadUcharArray(data,PHYSMEM_SIZE,state.data);
	ReadUcharArray(data,EXTVRAM_SIZE,state.extVRAM);
	ReadUcharArray(data,AV40_DICRAM_SIZE,state.av40DicRAM);
	ReadUcharArray(data,AV40_SUBRAMA_SIZE,state.av40SubRAMA);
	ReadUcharArray(data,AV40_SUBRAMB_SIZE,state.av40SubRAMB);

	state.av40SubRAMBWriteProtect=ReadBool(data);
	state.av40SubRAMABank=ReadUint32(data);
	state.av40SubRAMBBank=ReadUint32(data);
	state.av40SubKanjiLevel=ReadUint16(data);

	state.subMonType=ReadUint16(data);
	state.subFontType=ReadUint16(data);
	state.subROMSwitch=ReadBool(data);

	state.kanjiAddr=ReadUint32(data);

	return true;
}

////////////////////////////////////////////////////////////

MainCPUAccess::MainCPUAccess(class VMBase *vmPtr,PhysicalMemory *physMemPtr) : Device(vmPtr)
{
	this->physMemPtr=physMemPtr;
	Reset();
}

void MainCPUAccess::Reset(void)
{
	Device::Reset();
	state.exMMR=false;
	state.MMREnabled=false;
	state.MMRSEG=0;

	state.TWREnabled=false;
	state.TWRAddr=0;

	for(int i=0; i<8; ++i)
	{
		// Experimented on actual FM77AV
		// By default, MMR addresses are all zero.
		state.MMR[i][ 0]=0;
		state.MMR[i][ 1]=0;
		state.MMR[i][ 2]=0;
		state.MMR[i][ 3]=0;
		state.MMR[i][ 4]=0;
		state.MMR[i][ 5]=0;
		state.MMR[i][ 6]=0;
		state.MMR[i][ 7]=0;
		state.MMR[i][ 8]=0;
		state.MMR[i][ 9]=0;
		state.MMR[i][10]=0;
		state.MMR[i][11]=0;
		state.MMR[i][12]=0;
		state.MMR[i][13]=0;
		state.MMR[i][14]=0;
		state.MMR[i][15]=0;
	}
}

/* virtual */ void MainCPUAccess::IOWriteByte(unsigned int ioport,unsigned int data)
{
	switch(ioport)
	{
	case FM77AVIO_MMR_0://=                   0xFD80,
	case FM77AVIO_MMR_1://=                   0xFD81,
	case FM77AVIO_MMR_2://=                   0xFD82,
	case FM77AVIO_MMR_3://=                   0xFD83,
	case FM77AVIO_MMR_4://=                   0xFD84,
	case FM77AVIO_MMR_5://=                   0xFD85,
	case FM77AVIO_MMR_6://=                   0xFD86,
	case FM77AVIO_MMR_7://=                   0xFD87,
	case FM77AVIO_MMR_8://=                   0xFD88,
	case FM77AVIO_MMR_9://=                   0xFD89,
	case FM77AVIO_MMR_A://=                   0xFD8A,
	case FM77AVIO_MMR_B://=                   0xFD8B,
	case FM77AVIO_MMR_C://=                   0xFD8C,
	case FM77AVIO_MMR_D://=                   0xFD8D,
	case FM77AVIO_MMR_E://=                   0xFD8E,
	case FM77AVIO_MMR_F://=                   0xFD8F,
		// Moved to WriteFD8x
		break;
	case FM77AVIO_MMR_SEG://=                 0xFD90,
		// Oh!FM May 1989 issue pp.45 implies that there are 8 MMR segments in total.
		// However, if so F-BAISC 3.3 does not boot.  It messes up the MMR.
		if(true!=state.exMMR)
		{
			state.MMRSEG=data&MMR_SEGMENT_MASK;
		}
		else
		{
			state.MMRSEG=data&MMR_SEGMENT_MASK_EXMMR;
		}
		break;
	case FM77AVIO_WINDOW_OFFSET://=           0xFD92,
		// Experiment indicated TWR address 0 will map physical 0x07C00 to main CPU memory space 0x7C00.
		// TWR address 1 0x07D00, 0xFF 0x07C00.  It wraps around and comes back to 0x00000.
		state.TWRAddr=data;
		state.TWRAddr<<=8;
		state.TWRAddr+=0x7C00;
		state.TWRAddr&=0xFFFF;
		break;
	case FM77AVIO_MEMORY_MODE://=             0xFD93,
		state.MMREnabled=(0!=(data&0x80));
		state.TWREnabled=(0!=(data&0x40));
		// Boot ROM/RAM mode is controlled by physMem.
		// Bit 0 will be done in fm77avio.cpp
		break;
	case FM77AVIO_AV40_EXTMMR://=             0xFD94,
		// Moved to WriteFD94
		break;
	}
}

void MainCPUAccess::WriteFD8x(uint16_t ioAddr,uint8_t data)
{
	if(true!=state.exMMR)
	{
		data&=0x3F;
	}
	else
	{
		data&=0x7F;
	}
	// According to the FM77AC40EX/20EX Hardware Manual, unlike the other MMR registers, A19 and A18 are not effective on $FD8F.
	if(FM77AVIO_MMR_F==ioAddr)
	{
		data&=0x3F;
	}
	state.MMR[state.MMRSEG][ioAddr-FM77AVIO_MMR_0]=data;
	state.MMR[state.MMRSEG][ioAddr-FM77AVIO_MMR_0]<<=12;
}

void MainCPUAccess::WriteFD94(uint8_t data)
{
	state.exMMR=(0!=(data&0x80));
}
uint8_t MainCPUAccess::NonDestructiveIOReadByte(unsigned int ioport) const
{
	switch(ioport)
	{
	case FM77AVIO_MMR_0://=                   0xFD80,
	case FM77AVIO_MMR_1://=                   0xFD81,
	case FM77AVIO_MMR_2://=                   0xFD82,
	case FM77AVIO_MMR_3://=                   0xFD83,
	case FM77AVIO_MMR_4://=                   0xFD84,
	case FM77AVIO_MMR_5://=                   0xFD85,
	case FM77AVIO_MMR_6://=                   0xFD86,
	case FM77AVIO_MMR_7://=                   0xFD87,
	case FM77AVIO_MMR_8://=                   0xFD88,
	case FM77AVIO_MMR_9://=                   0xFD89,
	case FM77AVIO_MMR_A://=                   0xFD8A,
	case FM77AVIO_MMR_B://=                   0xFD8B,
	case FM77AVIO_MMR_C://=                   0xFD8C,
	case FM77AVIO_MMR_D://=                   0xFD8D,
	case FM77AVIO_MMR_E://=                   0xFD8E,
	case FM77AVIO_MMR_F://=                   0xFD8F,
		return (state.MMR[state.MMRSEG][ioport-FM77AVIO_MMR_0]>>12);
	case FM77AVIO_MMR_SEG://=                 0xFD90,
		break;  // Confirmed write-only.
	case FM77AVIO_MEMORY_MODE://=             0xFD93,
		{
			uint8_t data=0x3f;
			if(true==state.MMREnabled)
			{
				data|=0x80;
			}
			if(true==state.TWREnabled)
			{
				data|=0x40;
			}
			// Boot ROM/RAM mode is controlled by physMem.
			// Bit 0 will be done in fm77avio.cpp
			return data;
		}
		break;
	case FM77AVIO_AV40_EXTMMR://=             0xFD94,
		break;
	}
	return 0xFF;
}
/* virtual */ void MainCPUAccess::CLR(uint16_t addr)
{
	if(true==state.TWREnabled && (0x7C00==(addr&0xFC00)))
	{
		physMemPtr->CLR(TWRAddressTranslation(addr));
	}
	else if(true==state.MMREnabled)
	{
		physMemPtr->CLR(MMRAddressTranslation(addr));
	}
	else
	{
		physMemPtr->CLR(MAINCPU_ADDR_BASE+addr);
	}
}
/* virtual */ uint8_t MainCPUAccess::FetchByte(uint16_t addr)
{
	if(true==state.TWREnabled && (0x7C00==(addr&0xFC00)))
	{
		return physMemPtr->FetchByte(TWRAddressTranslation(addr));
	}
	else if(true==state.MMREnabled)
	{
		return physMemPtr->FetchByte(MMRAddressTranslation(addr));
	}
	else
	{
		return physMemPtr->FetchByte(MAINCPU_ADDR_BASE+addr);
	}
}
/* virtual */ uint16_t MainCPUAccess::FetchWord(uint16_t addr)
{
	if(true==state.TWREnabled) // There is a possibility like LDX $7BFF or LDX $7FFF.
	{
		// Making sure to the high byte is accessed first.
		uint16_t res;
		res  = FetchByte(addr  )<<8;
		res |= FetchByte(addr+1);
		return res;
	}
	else if(true==state.MMREnabled)
	{
		return physMemPtr->FetchWord(
			MMRAddressTranslation(addr), 
			MMRAddressTranslation(addr+1));
	}
	else
	{
		return physMemPtr->FetchWord(
			MAINCPU_ADDR_BASE+addr,
			MAINCPU_ADDR_BASE+addr+1);
	}
}
/* virtual */ void MainCPUAccess::StoreByte(uint16_t addr,uint8_t data)
{
	if(true==state.TWREnabled && (0x7C00==(addr&0xFC00)))
	{
		physMemPtr->StoreByte(TWRAddressTranslation(addr),data);
	}
	else if(true==state.MMREnabled)
	{
		physMemPtr->StoreByte(MMRAddressTranslation(addr),data);
	}
	else
	{
		physMemPtr->StoreByte(MAINCPU_ADDR_BASE+addr,data);
	}
}
/* virtual */ void MainCPUAccess::StoreWord(uint16_t addr,uint16_t data)
{
	if(true==state.TWREnabled) // There is a possibility like LDX $7BFF or LDX $7FFF.
	{
		StoreByte(addr  ,(data>>8));
		StoreByte(addr+1,data&0xFF);
	}
	else if(true==state.MMREnabled)
	{
		physMemPtr->StoreWord(
			MMRAddressTranslation(addr),
			MMRAddressTranslation(addr+1),
			data);
	}
	else
	{
		physMemPtr->StoreWord(
			MAINCPU_ADDR_BASE+addr,
			MAINCPU_ADDR_BASE+addr+1,
			data);
	}
}
/* virtual */ uint8_t MainCPUAccess::NonDestructiveFetchByte(uint16_t addr) const
{
	if(true==state.TWREnabled && (0x7C00==(addr&0xFC00)))
	{
		return physMemPtr->NonDestructiveFetchByte(TWRAddressTranslation(addr));
	}
	else if(true==state.MMREnabled)
	{
		return physMemPtr->NonDestructiveFetchByte(MMRAddressTranslation(addr));
	}
	else
	{
		return physMemPtr->NonDestructiveFetchByte(MAINCPU_ADDR_BASE+addr);
	}
}
/* virtual */ uint16_t MainCPUAccess::NonDestructiveFetchWord(uint16_t addr) const
{
	if(true==state.TWREnabled) // There is a possibility like LDX $7BFF or LDX $7FFF.
	{
		uint16_t res;
		res  = NonDestructiveFetchByte(addr)<<8;
		res |= NonDestructiveFetchByte(addr+1);
		return res;
	}
	else if(true==state.MMREnabled)
	{
		uint8_t hiByte = physMemPtr->NonDestructiveFetchByte(MMRAddressTranslation(addr));
		uint8_t loByte = physMemPtr->NonDestructiveFetchByte(MMRAddressTranslation(addr+1));
		return mc6809util::FetchWord(hiByte, loByte);
	}
	else
	{
		uint8_t hiByte = physMemPtr->NonDestructiveFetchByte(MAINCPU_ADDR_BASE+addr);
		uint8_t loByte = physMemPtr->NonDestructiveFetchByte(MAINCPU_ADDR_BASE+addr+1);
		return mc6809util::FetchWord(hiByte, loByte);
	}
}

/* virtual */ uint32_t MainCPUAccess::SerializeVersion(void) const
{
	return 0;
}
/* virtual */ void MainCPUAccess::SpecificSerialize(std::vector <unsigned char> &data,std::string stateFName) const
{
	PushBool(data,state.exMMR);
	PushBool(data,state.MMREnabled);
	PushBool(data,state.TWREnabled);
	PushUint16(data,state.MMRSEG);
	for(int i=0; i<NUM_MMR_SEGMENTS; ++i)
	{
		for(int j=0; j<16; ++j)
		{
			PushUint32(data,state.MMR[i][j]);
		}
	}
	PushUint32(data,state.TWRAddr);
}
/* virtual */ bool MainCPUAccess::SpecificDeserialize(const unsigned char *&data,std::string stateFName,uint32_t version)
{
	state.exMMR=ReadBool(data);
	state.MMREnabled=ReadBool(data);
	state.TWREnabled=ReadBool(data);
	state.MMRSEG=ReadUint16(data);
	for(int i=0; i<NUM_MMR_SEGMENTS; ++i)
	{
		for(int j=0; j<16; ++j)
		{
			state.MMR[i][j]=ReadUint32(data);
		}
	}
	state.TWRAddr=ReadUint32(data);
	return true;
}

////////////////////////////////////////////////////////////

SubCPUAccess::SubCPUAccess(class VMBase *vmPtr,PhysicalMemory *physMemPtr) : Device(vmPtr)
{
	this->physMemPtr=physMemPtr;
}
/* virtual */ void SubCPUAccess::CLR(uint16_t addr)
{
	return physMemPtr->CLR(SUBCPU_ADDR_BASE+addr);
}
/* virtual */ uint8_t SubCPUAccess::FetchByte(uint16_t addr)
{
	return physMemPtr->FetchByte(SUBCPU_ADDR_BASE+addr);
}
/* virtual */ uint16_t SubCPUAccess::FetchWord(uint16_t addr)
{
	return physMemPtr->FetchWord(
		SUBCPU_ADDR_BASE+addr,
		SUBCPU_ADDR_BASE+((addr+1)&0xFFFF));
}
/* virtual */ void SubCPUAccess::StoreByte(uint16_t addr,uint8_t data)
{
	physMemPtr->StoreByte(SUBCPU_ADDR_BASE+addr,data);
}
/* virtual */ void SubCPUAccess::StoreWord(uint16_t addr,uint16_t data)
{
	physMemPtr->StoreWord(
		SUBCPU_ADDR_BASE+addr,
		SUBCPU_ADDR_BASE+((addr+1)&0xFFFF),
		data);
}
/* virtual */ uint8_t SubCPUAccess::NonDestructiveFetchByte(uint16_t addr) const
{
	return physMemPtr->NonDestructiveFetchByte(SUBCPU_ADDR_BASE+addr);
}
/* virtual */ uint16_t SubCPUAccess::NonDestructiveFetchWord(uint16_t addr) const
{
	uint8_t hiByte = physMemPtr->NonDestructiveFetchByte(SUBCPU_ADDR_BASE+addr);
	uint8_t loByte = physMemPtr->NonDestructiveFetchByte(SUBCPU_ADDR_BASE+((addr+1)&0xFFFF));
	return mc6809util::FetchWord(hiByte, loByte);
}

std::vector <std::string> MainCPUAccess::GetStatusText(void) const
{
	std::vector <std::string> text;

	text.push_back("MMR:");
	if(true==state.MMREnabled)
	{
		text.back()+="Enabled ";
	}
	else
	{
		text.back()+="Disabled ";
	}

	text.back()+="MMRSEG:";
	text.back()+=cpputil::Ubtox(state.MMRSEG);
	text.back()+=" ";

	text.back()+="TWR:";
	if(true==state.TWREnabled)
	{
		text.back()+="Enabled ";
	}
	else
	{
		text.back()+="Disabled ";
	}
	text.back()+="TWR Addr:0";
	text.back()+=cpputil::Ustox(state.TWRAddr);

	text.push_back("Current MMR");
	text.push_back("");
	for(auto R : state.MMR[state.MMRSEG])
	{
		text.back()+=cpputil::Ubtox(R>>12);
		text.back()+=" ";
	}
	text.push_back("MMR Segments");
	for(int i=0; i<8; ++i)
	{
		text.push_back("");
		for(auto R : state.MMR[i])
		{
			text.back()+=cpputil::Ubtox(R>>12);
			text.back()+=" ";
		}
	}

	return text;
}

/* virtual */ uint32_t SubCPUAccess::SerializeVersion(void) const
{
	return 0;
}
/* virtual */ void SubCPUAccess::SpecificSerialize(std::vector <unsigned char> &data,std::string stateFName) const
{
}
/* virtual */ bool SubCPUAccess::SpecificDeserialize(const unsigned char *&data,std::string stateFName,uint32_t version)
{
	return true;
}
