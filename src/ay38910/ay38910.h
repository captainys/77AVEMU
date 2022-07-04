#ifndef AY38910_IS_INCLUDED
#define AY38910_IS_INCLUDED
/* { */



#include <stdint.h>
class AY38910
{
public:
	enum
	{
		NUM_REGS=16,
		REG_MASK=NUM_REGS-1,
		FREQ_CONST=122880,  // FM-Techknow pp.230, 200000 for PC88?
	};

	class State
	{
	public:
		uint8_t regs[NUM_REGS];
	};
	State state;

	void Reset(void);
	uint8_t Read(uint8_t reg) const;
	void Write(uint8_t reg,uint8_t value);

	bool IsPlaying(void) const;
};

/* } */
#endif
