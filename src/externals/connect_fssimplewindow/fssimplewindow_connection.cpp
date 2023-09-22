/* LICENSE>>
Copyright 2020 Soji Yamakawa (CaptainYS, http://www.ysflight.com)

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

<< LICENSE */
#include <stdio.h>
#include <iostream>

#include "fssimplewindow.h"
#include "yssimplesound.h"
#include "fssimplewindow_connection.h"
#include "ysglfontdata.h"

// G** D*** Windows headers! >>
#ifdef REG_NONE
#undef REG_NONE
#endif
#ifdef OUT
#undef OUT
#endif
// *od *amn Windows headers! <<

#include "cpputil.h"
#include "fm77av.h"
#include "icons.h"
#include "ysgamepad.h"
#include "fm77avparam.h"
#include "fm77avkey.h"
#include "fm77avkeyboard.h"
#include "ym2612.h"


FsSimpleWindowConnection::FsSimpleWindowConnection()
{
	FSKEYtoFM77AVKEY=new unsigned int [FSKEY_NUM_KEYCODE];
	FSKEYState=new unsigned int [FSKEY_NUM_KEYCODE];

	SetKeyboardMode(FM77AV_KEYBOARD_MODE_DIRECT);
	SetKeyboardLayout(KEYBOARD_LAYOUT_US);

	for(int i=0; i<FSKEY_NUM_KEYCODE; ++i)
	{
		FSKEYState[i]=0;
	}

	if(true!=gamePadInitialized)
	{
		YsGamePadInitialize();
		gamePadInitialized=true;
	}

	auto nGameDevs=YsGamePadGetNumDevices();
	if(0<nGameDevs)
	{
		gamePads.resize(nGameDevs);
		prevGamePads.resize(nGameDevs);
		for(unsigned int i=0; i<nGameDevs; ++i)
		{
			YsGamePadRead(&gamePads[i],i);
			prevGamePads[i]=gamePads[i];
		}
	}
}
FsSimpleWindowConnection::~FsSimpleWindowConnection()
{
	delete [] FSKEYtoFM77AVKEY;
	delete [] FSKEYState;
}

/* virtual */ std::vector <std::string> FsSimpleWindowConnection::MakeKeyMappingText(void) const
{
	std::vector <std::string> text;
	for(int i=0; i<FSKEY_NUM_KEYCODE; ++i)
	{
		text.push_back("");
		text.back()+=FsKeyCodeToString(i);
		text.back()+=" ";
		while(text.back().size()<24)
		{
			text.back()+=" ";
		}
		text.back()+=FM77AVKeyCodeToKeyLabel(FSKEYtoFM77AVKEY[i]);
	}
	return text;
}
/* virtual */ void FsSimpleWindowConnection::LoadKeyMappingFromText(const std::vector <std::string> &text)
{
	for(int i=0; i<FSKEY_NUM_KEYCODE; ++i)
	{
		FSKEYtoFM77AVKEY[i]=AVKEY_NULL;
	}
	for(auto str : text)
	{
		auto argv=cpputil::Parser(str.c_str());
		if(2==argv.size())
		{
			auto fsKey=FsStringToKeyCode(argv[0].c_str());
			auto FM77AVKey=FM77AVKeyLabelToKeyCode(argv[1]);
			FSKEYtoFM77AVKEY[fsKey]=FM77AVKey;
		}
	}
}

static std::vector <unsigned char> MakeIcon(const unsigned char src[],int wid,int hei)
{
	std::vector <unsigned char> rgba;
	rgba.resize(4*wid*hei);
	for(int y=0; y<hei; ++y)
	{
		int Y=hei-1-y;
		for(int x=0; x<wid; ++x)
		{
			rgba[(y*wid+x)*4  ]=src[(Y*wid+x)*4  ];
			rgba[(y*wid+x)*4+1]=src[(Y*wid+x)*4+1];
			rgba[(y*wid+x)*4+2]=src[(Y*wid+x)*4+2];
			rgba[(y*wid+x)*4+3]=src[(Y*wid+x)*4+3];
		}
	}
	return rgba;
}

/* virtual */ void FsSimpleWindowConnection::Start(void)
{
}
/* virtual */ void FsSimpleWindowConnection::Stop(void)
{
}

/* virtual */ void FsSimpleWindowConnection::DevicePolling(class FM77AV &fm77av)
{
	FsPollDevice();
	bool ctrlKey=(0!=FsGetKeyState(FSKEY_CTRL));
	bool shiftKey=(0!=FsGetKeyState(FSKEY_SHIFT));
	bool graphKey=(0!=FsGetKeyState(GRAPH_KEY_CODE));

	uint16_t keyFlags=
	    (ctrlKey==true ? FM77AVKeyboard::KEYFLAG_CTRL : 0)|
	    (shiftKey==true ? FM77AVKeyboard::KEYFLAG_SHIFT : 0)|
	    (graphKey==true ? FM77AVKeyboard::KEYFLAG_GRAPH : 0);

	PollGamePads();

	int lb,mb,rb,mx,my;
	auto mosEvt=FsGetMouseEvent(lb,mb,rb,mx,my);
	if(LOWER_RIGHT_NONE!=lowerRightIcon && FSMOUSEEVENT_LBUTTONDOWN==mosEvt)
	{
		int wid,hei;
		FsGetWindowSize(wid,hei);

		int iconWid=0;
		int iconHei=0;
		switch(lowerRightIcon)
		{
		case LOWER_RIGHT_NONE:
			break;
		case LOWER_RIGHT_PAUSE:
			iconWid=PAUSE_wid;
			iconHei=PAUSE_hei;
			break;
		case LOWER_RIGHT_MENU:
			iconWid=MENU_wid;
			iconHei=MENU_hei;
			break;
		}
		if(wid-iconWid<mx && hei-iconHei<my)
		{
			this->pauseKey=true;
		}
	}


	bool gamePadEmulationByKey=false; // Emulate a gamepad with keyboard
	bool mouseEmulationByNumPad=false; // Emulate mouse with keyboard numpad
	for(unsigned int portId=0; portId<FM77AV_NUM_GAMEPORTS; ++portId)
	{
		if(FM77AV_GAMEPORTEMU_KEYBOARD==gamePort[portId] ||
		   FM77AV_GAMEPORTEMU_MOUSE_BY_KEY==gamePort[portId])
		{
			gamePadEmulationByKey=true;
		}
		if(FM77AV_GAMEPORTEMU_MOUSE_BY_NUMPAD==gamePort[portId])
		{
			mouseEmulationByNumPad=true;
		}
	}

	for(auto vk : virtualKeys)
	{
		if(0<=vk.physicalId && vk.physicalId<gamePads.size())
		{
			if(prevGamePads[vk.physicalId].buttons[vk.button]!=gamePads[vk.physicalId].buttons[vk.button])
			{
				if(0!=gamePads[vk.physicalId].buttons[vk.button])
				{
					fm77av.keyboard.Press(keyFlags,vk.fm77avKey);
				}
				else
				{
					fm77av.keyboard.Release(keyFlags,vk.fm77avKey);
				}
			}
		}
	}

	if(FM77AV_KEYBOARD_MODE_DIRECT!=keyboardMode) // Means one of the translation modes.
	{
		unsigned int c;
		while(0!=(c=FsInkeyChar()))
		{
			if(0==FsGetKeyState(FSKEY_CTRL))
			{
				if(' '<=c)
				{
					auto keyComb=FM77AVTranslateCharToCode(c);
					if(AVKEY_NULL!=keyComb.keyCode)
					{
						uint16_t keyFlags=
						    (keyComb.ctrl==true ? FM77AVKeyboard::KEYFLAG_CTRL : 0)|
						    (keyComb.shift==true ? FM77AVKeyboard::KEYFLAG_SHIFT : 0)|
						    (keyComb.graph==true ? FM77AVKeyboard::KEYFLAG_GRAPH : 0);
						fm77av.keyboard.Press(keyFlags,keyComb.keyCode);
						fm77av.keyboard.Release(keyFlags,keyComb.keyCode);
					}
				}
			}
		}
		while(0!=(c=FsInkey()))
		{
			if(PAUSE_KEY_CODE==c)
			{
				this->pauseKey=true;
			}
			if(hostShortCut[c].inUse && hostShortCut[c].ctrl==ctrlKey && hostShortCut[c].shift==shiftKey)
			{
				this->commandQueue.push(hostShortCut[c].cmdStr);
				continue;
			}

			this->ProcessInkey(fm77av,FSKEYtoFM77AVKEY[c]);
			switch(c)
			{
			default:
				// CTRL+C, CTRL+S, CTRL+Q...
				if(ctrlKey && FSKEY_A<=c && c<=FSKEY_Z)
				{
					// Can take Ctrl+? and Ctrl+Shift+?, but Shift+? is taken by FsInkeyChar() already.
					// Therefore this block should only process only if Ctrl key is held down.
					fm77av.keyboard.Press(keyFlags,FSKEYtoFM77AVKEY[c]);
					fm77av.keyboard.Release(keyFlags,FSKEYtoFM77AVKEY[c]);
				}
				break;
			case FSKEY_ESC:
				if(FM77AV_KEYBOARD_MODE_TRANSLATION1==keyboardMode)
				{
					fm77av.keyboard.Press(keyFlags,  AVKEY_BREAK);
					fm77av.keyboard.Release(keyFlags,AVKEY_BREAK);
					fm77av.keyboard.Press(keyFlags,  AVKEY_ESC);
					fm77av.keyboard.Release(keyFlags,AVKEY_ESC);
				}
				else if(FM77AV_KEYBOARD_MODE_TRANSLATION2==keyboardMode)
				{
					fm77av.keyboard.Press(keyFlags,  AVKEY_ESC);
					fm77av.keyboard.Release(keyFlags,AVKEY_ESC);
				}
				else if(FM77AV_KEYBOARD_MODE_TRANSLATION3==keyboardMode)
				{
					fm77av.keyboard.Press(keyFlags,  AVKEY_BREAK);
					fm77av.keyboard.Release(keyFlags,AVKEY_BREAK);
				}
				break;
			case FSKEY_ENTER:
			case FSKEY_BS:
			case FSKEY_TAB:
			case FSKEY_HOME:
			case FSKEY_END:
			case FSKEY_PAGEUP:
			case FSKEY_PAGEDOWN:
			case FSKEY_NUMLOCK:
			case FSKEY_ALT:
			case FSKEY_INS:
			case FSKEY_DEL:
			case FSKEY_F1:
			case FSKEY_F2:
			case FSKEY_F3:
			case FSKEY_F4:
			case FSKEY_F5:
			case FSKEY_F6:
			case FSKEY_F7:
			case FSKEY_F8:
			case FSKEY_F9:
			case FSKEY_F10:
			case FSKEY_F11:
			case FSKEY_F12:
			case FSKEY_CAPSLOCK:
			case FSKEY_CONVERT:
			case FSKEY_NONCONVERT:
			case FSKEY_KANA:       // Japanese JIS Keyboard Only => Win32 VK_KANA
			case FSKEY_RO:         // Japanese JIS Keyboard Only => Win32 VK_OEM_102
			case FSKEY_ZENKAKU:    // Japanese JIS Keyboard Only => Full Pitch/Half Pitch
			case FSKEY_WHEELUP:
			case FSKEY_WHEELDOWN:
			case FSKEY_CONTEXT:
			case FSKEY_UP:
			case FSKEY_DOWN:
			case FSKEY_LEFT:
			case FSKEY_RIGHT:
				fm77av.keyboard.Press(keyFlags,  FSKEYtoFM77AVKEY[c]);
				fm77av.keyboard.Release(keyFlags,FSKEYtoFM77AVKEY[c]);
				break;
			}
		}
	}
	else // if(FM77AV_KEYBOARD_MODE_DIRECT==keyboardMode)
	{
		unsigned int c;
		while(0!=(c=FsInkey()))
		{
			unsigned char byteData=0;
			this->ProcessInkey(fm77av,FSKEYtoFM77AVKEY[c]);
			if(PAUSE_KEY_CODE==c)
			{
				this->pauseKey=true;
			}
			if(hostShortCut[c].inUse && hostShortCut[c].ctrl==ctrlKey && hostShortCut[c].shift==shiftKey)
			{
				this->commandQueue.push(hostShortCut[c].cmdStr);
				continue;
			}

			if(AVKEY_NULL!=FSKEYtoFM77AVKEY[c])
			{
				if(true==gamePadEmulationByKey &&
				   (FSKEY_Z==c ||
				    FSKEY_X==c ||
				    FSKEY_A==c ||
				    FSKEY_S==c ||
				    FSKEY_LEFT==c ||
				    FSKEY_RIGHT==c ||
				    FSKEY_UP==c ||
				    FSKEY_DOWN==c))
				{
					break;
				}
				if(true==mouseEmulationByNumPad &&
				   (FSKEY_TEN0==c ||
				    FSKEY_TEN1==c ||
				    FSKEY_TEN2==c ||
				    FSKEY_TEN3==c ||
				    FSKEY_TEN4==c ||
				    FSKEY_TEN5==c ||
				    FSKEY_TEN6==c ||
				    FSKEY_TEN7==c ||
				    FSKEY_TEN8==c ||
				    FSKEY_TEN9==c ||
				    FSKEY_TENSTAR==c ||
				    FSKEY_TENSLASH==c))
				{
					break;
				}
			}
		}
		for(int key=FSKEY_NULL; key<FSKEY_NUM_KEYCODE; ++key)
		{
			if(FSKEY_SHIFT==key || FSKEY_CTRL==key || FSKEY_ALT==key)
			{
				// Should check FSKEY_LEFT_* and FSKEY_RIGHT_* instead.
				continue;
			}

			if(true==gamePadEmulationByKey &&
			   (FSKEY_Z==key ||
			    FSKEY_X==key ||
			    FSKEY_A==key ||
			    FSKEY_S==key ||
			    FSKEY_LEFT==key ||
			    FSKEY_RIGHT==key ||
			    FSKEY_UP==key ||
			    FSKEY_DOWN==key))
			{
				continue;
			}
			if(true==mouseEmulationByNumPad &&
			   (FSKEY_TEN0==key ||
			    FSKEY_TEN1==key ||
			    FSKEY_TEN2==key ||
			    FSKEY_TEN3==key ||
			    FSKEY_TEN4==key ||
			    FSKEY_TEN5==key ||
			    FSKEY_TEN6==key ||
			    FSKEY_TEN7==key ||
			    FSKEY_TEN8==key ||
			    FSKEY_TEN9==key ||
			    FSKEY_TENSTAR==key ||
			    FSKEY_TENSLASH==key))
			{
				continue;
			}

			unsigned char byteData=0;
			auto sta=FsGetKeyState(key);
			if(0!=FSKEYtoFM77AVKEY[key] && FSKEYState[key]!=sta)
			{
				if(0!=sta)
				{
					nextKeyRepeatTime=fm77av.state.fm77avTime+fm77av.keyboard.GetKeyRepeatStartTime();
					fm77av.keyboard.Press(KeyFlagsFilter(keyFlags,key),FSKEYtoFM77AVKEY[key]);
					lastPressedFsKey=key;
				}
				else
				{
					fm77av.keyboard.Release(KeyFlagsFilter(keyFlags,key),FSKEYtoFM77AVKEY[key]);
				}
				FSKEYState[key]=sta;
			}
		}

		if(0!=lastPressedFsKey &&
		   0!=FSKEYState[lastPressedFsKey] &&
		   nextKeyRepeatTime<fm77av.state.fm77avTime &&
		   true==fm77av.keyboard.state.keyRepeat)
		{
			fm77av.keyboard.Press(KeyFlagsFilter(keyFlags,lastPressedFsKey),FSKEYtoFM77AVKEY[lastPressedFsKey]);
			nextKeyRepeatTime=fm77av.state.fm77avTime+fm77av.keyboard.GetKeyRepeatInterval();
		}

		while(0!=FsInkeyChar())
		{
		}
	}

 	if(fm77av.eventLog.state!=FM77AVEventLog::STATE_PLAYBACK)
 	{
 		bool mouseEmulationByAnalogAxis=false;
 		for(unsigned int portId=0; portId<FM77AV_NUM_GAMEPORTS; ++portId)
 		{
 			switch(gamePort[portId])
 			{
 			default:
 				break;
 			case FM77AV_GAMEPORTEMU_KEYBOARD:
 				{
 					bool Abutton=(0!=FsGetKeyState(FSKEY_Z));
 					bool Bbutton=(0!=FsGetKeyState(FSKEY_X));
 					bool run=(0!=FsGetKeyState(FSKEY_A));
 					bool pause=(0!=FsGetKeyState(FSKEY_S));
 					bool left=(0!=FsGetKeyState(FSKEY_LEFT));
 					bool right=(0!=FsGetKeyState(FSKEY_RIGHT));
 					if(true==left && true==right)
 					{
 						right=false;
 					}
 					bool up=(0!=FsGetKeyState(FSKEY_UP));
 					bool down=(0!=FsGetKeyState(FSKEY_DOWN));
 					if(true==up && true==down)
 					{
 						down=false;
 					}
 					fm77av.SetGamePadState(portId,Abutton,Bbutton,left,right,up,down,run,pause);
 				}
 				break;
 			case FM77AV_GAMEPORTEMU_PHYSICAL0:
 			case FM77AV_GAMEPORTEMU_PHYSICAL1:
 			case FM77AV_GAMEPORTEMU_PHYSICAL2:
 			case FM77AV_GAMEPORTEMU_PHYSICAL3:
 			case FM77AV_GAMEPORTEMU_PHYSICAL4:
 			case FM77AV_GAMEPORTEMU_PHYSICAL5:
 			case FM77AV_GAMEPORTEMU_PHYSICAL6:
 			case FM77AV_GAMEPORTEMU_PHYSICAL7:
 				if(true!=gamePadInitialized)
 				{
 					YsGamePadInitialize();
 					gamePadInitialized=true;
 				}
 				{
 					int padId=gamePort[portId]-FM77AV_GAMEPORTEMU_PHYSICAL0;
 					if(0<=padId && padId<gamePads.size())
 					{
 						auto &reading=gamePads[padId];
 						fm77av.SetGamePadState(
 						    portId,
 						    reading.buttons[0],
 						    reading.buttons[1],
 						    reading.dirs[0].upDownLeftRight[2],
 						    reading.dirs[0].upDownLeftRight[3],
 						    reading.dirs[0].upDownLeftRight[0],
 						    reading.dirs[0].upDownLeftRight[1],
 						    reading.buttons[2],
 						    reading.buttons[3]);
 					}
 				}
 				break;
// 			case FM77AV_GAMEPORTEMU_ANALOG0:
// 			case FM77AV_GAMEPORTEMU_ANALOG1:
// 			case FM77AV_GAMEPORTEMU_ANALOG2:
// 			case FM77AV_GAMEPORTEMU_ANALOG3:
// 			case FM77AV_GAMEPORTEMU_ANALOG4:
// 			case FM77AV_GAMEPORTEMU_ANALOG5:
// 			case FM77AV_GAMEPORTEMU_ANALOG6:
// 			case FM77AV_GAMEPORTEMU_ANALOG7:
// 				if(true!=gamePadInitialized)
// 				{
// 					YsGamePadInitialize();
// 					gamePadInitialized=true;
// 				}
// 				{
// 					int padId=gamePort[portId]-FM77AV_GAMEPORTEMU_ANALOG0;
// 					if(0<=padId && padId<gamePads.size())
// 					{
// 						auto &reading=gamePads[padId];
// 						YsGamdPadTranslateAnalogToDigital(&reading.dirs[0],reading.axes[0],reading.axes[1]);
// 						fm77av.SetGamePadState(
// 						    portId,
// 						    reading.buttons[0],
// 						    reading.buttons[1],
// 						    reading.dirs[0].upDownLeftRight[2],
// 						    reading.dirs[0].upDownLeftRight[3],
// 						    reading.dirs[0].upDownLeftRight[0],
// 						    reading.dirs[0].upDownLeftRight[1],
// 						    reading.buttons[2],
// 						    reading.buttons[3]);
// 					}
// 				}
// 				break;
// 
// 			case FM77AV_GAMEPORTEMU_CAPCOM_BY_PHYSICAL0:
// 			case FM77AV_GAMEPORTEMU_CAPCOM_BY_PHYSICAL1:
// 			case FM77AV_GAMEPORTEMU_CAPCOM_BY_PHYSICAL2:
// 			case FM77AV_GAMEPORTEMU_CAPCOM_BY_PHYSICAL3:
// 			case FM77AV_GAMEPORTEMU_CAPCOM_BY_PHYSICAL4:
// 			case FM77AV_GAMEPORTEMU_CAPCOM_BY_PHYSICAL5:
// 			case FM77AV_GAMEPORTEMU_CAPCOM_BY_PHYSICAL6:
// 			case FM77AV_GAMEPORTEMU_CAPCOM_BY_PHYSICAL7:
// 				if(true!=gamePadInitialized)
// 				{
// 					YsGamePadInitialize();
// 					gamePadInitialized=true;
// 				}
// 				{
// 					int padId=gamePort[portId]-FM77AV_GAMEPORTEMU_CAPCOM_BY_PHYSICAL0;
// 					if(0<=padId && padId<gamePads.size())
// 					{
// 						auto &reading=gamePads[padId];
// 						bool up=reading.dirs[0].upDownLeftRight[2];
// 						bool dn=reading.dirs[0].upDownLeftRight[3];
// 						bool lf=reading.dirs[0].upDownLeftRight[0];
// 						bool ri=reading.dirs[0].upDownLeftRight[1];
// 
// 						// Muscle Bomber cannot start without pressing START button.
// 						// So, I use physical buttons 8 and 9 for START/SELECT.
// 						if(reading.buttons[8])
// 						{
// 							up=true;
// 							dn=true;
// 						}
// 						if(reading.buttons[9])
// 						{
// 							lf=true;
// 							ri=true;
// 						}
// 
// 						fm77av.SetCAPCOMCPSFState(
// 						    portId,
// 						    up,
// 						    dn,
// 						    lf,
// 						    ri,
// 						    reading.buttons[0],
// 						    reading.buttons[1],
// 						    reading.buttons[2],
// 						    reading.buttons[3],
// 						    reading.buttons[4],
// 						    reading.buttons[5],
// 						    reading.buttons[6],
// 						    reading.buttons[7]);
// 					}
// 				}
// 				break;
// 
// 			case FM77AV_GAMEPORTEMU_MOUSE_BY_KEY:
// 			case FM77AV_GAMEPORTEMU_MOUSE_BY_NUMPAD:
// 			case FM77AV_GAMEPORTEMU_MOUSE_BY_PHYSICAL0:
// 			case FM77AV_GAMEPORTEMU_MOUSE_BY_PHYSICAL1:
// 			case FM77AV_GAMEPORTEMU_MOUSE_BY_PHYSICAL2:
// 			case FM77AV_GAMEPORTEMU_MOUSE_BY_PHYSICAL3:
// 			case FM77AV_GAMEPORTEMU_MOUSE_BY_PHYSICAL4:
// 			case FM77AV_GAMEPORTEMU_MOUSE_BY_PHYSICAL5:
// 			case FM77AV_GAMEPORTEMU_MOUSE_BY_PHYSICAL6:
// 			case FM77AV_GAMEPORTEMU_MOUSE_BY_PHYSICAL7:
// 				{
// 					if(FM77AV_GAMEPORTEMU_MOUSE_BY_KEY!=gamePort[portId] &&
// 					   FM77AV_GAMEPORTEMU_MOUSE_BY_NUMPAD!=gamePort[portId] &&
// 					   true!=gamePadInitialized)
// 					{
// 						YsGamePadInitialize();
// 						gamePadInitialized=true;
// 					}
// 					{
// 						const int accel=1;
// 						const int maxSpeed=80;
// 						const int div=20;
// 
// 						bool upDownLeftRight[4]={false,false,false,false};
// 						bool button[2]={false,false};
// 
// 						mouseEmulationByAnalogAxis=true;
// 						if(FM77AV_GAMEPORTEMU_MOUSE_BY_KEY==gamePort[portId])
// 						{
// 							upDownLeftRight[0]=(0!=FsGetKeyState(FSKEY_UP));
// 							upDownLeftRight[1]=(0!=FsGetKeyState(FSKEY_DOWN));
// 							upDownLeftRight[2]=(0!=FsGetKeyState(FSKEY_LEFT));
// 							upDownLeftRight[3]=(0!=FsGetKeyState(FSKEY_RIGHT));
// 							button[0]=(0!=FsGetKeyState(FSKEY_Z));
// 							button[1]=(0!=FsGetKeyState(FSKEY_X));
// 						}
// 						else if(FM77AV_GAMEPORTEMU_MOUSE_BY_NUMPAD==gamePort[portId])
// 						{
// 							upDownLeftRight[0]=(0!=FsGetKeyState(FSKEY_TEN7) || 0!=FsGetKeyState(FSKEY_TEN8) || 0!=FsGetKeyState(FSKEY_TEN9));
// 							upDownLeftRight[1]=(0!=FsGetKeyState(FSKEY_TEN1) || 0!=FsGetKeyState(FSKEY_TEN2) || 0!=FsGetKeyState(FSKEY_TEN3));
// 							upDownLeftRight[2]=(0!=FsGetKeyState(FSKEY_TEN1) || 0!=FsGetKeyState(FSKEY_TEN4) || 0!=FsGetKeyState(FSKEY_TEN7));
// 							upDownLeftRight[3]=(0!=FsGetKeyState(FSKEY_TEN3) || 0!=FsGetKeyState(FSKEY_TEN6) || 0!=FsGetKeyState(FSKEY_TEN9));
// 							button[0]=(0!=FsGetKeyState(FSKEY_TENSLASH));
// 							button[1]=(0!=FsGetKeyState(FSKEY_TENSTAR));
// 						}
// 						else
// 						{
// 							int padId=gamePort[portId]-FM77AV_GAMEPORTEMU_MOUSE_BY_PHYSICAL0;
// 							if(0<=padId && padId<gamePads.size())
// 							{
// 								const auto &reading=gamePads[padId];
// 								upDownLeftRight[0]=(0!=reading.dirs[0].upDownLeftRight[0]);
// 								upDownLeftRight[1]=(0!=reading.dirs[0].upDownLeftRight[1]);
// 								upDownLeftRight[2]=(0!=reading.dirs[0].upDownLeftRight[2]);
// 								upDownLeftRight[3]=(0!=reading.dirs[0].upDownLeftRight[3]);
// 								button[0]=(0!=reading.buttons[0]);
// 								button[1]=(0!=reading.buttons[1]);
// 							}
// 						}
// 						if(true==upDownLeftRight[0])
// 						{
// 							mouseDY+=accel;
// 						}
// 						else if(true==upDownLeftRight[1])
// 						{
// 							mouseDY-=accel;
// 						}
// 						else
// 						{
// 							mouseDY=0;
// 						}
// 						if(mouseDY<-maxSpeed)
// 						{
// 							mouseDY=-maxSpeed;
// 						}
// 						if(mouseDY>maxSpeed)
// 						{
// 							mouseDY=maxSpeed;
// 						}
// 						if(0!=true==upDownLeftRight[2])
// 						{
// 							mouseDX+=accel;
// 						}
// 						else if(0!=true==upDownLeftRight[3])
// 						{
// 							mouseDX-=accel;
// 						}
// 						else
// 						{
// 							mouseDX=0;
// 						}
// 						if(mouseDX<-maxSpeed)
// 						{
// 							mouseDX=-maxSpeed;
// 						}
// 						if(mouseDX>maxSpeed)
// 						{
// 							mouseDX=maxSpeed;
// 						}
// 						fm77av.SetMouseMotion(portId,mouseDX/div,mouseDY/div);
// 						fm77av.SetMouseButtonState(button[0],button[1]);
// 					}
// 				}
// 				break;
// 			case FM77AV_GAMEPORTEMU_MOUSE_BY_ANALOG0:
// 			case FM77AV_GAMEPORTEMU_MOUSE_BY_ANALOG1:
// 			case FM77AV_GAMEPORTEMU_MOUSE_BY_ANALOG2:
// 			case FM77AV_GAMEPORTEMU_MOUSE_BY_ANALOG3:
// 			case FM77AV_GAMEPORTEMU_MOUSE_BY_ANALOG4:
// 			case FM77AV_GAMEPORTEMU_MOUSE_BY_ANALOG5:
// 			case FM77AV_GAMEPORTEMU_MOUSE_BY_ANALOG6:
// 			case FM77AV_GAMEPORTEMU_MOUSE_BY_ANALOG7:
// 				{
// 					if(true!=gamePadInitialized)
// 					{
// 						YsGamePadInitialize();
// 						gamePadInitialized=true;
// 					}
// 					{
// 						const double maxSpeed=20.0;
// 
// 						mouseEmulationByAnalogAxis=true;
// 						int padId=gamePort[portId]-FM77AV_GAMEPORTEMU_MOUSE_BY_ANALOG0;
// 						if(0<=padId && padId<gamePads.size())
// 						{
// 							const auto &reading=gamePads[padId];
// 							float dx=reading.axes[0]*maxSpeed;
// 							float dy=reading.axes[1]*maxSpeed;
// 							fm77av.SetMouseMotion(portId,-dx,-dy);
// 							fm77av.SetMouseButtonState(0!=reading.buttons[0],0!=reading.buttons[1]);
// 						}
// 					}
// 				}
// 				break;
// 
// 			case FM77AV_GAMEPORTEMU_PHYSICAL0_AS_CYBERSTICK:
// 			case FM77AV_GAMEPORTEMU_PHYSICAL1_AS_CYBERSTICK:
// 			case FM77AV_GAMEPORTEMU_PHYSICAL2_AS_CYBERSTICK:
// 			case FM77AV_GAMEPORTEMU_PHYSICAL3_AS_CYBERSTICK:
// 			case FM77AV_GAMEPORTEMU_PHYSICAL4_AS_CYBERSTICK:
// 			case FM77AV_GAMEPORTEMU_PHYSICAL5_AS_CYBERSTICK:
// 			case FM77AV_GAMEPORTEMU_PHYSICAL6_AS_CYBERSTICK:
// 			case FM77AV_GAMEPORTEMU_PHYSICAL7_AS_CYBERSTICK:
// 				{
// 					auto physId=gamePort[portId]-FM77AV_GAMEPORTEMU_PHYSICAL0_AS_CYBERSTICK;
// 					if(0<=physId && physId<gamePads.size())
// 					{
// 						auto axisReading=gamePads[physId];
// 
// 						float x=axisReading.axes[0];
// 						float y=axisReading.axes[1];
// 						float z=axisReading.axes[2];
// 						float w=axisReading.axes[3];
// 						int ix=x*127.0;
// 						int iy=y*127.0;
// 						int iz=z*127.0;
// 						int iw=w*127.0;
// 						ix=cpputil::Clamp(ix,-128,127);
// 						iy=cpputil::Clamp(iy,-128,127);
// 						iz=cpputil::Clamp(iz,-128,127);
// 						iw=cpputil::Clamp(iw,-128,127);
// 
// 						unsigned int trig=0;
// 						trig|=(axisReading.buttons[0] ? 0x01 : 0);
// 						trig|=(axisReading.buttons[1] ? 0x02 : 0);
// 						trig|=(axisReading.buttons[2] ? 0x04 : 0);
// 						trig|=(axisReading.buttons[3] ? 0x08 : 0);
// 						trig|=(axisReading.buttons[4] ? 0x10 : 0);
// 						trig|=(axisReading.buttons[5] ? 0x20 : 0);
// 						trig|=(axisReading.buttons[6] ? 0x40 : 0);
// 						trig|=(axisReading.buttons[7] ? 0x80 : 0);
// 						trig|=(axisReading.buttons[8] ? 0x100 : 0);
// 						trig|=(axisReading.buttons[9] ? 0x200 : 0);
// 						trig|=(axisReading.buttons[10] ? 0x400 : 0);
// 						trig|=(axisReading.buttons[11] ? 0x800 : 0);
// 						fm77av.SetCyberStickState(portId,ix,iy,iz,iw,trig);
// 					}
// 				}
// 				break;
// 
// 			case FM77AV_GAMEPORTEMU_CYBERSTICK:
// 				if(true==cyberStickAssignment && 0<=mouseByFlightstickPhysicalId && mouseByFlightstickPhysicalId<gamePads.size())
// 				{
// 					auto axisReading=gamePads[mouseByFlightstickPhysicalId];
// 					decltype(axisReading) throttleReading;
// 					float z=0;
// 					if(0<=throttlePhysicalId && throttlePhysicalId<gamePads.size())
// 					{
// 						throttleReading=gamePads[throttlePhysicalId];
// 						z=gamePads[throttlePhysicalId].axes[throttleAxis];
// 					}
// 					else
// 					{
// 						throttleReading=axisReading;
// 						z=gamePads[throttlePhysicalId].axes[2];
// 					}
// 
// 					float x=axisReading.axes[0];
// 					float y=axisReading.axes[1];
// 					int ix=x*127.0;
// 					int iy=y*127.0;
// 					int iz=z*127.0;
// 					int iw=0;
// 					ix=cpputil::Clamp(ix,-128,127);
// 					iy=cpputil::Clamp(iy,-128,127);
// 					iz=cpputil::Clamp(iz,-128,127);
// 
// 					unsigned int trig=0;
// 					trig|=((axisReading.buttons[0] || throttleReading.buttons[0]) ? 0x01 : 0);
// 					trig|=((axisReading.buttons[1] || throttleReading.buttons[1]) ? 0x02 : 0);
// 					trig|=((axisReading.buttons[2] || throttleReading.buttons[2]) ? 0x04 : 0);
// 					trig|=((axisReading.buttons[3] || throttleReading.buttons[3]) ? 0x08 : 0);
// 					trig|=((axisReading.buttons[4] || throttleReading.buttons[4]) ? 0x10 : 0);
// 					trig|=((axisReading.buttons[5] || throttleReading.buttons[5]) ? 0x20 : 0);
// 					trig|=((axisReading.buttons[6] || throttleReading.buttons[6]) ? 0x40 : 0);
// 					trig|=((axisReading.buttons[7] || throttleReading.buttons[7]) ? 0x80 : 0);
// 					trig|=((axisReading.buttons[8] || throttleReading.buttons[8]) ? 0x100 : 0);
// 					trig|=((axisReading.buttons[9] || throttleReading.buttons[9]) ? 0x200 : 0);
// 					trig|=((axisReading.buttons[10] || throttleReading.buttons[10]) ? 0x400 : 0);
// 					trig|=((axisReading.buttons[11] || throttleReading.buttons[11]) ? 0x800 : 0);
// 					fm77av.SetCyberStickState(portId,ix,iy,iz,iw,trig);
// 				}
// 				break;
 			}
 		}

//		if(mouseEmulationByAnalogAxis!=true)
		{
			struct YsGamePadReading reading;
			mx-=this->dx;
			my-=this->dy;
			{
				int wid,hei;
				FsGetWindowSize(wid,hei);
				if(mx<0)
				{
					mx=0;
				}
				else if(wid<=mx)
				{
					mx=wid-1;
				}
				if(my<0)
				{
					my=0;
				}
				else if(hei<=my)
				{
					my=hei-1;
				}
				if(0!=scaling) // Just in case
				{
					mx=mx*100/scaling;
					my=my*100/scaling;
				}
				this->ProcessMouse(fm77av,lb,mb,rb,mx,my);
			}
		}
	} // if(fm77av.eventLog.mode!=FM77AVEventLog::MODE_PLAYBACK)
}
unsigned int FsSimpleWindowConnection::KeyFlagsFilter(unsigned int keyFlags,unsigned int fsKey)
{
	if(FSKEY_LEFT_SHIFT==fsKey || FSKEY_RIGHT_SHIFT==fsKey)
	{
		return keyFlags&~FM77AVKeyboard::KEYFLAG_SHIFT;
	}
	if(FSKEY_LEFT_CTRL==fsKey || FSKEY_RIGHT_CTRL==fsKey)
	{
		return keyFlags&~FM77AVKeyboard::KEYFLAG_CTRL;
	}
	return keyFlags;
}
void FsSimpleWindowConnection::PollGamePads(void)
{
	if(true!=Outside_World::gameDevsNeedUpdateCached)
	{
		std::cout << "Squawk!  Game Devices that need updates not cached!" << std::endl;
	}
	for(auto padId : Outside_World::gamePadsNeedUpdate)
	{
		if(padId<gamePads.size())
		{
			prevGamePads[padId]=gamePads[padId];
			YsGamePadRead(&gamePads[padId],padId);
		}
	}
}
/* virtual */ void FsSimpleWindowConnection::SetKeyboardLayout(unsigned int layout)
{
	for(int i=0; i<FSKEY_NUM_KEYCODE; ++i)
	{
		FSKEYtoFM77AVKEY[i]=AVKEY_NULL;
	}
	FSKEYtoFM77AVKEY[FSKEY_NULL]=        AVKEY_NULL;
	FSKEYtoFM77AVKEY[FSKEY_SPACE]=       AVKEY_MID_SPACE;
	FSKEYtoFM77AVKEY[FSKEY_0]=           AVKEY_0;
	FSKEYtoFM77AVKEY[FSKEY_1]=           AVKEY_1;
	FSKEYtoFM77AVKEY[FSKEY_2]=           AVKEY_2;
	FSKEYtoFM77AVKEY[FSKEY_3]=           AVKEY_3;
	FSKEYtoFM77AVKEY[FSKEY_4]=           AVKEY_4;
	FSKEYtoFM77AVKEY[FSKEY_5]=           AVKEY_5;
	FSKEYtoFM77AVKEY[FSKEY_6]=           AVKEY_6;
	FSKEYtoFM77AVKEY[FSKEY_7]=           AVKEY_7;
	FSKEYtoFM77AVKEY[FSKEY_8]=           AVKEY_8;
	FSKEYtoFM77AVKEY[FSKEY_9]=           AVKEY_9;
	FSKEYtoFM77AVKEY[FSKEY_A]=           AVKEY_A;
	FSKEYtoFM77AVKEY[FSKEY_B]=           AVKEY_B;
	FSKEYtoFM77AVKEY[FSKEY_C]=           AVKEY_C;
	FSKEYtoFM77AVKEY[FSKEY_D]=           AVKEY_D;
	FSKEYtoFM77AVKEY[FSKEY_E]=           AVKEY_E;
	FSKEYtoFM77AVKEY[FSKEY_F]=           AVKEY_F;
	FSKEYtoFM77AVKEY[FSKEY_G]=           AVKEY_G;
	FSKEYtoFM77AVKEY[FSKEY_H]=           AVKEY_H;
	FSKEYtoFM77AVKEY[FSKEY_I]=           AVKEY_I;
	FSKEYtoFM77AVKEY[FSKEY_J]=           AVKEY_J;
	FSKEYtoFM77AVKEY[FSKEY_K]=           AVKEY_K;
	FSKEYtoFM77AVKEY[FSKEY_L]=           AVKEY_L;
	FSKEYtoFM77AVKEY[FSKEY_M]=           AVKEY_M;
	FSKEYtoFM77AVKEY[FSKEY_N]=           AVKEY_N;
	FSKEYtoFM77AVKEY[FSKEY_O]=           AVKEY_O;
	FSKEYtoFM77AVKEY[FSKEY_P]=           AVKEY_P;
	FSKEYtoFM77AVKEY[FSKEY_Q]=           AVKEY_Q;
	FSKEYtoFM77AVKEY[FSKEY_R]=           AVKEY_R;
	FSKEYtoFM77AVKEY[FSKEY_S]=           AVKEY_S;
	FSKEYtoFM77AVKEY[FSKEY_T]=           AVKEY_T;
	FSKEYtoFM77AVKEY[FSKEY_U]=           AVKEY_U;
	FSKEYtoFM77AVKEY[FSKEY_V]=           AVKEY_V;
	FSKEYtoFM77AVKEY[FSKEY_W]=           AVKEY_W;
	FSKEYtoFM77AVKEY[FSKEY_X]=           AVKEY_X;
	FSKEYtoFM77AVKEY[FSKEY_Y]=           AVKEY_Y;
	FSKEYtoFM77AVKEY[FSKEY_Z]=           AVKEY_Z;
	FSKEYtoFM77AVKEY[FSKEY_ESC]=         AVKEY_BREAK;
	FSKEYtoFM77AVKEY[FSKEY_F1]=          AVKEY_PF1;
	FSKEYtoFM77AVKEY[FSKEY_F2]=          AVKEY_PF2;
	FSKEYtoFM77AVKEY[FSKEY_F3]=          AVKEY_PF3;
	FSKEYtoFM77AVKEY[FSKEY_F4]=          AVKEY_PF4;
	FSKEYtoFM77AVKEY[FSKEY_F5]=          AVKEY_PF5;
	FSKEYtoFM77AVKEY[FSKEY_F6]=          AVKEY_PF6;
	FSKEYtoFM77AVKEY[FSKEY_F7]=          AVKEY_PF7;
	FSKEYtoFM77AVKEY[FSKEY_F8]=          AVKEY_PF8;
	FSKEYtoFM77AVKEY[FSKEY_F9]=          AVKEY_PF9;
	FSKEYtoFM77AVKEY[FSKEY_F10]=         AVKEY_PF10;
	FSKEYtoFM77AVKEY[FSKEY_F11]=         AVKEY_NULL;
	FSKEYtoFM77AVKEY[FSKEY_F12]=         AVKEY_NULL;
	FSKEYtoFM77AVKEY[FSKEY_PRINTSCRN]=   AVKEY_NULL;
	FSKEYtoFM77AVKEY[FSKEY_CAPSLOCK]=    AVKEY_CAPS;
	FSKEYtoFM77AVKEY[FSKEY_SCROLLLOCK]=  AVKEY_NULL; // Can assign something later.
	FSKEYtoFM77AVKEY[FSKEY_PAUSEBREAK]=  AVKEY_BREAK;
	FSKEYtoFM77AVKEY[FSKEY_BS]=          AVKEY_BACKSPACE;
	FSKEYtoFM77AVKEY[FSKEY_TAB]=         AVKEY_TAB;
	FSKEYtoFM77AVKEY[FSKEY_ENTER]=       AVKEY_RETURN;
	FSKEYtoFM77AVKEY[FSKEY_SHIFT]=       AVKEY_LEFT_SHIFT;
	FSKEYtoFM77AVKEY[FSKEY_CTRL]=        AVKEY_CTRL;
	FSKEYtoFM77AVKEY[FSKEY_ALT]=         AVKEY_NULL; // Can assign something later.
	FSKEYtoFM77AVKEY[FSKEY_INS]=         AVKEY_INS;
	FSKEYtoFM77AVKEY[FSKEY_DEL]=         AVKEY_DEL;
	FSKEYtoFM77AVKEY[FSKEY_HOME]=        AVKEY_HOME;
	FSKEYtoFM77AVKEY[FSKEY_END]=         AVKEY_NULL; // Should be translated as SHIFT+DEL
	FSKEYtoFM77AVKEY[FSKEY_PAGEUP]=      AVKEY_NULL;
	FSKEYtoFM77AVKEY[FSKEY_PAGEDOWN]=    AVKEY_NULL;
	FSKEYtoFM77AVKEY[FSKEY_UP]=          AVKEY_UP;
	FSKEYtoFM77AVKEY[FSKEY_DOWN]=        AVKEY_DOWN;
	FSKEYtoFM77AVKEY[FSKEY_LEFT]=        AVKEY_LEFT;
	FSKEYtoFM77AVKEY[FSKEY_RIGHT]=       AVKEY_RIGHT;
	FSKEYtoFM77AVKEY[FSKEY_NUMLOCK]=     AVKEY_NULL; // Can assign something later.
	FSKEYtoFM77AVKEY[FSKEY_TILDA]=       AVKEY_ESC;
	FSKEYtoFM77AVKEY[FSKEY_MINUS]=       AVKEY_MINUS;
	FSKEYtoFM77AVKEY[FSKEY_PLUS]=        AVKEY_HAT;
	FSKEYtoFM77AVKEY[FSKEY_LBRACKET]=    AVKEY_LEFT_SQUARE_BRACKET;
	FSKEYtoFM77AVKEY[FSKEY_RBRACKET]=    AVKEY_RIGHT_SQUARE_BRACKET;
	FSKEYtoFM77AVKEY[FSKEY_BACKSLASH]=   AVKEY_YEN;
	FSKEYtoFM77AVKEY[FSKEY_SEMICOLON]=   AVKEY_SEMICOLON;
	FSKEYtoFM77AVKEY[FSKEY_SINGLEQUOTE]= AVKEY_COLON;
	FSKEYtoFM77AVKEY[FSKEY_COMMA]=       AVKEY_COMMA;
	FSKEYtoFM77AVKEY[FSKEY_DOT]=         AVKEY_DOT;
	FSKEYtoFM77AVKEY[FSKEY_SLASH]=       AVKEY_SLASH;
	FSKEYtoFM77AVKEY[FSKEY_TEN0]=        AVKEY_NUM_0;
	FSKEYtoFM77AVKEY[FSKEY_TEN1]=        AVKEY_NUM_1;
	FSKEYtoFM77AVKEY[FSKEY_TEN2]=        AVKEY_NUM_2;
	FSKEYtoFM77AVKEY[FSKEY_TEN3]=        AVKEY_NUM_3;
	FSKEYtoFM77AVKEY[FSKEY_TEN4]=        AVKEY_NUM_4;
	FSKEYtoFM77AVKEY[FSKEY_TEN5]=        AVKEY_NUM_5;
	FSKEYtoFM77AVKEY[FSKEY_TEN6]=        AVKEY_NUM_6;
	FSKEYtoFM77AVKEY[FSKEY_TEN7]=        AVKEY_NUM_7;
	FSKEYtoFM77AVKEY[FSKEY_TEN8]=        AVKEY_NUM_8;
	FSKEYtoFM77AVKEY[FSKEY_TEN9]=        AVKEY_NUM_9;
	FSKEYtoFM77AVKEY[FSKEY_TENDOT]=      AVKEY_NUM_DOT;
	FSKEYtoFM77AVKEY[FSKEY_TENSLASH]=    AVKEY_NUM_SLASH;
	FSKEYtoFM77AVKEY[FSKEY_TENSTAR]=     AVKEY_NUM_STAR;
	FSKEYtoFM77AVKEY[FSKEY_TENMINUS]=    AVKEY_NUM_MINUS;
	FSKEYtoFM77AVKEY[FSKEY_TENPLUS]=     AVKEY_NUM_PLUS;
	FSKEYtoFM77AVKEY[FSKEY_TENENTER]=    AVKEY_NUM_RETURN;
	FSKEYtoFM77AVKEY[FSKEY_WHEELUP]=     AVKEY_UP;
	FSKEYtoFM77AVKEY[FSKEY_WHEELDOWN]=   AVKEY_DOWN;
	FSKEYtoFM77AVKEY[FSKEY_CONTEXT]=     AVKEY_GRAPH;

	// The following keys do not exist on the US keyboard.
	FSKEYtoFM77AVKEY[FSKEY_CONVERT]=     AVKEY_LEFT_SPACE;
	FSKEYtoFM77AVKEY[FSKEY_NONCONVERT]=  AVKEY_RIGHT_SPACE;
	FSKEYtoFM77AVKEY[FSKEY_KANA]=        AVKEY_KANA;
	FSKEYtoFM77AVKEY[FSKEY_COLON]=       AVKEY_COLON; // Need to switch with single quote
	FSKEYtoFM77AVKEY[FSKEY_AT]=          AVKEY_AT;  // FSKEY_AT collides with FSKEY_TILDA. This disables ESC.
	FSKEYtoFM77AVKEY[FSKEY_RO]=          AVKEY_DOUBLE_QUOTE;

	FSKEYtoFM77AVKEY[FSKEY_LEFT_CTRL]=   AVKEY_CTRL;
	FSKEYtoFM77AVKEY[FSKEY_RIGHT_CTRL]=  AVKEY_CTRL;
	FSKEYtoFM77AVKEY[FSKEY_LEFT_SHIFT]=  AVKEY_LEFT_SHIFT;
	FSKEYtoFM77AVKEY[FSKEY_RIGHT_SHIFT]= AVKEY_RIGHT_SHIFT;

	// The following key codes won't be returned by FsInkey()
	// These may return non zero for FsGetKeyState
	FSKEYtoFM77AVKEY[FSKEY_RIGHT_CTRL]=  AVKEY_DOUBLE_QUOTE;
	FSKEYtoFM77AVKEY[FSKEY_LEFT_ALT]=    AVKEY_LEFT_SPACE;
	FSKEYtoFM77AVKEY[FSKEY_RIGHT_ALT]=   AVKEY_RIGHT_SPACE;

	for(int i=0; i<FSKEY_NUM_KEYCODE; ++i)
	{
		if(AVKEY_GRAPH==FSKEYtoFM77AVKEY[i])
		{
			GRAPH_KEY_CODE=i;
		}
	}
}

/* virtual */ void FsSimpleWindowConnection::RegisterHostShortCut(std::string hostKeyLabel,bool ctrl,bool shift,std::string cmdStr)
{
	auto fsKey=FsStringToKeyCode(hostKeyLabel.c_str());
	if(FSKEY_NULL!=fsKey)
	{
		hostShortCut[fsKey].inUse=true;
		hostShortCut[fsKey].ctrl=ctrl;
		hostShortCut[fsKey].shift=shift;
		hostShortCut[fsKey].cmdStr=cmdStr;
	}
}

/* virtual */ void FsSimpleWindowConnection::RegisterPauseResume(std::string hostKeyLabel)
{
	auto fsKey=FsStringToKeyCode(hostKeyLabel.c_str());
	if(FSKEY_NULL!=fsKey)
	{
		PAUSE_KEY_CODE=fsKey;
	}
	else
	{
		PAUSE_KEY_CODE=DEFAULT_PAUSE_KEY_CODE;
	}
}

////////////////////////////////////////////////////////////

void FsSimpleWindowConnection::WindowConnection::Start(void)
{
	int wid=640*scaling/100;
	int hei=400*scaling/100;

	int winY0=0;
	if(true==windowShift)
	{
		winY0=48;
	}


	// In tight GUI integration, FsResizeWindow will re-enter OnDraw call-back, and will crash inside
	// unless bitmaps are ready.  Do it before FsResizeWindow.

	// Make PAUSE and MENU icons.  Used only in the tightly-integrated GUI.
	PAUSEicon=MakeIcon(PAUSE,PAUSE_wid,PAUSE_hei);
	MENUicon=MakeIcon(MENU,MENU_wid,MENU_hei);
	CAPSicon=MakeIcon(CAPS,CAPS_wid,CAPS_hei);
	KANAicon=MakeIcon(KANA,KANA_wid,KANA_hei);
	INSicon=MakeIcon(INS,INS_wid,INS_hei);

	FD_IDLEicon=MakeIcon(FD_IDLE,16,16);
	FD_BUSYicon=MakeIcon(FD_BUSY,16,16);
	TAPE_IDLEicon=MakeIcon(TAPE_IDLE,16,16);
	TAPE_LOADINGicon=MakeIcon(TAPE_LOADING,16,16);
	TAPE_SAVINGicon=MakeIcon(TAPE_SAVING,16,16);



	if(0==FsCheckWindowOpen())
	{
		FsOpenWindow(0,winY0,wid,hei+STATUS_HEI,1);
	}
	else
	{
		FsResizeWindow(wid,hei+STATUS_HEI);
	}

	switch(windowModeOnStartUp)
	{
	case FM77AVParam::WINDOW_MAXIMIZE:
		FsPollDevice();
		FsMaximizeWindow();
		for(int i=0; i<10; ++i)
		{
			FsPollDevice();
		}
		break;
	case FM77AVParam::WINDOW_FULLSCREEN:
		FsPollDevice();
		FsMakeFullScreen();
		for(int i=0; i<10; ++i)
		{
			FsPollDevice();
		}
		autoScaling=true;
		break;
	}

	this->winWid=640;
	this->winHei=400;
	FsSetWindowTitle("FM-7/FM77AV/FM77AV40 Emulator - MUTSU");

	glClearColor(0,0,0,0);
	mainTexId=GenTexture();
	statusTexId=GenTexture();

	pauseIconTexId=GenTexture();
	UpdateTexture(pauseIconTexId,PAUSE_wid,PAUSE_hei,PAUSEicon.data());
	menuIconTexId=GenTexture();
	UpdateTexture(menuIconTexId,MENU_wid,MENU_hei,MENUicon.data());

	// Make initial status bitmap
	for(int fd=0; fd<4; ++fd)
	{
		Put16x16Invert(16*fd,15,FD_IDLE);
	}
	Put16x16Invert(64,15,TAPE_IDLE);
}

void FsSimpleWindowConnection::WindowConnection::Stop(void)
{
	if(FM77AVParam::WINDOW_NORMAL!=windowModeOnStartUp)
	{
		FsUnmaximizeWindow();
	}
}

/*! Called in the window thread.
*/
void FsSimpleWindowConnection::WindowConnection::Interval(void) const
{
}

/*! Called in the VM thread.
*/
void FsSimpleWindowConnection::WindowConnection::Communicate(Outside_World *outside_world)
{
	indicatedTapePosition=outside_world->indicatedTapePosition;
	lowerRightIcon=outside_world->lowerRightIcon;
	visualizeAudioOut=outside_world->visualizeAudioOut;

	outside_world->dx=dx;
	outside_world->dy=dy;
	outside_world->scaling=scaling;
}

void FsSimpleWindowConnection::WindowConnection::Render(const FM77AVRender::Image &img,const class FM77AV &fm77av)
{
	RenderBeforeSwapBuffers(img,fm77av);
	FsSwapBuffers();
}
void FsSimpleWindowConnection::WindowConnection::RenderBeforeSwapBuffers(const FM77AVRender::Image &img,const class FM77AV &fm77av)
{
	int winWid,winHei;
	FsGetWindowSize(winWid,winHei);

	if(true==autoScaling)
	{
		if(0<img.wid && 0<img.hei)
		{
			unsigned int scaleX=100*winWid/img.wid;
			unsigned int scaleY=100*(winHei-STATUS_HEI)/img.hei;
			this->scaling=std::min(scaleX,scaleY);
		}
	}
	else
	{
		if(320==img.wid)
		{
			this->scaling=200;
		}
		else
		{
			this->scaling=100;
		}
		if(this->winWid!=img.wid*this->scaling/100 || this->winHei!=img.hei*this->scaling/100)
		{
			this->winWid=img.wid;
			this->winHei=img.hei;
			sinceLastResize=10;
		}
		else if(0<sinceLastResize)
		{
			--sinceLastResize;
			if(0==sinceLastResize)
			{
				FsResizeWindow(this->winWid*scaling/100,this->winHei*scaling/100+STATUS_HEI);
			}
		}
	}

	unsigned int renderWid=img.wid*this->scaling/100;
	unsigned int renderHei=img.hei*this->scaling/100;
	this->dx=(renderWid<winWid ? (winWid-renderWid)/2 : 0);
	this->dy=(renderHei<(winHei-STATUS_HEI) ? (winHei-STATUS_HEI-renderHei)/2 : 0);

	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0,0,winWid,winHei);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f,float(winWid),float(winHei),0.0f,-1,1);


	glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glColor3f(1,1,1);

	UpdateTexture(statusTexId,STATUS_WID,STATUS_HEI,statusBitmap);
	DrawTextureRect(0,winHei-1-STATUS_HEI,STATUS_WID,winHei-1);

	/* glRasterPos2i(0,winHei-1);
	glPixelZoom(1,1);
	glDrawPixels(STATUS_WID,STATUS_HEI,GL_RGBA,GL_UNSIGNED_BYTE,statusBitmap); */

	switch(lowerRightIcon)
	{
	case LOWER_RIGHT_NONE:
		break;
	case LOWER_RIGHT_PAUSE:
		glBindTexture(GL_TEXTURE_2D,pauseIconTexId);
		DrawTextureRect(winWid-PAUSE_wid,winHei-1-PAUSE_hei,winWid,winHei-1);
		/* glRasterPos2i(winWid-PAUSE_wid,winHei-1);
		glDrawPixels(PAUSE_wid,PAUSE_hei,GL_RGBA,GL_UNSIGNED_BYTE,PAUSEicon.data()); */
		break;
	case LOWER_RIGHT_MENU:
		glBindTexture(GL_TEXTURE_2D,menuIconTexId);
		DrawTextureRect(winWid-MENU_wid,winHei-1-MENU_hei,winWid,winHei-1);
		/* glRasterPos2i(winWid-MENU_wid,winHei-1);
		// glDrawPixels(MENU_wid,MENU_hei,GL_RGBA,GL_UNSIGNED_BYTE,MENUicon.data()); */
		break;
	}


	UpdateTexture(mainTexId,img.wid,img.hei,img.rgba);
	DrawTextureRect(this->dx,this->dy+img.hei*scaling/100,this->dx+img.wid*scaling/100,this->dy);

	glDisable(GL_TEXTURE_2D);

	if(true==visualizeAudioOut)
	{
		glColor3ub(255,255,0);
		const int barWid=16;
		const int x0=winWid-MENU_wid-barWid*12;
		glBegin(GL_LINES);
		for(int i=0; i<=3; ++i)
		{
			auto x=x0+barWid*i;
			glVertex2i(x,winHei);
			glVertex2i(x,winHei-1-MENU_hei);
		}
		glEnd();
		glBegin(GL_QUADS);
		for(int i=0; i<3; ++i)
		{
			auto &ch=fm77av.sound.state.ym2203c.state.channels[i];
			auto outLevel=ch.lastAmplitudeMax;
			outLevel*=MENU_hei;
			outLevel/=YM2612::WAVE_OUTPUT_AMPLITUDE_MAX_DEFAULT;

			auto x=x0+barWid*i;
			glVertex2i(x       ,winHei);
			glVertex2i(x+barWid,winHei);
			glVertex2i(x+barWid,winHei-outLevel);
			glVertex2i(x       ,winHei-outLevel);
		}
		glEnd();
	}

	/*glPixelZoom((float)scaling/100.0f,(float)scaling/100.0f);
	glRasterPos2i(this->dx,(img.hei*scaling/100)+dy);
	glDrawPixels(img.wid,img.hei,GL_RGBA,GL_UNSIGNED_BYTE,img.rgba);*/
}
void FsSimpleWindowConnection::WindowConnection::UpdateStatusBitmap(class FM77AV &fm77av)
{
	// Update Status Bitmap
	for(int fd=0; fd<4; ++fd)
	{
		bool busy=(fd==fm77av.fdc.DriveSelect() && true==fm77av.fdc.state.busy);
		if(fdAccessLamp[fd]!=busy)
		{
			Put16x16SelectInvert(16*fd,15,FD_IDLE,FD_BUSY,busy);
			fdAccessLamp[fd]=busy;
		}
	}

	// if tape status changed
	// unsigned int tapeStatusBitmap=0;
	// Tape icon will take 16 pixels from X=64
	if(prevTapePosition!=indicatedTapePosition)
	{
		char str[256];
		sprintf(str,"%u        ",indicatedTapePosition);
		for(int i=0; i<8; ++i)
		{
			int x0=STATUS_TAPEPOS_X+i*8;
			auto fontPtr=YsFont8x12[str[i]];
			auto rgbaPtr=statusBitmap+(2*STATUS_WID+x0)*4;
			for(int y=0; y<12; ++y)
			{
				auto byteData=fontPtr[y*4];
				for(int x=0; x<8; ++x)
				{
					if(0!=(byteData&0x80))
					{
						*(rgbaPtr++)=0;
						*(rgbaPtr++)=0;
						*(rgbaPtr++)=0;
						*(rgbaPtr++)=255;
					}
					else
					{
						*(rgbaPtr++)=255;
						*(rgbaPtr++)=255;
						*(rgbaPtr++)=255;
						*(rgbaPtr++)=255;
					}
					byteData<<=1;
				}
				rgbaPtr+=4*(STATUS_WID-8);
			}
		}
	}

	// Don't forget ins, caps, kana LEDs.
	int iconX=80;
	if(capsLED!=fm77av.keyboard.state.CAPS)
	{
		if(true==fm77av.keyboard.state.CAPS)
		{
			PutWx16Invert(iconX,15,CAPS_wid,::CAPS);
		}
		else
		{
			ClearWx16(iconX,15,CAPS_wid);
		}
		capsLED=fm77av.keyboard.state.CAPS;
	}
	iconX+=CAPS_wid;
	if(kanaLED!=fm77av.keyboard.state.KANA)
	{
		if(true==fm77av.keyboard.state.KANA)
		{
			PutWx16Invert(iconX,15,KANA_wid,::KANA);
		}
		else
		{
			ClearWx16(iconX,15,KANA_wid);
		}
		kanaLED=fm77av.keyboard.state.KANA;
	}
	iconX+=KANA_wid;
	if(insLED!=fm77av.keyboard.state.INSLED)
	{
		if(true==fm77av.keyboard.state.INSLED)
		{
			PutWx16Invert(iconX,15,KANA_wid,::INS);
		}
		else
		{
			ClearWx16(iconX,15,INS_wid);
		}
		insLED=fm77av.keyboard.state.INSLED;
	}

}
bool FsSimpleWindowConnection::WindowConnection::ImageNeedsFlip(void)
{
	return false;
}

GLuint FsSimpleWindowConnection::WindowConnection::GenTexture(void)
{
	GLuint texId;

	glGenTextures(1,&texId);  // Reserve one texture identifier
	glBindTexture(GL_TEXTURE_2D,texId);  // Making the texture identifier current (or bring it to the deck)

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

	return texId;
}

void FsSimpleWindowConnection::WindowConnection::UpdateTexture(GLuint texId,int wid,int hei,const unsigned char *rgba) const
{
	glBindTexture(GL_TEXTURE_2D,texId);
	glTexImage2D
	    (GL_TEXTURE_2D,
	     0,
	     GL_RGBA,
	     wid,
	     hei,
	     0,
	     GL_RGBA,
	     GL_UNSIGNED_BYTE,
	     rgba);
}
void FsSimpleWindowConnection::WindowConnection::DrawTextureRect(int x0,int y0,int x1,int y1) const
{
	glBegin(GL_QUADS);

	glTexCoord2f(0,0);
	glVertex2i(x0,y1);

	glTexCoord2f(1,0);
	glVertex2i(x1,y1);

	glTexCoord2f(1,1);
	glVertex2i(x1,y0);

	glTexCoord2f(0,1);
	glVertex2i(x0,y0);

	glEnd();
}


FsSimpleWindowConnection::WindowInterface *FsSimpleWindowConnection::CreateWindowInterface(void) const
{
	return new WindowConnection;
}
void FsSimpleWindowConnection::DeleteWindowInterface(WindowInterface *ptr) const
{
	auto realPtr=dynamic_cast<WindowConnection *>(ptr);
	delete realPtr;
}

////////////////////////////////////////////////////////////


Outside_World::Sound *FsSimpleWindowConnection::CreateSound(void) const
{
	return new SoundConnection;
}
void FsSimpleWindowConnection::DeleteSound(Sound *ptr) const
{
	auto realPtr=dynamic_cast <SoundConnection *>(ptr);
	delete realPtr;
}

void FsSimpleWindowConnection::SoundConnection::Start(void)
{
	soundPlayer.Start();
#ifdef AUDIO_USE_STREAMING
	YsSoundPlayer::StreamingOption FMPSGStreamOpt;
	FMPSGStreamOpt.ringBufferLengthMillisec=FM77AVSound::MILLISEC_PER_WAVE*2;
	soundPlayer.StartStreaming(FMPSGStream,FMPSGStreamOpt);
#endif
}

void FsSimpleWindowConnection::SoundConnection::Stop(void)
{
	soundPlayer.End();
}

void FsSimpleWindowConnection::SoundConnection::Polling(void)
{
	soundPlayer.KeepPlaying();
}

/* virtual */ void FsSimpleWindowConnection::SoundConnection::FMPSGPlay(std::vector <unsigned char> &wave)
{
#ifdef AUDIO_USE_STREAMING
	YsSoundPlayer::SoundData nextWave;
	nextWave.CreateFromSigned16bitStereo(YM2612::WAVE_SAMPLING_RATE,wave);
	soundPlayer.AddNextStreamingSegment(FMPSGStream,nextWave);
#else
	FMPSGChannel.CreateFromSigned16bitStereo(YM2612::WAVE_SAMPLING_RATE,wave);
	soundPlayer.PlayOneShot(FMPSGChannel);
#endif
}
/* virtual */ void FsSimpleWindowConnection::SoundConnection::FMPSGPlayStop(void)
{
}
/* virtual */ bool FsSimpleWindowConnection::SoundConnection::FMPSGChannelPlaying(void)
{
#ifdef AUDIO_USE_STREAMING
	unsigned int numSamples=(FM77AVSound::MILLISEC_PER_WAVE*YM2612::WAVE_SAMPLING_RATE+999)/1000;
	return YSTRUE!=soundPlayer.StreamPlayerReadyToAcceptNextNumSample(FMPSGStream,numSamples);
#else
	return YSTRUE==soundPlayer.IsPlaying(FMPSGChannel);
#endif
}
