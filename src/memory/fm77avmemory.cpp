#include <iostream>
#include <fstream>
#include <string>
#include "memory.h"
#include "mc6809util.h"
#include "fm77av.h"
#include "cpputil.h"
#include "fm77avmemory.h"

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
}

bool PhysicalMemory::LoadROMFiles(std::string ROMPath)
{
	struct ToLoad
	{
		bool mandatory;
		uint8_t *ptr;
		uint64_t length;
		std::string fName,fNameAlt;
	};

	std::vector <ToLoad> toLoad;
	FM77AV *fm77av=(FM77AV *)vmPtr;

	// FM-7
	//   BOOT_BAS.ROM
	//   BOOT_DOS.ROM
	//   FBASIC30.ROM
	//   SUBSYS_C.ROM or SUBSYS.ROM
	if(fm77av->state.machineType<MACHINETYPE_FM77AV)
	{
		std::vector <struct ToLoad> lst=
		{
			{true, ROM_BOOT_BASIC,BOOT_ROM_SIZE,                              "BOOT_BAS.ROM",""},
			{true, ROM_BOOT_DOS,  BOOT_ROM_SIZE,                              "BOOT_DOS.ROM",""},
			{true, ROM_FBASIC,    FBASIC_ROM_SIZE,                            "FBASIC30.ROM",""},
			{true, ROM_SUBSYS_C,  SUBSYS_MONITOR_ROM_SIZE+ASCII_FONT_ROM_SIZE,"SUBSYS_C.ROM","SUBSYS.ROM"},
			{false,ROM_KANJI,     KANJI_ROM_SIZE,                             "KANJI1.ROM","KANJI.ROM"},
		};
		std::swap(toLoad,lst);
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
	else if(MACHINETYPE_FM77AV==fm77av->state.machineType)
	{
		std::vector <struct ToLoad> lst=
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
		std::swap(toLoad,lst);
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
	else if(MACHINETYPE_FM77AV40==fm77av->state.machineType)
	{
		std::vector <struct ToLoad> lst=
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
		std::swap(toLoad,lst);
	}

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
				std::cout << "Which is mandatory for emulating " << fm77av->MachineTypeStr() << std::endl;
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

void PhysicalMemory::IOWriteByte(unsigned int ioport,unsigned int data)
{
	switch(ioport)
	{
	case FM77AVIO_INITIATOR_ROM://=           0xFD10,
		state.avBootROM=(0==(data&2));
		break;
	}
}

void PhysicalMemory::Reset(void)
{
	Device::Reset();
	state.VRAMAccessMask=0;
	state.FE00ROMMode=true;
	state.shadowRAMEnabled=false;
	state.avBootROM=false;

	auto fm77avPtr=(FM77AV *)vmPtr;
	if(MACHINETYPE_FM77AV<=fm77avPtr->state.machineType)
	{
		state.avBootROM=true;
	}
}

const uint8_t *PhysicalMemory::GetVRAMBank(int bank) const
{
	switch(bank)
	{
	case 0:
		return state.data+SUBSYS_VRAM_BEGIN;
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
	}
	std::cout << "Bank not supported." << std::endl;
	return nullptr;
}
uint32_t PhysicalMemory::GetVRAMBankSize(int bank) const
{
	return 0xC000;
}

uint8_t PhysicalMemory::FetchByteConst(uint32_t addr) const
{
	auto fm77avPtr=(FM77AV *)vmPtr;
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
		// if(different bank)
		// {
		//	return byte from the correct bank.
		// }
		return state.data[addr];

	case MEMTYPE_SUBSYS_FONT_ROM:
		// if(subsys monitor A or B) use appropriate font bank.
		return ROM_SUBSYS_C[addr-SUBSYS_FONT_ROM_BEGIN];
	case MEMTYPE_SUBSYS_MONITOR_ROM:
		// if(subsys monitor A or B) use appropriate font bank.
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
		if(true!=state.FE00ROMMode)
		{
			return state.data[addr];
		}
		if(true==state.DOSMode)
		{
			return ROM_BOOT_DOS[addr&(BOOT_ROM_SIZE-1)];
		}
		return ROM_BOOT_BASIC[addr&(BOOT_ROM_SIZE-1)];

	case MEMTYPE_SUBSYS_IO:
	case MEMTYPE_MAINSYS_IO:
		Abort("Not supposed to come here.");
		return 0;
	}
}

uint8_t PhysicalMemory::FetchByte(uint32_t addr)
{
	auto fm77avPtr=(FM77AV *)vmPtr;
	switch(memType[addr])
	{
	case MEMTYPE_SUBSYS_IO:
	case MEMTYPE_MAINSYS_IO:
		return fm77avPtr->IORead(addr&0xFFFF);
	}
	return FetchByteConst(addr);
}
uint16_t PhysicalMemory::FetchWord(uint32_t addr0,uint32_t addr1)
{
	return mc6809util::FetchWord(FetchByte(addr0),FetchByte(addr1));
}
void PhysicalMemory::StoreByte(uint32_t addr,uint8_t d)
{
	auto fm77avPtr=(FM77AV *)vmPtr;
	switch(memType[addr])
	{
	case MEMTYPE_RAM:
	case MEMTYPE_SUBSYS_SHARED_RAM:
		state.data[addr]=d;
		return;
	case MEMTYPE_NOT_EXIST:
		return;
	case MEMTYPE_SUBSYS_VRAM:
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
		// if(different bank)
		// {
		//	write to the correct bank.
		// }
		state.data[addr]=d;
		return;

	case MEMTYPE_SUBSYS_IO:
	case MEMTYPE_MAINSYS_IO:
		fm77avPtr->IOWrite(addr,d);
		return;

	case MEMTYPE_SUBSYS_FONT_ROM:
	case MEMTYPE_SUBSYS_MONITOR_ROM:
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
		if(true!=state.FE00ROMMode)
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

////////////////////////////////////////////////////////////

MainCPUAccess::MainCPUAccess(class VMBase *vmPtr,PhysicalMemory *physMemPtr) : Device(vmPtr)
{
	this->physMemPtr=physMemPtr;
	Reset();
}

void MainCPUAccess::Reset(void)
{
	Device::Reset();
	MMREnabled=false;
	MMRSEG=0;
	for(int i=0; i<8; ++i)
	{
		MMR[i][ 0]=MAINCPU_ADDR_BASE+0x0000;
		MMR[i][ 1]=MAINCPU_ADDR_BASE+0x1000;
		MMR[i][ 2]=MAINCPU_ADDR_BASE+0x2000;
		MMR[i][ 3]=MAINCPU_ADDR_BASE+0x3000;
		MMR[i][ 4]=MAINCPU_ADDR_BASE+0x4000;
		MMR[i][ 5]=MAINCPU_ADDR_BASE+0x5000;
		MMR[i][ 6]=MAINCPU_ADDR_BASE+0x6000;
		MMR[i][ 7]=MAINCPU_ADDR_BASE+0x7000;
		MMR[i][ 8]=MAINCPU_ADDR_BASE+0x8000;
		MMR[i][ 9]=MAINCPU_ADDR_BASE+0x9000;
		MMR[i][10]=MAINCPU_ADDR_BASE+0xA000;
		MMR[i][11]=MAINCPU_ADDR_BASE+0xB000;
		MMR[i][12]=MAINCPU_ADDR_BASE+0xC000;
		MMR[i][13]=MAINCPU_ADDR_BASE+0xD000;
		MMR[i][14]=MAINCPU_ADDR_BASE+0xE000;
		MMR[i][15]=MAINCPU_ADDR_BASE+0xF000;
	}
}

/* virtual */ uint8_t MainCPUAccess::FetchByte(uint16_t addr)
{
	if(true==MMREnabled)
	{
		return physMemPtr->FetchByte(CPUAddrToPhysicalAddr(addr));
	}
	else
	{
		return physMemPtr->FetchByte(MAINCPU_ADDR_BASE+addr);
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
		MMR[MMRSEG][ioport-FM77AVIO_MMR_0]=(data<<12);
		break;
	case FM77AVIO_MMR_SEG://=                 0xFD90,
		MMRSEG=data&7;
		break;
	case FM77AVIO_MEMORY_MODE://=             0xFD93,
		MMREnabled=(0!=(data&0x80));
		// TWREnabled=(0!=(data&0x40));
		break;
	case FM77AVIO_AV40_EXTMMR://=             0xFD94,
		break;
	}
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
		return (MMR[MMRSEG][ioport-FM77AVIO_MMR_0]>>12);
	case FM77AVIO_MMR_SEG://=                 0xFD90,
		return MMRSEG|0xF8;
		break;
	case FM77AVIO_MEMORY_MODE://=             0xFD93,
		{
			uint8_t data=0x3f;
			if(true==MMREnabled)
			{
				data|=0x80;
			}
			// if(true==TWREnabled)
			//{
			//	data|=0x40;
			//}
		}
		break;
	case FM77AVIO_AV40_EXTMMR://=             0xFD94,
		break;
	}
	return 0xFF;
}
/* virtual */ uint16_t MainCPUAccess::FetchWord(uint16_t addr)
{
	if(true==MMREnabled)
	{
		return physMemPtr->FetchWord(
			CPUAddrToPhysicalAddr(addr),
			CPUAddrToPhysicalAddr(addr+1));
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
	if(true==MMREnabled)
	{
		physMemPtr->StoreByte(CPUAddrToPhysicalAddr(addr),data);
	}
	else
	{
		physMemPtr->StoreByte(MAINCPU_ADDR_BASE+addr,data);
	}
}
/* virtual */ void MainCPUAccess::StoreWord(uint16_t addr,uint16_t data)
{
	if(true==MMREnabled)
	{
		physMemPtr->StoreWord(
			CPUAddrToPhysicalAddr(addr),
			CPUAddrToPhysicalAddr(addr+1),
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
	if(true==MMREnabled)
	{
		return physMemPtr->NonDestructiveFetchByte(CPUAddrToPhysicalAddr(addr));
	}
	else
	{
		return physMemPtr->NonDestructiveFetchByte(MAINCPU_ADDR_BASE+addr);
	}
}
/* virtual */ uint16_t MainCPUAccess::NonDestructiveFetchWord(uint16_t addr) const
{
	if(true==MMREnabled)
	{
		return mc6809util::FetchWord(
			physMemPtr->NonDestructiveFetchByte(CPUAddrToPhysicalAddr(addr)),
			physMemPtr->NonDestructiveFetchByte(CPUAddrToPhysicalAddr(addr+1)));
	}
	else
	{
		return mc6809util::FetchWord(
			physMemPtr->NonDestructiveFetchByte(MAINCPU_ADDR_BASE+addr),
			physMemPtr->NonDestructiveFetchByte(MAINCPU_ADDR_BASE+addr+1));
	}
}

////////////////////////////////////////////////////////////

SubCPUAccess::SubCPUAccess(class VMBase *vmPtr,PhysicalMemory *physMemPtr) : Device(vmPtr)
{
	this->physMemPtr=physMemPtr;
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
	return mc6809util::FetchWord(
		physMemPtr->NonDestructiveFetchByte(SUBCPU_ADDR_BASE+addr),
		physMemPtr->NonDestructiveFetchByte(SUBCPU_ADDR_BASE+((addr+1)&0xFFFF)));
}
