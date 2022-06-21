#ifndef MC6809_IS_INCLUDED
#define MC6809_IS_INCLUDED
/* { */

#include <stdint.h>
#include "vmbase.h"
#include "memory.h"

class MC6809 : public Device
{
public:
	class Instruction
	{
	public:
		
	};

	class RegisterSet
	{
	public:
		uint8_t A,B,DP,CC;
		uint16_t X,Y,U,S,PC;

		inline uint16_t D(void) const
		{
			uint16_t D;
			D=A;
			D<<=8;
			return D|B;
		}
		inline void SetD(uint16_t D)
		{
			A=(D>>8);
			B=(D&0xFF);
		}
	};

	enum
	{
		CF=0x01,
		VF=0x02,
		ZF=0x04,
		SF=0x08, // Is it officially called Negative Flag?  I always called it Sign Flag.
		IRQMASK=0x10,
		HF=0x10,
		FIRQMASK=0x40,
		EF=0x80,
	};

	class State : public RegisterSet
	{
	};
	State state;


	class Debugger
	{
	public:
		bool enabled=false;
		bool stop=false;

		void ClearStopFlag(void);
	};
	Debugger debugger;


	virtual const char *DeviceName(void) const{return "MC6809";}
	MC6809(VMBase *vmBase);
	uint32_t RunOneInstruction(class MemoryAccess *mem); // Returns the number of clocks passed
};

/* } */
#endif
