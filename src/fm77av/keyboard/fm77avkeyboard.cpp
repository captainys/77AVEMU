#include "fm77av.h"
#include "fm77avkeyboard.h"
#include "fm77avkey.h"
#include "cpputil.h"



FM77AVKeyboard::FM77AVKeyboard(VMBase *vmBase) : Device(vmBase)
{
	for(int i=0; nullptr!=rKanaTable[i].keyComb; ++i)
	{
		RomajiMap[std::string(rKanaTable[i].keyComb)]=rKanaTable[i];
	}

	for(auto &c : AVKeyToScanCode)
	{
		c=0;
	}

	AVKeyToScanCode[AVKEY_BREAK]=0x5C;
	AVKeyToScanCode[AVKEY_PF1]=0x5D;
	AVKeyToScanCode[AVKEY_PF2]=0x5E;
	AVKeyToScanCode[AVKEY_PF3]=0x5F;
	AVKeyToScanCode[AVKEY_PF4]=0x60;
	AVKeyToScanCode[AVKEY_PF5]=0x61;
	AVKeyToScanCode[AVKEY_PF6]=0x62;
	AVKeyToScanCode[AVKEY_PF7]=0x63;
	AVKeyToScanCode[AVKEY_PF8]=0x64;
	AVKeyToScanCode[AVKEY_PF9]=0x65;
	AVKeyToScanCode[AVKEY_PF10]=0x66;
	AVKeyToScanCode[AVKEY_EL]=0x49;
	AVKeyToScanCode[AVKEY_CLS]=0x4A;
	AVKeyToScanCode[AVKEY_DUP]=0x4C;
	AVKeyToScanCode[AVKEY_HOME]=0x4E;
	AVKeyToScanCode[AVKEY_INS]=0x48;
	AVKeyToScanCode[AVKEY_DEL]=0x4B;
	AVKeyToScanCode[AVKEY_LEFT]=0x4F;
	AVKeyToScanCode[AVKEY_RIGHT]=0x51;
	AVKeyToScanCode[AVKEY_UP]=0x4D;
	AVKeyToScanCode[AVKEY_DOWN]=0x50;

	AVKeyToScanCode[AVKEY_ESC]=0x01;
	AVKeyToScanCode[AVKEY_0]=0x02;
	AVKeyToScanCode[AVKEY_1]=0x03;
	AVKeyToScanCode[AVKEY_2]=0x04;
	AVKeyToScanCode[AVKEY_3]=0x05;
	AVKeyToScanCode[AVKEY_4]=0x06;
	AVKeyToScanCode[AVKEY_5]=0x07;
	AVKeyToScanCode[AVKEY_6]=0x08;
	AVKeyToScanCode[AVKEY_7]=0x09;
	AVKeyToScanCode[AVKEY_8]=0x0A;
	AVKeyToScanCode[AVKEY_9]=0x0B;
	AVKeyToScanCode[AVKEY_MINUS]=0x0C;
	AVKeyToScanCode[AVKEY_HAT]=0x0D;
	AVKeyToScanCode[AVKEY_YEN]=0x0E;
	AVKeyToScanCode[AVKEY_BACKSPACE]=0x0F;

	AVKeyToScanCode[AVKEY_TAB]=0x10;
	AVKeyToScanCode[AVKEY_Q]=0x11;
	AVKeyToScanCode[AVKEY_W]=0x12;
	AVKeyToScanCode[AVKEY_E]=0x13;
	AVKeyToScanCode[AVKEY_R]=0x14;
	AVKeyToScanCode[AVKEY_T]=0x15;
	AVKeyToScanCode[AVKEY_Y]=0x16;
	AVKeyToScanCode[AVKEY_U]=0x17;
	AVKeyToScanCode[AVKEY_I]=0x18;
	AVKeyToScanCode[AVKEY_O]=0x19;
	AVKeyToScanCode[AVKEY_P]=0x1A;
	AVKeyToScanCode[AVKEY_AT]=0x1B;
	AVKeyToScanCode[AVKEY_LEFT_SQUARE_BRACKET]=0x1C;
	AVKeyToScanCode[AVKEY_RETURN]=0x1D;

	AVKeyToScanCode[AVKEY_CTRL]=0x52;
	AVKeyToScanCode[AVKEY_A]=0x1E;
	AVKeyToScanCode[AVKEY_S]=0x1F;
	AVKeyToScanCode[AVKEY_D]=0x20;
	AVKeyToScanCode[AVKEY_F]=0x21;
	AVKeyToScanCode[AVKEY_G]=0x22;
	AVKeyToScanCode[AVKEY_H]=0x23;
	AVKeyToScanCode[AVKEY_J]=0x24;
	AVKeyToScanCode[AVKEY_K]=0x25;
	AVKeyToScanCode[AVKEY_L]=0x26;
	AVKeyToScanCode[AVKEY_SEMICOLON]=0x27;
	AVKeyToScanCode[AVKEY_COLON]=0x28;
	AVKeyToScanCode[AVKEY_RIGHT_SQUARE_BRACKET]=0x29;

	AVKeyToScanCode[AVKEY_LEFT_SHIFT]=0x53;
	AVKeyToScanCode[AVKEY_Z]=0x2A;
	AVKeyToScanCode[AVKEY_X]=0x2B;
	AVKeyToScanCode[AVKEY_C]=0x2C;
	AVKeyToScanCode[AVKEY_V]=0x2D;
	AVKeyToScanCode[AVKEY_B]=0x2E;
	AVKeyToScanCode[AVKEY_N]=0x2F;
	AVKeyToScanCode[AVKEY_M]=0x30;
	AVKeyToScanCode[AVKEY_COMMA]=0x31;
	AVKeyToScanCode[AVKEY_DOT]=0x32;
	AVKeyToScanCode[AVKEY_SLASH]=0x33;
	AVKeyToScanCode[AVKEY_DOUBLE_QUOTE]=0x34;
	AVKeyToScanCode[AVKEY_RIGHT_SHIFT]=0x54;

	AVKeyToScanCode[AVKEY_CAPS]=0x55;
	AVKeyToScanCode[AVKEY_GRAPH]=0x56;
	AVKeyToScanCode[AVKEY_LEFT_SPACE]=0x57;
	AVKeyToScanCode[AVKEY_MID_SPACE]=0x58;
	AVKeyToScanCode[AVKEY_RIGHT_SPACE]=0x35;
	AVKeyToScanCode[AVKEY_KANA]=0x5A;

	AVKeyToScanCode[AVKEY_NUM_STAR]=0x36;
	AVKeyToScanCode[AVKEY_NUM_SLASH]=0x37;
	AVKeyToScanCode[AVKEY_NUM_PLUS]=0x38;
	AVKeyToScanCode[AVKEY_NUM_MINUS]=0x39;
	AVKeyToScanCode[AVKEY_NUM_EQUAL]=0x3D;
	AVKeyToScanCode[AVKEY_NUM_COMMA]=0x41;
	AVKeyToScanCode[AVKEY_NUM_RETURN]=0x45;
	AVKeyToScanCode[AVKEY_NUM_DOT]=0x47;
	AVKeyToScanCode[AVKEY_NUM_0]=0x46;
	AVKeyToScanCode[AVKEY_NUM_1]=0x42;
	AVKeyToScanCode[AVKEY_NUM_2]=0x43;
	AVKeyToScanCode[AVKEY_NUM_3]=0x44;
	AVKeyToScanCode[AVKEY_NUM_4]=0x3E;
	AVKeyToScanCode[AVKEY_NUM_5]=0x3F;
	AVKeyToScanCode[AVKEY_NUM_6]=0x40;
	AVKeyToScanCode[AVKEY_NUM_7]=0x3A;
	AVKeyToScanCode[AVKEY_NUM_8]=0x3B;
	AVKeyToScanCode[AVKEY_NUM_9]=0x3C;

	for(auto &b : AVKeyToRKanaKey)
	{
		b=0;
	}
	AVKeyToRKanaKey[AVKEY_A]='a';
	AVKeyToRKanaKey[AVKEY_B]='b';
	AVKeyToRKanaKey[AVKEY_C]='c';
	AVKeyToRKanaKey[AVKEY_D]='d';
	AVKeyToRKanaKey[AVKEY_E]='e';
	AVKeyToRKanaKey[AVKEY_F]='f';
	AVKeyToRKanaKey[AVKEY_G]='g';
	AVKeyToRKanaKey[AVKEY_H]='h';
	AVKeyToRKanaKey[AVKEY_I]='i';
	AVKeyToRKanaKey[AVKEY_J]='j';
	AVKeyToRKanaKey[AVKEY_K]='k';
	AVKeyToRKanaKey[AVKEY_L]='l';
	AVKeyToRKanaKey[AVKEY_M]='m';
	AVKeyToRKanaKey[AVKEY_N]='n';
	AVKeyToRKanaKey[AVKEY_O]='o';
	AVKeyToRKanaKey[AVKEY_P]='p';
	AVKeyToRKanaKey[AVKEY_Q]='q';
	AVKeyToRKanaKey[AVKEY_R]='r';
	AVKeyToRKanaKey[AVKEY_S]='s';
	AVKeyToRKanaKey[AVKEY_T]='t';
	AVKeyToRKanaKey[AVKEY_U]='u';
	AVKeyToRKanaKey[AVKEY_V]='v';
	AVKeyToRKanaKey[AVKEY_W]='w';
	AVKeyToRKanaKey[AVKEY_X]='x';
	AVKeyToRKanaKey[AVKEY_Y]='y';
	AVKeyToRKanaKey[AVKEY_Z]='z';
	AVKeyToRKanaKey[AVKEY_MINUS]='-';
	AVKeyToRKanaKey[AVKEY_NUM_MINUS]='-';

	for(auto &b : IsNumKey)
	{
		b=false;
	}
	IsNumKey[AVKEY_NUM_STAR]=true;
	IsNumKey[AVKEY_NUM_SLASH]=true;
	IsNumKey[AVKEY_NUM_PLUS]=true;
	IsNumKey[AVKEY_NUM_MINUS]=true;
	IsNumKey[AVKEY_NUM_EQUAL]=true;
	IsNumKey[AVKEY_NUM_COMMA]=true;
	IsNumKey[AVKEY_NUM_RETURN]=true;
	IsNumKey[AVKEY_NUM_DOT]=true;
	IsNumKey[AVKEY_NUM_0]=true;
	IsNumKey[AVKEY_NUM_1]=true;
	IsNumKey[AVKEY_NUM_2]=true;
	IsNumKey[AVKEY_NUM_3]=true;
	IsNumKey[AVKEY_NUM_4]=true;
	IsNumKey[AVKEY_NUM_5]=true;
	IsNumKey[AVKEY_NUM_6]=true;
	IsNumKey[AVKEY_NUM_7]=true;
	IsNumKey[AVKEY_NUM_8]=true;
	IsNumKey[AVKEY_NUM_9]=true;

	for(auto &b : IsArrowKey)
	{
		b=false;
	}
	IsArrowKey[AVKEY_LEFT]=true;
	IsArrowKey[AVKEY_RIGHT]=true;
	IsArrowKey[AVKEY_UP]=true;
	IsArrowKey[AVKEY_DOWN]=true;

	for(int i=0; i<AVKEY_NUM_KEYCODE; ++i)
	{
		AVKeyRemap[i]=i;
	}
}

void FM77AVKeyboard::Reset(void)
{
	Device::Reset();

	state.encodingMode=ENCODING_JIS;
	state.encoderCmd=0xFF;
	state.nEncoderParam=0;
	for(auto &p : state.encoderParam)
	{
		p=0;
	}

	state.CAPS=false;
	state.KANA=false;
	state.videoCaptureMode=0;
	state.videoCaptureBrightness=0;
	state.keyRepeat=true;
	state.keyRepeatStartTime=700000000;
	state.keyRepeatInterval=  70000000;

	// Death Force Expects non-zero read from $FD01 on reset.
	state.lastKeyCode=0xFF;

	decltype(state.keyCodeQueue) emptyKeyCodeQueue;
	std::swap(state.keyCodeQueue,emptyKeyCodeQueue);

	decltype(state.encoderQueue) emptyEncoderQueue;
	std::swap(state.encoderQueue,emptyEncoderQueue);

	for(auto &b : heldDown)
	{
		b=false;
	}
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
void FM77AVKeyboard::ClearEncoderQueue(void)
{
	std::queue <uint8_t> empty;
	state.encoderQueue.swap(empty);
}

/* virtual */ void FM77AVKeyboard::RunScheduledTask(unsigned long long int fm77avTime)
{
	FM77AV *fm77av=(FM77AV *)vmPtr;
	if(true!=var.autoType.empty())
	{
		if(true==fm77av->KeyIRQFlagSet())
		{
			// Previous key was not read.
			fm77av->ScheduleDeviceCallBack(*this,fm77avTime+AUTOTYPE_INTERVAL);
			return;
		}

		auto ASCIICode=var.autoType.front();
		var.autoType.pop();

		switch(state.encodingMode)
		{
		case ENCODING_JIS:
		case ENCODING_FM16BETA:
			fm77av->SetKeyIRQFlag();
			state.lastKeyCode=ASCIICode;
			break;

		case ENCODING_SCANCODE:
			std::cout << "Auto Typing Not Supported in the Scan mode." << std::endl;
			break;
		}

		if(true!=var.autoType.empty())
		{
			if(0x0D==state.lastKeyCode)
			{
				fm77av->ScheduleDeviceCallBack(*this,fm77avTime+AUTOTYPE_INTERVAL_RETURN);
			}
			else
			{
				fm77av->ScheduleDeviceCallBack(*this,fm77avTime+AUTOTYPE_INTERVAL);
			}
		}
	}
}

void FM77AVKeyboard::Type(unsigned int ASCIICode)
{
	FM77AV *fm77av=(FM77AV *)vmPtr;
	var.autoType.push(ASCIICode);
	fm77av->ScheduleDeviceCallBack(*this,fm77av->state.fm77avTime+AUTOTYPE_INTERVAL);
}

void FM77AVKeyboard::Press(unsigned int keyFlags,unsigned int keyCode)
{
	FM77AV *fm77av=(FM77AV *)vmPtr;
	uint8_t eightBit=keyCode;

	keyCode=AVKeyRemap[keyCode];
	heldDown[keyCode]=true;

	// std::cout << "Press " << FM77AVKeyCodeToKeyLabel(keyCode) << std::endl;

	switch(state.encodingMode)
	{
	case ENCODING_JIS:
	case ENCODING_FM16BETA:
		{
			if(AVKEY_BREAK==keyCode)
			{
				fm77av->SetBreakKeyFIRQFlag();
				break;
			}

			PushKeyToQueueJISMode(keyFlags,keyCode);

			switch(var.autoStopAfterThis)
			{
			case AUTOSTOP_AFTER_NUM_RELEASE:
				break;
			case AUTOSTOP_AFTER_ARROW_RELEASE:
				break;
			case AUTOSTOP_AFTER_NUM_RELEASE_AND_RETYPE:
				if(true==IsNumKey[keyCode])
				{
					var.autoStopRetypeKey=keyCode;
				}
				else if(AVKEY_NULL!=var.autoStopRetypeKey)
				{
					PushKeyToQueueJISMode(0,var.autoStopRetypeKey);
				}
				break;
			case AUTOSTOP_AFTER_ARROW_RELEASE_AND_RETYPE:
				if(true==IsArrowKey[keyCode])
				{
					var.autoStopRetypeKey=keyCode;
				}
				else if(AVKEY_NULL!=var.autoStopRetypeKey)
				{
					PushKeyToQueueJISMode(0,var.autoStopRetypeKey);
				}
				break;
			case AUTOSTOP_AFTER_ANY_KEY_RELEASE:
				break;
			}
		}
		break;
	case ENCODING_SCANCODE:
		if(0!=AVKeyToScanCode[eightBit])
		{
			auto code=AVKeyToScanCode[eightBit];
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
	}
}
void FM77AVKeyboard::Release(unsigned int keyFlags,unsigned int keyCode)
{
	FM77AV *fm77av=(FM77AV *)vmPtr;
	uint8_t eightBit=keyCode;

	keyCode=AVKeyRemap[keyCode];
	heldDown[keyCode]=false;

	// std::cout << "Release " << FM77AVKeyCodeToKeyLabel(keyCode) << std::endl;

	switch(state.encodingMode)
	{
	case ENCODING_JIS:
	case ENCODING_FM16BETA:
		{
			if(keyCode==var.autoStopRetypeKey)
			{
				var.autoStopRetypeKey=AVKEY_NULL;
			}
			switch(var.autoStopAfterThis)
			{
			case AUTOSTOP_AFTER_NUM_RELEASE:
				if(true==IsNumKey[keyCode] && true!=NumKeyHeldDown())
				{
					PushKeyToQueueJISMode(0,var.autoStopKey);
				}
				break;
			case AUTOSTOP_AFTER_ARROW_RELEASE:
				if(true==IsArrowKey[keyCode] && true!=ArrowKeyHeldDown())
				{
					PushKeyToQueueJISMode(0,var.autoStopKey);
				}
				break;
			case AUTOSTOP_AFTER_NUM_RELEASE_AND_RETYPE:
				if(true==IsNumKey[keyCode] && true!=NumKeyHeldDown())
				{
					PushKeyToQueueJISMode(0,var.autoStopKey);
				}
				break;
			case AUTOSTOP_AFTER_ARROW_RELEASE_AND_RETYPE:
				if(true==IsArrowKey[keyCode] && true!=ArrowKeyHeldDown())
				{
					PushKeyToQueueJISMode(0,var.autoStopKey);
				}
				break;
			case AUTOSTOP_AFTER_ANY_KEY_RELEASE:
				PushKeyToQueueJISMode(0,var.autoStopKey);
				break;
			}

			if(AVKEY_BREAK==keyCode)
			{
				fm77av->ClearBreakKeyFIRQFlag();
			}
		}
		break;
	case ENCODING_SCANCODE:
		if(0!=AVKeyToScanCode[eightBit])
		{
			auto code=AVKeyToScanCode[eightBit]|0x80;
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
	}
}
void FM77AVKeyboard::PushKeyToQueueJISMode(unsigned int keyFlags,unsigned int keyCode)
{
	if(true==var.rKanaMode)
	{
		char ascii=AVKeyToRKanaKey[keyCode];
		if(0!=ascii)
		{
			var.romaji.push_back(ascii);
			auto found=RomajiMap.find(var.romaji);
			if(RomajiMap.end()!=found)
			{
				for(int i=0; i<4 && 0!=found->second.output[i]; ++i)
				{
					if(0==i)
					{
						Type(found->second.output[i]);
					}
					else
					{
						var.autoType.push(found->second.output[i]);
					}
				}
				if(true==found->second.leaveFirstChar)
				{
					var.romaji.erase(var.romaji.begin());
				}
				else
				{
					var.romaji.clear();
				}
			}
			else
			{
				if(3<=var.romaji.size())
				{
					var.romaji.clear();
				}
			}
			return;
		}
		var.romaji.clear();
	}
	FM77AVKeyCombination keyComb;
	keyComb.shift=(0!=(keyFlags&KEYFLAG_SHIFT));
	keyComb.ctrl=(0!=(keyFlags&KEYFLAG_CTRL));
	keyComb.graph=(0!=(keyFlags&KEYFLAG_GRAPH));
	keyComb.keyCode=keyCode;
	auto code=FM77AVTranslateKeyCombinationToChar(keyComb);
std::cout << code << std::endl;
	PushASCIICodeToQueueJISMode(code);
}
void FM77AVKeyboard::PushASCIICodeToQueueJISMode(uint16_t ascii)
{
	if(0!=ascii)
	{
		FM77AV *fm77av=(FM77AV *)vmPtr;
		if(true!=fm77av->KeyIRQFlagSet())
		{
			fm77av->SetKeyIRQFlag();
			state.lastKeyCode=ascii;
		}
		else
		{
			state.keyCodeQueue.push(ascii);
		}
	}
}
bool FM77AVKeyboard::NumKeyHeldDown(void) const
{
	return
		heldDown[AVKEY_NUM_STAR] ||
		heldDown[AVKEY_NUM_SLASH] ||
		heldDown[AVKEY_NUM_PLUS] ||
		heldDown[AVKEY_NUM_MINUS] ||
		heldDown[AVKEY_NUM_EQUAL] ||
		heldDown[AVKEY_NUM_COMMA] ||
		heldDown[AVKEY_NUM_RETURN] ||
		heldDown[AVKEY_NUM_DOT] ||
		heldDown[AVKEY_NUM_0] ||
		heldDown[AVKEY_NUM_1] ||
		heldDown[AVKEY_NUM_2] ||
		heldDown[AVKEY_NUM_3] ||
		heldDown[AVKEY_NUM_4] ||
		heldDown[AVKEY_NUM_5] ||
		heldDown[AVKEY_NUM_6] ||
		heldDown[AVKEY_NUM_7] ||
		heldDown[AVKEY_NUM_8] ||
		heldDown[AVKEY_NUM_9];
}
bool FM77AVKeyboard::ArrowKeyHeldDown(void) const
{
	return
		heldDown[AVKEY_LEFT] ||
		heldDown[AVKEY_RIGHT] ||
		heldDown[AVKEY_UP] ||
		heldDown[AVKEY_DOWN];
}

// Sub-System Monitor A Waits for $D432 bit 8 to be low (Ready to Receive) before reading $D431.
// Sub-System Monitor A Write to $D431 then wait for $D432 bit 0 to clear.
// Sub-System Monitor A writes $80 to $D431 and waits for $D432 bit 0 to clear on start up.
void FM77AVKeyboard::WriteD431(uint8_t data)
{
std::cout << cpputil::Ubtox(data) << std::endl;

	switch(state.encoderCmd)
	{
	default:
		state.encoderCmd=data;
		state.nEncoderParam=0;
		// Immediate response
		switch(data)
		{
		case CMD_GET_CODING://0x01, // 0-byte ->  1-byte
			ClearEncoderQueue();
			state.encoderQueue.push(state.encodingMode);
			state.encoderCmd=CMD_NULL;
			break;
		case CMD_GET_LED://0x03,    // 0-byte -> 1-byte Bit0:CAPS Bit1:KANA
			ClearEncoderQueue();
			state.encoderQueue.push((true==state.CAPS ? 1 : 0)|(true==state.KANA ? 2 : 0));
			state.encoderCmd=CMD_NULL;
			break;
		case CMD_GET_SCREEN_MODE://0x83, // 0-byte -> 1-byte Screen Mode
			ClearEncoderQueue();
			state.encoderQueue.push(state.videoCaptureMode);
			state.encoderCmd=CMD_NULL;
			break;
		}
		break;
	case CMD_SET_CODING://0x00, // 1-byte 00:FM-7  01:FM-16beta  02:Scan Code  -> 0-byte
		if(data<=2)
		{
			state.encodingMode=data;
		}
		state.encoderCmd=CMD_NULL;
		state.nEncoderParam=0;
		break;
	case CMD_SET_LED://0x02,    // 1-byte 00:CAPS on  01:CAPS off  02:KANA on  03:KANA off
		switch(data)
		{
		case 0:
			state.CAPS=true;
			break;
		case 1:
			state.CAPS=false;
			break;
		case 2:
			state.KANA=true;
			break;
		case 3:
			state.KANA=false;
			break;
		}
		state.encoderCmd=CMD_NULL;
		break;
	case CMD_SET_AUTO_REPEAT://0x04, // 1-byte 00:Repeat ON  01:Repeat OFF
		switch(data)
		{
		case 0:
			state.keyRepeat=true;
			break;
		case 1:
			state.keyRepeat=false;
			break;
		}
		state.encoderCmd=CMD_NULL;
		break;
	case CMD_SET_AUTO_REPEAT_INTERVAL://0x05, // 2-bytes [0]Repeat Start Time [1]Repeat Interval in millisec
		state.encoderParam[state.nEncoderParam++]=data;
		if(2<=state.nEncoderParam)
		{
			state.keyRepeatStartTime=state.encoderParam[0]*10;
			state.keyRepeatStartTime*=1000000;  // Convert to nanosec
			state.keyRepeatInterval=state.encoderParam[1]*10;
			state.keyRepeatInterval*=1000000;  // Convert to nanosec
			state.nEncoderParam=0;
			state.encoderCmd=CMD_NULL;
		}
		break;
	case CMD_REAL_TIME_CLOCK://0x80, // 1-byte 00:Read -> 7-bytes  01:Write+7 more parameters.
		if(0==state.nEncoderParam)
		{
			if(0==data)
			{
				state.encoderQueue.push(0); // FM Techknow pp.141 Must return RTC
				state.encoderQueue.push(0); // Will take it from host clock.
				state.encoderQueue.push(0);
				state.encoderQueue.push(0);
				state.encoderQueue.push(0);
				state.encoderQueue.push(0);
				state.encoderQueue.push(0);
				state.nEncoderParam=0;
				state.encoderCmd=CMD_NULL;
			}
			else if(1==data)
			{
				state.encoderParam[0]=data;
				state.nEncoderParam=1;
			}
			else
			{
				state.encoderCmd=CMD_NULL;
			}
		}
		else
		{
			state.encoderParam[state.nEncoderParam++]=data;
			if(8<=state.nEncoderParam)
			{
				// Supposed to set RTC, but I'll take it from host clock.
				state.nEncoderParam=0;
				state.encoderCmd=CMD_NULL;
			}
		}
		break; 
	case CMD_DIGITIZE_MODE://0x81, // 1-byte parameter 0x00
		// I may do something when I implement video-digitize.
		state.nEncoderParam=0;
		state.encoderCmd=CMD_NULL;
		break;
	case CMD_SET_SCREEN_MODE://0x82, // 1-byte 00:Computer Only  01:Superimpose  02:TV only  03:Digtize
		state.videoCaptureMode=data;
		state.nEncoderParam=0;
		state.encoderCmd=CMD_NULL;
		break;
	case CMD_SET_TV_BRIGHTNESS://0x84, // 1-byte 00:Bright  01:Dark
		state.videoCaptureBrightness=data;
		state.nEncoderParam=0;
		state.encoderCmd=CMD_NULL;
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
	uint8_t byteData=0xFF;
	const FM77AV *fm77av=(const FM77AV *)vmPtr;
	if(fm77av->state.fm77avTime<=state.encoderAcknowledgeBy)
	{
		byteData&=0xFE;
	}
	if(true!=state.encoderQueue.empty() && state.encoderDataReadyBy<fm77av->state.fm77avTime)
	{
		byteData&=0x7F;
	}
	return byteData;
}
uint64_t FM77AVKeyboard::GetKeyRepeatStartTime(void) const
{
	return state.keyRepeatStartTime;
}
uint64_t FM77AVKeyboard::GetKeyRepeatInterval(void) const
{
	return state.keyRepeatInterval;
}


/* virtual */ uint32_t FM77AVKeyboard::SerializeVersion(void) const
{
	return 0;
}
/* virtual */ void FM77AVKeyboard::SpecificSerialize(std::vector <unsigned char> &data,std::string stateFName) const
{
	PushUint16(data,state.encodingMode);
	// Death Force Expects non-zero read from $FD01 on reset.
	PushUint16(data,state.lastKeyCode);
	{
		auto queueCopy=state.keyCodeQueue;
		PushUint32(data,queueCopy.size());
		while(true!=queueCopy.empty())
		{
			PushUint16(data,queueCopy.front());
			queueCopy.pop();
		}
	}

	PushUint64(data,state.encoderAcknowledgeBy);
	PushUint64(data,state.encoderDataReadyBy);
	PushUint16(data,state.encoderCmd);
	PushUint16(data,state.nEncoderParam);
	for(int i=0; i<8; ++i)
	{
		PushUint16(data,state.encoderParam[i]); // RTC-set (80 01) takes the longest 8-byte parameter.
	}
	{
		auto queueCopy=state.encoderQueue;
		PushUint32(data,queueCopy.size());
		while(true!=queueCopy.empty())
		{
			PushUint16(data,queueCopy.front());
			queueCopy.pop();
		}
	}

	PushBool(data,state.CAPS);
	PushBool(data,state.KANA);
	PushBool(data,state.INSLED);
	PushUint16(data,state.videoCaptureMode);
	PushUint16(data,state.videoCaptureBrightness);
	PushBool(data,state.keyRepeat);
	PushUint64(data,state.keyRepeatStartTime);
	PushUint64(data,state.keyRepeatInterval);
}
/* virtual */ bool FM77AVKeyboard::SpecificDeserialize(const unsigned char *&data,std::string stateFName,uint32_t version)
{
	state.encodingMode=ReadUint16(data);
	// Death Force Expects non-zero read from $FD01 on reset.
	state.lastKeyCode=ReadUint16(data);
	{
		decltype(state.keyCodeQueue) empty;
		std::swap(state.keyCodeQueue,empty);
		uint32_t n=ReadUint32(data);
		for(int i=0; i<n; ++i)
		{
			state.keyCodeQueue.push(ReadUint16(data));
		}
	}

	state.encoderAcknowledgeBy=ReadUint64(data);
	state.encoderDataReadyBy=ReadUint64(data);
	state.encoderCmd=ReadUint16(data);
	state.nEncoderParam=ReadUint16(data);
	for(int i=0; i<8; ++i)
	{
		state.encoderParam[i]=ReadUint16(data);
	}
	{
		decltype(state.encoderQueue) empty;
		std::swap(state.encoderQueue,empty);
		uint32_t n=ReadUint32(data);
		for(int i=0; i<n; ++i)
		{
			state.encoderQueue.push(ReadUint16(data));
		}
	}

	state.CAPS=ReadBool(data);
	state.KANA=ReadBool(data);
	state.INSLED=ReadBool(data);
	state.videoCaptureMode=ReadUint16(data);
	state.videoCaptureBrightness=ReadUint16(data);
	state.keyRepeat=ReadBool(data);
	state.keyRepeatStartTime=ReadUint64(data);
	state.keyRepeatInterval=ReadUint64(data);

	for(auto &b : heldDown)
	{
		b=false;
	}

	return true;
}

/* static */ std::string FM77AVKeyboard::AutoStopToStr(unsigned int autoStopType)
{
	switch(autoStopType)
	{
	case AUTOSTOP_NONE:
		return "NONE";
	case AUTOSTOP_AFTER_NUM_RELEASE:
		return "AFTER_NUM_RELEASE";
	case AUTOSTOP_AFTER_ARROW_RELEASE:
		return "AFTER_ARROW_RELEASE";
	case AUTOSTOP_AFTER_NUM_RELEASE_AND_RETYPE:
		return "AFTER_NUM_RELEASE_AND_RETYPE";
	case AUTOSTOP_AFTER_ARROW_RELEASE_AND_RETYPE:
		return "AFTER_ARROW_RELEASE_AND_RETYPE";
	case AUTOSTOP_AFTER_ANY_KEY_RELEASE:
		return "AFTER_ANY_KEY_RELEASE";
	}
	return "NONE";
}
/* static */ unsigned int FM77AVKeyboard::StrToAutoStop(std::string str)
{
	auto STR=str;
	cpputil::Capitalize(STR);

	if(STR=="NONE")
	{
		return AUTOSTOP_NONE;
	}
	if(STR=="AFTER_NUM_RELEASE")
	{
		return AUTOSTOP_AFTER_NUM_RELEASE;
	}
	if(STR=="AFTER_ARROW_RELEASE")
	{
		return AUTOSTOP_AFTER_ARROW_RELEASE;
	}
	if(STR=="AFTER_NUM_RELEASE_AND_RETYPE")
	{
		return AUTOSTOP_AFTER_NUM_RELEASE_AND_RETYPE;
	}
	if(STR=="AFTER_ARROW_RELEASE_AND_RETYPE")
	{
		return AUTOSTOP_AFTER_ARROW_RELEASE_AND_RETYPE;
	}
	if(STR=="AFTER_ANY_KEY_RELEASE")
	{
		return AUTOSTOP_AFTER_ANY_KEY_RELEASE;
	}
	return AUTOSTOP_NONE;

}
