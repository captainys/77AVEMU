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
		MEMTYPE_MAIN_RESET_VECTOR,
	};
	enum
	{
		SUBMON_A,
		SUBMON_B,
		SUBMON_C,
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
		bool DOSMode=false;
		bool FE00ROMMode=true;
		uint8_t VRAMAccessMask=0;
		bool shadowRAMEnabled=false;  // true->RAM mode,  false->F-BASIC ROM mode.
		bool avBootROM=false;
		uint8_t data[PHYSMEM_SIZE];
		uint8_t extVRAM[EXTVRAM_SIZE];

		uint8_t subMonType=SUBMON_C;
		uint8_t subFontType=0;
		bool subROMSwitch=false;

		unsigned int kanjiAddr=0;
	};
	State state;

	virtual const char *DeviceName(void) const{return "PHYSMEM";}
	PhysicalMemory(VMBase *vmBase);

	void Reset(void);

	bool LoadROMFiles(std::string ROMPath);

	void IOWriteByte(unsigned int ioport,unsigned int data);

	void WriteFD20(uint8_t data);
	void WriteFD21(uint8_t data);
	uint8_t ReadFD22(void) const;
	uint8_t ReadFD23(void) const;

	void WriteFD13(uint8_t data);
	void WriteD430(uint8_t data);

	const uint8_t *GetVRAMBank(int bank) const;
	uint8_t *GetVRAMBank(int bank);
	uint32_t GetVRAMBankSize(int bank) const;
	uint8_t *GetCurrentVRAMBank(void);

	uint8_t FetchByteConst(uint32_t addr) const;

	uint8_t FetchByte(uint32_t addr);
	uint16_t FetchWord(uint32_t addr0,uint32_t addr1);
	void StoreByte(uint32_t addr,uint8_t data);
	void StoreWord(uint32_t addr0,uint32_t addr1,uint16_t data);
	uint8_t NonDestructiveFetchByte(uint32_t addr) const;
	uint16_t NonDestructiveFetchWord(uint32_t addr) const;

	std::vector <std::string> GetStatusText(void) const;
};

class MainCPUAccess : public MemoryAccess,public Device
{
public:
	enum
	{
		MAINCPU_ADDR_BASE=0x30000,
		NUM_MMR_SEGMENTS=8,
		MMR_SEGMENT_MASK=3,
		MMR_SEGMENT_MASK_EXMMR=7,
	};

	PhysicalMemory *physMemPtr;

	bool exMMR=false; // Expanded MMR for AV40 and later.
	bool MMREnabled=false;
	bool TWREnabled=false;
	uint8_t MMRSEG=0;
	uint32_t MMR[NUM_MMR_SEGMENTS][16];
	uint32_t TWRAddr=0;

	virtual const char *DeviceName(void) const{return "MAINMEMACCESS";}

	inline uint32_t MMRAddressTranslation(uint16_t cpuAddr) const
	{
		if(cpuAddr<0xFC00) // FM77AV40 Hardware Manual pp.146
		{
			return MMR[MMRSEG][cpuAddr>>12]+(cpuAddr&0xFFF);
		}
		else
		{
			return MAINCPU_ADDR_BASE+cpuAddr;
		}
	}
	inline uint32_t TWRAddressTranslation(uint16_t cpuAddr) const
	{
		return (TWRAddr+(cpuAddr&0x3FF))&0xFFFF;
	}

	void Reset(void);
	virtual void IOWriteByte(unsigned int ioport,unsigned int data);
	uint8_t NonDestructiveIOReadByte(unsigned int ioport) const;

	MainCPUAccess(class VMBase *vmPtr,PhysicalMemory *physMemPtr);
	virtual uint8_t FetchByte(uint16_t addr);
	virtual uint16_t FetchWord(uint16_t addr);
	virtual void StoreByte(uint16_t addr,uint8_t data);
	virtual void StoreWord(uint16_t addr,uint16_t data);
	virtual uint8_t NonDestructiveFetchByte(uint16_t addr) const;
	virtual uint16_t NonDestructiveFetchWord(uint16_t addr) const;

	std::vector <std::string> GetStatusText(void) const;
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
