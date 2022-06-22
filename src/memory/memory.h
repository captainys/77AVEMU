#ifndef MEMORY_IS_INCLUDED
#define MEMORY_IS_INCLUDED
/* { */

#include <stdint.h>
#include "device.h"



class MemoryAccess
{
public:
	virtual uint8_t FetchByte(uint16_t addr)=0;
	virtual uint16_t FetchWord(uint16_t addr)=0;
	virtual void StoreByte(uint16_t addr,uint8_t data)=0;
	virtual void StoreWord(uint16_t addr,uint16_t data)=0;
};

/* } */
#endif
