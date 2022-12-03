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
	virtual uint8_t FetchByte(const CanAccessMemory *accessFrom,uint16_t addr) override;
	virtual uint16_t FetchWord(const CanAccessMemory *accessFrom,uint16_t addr) override;
	virtual void StoreByte(const CanAccessMemory *accessFrom,uint16_t addr,uint8_t data) override;
	virtual void StoreWord(const CanAccessMemory *accessFrom,uint16_t addr,uint16_t data) override;
	virtual uint8_t NonDestructiveFetchByte(uint16_t addr) const override;
	virtual uint16_t NonDestructiveFetchWord(uint16_t addr) const override;
};

/* } */
#endif
