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

		uint64_t encoderAcknowledgeBy=0;
		uint64_t encoderDataReadyBy=0;
		uint8_t encoderCmd=0;
		std::queue <uint8_t> encoderQueue;
	};
	State state;

	FM77AVKeyboard(VMBase *vmBase) : Device(vmBase){}

	void WriteD431(uint8_t data);
	void WriteD432(uint8_t data);
	void AfterReadD431(void);
	uint8_t NonDestructiveReadD431(void) const;
	uint8_t NonDestructiveReadD432(void) const;

	void ProcessKeyCodeInQueue(void);

	void Reset(void);
	void Press(unsigned int keyFlags,unsigned int keyCode);
	void Release(unsigned int keyFlags,unsigned int keyCode);
};

/* } */
#endif
