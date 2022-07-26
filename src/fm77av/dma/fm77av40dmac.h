#ifndef FM77AV40DMA_IS_INCLUDED
#define FM77AV40DMA_IS_INCLUDED
/* { */



#include "device.h"

class FM77AV40DMAC : public Device
{
public:
	virtual const char *DeviceName(void) const{return "DMAC";}
	FM77AV40DMAC(VMBase *vmBase);

	enum
	{
		NUM_REGS=32,
	};

	class State
	{
	public:
		uint8_t addrLatch=0;
		uint8_t regs[NUM_REGS];
		bool enabled=false;
	};
	State state;

	void Reset(void);

	void WriteFD98(uint8_t data);
	void WriteFD99(uint8_t data);
	uint8_t NonDestructiveReadFD99(void) const;
};

/* } */
#endif
