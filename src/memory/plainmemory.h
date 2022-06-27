#ifndef PLAINMEMORY_IS_INCLUDED
#define PLAINMEMORY_IS_INCLUDED
/* { */

#include <stdint.h>
#include "memory.h"

class PlainMemoryAccess : public MemoryAccess
{
public:
	uint8_t RAM[64*1024];

	PlainMemoryAccess();
	virtual uint8_t FetchByte(uint16_t addr);
	virtual uint16_t FetchWord(uint16_t addr);
	virtual void StoreByte(uint16_t addr,uint8_t data);
	virtual void StoreWord(uint16_t addr,uint16_t data);
	virtual uint8_t NonDestructiveFetchByte(uint16_t addr) const;
	virtual uint16_t NonDestructiveFetchWord(uint16_t addr) const;
};

/* } */
#endif
