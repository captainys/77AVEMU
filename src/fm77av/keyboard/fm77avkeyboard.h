#ifndef FM77AVKEYBOARD_IS_INCLUDED
#define FM77AVKEYBOARD_IS_INCLUDED
/* { */

#include "device.h"
#include "fm77avkey.h"
#include <queue>

class FM77AVKeyboard : public Device
{
public:
	virtual const char *DeviceName(void) const{return "KEYBOARD";}

	enum
	{
		KEYFLAG_SHIFT=1,
		KEYFLAG_CTRL=2,
		KEYFLAG_GRAPH=4,
	};
	enum
	{
		ENCODING_JIS,
		ENCODING_FM16BETA,
		ENCODING_SCANCODE
	};

	class State
	{
	public:
		uint16_t encodingMode=ENCODING_JIS;
		uint16_t lastKeyCode=0;
		std::queue <uint16_t> keyCodeQueue;
	};
	State state;

	FM77AVKeyboard(VMBase *vmBase) : Device(vmBase){}

	void ProcessKeyCodeInQueue(void);

	void Reset(void);
	void Press(unsigned int keyFlags,unsigned int keyCode);
	void Release(unsigned int keyFlags,unsigned int keyCode);
};

/* } */
#endif
