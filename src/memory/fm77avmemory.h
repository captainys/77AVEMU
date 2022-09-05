#ifndef FM77AVMEMORY_IS_INCLUDED
#define FM77AVMEMORY_IS_INCLUDED
/* { */

#include <stdint.h>
#include "memory.h"

class HasROMImages
{
public:
	enum
	{
		PHYSMEM_SIZE=512*1024,     // Max RAM for FM77AV40 MMR
		PHYSMEM_ADDR_SIZE=1024*1024, // Turned out FM77AV40 can address up to 1MB address.  But, I go with 512KB max as Fujitsu said in spec.
		                           // To prevent access beyond PHYSMEM_SIZE, memType[PHYSMEM_SIZE] and later are set to MEMTYPE_NOT_EXIST.
		BOOT_ROM_SIZE=512,         // 0xFE00 to 0xFFFF
		FBASIC_ROM_SIZE=0x7C00,    // 0x8000 to 0xFBFF
		INITIATOR_ROM_SIZE=0x2000, // 0x6000 to 0x7FFF
		KANJI_ROM_SIZE=0x20000,
		DIC_ROM_SIZE=0x40000,

		ASCII_FONT_ROM_SIZE=0x800, // 0xD800 to 0xDFFF
		SUBSYS_MONITOR_ROM_SIZE=0x2000, // 0xE000 to 0xFFFF

		EXTVRAM_SIZE=96*1024,

		AV40_DICRAM_SIZE=0x2000,
		AV40_SUBRAMA_SIZE=0x3000, // 4KB times 4 banks. 1 bank covered by default RAM.
		AV40_SUBRAMB_SIZE=0x800*7, // 2KB times 8 banks.  1 bank covered by default RAM.
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

	struct ROMToLoad
	{
		bool mandatory;
		uint8_t *ptr;
		uint64_t length;
		std::string fName,fNameAlt;
	};
	std::vector <ROMToLoad> GetROMToLoad(unsigned int machineType);
	bool LoadROMFiles(std::string ROMPath,unsigned int machineType);
};

class PhysicalMemory : public Device, public HasROMImages
{
public:
	// IO *io;
	enum
	{
		RAM0_BEGIN=0x00000,
		RAM0_END=0x10000,

		SUBSYS_BEGIN=0x10000,
			SUBSYS_VRAM_BEGIN=          0x10000,
			SUBSYS_VRAM_END=            0x1C000,

			SUBSYS_RAM_C000_BEGIN=      0x1C000,
			SUBSYS_RAM_C000_END=        0x1D000,

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

		AV40_DICRAM_BEGIN=0x28000,
		AV40_DICRAM_END=0x2A000,

		AV40_DICROM_BEGIN=0x2E000,
		AV40_DICROM_END=0x2F000,

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
		MEMTYPE_SUBSYS_RAM_C000,
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
		MEMTYPE_AV40_DICRAM,
		MEMTYPE_AV40_DICROM,
	};
	enum
	{
		SUBMON_A,
		SUBMON_B,
		SUBMON_C,
		SUBMON_RAM,
	};

	uint8_t memType[PHYSMEM_ADDR_SIZE];

	class State
	{
	public:
		bool DOSMode=false;

		// Confirmed on actual FM77AV
		// This flag ($FD93 bit0) does not work like Shadow RAM and F-BASIC ROM, which
		// if I disable Shadow RAM, F-BASIC ROM will re-appear.
		// Once I set to RAM mode, change bytes in FE00 to FFE0, change back to ROM mode,
		// the changed bytes stays.  Original DOS or BASIC BOOT ROM won't re-appear.
		bool FE00ROMMode=true;
		uint8_t VRAMAccessMask=0;
		bool shadowRAMEnabled=false;  // true->RAM mode,  false->F-BASIC ROM mode.
		uint8_t VRAMBank=0;
		bool avBootROM=false;
		bool av40DicRAMEnabled=false;
		bool av40DicROMEnabled=false;
		uint32_t av40DicROMBank=0;
		uint8_t data[PHYSMEM_SIZE];
		uint8_t extVRAM[EXTVRAM_SIZE];
		uint8_t av40DicRAM[AV40_DICRAM_SIZE];
		uint8_t av40SubRAMA[AV40_SUBRAMA_SIZE];
		uint8_t av40SubRAMB[AV40_SUBRAMB_SIZE];

		// bool av40SubRAMMode=false;  Rather use subMonType==SUBMON_RAM
		bool av40SubRAMBWriteProtect=false;
		uint32_t av40SubRAMABank=0;
		uint32_t av40SubRAMBBank=0;
		uint8_t av40SubKanjiLevel=0;

		uint8_t subMonType=SUBMON_C;
		uint8_t subFontType=0;
		bool subROMSwitch=false;

		unsigned int kanjiAddr=0;
	};
	State state;


	class MemoryAttribute
	{
	public:
		bool brkOnWrite=false;
		bool justMonitorDontBreakOnWrite=false;
		uint8_t brkOnWriteMinMax[2]={0,255};
		bool brkOnRead=false;
		bool justMonitorDontBreakOnRead=false;
		uint8_t brkOnReadMinMax[2]={0,255};
	};
	class Variable
	{
	public:
		MemoryAttribute memAttr[PHYSMEM_SIZE];
	};
	Variable var;


	virtual const char *DeviceName(void) const{return "PHYSMEM";}
	PhysicalMemory(VMBase *vmBase);

	bool LoadROMFiles(std::string ROMPath);

	void Reset(void);

	void WriteFD20(uint8_t data);
	void WriteFD21(uint8_t data);
	uint8_t ReadFD22(void) const;
	uint8_t ReadFD23(void) const;

	void WriteFD04(uint8_t data);
	void WriteFD10(uint8_t data);
	void WriteFD13(uint8_t data);
	void WriteFD2E(uint8_t data);
	void WriteD42E(uint8_t data);
	void WriteD42F(uint8_t data);
	void WriteD430(uint8_t data);

	void WriteD406(uint8_t data);
	void WriteD407(uint8_t data);
	uint8_t NonDestructiveReadD406(void) const;
	uint8_t NonDestructiveReadD407(void) const;

	const uint8_t *GetVRAMBank(int bank) const;
	uint8_t *GetVRAMBank(int bank);
	uint32_t GetVRAMBankSize(int bank) const;
	uint8_t *GetCurrentVRAMBank(void);

	/*! See comment in memory.h
	*/
	void CLR(uint32_t addr);

	uint8_t FetchByteConst(uint32_t addr) const;

	uint8_t FetchByte(uint32_t addr);
	uint16_t FetchWord(uint32_t addr0,uint32_t addr1);
	void StoreByte(uint32_t addr,uint8_t data);
	void StoreWord(uint32_t addr0,uint32_t addr1,uint16_t data);
	uint8_t NonDestructiveFetchByte(uint32_t addr) const;
	uint16_t NonDestructiveFetchWord(uint32_t addr) const;

	std::vector <std::string> GetStatusText(void) const;

	/* virtual */ uint32_t SerializeVersion(void) const;
	/* virtual */ void SpecificSerialize(std::vector <unsigned char> &data,std::string stateFName) const;
	/* virtual */ bool SpecificDeserialize(const unsigned char *&data,std::string stateFName,uint32_t version);
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

	class State
	{
	public:
		bool exMMR=false; // Expanded MMR for AV40 and later.
		bool MMREnabled=false;
		bool TWREnabled=false;
		uint8_t MMRSEG=0;
		uint32_t MMR[NUM_MMR_SEGMENTS][16];
		uint32_t TWRAddr=0;
	};
	State state;

	virtual const char *DeviceName(void) const{return "MAINMEMACCESS";}

	inline uint32_t MMRAddressTranslation(uint16_t cpuAddr) const
	{
		if(cpuAddr<0xFC00) // FM77AV40 Hardware Manual pp.146
		{
			return state.MMR[state.MMRSEG][cpuAddr>>12]+(cpuAddr&0xFFF);
		}
		else
		{
			return MAINCPU_ADDR_BASE+cpuAddr;
		}
	}
	inline uint32_t TWRAddressTranslation(uint16_t cpuAddr) const
	{
		return (state.TWRAddr+(cpuAddr&0x3FF))&0xFFFF;
	}

	void Reset(void);
	virtual void IOWriteByte(unsigned int ioport,unsigned int data);
	uint8_t NonDestructiveIOReadByte(unsigned int ioport) const;

	MainCPUAccess(class VMBase *vmPtr,PhysicalMemory *physMemPtr);
	virtual void CLR(uint16_t addr);
	virtual uint8_t FetchByte(uint16_t addr);
	virtual uint16_t FetchWord(uint16_t addr);
	virtual void StoreByte(uint16_t addr,uint8_t data);
	virtual void StoreWord(uint16_t addr,uint16_t data);
	virtual uint8_t NonDestructiveFetchByte(uint16_t addr) const;
	virtual uint16_t NonDestructiveFetchWord(uint16_t addr) const;

	void WriteFD8x(uint16_t ioAddr,uint8_t data);
	void WriteFD94(uint8_t data);

	std::vector <std::string> GetStatusText(void) const;

	/* virtual */ uint32_t SerializeVersion(void) const;
	/* virtual */ void SpecificSerialize(std::vector <unsigned char> &data,std::string stateFName) const;
	/* virtual */ bool SpecificDeserialize(const unsigned char *&data,std::string stateFName,uint32_t version);
};

class SubCPUAccess : public MemoryAccess,public Device
{
public:
	enum
	{
		SUBCPU_ADDR_BASE=0x10000,
	};

	virtual const char *DeviceName(void) const{return "SUBMEMACCESS";}

	class State
	{
	};

	PhysicalMemory *physMemPtr;

	SubCPUAccess(class VMBase *vmPtr,PhysicalMemory *physMemPtr);
	virtual void CLR(uint16_t addr);
	virtual uint8_t FetchByte(uint16_t addr);
	virtual uint16_t FetchWord(uint16_t addr);
	virtual void StoreByte(uint16_t addr,uint8_t data);
	virtual void StoreWord(uint16_t addr,uint16_t data);
	virtual uint8_t NonDestructiveFetchByte(uint16_t addr) const;
	virtual uint16_t NonDestructiveFetchWord(uint16_t addr) const;

	/* virtual */ uint32_t SerializeVersion(void) const;
	/* virtual */ void SpecificSerialize(std::vector <unsigned char> &data,std::string stateFName) const;
	/* virtual */ bool SpecificDeserialize(const unsigned char *&data,std::string stateFName,uint32_t version);
};

/* } */
#endif
