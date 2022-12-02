#ifndef MEMORY_IS_INCLUDED
#define MEMORY_IS_INCLUDED
/* { */

#include <stdint.h>
#include "device.h"



class CanAccessMemory
{
};

class MemoryAccess
{
public:
	// OK, OK.  Now things got much more clear.  Thank you, Mr. Apollo for the information.
	// MC6809 data sheet has a flow-chart for every instruction, and PSHS,PSHU,PULS, and PULU,
	// does dummy-read a byte.  So, not just addressing.

	// Also, CLR instruction is in the same path as INC, NEG, COM, etc.  So, even though
	// CLR always writes a value zero, it does dummy read, and it takes 2 clocks after dummy
	// read til zero is written.

	// A FM-7 masterpiece MAGUS was doing CLR $D40A to clear BUSY flag.  It is a bug,
	// and it is most likely unintentional.  It does not vindicate MAGUS.  However,
	// now we know why the main-CPU thought the sub-CPU was ready.

	virtual void CLR(uint16_t addr){}; // Special treatment for CLR.

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
