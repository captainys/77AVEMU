#include "fm77av.h"
#include "fm77avkeyboard.h"
#include "fm77avkey.h"



void FM77AVKeyboard::Reset(void)
{
	Device::Reset();

	state.encodingMode=ENCODING_JIS;
	state.encoderCmd=0;

	decltype(state.keyCodeQueue) emptyKeyCodeQueue;
	std::swap(state.keyCodeQueue,emptyKeyCodeQueue);

	decltype(state.encoderQueue) emptyEncoderQueue;
	std::swap(state.encoderQueue,emptyEncoderQueue);
}
void FM77AVKeyboard::ProcessKeyCodeInQueue(void)
{
	FM77AV *fm77av=(FM77AV *)vmPtr;
	if(true!=state.keyCodeQueue.empty())
	{
		state.lastKeyCode=state.keyCodeQueue.front();
		state.keyCodeQueue.pop();
		fm77av->SetKeyIRQFlag();
	}
}
void FM77AVKeyboard::Press(unsigned int keyFlags,unsigned int keyCode)
{
	FM77AV *fm77av=(FM77AV *)vmPtr;

	switch(state.encodingMode)
	{
	case ENCODING_JIS:
	case ENCODING_FM16BETA:
		{
			if(AVKEY_BREAK==keyCode)
			{
				fm77av->SetBreakKeyFIRQFlag();
			}

			FM77AVKeyCombination keyComb;
			keyComb.shift=(0!=(keyFlags&KEYFLAG_SHIFT));
			keyComb.ctrl=(0!=(keyFlags&KEYFLAG_CTRL));
			keyComb.graph=(0!=(keyFlags&KEYFLAG_GRAPH));
			keyComb.keyCode=keyCode;
			auto code=FM77AVTranslateKeyCombinationToChar(keyComb);
			if(true!=fm77av->KeyIRQFlagSet())
			{
				fm77av->SetKeyIRQFlag();
				state.lastKeyCode=code;
			}
			else
			{
				state.keyCodeQueue.push(code);
			}
		}
		break;
	case ENCODING_SCANCODE:
		break;
	}
}
void FM77AVKeyboard::Release(unsigned int keyFlags,unsigned int keyCode)
{
	FM77AV *fm77av=(FM77AV *)vmPtr;

	switch(state.encodingMode)
	{
	case ENCODING_JIS:
	case ENCODING_FM16BETA:
		{
			if(AVKEY_BREAK==keyCode)
			{
				fm77av->ClearBreakKeyFIRQFlag();
			}
		}
		break;
	case ENCODING_SCANCODE:
		break;
	}
}

// Sub-System Monitor A Waits for $D432 bit 8 to be low (Ready to Receive) before reading $D431.
// Sub-System Monitor A Write to $D431 then wait for $D432 bit 0 to clear.
// Sub-System Monitor A writes $80 to $D431 and waits for $D432 bit 0 to clear on start up.
void FM77AVKeyboard::WriteD431(uint8_t data)
{
	state.encoderCmd<<=8;
	state.encoderCmd|=data;
	switch(state.encoderCmd)
	{
	case 0x8000: // ?? Probably Reset.  Subsys Monitor A expects to read 7 bytes after this command.
		break;
	}

	const FM77AV *fm77av=(const FM77AV *)vmPtr;
	state.encoderAcknowledgeBy=fm77av->state.fm77avTime+100*FM77AVTIME_MICROSEC;
}
void FM77AVKeyboard::WriteD432(uint8_t data)
{
}
void FM77AVKeyboard::AfterReadD431(void)
{
	const FM77AV *fm77av=(const FM77AV *)vmPtr;
	state.encoderDataReadyBy=fm77av->state.fm77avTime+100*FM77AVTIME_MICROSEC;
}
uint8_t FM77AVKeyboard::NonDestructiveReadD431(void) const
{
	return 0;
}
uint8_t FM77AVKeyboard::NonDestructiveReadD432(void) const
{
	return 0;
}
