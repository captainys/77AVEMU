#ifndef FM77AVMEMORY_IS_INCLUDED
#define FM77AVMEMORY_IS_INCLUDED
/* { */

#include <stdint.h>
#include "memory.h"

class PhysicalMemory : public Device
{
public:
	// IO *io;
	enum
	{
		PHYSMEM_SIZE=256*1024,
		BOOT_ROM_SIZE=512,         // 0xFE00 to 0xFFFF
		FBASIC_ROM_SIZE=0x7C00,    // 0x8000 to 0xFBFF
		INITIATOR_ROM_SIZE=0x2000, // 0x6000 to 0x7FFF
		KANJI_ROM_SIZE=0x20000,
		DIC_ROM_SIZE=0x40000,

		ASCII_FONT_ROM_SIZE=0x800, // 0xD800 to 0xDFFF
		SUBSYS_MONITOR_ROM_SIZE=0x2000, // 0xE000 to 0xFFFF

		EXTVRAM_SIZE=96*1024,
	};

	enum
	{
		RAM0_BEGIN=0x00000,
		RAM0_END=0x10000,

		SUBSYS_BEGIN=0x10000,
			SUBSYS_VRAM_BEGIN=          0x10000,
			SUBSYS_VRAM_END=            0x1C000,

			SUBSYS_SHARED_RAM_BEGIN=    0x1D380,
			SUBSYS_SHARED_RAM_END=      0x1D400,

			SUBSYS_IO_BEGIN=            0x1D400,
			SUBSYS_IO_END=              0x1D500,

			SUBSYS_FONT_ROM_BEGIN=      0x1D800,
			SUBSYS_FONT_ROM_END=        0x1E000,

			SUBSYS_MONITOR_ROM_BEGIN=   0x1E000,
			SUBSYS_MONITOR_ROM_END=     0x20000,

			SUBSYS_RESET_VECTOR=        0x1FFFE,
		SUBSYS_END=0x20000,

		RAM1_BEGIN=0x20000,
		RAM1_END=0x30000,

		MAINSYS_BEGIN=0x30000,
			MAINSYS_INITIATOR_ROM_BEGIN=0x36000,
			MAINSYS_INITIATOR_ROM_END=  0x38000,

			MAINSYS_TWR_BEGIN=          0x37C00,
			MAINSYS_TWR_END=            0x38000,

			MAINSYS_FBASIC_ROM_BEGIN=   0x38000,
			MAINSYS_FBASIC_ROM_END=     0x3FC00,

			MAINSYS_SHARED_RAM_BEGIN=   0x3FC80,
			MAINSYS_SHARED_RAM_END=     0x3FD00,

			MAINSYS_IO_BEGIN=           0x3FD00,
			MAINSYS_IO_END=             0x3FE00,

			MAINSYS_BOOT_ROM_BEGIN=     0x3FE00,
			MAINSYS_BOOT_ROM_END=       0x3FFE0,

			MAINSYS_INT_VECTOR=         0x3FFE0,
			MAINSYS_RESET_VECTOR=       0x3FFFE,
		MAINSYS_END=0x40000,
	};

	enum
	{
		MEMTYPE_RAM,
		MEMTYPE_NOT_EXIST,
		MEMTYPE_SUBSYS_VRAM,
		MEMTYPE_SUBSYS_SHARED_RAM,
		MEMTYPE_SUBSYS_IO,
		MEMTYPE_SUBSYS_FONT_ROM,
		MEMTYPE_SUBSYS_MONITOR_ROM,
		MEMTYPE_MAINSYS_INITIATOR_ROM, // Can be TWR
		MEMTYPE_MAINSYS_FBASIC_ROM,    // Can be Shadow RAM
		MEMTYPE_MAINSYS_SHARED_RAM,
		MEMTYPE_MAINSYS_IO,
		MEMTYPE_MAINSYS_BOOT_ROM,
	};

	uint8_t ROM_BOOT_DOS[BOOT_ROM_SIZE];
	uint8_t ROM_BOOT_BASIC[BOOT_ROM_SIZE];
	uint8_t ROM_FBASIC[FBASIC_ROM_SIZE];
	uint8_t ROM_INITIATOR[INITIATOR_ROM_SIZE];

	uint8_t ROM_KANJI[KANJI_ROM_SIZE];
	uint8_t ROM_KANJI2[KANJI_ROM_SIZE];
	uint8_t ROM_DIC[DIC_ROM_SIZE];

	uint8_t ROM_ASCII_FONT[4*ASCII_FONT_ROM_SIZE];
	uint8_t ROM_SUBSYS_A[SUBSYS_MONITOR_ROM_SIZE];
	uint8_t ROM_SUBSYS_B[SUBSYS_MONITOR_ROM_SIZE];
	uint8_t ROM_SUBSYS_C[SUBSYS_MONITOR_ROM_SIZE+ASCII_FONT_ROM_SIZE];

	uint8_t memType[PHYSMEM_SIZE];

	class State
	{
	public:
		bool shadowRAMEnabled=false;  // true->RAM mode,  false->F-BASIC ROM mode.
		uint8_t data[PHYSMEM_SIZE];
		uint8_t extVRAM[EXTVRAM_SIZE];
	};
	State state;

	virtual const char *DeviceName(void) const{return "PHYSMEM";}
	PhysicalMemory(VMBase *vmBase);

	bool LoadROMFiles(std::string ROMPath);

	const uint8_t *GetVRAMBank(int bank) const;
	uint8_t *GetVRAMBank(int bank);
	uint32_t GetVRAMBankSize(int bank) const;

	uint8_t FetchByteConst(uint32_t addr) const;

	uint8_t FetchByte(uint32_t addr);
	uint16_t FetchWord(uint32_t addr0,uint32_t addr1);
	void StoreByte(uint32_t addr,uint8_t data);
	void StoreWord(uint32_t addr0,uint32_t addr1,uint16_t data);
	uint8_t NonDestructiveFetchByte(uint32_t addr) const;
	uint16_t NonDestructiveFetchWord(uint32_t addr) const;
};

class MainCPUAccess : public MemoryAccess,public Device
{
public:
	enum
	{
		MAINCPU_ADDR_BASE=0x30000,
	};

	PhysicalMemory *physMemPtr;

	bool MMREnabled=false;
	uint32_t MMR[16]={
		MAINCPU_ADDR_BASE+0x0000,
		MAINCPU_ADDR_BASE+0x1000,
		MAINCPU_ADDR_BASE+0x2000,
		MAINCPU_ADDR_BASE+0x3000,
		MAINCPU_ADDR_BASE+0x4000,
		MAINCPU_ADDR_BASE+0x5000,
		MAINCPU_ADDR_BASE+0x6000,
		MAINCPU_ADDR_BASE+0x7000,
		MAINCPU_ADDR_BASE+0x8000,
		MAINCPU_ADDR_BASE+0x9000,
		MAINCPU_ADDR_BASE+0xA000,
		MAINCPU_ADDR_BASE+0xB000,
		MAINCPU_ADDR_BASE+0xC000,
		MAINCPU_ADDR_BASE+0xD000,
		MAINCPU_ADDR_BASE+0xE000,
		MAINCPU_ADDR_BASE+0xF000,
	};

	virtual const char *DeviceName(void) const{return "MAINMEMACCESS";}

	inline uint32_t CPUAddrToPhysicalAddr(uint16_t cpuAddr) const
	{
		return MMR[cpuAddr>>12]+(cpuAddr&0xFFF);
	}

	MainCPUAccess(class VMBase *vmPtr,PhysicalMemory *physMemPtr);
	virtual uint8_t FetchByte(uint16_t addr);
	virtual uint16_t FetchWord(uint16_t addr);
	virtual void StoreByte(uint16_t addr,uint8_t data);
	virtual void StoreWord(uint16_t addr,uint16_t data);
	virtual uint8_t NonDestructiveFetchByte(uint16_t addr) const;
	virtual uint16_t NonDestructiveFetchWord(uint16_t addr) const;
};

class SubCPUAccess : public MemoryAccess,public Device
{
public:
	enum
	{
		SUBCPU_ADDR_BASE=0x10000,
	};

	virtual const char *DeviceName(void) const{return "SUBMEMACCESS";}

	PhysicalMemory *physMemPtr;

	SubCPUAccess(class VMBase *vmPtr,PhysicalMemory *physMemPtr);
	virtual uint8_t FetchByte(uint16_t addr);
	virtual uint16_t FetchWord(uint16_t addr);
	virtual void StoreByte(uint16_t addr,uint8_t data);
	virtual void StoreWord(uint16_t addr,uint16_t data);
	virtual uint8_t NonDestructiveFetchByte(uint16_t addr) const;
	virtual uint16_t NonDestructiveFetchWord(uint16_t addr) const;
};

/* } */
#endif
