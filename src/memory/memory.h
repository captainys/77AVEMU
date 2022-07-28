#ifndef MEMORY_IS_INCLUDED
#define MEMORY_IS_INCLUDED
/* { */

#include <stdint.h>
#include "device.h"



class MemoryAccess
{
public:
	// Apparently, PSHS/PSHU/PULU/PULS instruction just emits address to the address bus
	// one byte more than it actually reads/writes.  FM77AV's hardware drawing seems to
	// reacts to the extra address bus emission and writes to the VRAM.  Regardless of
	// read or write.  To emulate that, in addition to read/write, I need Address function.
	virtual void Address(uint16_t addr){};

	virtual uint8_t FetchByte(uint16_t addr)=0;
	virtual uint16_t FetchWord(uint16_t addr)=0;
	virtual void StoreByte(uint16_t addr,uint8_t data)=0;
	virtual void StoreWord(uint16_t addr,uint16_t data)=0;

	// What are these?
	// 6809 uses memory-mapped I/O.  Memory-Read may change the device status.
	// Fetching a byte for debugging purpose like memory dump or disassembly
	// shouldn't change the device status.  For those purposes, non-destructive
	// fetch is necessary.
	virtual uint8_t NonDestructiveFetchByte(uint16_t addr) const=0;
	virtual uint16_t NonDestructiveFetchWord(uint16_t addr) const=0;
};

/* } */
#endif
