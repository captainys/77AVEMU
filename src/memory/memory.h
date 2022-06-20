#ifndef MEMORY_IS_INCLUDED
#define MEMORY_IS_INCLUDED
/* { */

#include <stdint.h>
#include "device.h"
#include "fm77av.h"



class PhysicalMemory
{
public:
	// IO *io;
	enum
	{
		PHYSMEM_SIZE=256*1024,
	};
	uint8_t data[PHYSMEM_SIZE];
	PhysicalMemory();
	uint8_t FetchByte(uint16_t addr);
	uint16_t FetchWord(uint16_t addr0,uint16_t addr1);
	void StoreByte(uint16_t addr,uint8_t data);
	void StoreWord(uint16_t addr0,uint16_t addr1,uint16_t data);
};

class MemoryAccess
{
public:
	virtual uint8_t FetchByte(uint16_t addr)=0;
	virtual uint16_t FetchWord(uint16_t addr)=0;
	virtual void StoreByte(uint16_t addr,uint8_t data)=0;
	virtual void StoreWord(uint16_t addr,uint16_t data)=0;
};

class MainCPUAccess : public MemoryAccess
{
public:
	enum
	{
		MAINCPU_ADDR_BASE=0x30000,
	};
};

class MainCPUAccessNoMMR : public MainCPUAccess,public Device
{
public:
	PhysicalMemory *physMemPtr;

	MainCPUAccessNoMMR(FM77AV *vmPtr,PhysicalMemory *physMemPtr);
	virtual uint8_t FetchByte(uint16_t addr);
	virtual uint16_t FetchWord(uint16_t addr);
	virtual void StoreByte(uint16_t addr,uint8_t data);
	virtual void StoreWord(uint16_t addr,uint16_t data);
};

class MainCPUAccessMMR : public MainCPUAccess,public Device
{
public:
	PhysicalMemory *physMemPtr;

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

	inline uint32_t CPUAddrToPhysicalAddr(uint16_t cpuAddr) const
	{
		return MMR[cpuAddr>>12]+(cpuAddr&0xFFF);
	}

	MainCPUAccessMMR(FM77AV *vmPtr,PhysicalMemory *physMemPtr);
	virtual uint8_t FetchByte(uint16_t addr);
	virtual uint16_t FetchWord(uint16_t addr);
	virtual void StoreByte(uint16_t addr,uint8_t data);
	virtual void StoreWord(uint16_t addr,uint16_t data);
};

class SubCPUAccess : public MemoryAccess,public Device
{
public:
	enum
	{
		SUBCPU_ADDR_BASE=0x10000,
	};

	PhysicalMemory *physMemPtr;

	SubCPUAccess(FM77AV *vmPtr,PhysicalMemory *physMemPtr);
	virtual uint8_t FetchByte(uint16_t addr);
	virtual uint16_t FetchWord(uint16_t addr);
	virtual void StoreByte(uint16_t addr,uint8_t data);
	virtual void StoreWord(uint16_t addr,uint16_t data);
};

/* } */
#endif
