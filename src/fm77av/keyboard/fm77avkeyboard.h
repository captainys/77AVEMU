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

	static uint8_t AVKeyToScanCode[256];

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

	enum
	{
		AUTOTYPE_INTERVAL=50000000, // 20 chars per sec.
		AUTOTYPE_INTERVAL_RETURN=500000000, // Longer wait after RETURN key.
	};

	enum  // FM77AV40 Hardware Reference pp.230
	{
		CMD_SET_CODING=0x00, // 1-byte 00:FM-7  01:FM-16beta  02:Scan Code  -> 0-byte
		CMD_GET_CODING=0x01, // 0-byte ->  1-byte
		CMD_SET_LED=0x02,    // 1-byte 00:CAPS on  01:CAPS off  02:KANA on  03:KANA off
		CMD_GET_LED=0x03,    // 0-byte -> 1-byte Bit0:CAPS Bit1:KANA
		CMD_SET_AUTO_REPEAT=0x04, // 1-byte 00:Repeat ON  01:Repeat OFF
		CMD_SET_AUTO_REPEAT_INTERVAL=0x05, // 2-bytes [0]Repeat Start Time [1]Repeat Interval in millisec
		CMD_REAL_TIME_CLOCK=0x80, // 1-byte 00:Read -> 7-bytes  01:Write+7 more parameters.
		CMD_DIGITIZE_MODE=0x81, // 1-byte parameter 0x00
		CMD_SET_SCREEN_MODE=0x82, // 1-byte 00:Computer Only  01:Superimpose  02:TV only  03:Digtize
		CMD_GET_SCREEN_MODE=0x83, // 0-byte -> 1-byte Screen Mode
		CMD_SET_TV_BRIGHTNESS=0x84, // 1-byte 00:Bright  01:Dark

		CMD_NULL=0xFF,
	};
	// For command 00,02,04,05,80,81,82,84, undefined parameter will be ignored
	// and taken as a new command. pp.235


	class State
	{
	public:
		uint16_t encodingMode=ENCODING_JIS;
		uint16_t lastKeyCode=0;
		std::queue <uint16_t> keyCodeQueue;

		uint64_t encoderAcknowledgeBy=0;
		uint64_t encoderDataReadyBy=0;
		uint16_t encoderCmd=0xFF;
		uint8_t nEncoderParam=0;
		uint8_t encoderParam[8]; // RTC-set (80 01) takes the longest 8-byte parameter.
		std::queue <uint8_t> encoderQueue;

		bool CAPS=false,KANA=false;
		bool INSLED=false;
		uint8_t videoCaptureMode; // Should I implement video digitize/superimpose?  I guess video capture card for Tsugaru before that.
		uint8_t videoCaptureBrightness;
		bool keyRepeat=true;
		unsigned int keyRepeatStartTime=50,keyRepeatInterval=100;
	};
	State state;

	class Variable
	{
	public:
		std::queue <uint16_t> autoType;
	};
	Variable var;

	FM77AVKeyboard(VMBase *vmBase) : Device(vmBase){}

	void WriteD431(uint8_t data);
	void WriteD432(uint8_t data);
	void AfterReadD431(void);
	uint8_t NonDestructiveReadD431(void) const;
	uint8_t NonDestructiveReadD432(void) const;

	void ClearEncoderQueue(void);
	void ProcessKeyCodeInQueue(void);

	virtual void RunScheduledTask(unsigned long long int fm77avTime);

	void Reset(void);

	void Type(unsigned int ASCIICode); // Virtually type a letter.

	void Press(unsigned int keyFlags,unsigned int keyCode);
	void Release(unsigned int keyFlags,unsigned int keyCode);
};

/* } */
#endif
