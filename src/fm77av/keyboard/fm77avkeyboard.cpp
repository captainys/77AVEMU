#include "fm77av.h"
#include "fm77avkeyboard.h"
#include "fm77avkey.h"



void FM77AVKeyboard::Reset(void)
{
	Device::Reset();

	state.encodingMode=ENCODING_JIS;

	decltype(state.keyCodeQueue) emptyQueue;
	std::swap(state.keyCodeQueue,emptyQueue);
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
