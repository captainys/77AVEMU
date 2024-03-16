/* LICENSE>>
Copyright 2020 Soji Yamakawa (CaptainYS, http://www.ysflight.com)

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

<< LICENSE */
#include "device.h"
#include "fm77av.h"
#include "fm77avdef.h"
#include "fm77avgameport.h"
#include "cpputil.h"


/* static */ unsigned int FM77AVGamePort::EmulationTypeToDeviceType(unsigned int emulationType)
{
	switch(emulationType)
	{
	case FM77AV_GAMEPORTEMU_NONE:
		return NONE;
	case FM77AV_GAMEPORTEMU_MOUSE:
	case FM77AV_GAMEPORTEMU_MOUSE_BY_KEY:
	case FM77AV_GAMEPORTEMU_MOUSE_BY_NUMPAD:
	case FM77AV_GAMEPORTEMU_MOUSE_BY_PHYSICAL0:
	case FM77AV_GAMEPORTEMU_MOUSE_BY_PHYSICAL1:
	case FM77AV_GAMEPORTEMU_MOUSE_BY_PHYSICAL2:
	case FM77AV_GAMEPORTEMU_MOUSE_BY_PHYSICAL3:
	case FM77AV_GAMEPORTEMU_MOUSE_BY_PHYSICAL4:
	case FM77AV_GAMEPORTEMU_MOUSE_BY_PHYSICAL5:
	case FM77AV_GAMEPORTEMU_MOUSE_BY_PHYSICAL6:
	case FM77AV_GAMEPORTEMU_MOUSE_BY_PHYSICAL7:
	case FM77AV_GAMEPORTEMU_MOUSE_BY_ANALOG0:
	case FM77AV_GAMEPORTEMU_MOUSE_BY_ANALOG1:
	case FM77AV_GAMEPORTEMU_MOUSE_BY_ANALOG2:
	case FM77AV_GAMEPORTEMU_MOUSE_BY_ANALOG3:
	case FM77AV_GAMEPORTEMU_MOUSE_BY_ANALOG4:
	case FM77AV_GAMEPORTEMU_MOUSE_BY_ANALOG5:
	case FM77AV_GAMEPORTEMU_MOUSE_BY_ANALOG6:
	case FM77AV_GAMEPORTEMU_MOUSE_BY_ANALOG7:
		return MOUSE;
	case FM77AV_GAMEPORTEMU_PHYSICAL0_AS_CYBERSTICK:
	case FM77AV_GAMEPORTEMU_PHYSICAL1_AS_CYBERSTICK:
	case FM77AV_GAMEPORTEMU_PHYSICAL2_AS_CYBERSTICK:
	case FM77AV_GAMEPORTEMU_PHYSICAL3_AS_CYBERSTICK:
	case FM77AV_GAMEPORTEMU_PHYSICAL4_AS_CYBERSTICK:
	case FM77AV_GAMEPORTEMU_PHYSICAL5_AS_CYBERSTICK:
	case FM77AV_GAMEPORTEMU_PHYSICAL6_AS_CYBERSTICK:
	case FM77AV_GAMEPORTEMU_PHYSICAL7_AS_CYBERSTICK:
	case FM77AV_GAMEPORTEMU_CYBERSTICK:
		return CYBERSTICK;
	case FM77AV_GAMEPORTEMU_CAPCOM_BY_PHYSICAL0:
	case FM77AV_GAMEPORTEMU_CAPCOM_BY_PHYSICAL1:
	case FM77AV_GAMEPORTEMU_CAPCOM_BY_PHYSICAL2:
	case FM77AV_GAMEPORTEMU_CAPCOM_BY_PHYSICAL3:
	case FM77AV_GAMEPORTEMU_CAPCOM_BY_PHYSICAL4:
	case FM77AV_GAMEPORTEMU_CAPCOM_BY_PHYSICAL5:
	case FM77AV_GAMEPORTEMU_CAPCOM_BY_PHYSICAL6:
	case FM77AV_GAMEPORTEMU_CAPCOM_BY_PHYSICAL7:
		return CAPCOMCPSF;
	default:
		return GAMEPAD;
	}
	return NONE;
}


void FM77AVGamePort::Port::Write(long long int fm77avTime,bool COM,unsigned char TRIG)
{
	if(MOUSE==device && this->COM!=COM)
	{
		if((MOUSESTATE_XHIGH==state || MOUSESTATE_YHIGH==state) && true!=COM)
		{
			++state;
		}
		else if((MOUSESTATE_XLOW==state || MOUSESTATE_YLOW==state) && true==COM)
		{
			++state;
		}
		if(MOUSESTATE_YLOW<state)
		{
			state=MOUSESTATE_XHIGH;
		}
	}
	if(CYBERSTICK==device && CYBERSTICK_BOOT_IDLE_TIME<fm77avTime) // CYBERSTICK_BOOT_IDLE_TIME is for preventing boot menu
	{
		if((true==this->COM || lastAccessTime+CYBERSTICK_TIMEOUT<=fm77avTime) && true!=COM)
		{
			state=CYBERSTICK_00B;
			lastStateChangeTime=fm77avTime;
			mouseMotionCopy=mouseMotion;
			zAxisCopy=zAxis;
			wAxisCopy=wAxis;
		}
	}
	this->COM=COM;
	this->TRIG=TRIG;
}
unsigned char FM77AVGamePort::Port::Read(long long int fm77avTime)
{
	// FM Techknow pp.131 tells that high 2 bits are always 1.
	// Bothtec Hot Dog expect high 2 bits to be 1.
	// However, some games checks that this byte is not $FF for checking YM2203C installation.
	// Looks like b6 of joystick data is complement of COM.
	unsigned char data=0;
	if(true!=COM)
	{
		data|=0x40;
	}

	bool buttonVirtual[2]={button[0],button[1]};
	for(int b=0; b<2; ++b)
	{
		if(0!=maxButtonHoldTime[b])
		{
			if(false==lastButtonRead[b] && true==button[b])
			{
				lastButtonDownTime[b]=fm77avTime;
			}
			if(lastButtonDownTime[b]+maxButtonHoldTime[b]<=fm77avTime)
			{
				buttonVirtual[b]=false;
			}
		}
		lastButtonRead[b]=button[b];
	}


	if(MOUSE==device)
	{
		if(MOUSEREAD_RESET_TIMEOUT<(fm77avTime-lastAccessTime))
		{
			state=MOUSESTATE_XHIGH;
		}
		if(true!=buttonVirtual[0])
		{
			data|=0x10;
		}
		if(true!=buttonVirtual[1])
		{
			data|=0x20;
		}
		switch(state)
		{
		case MOUSESTATE_XHIGH:
			mouseMotionCopy=mouseMotion;
			if(0<mouseHold)
			{
				mouseMotionCopy.Set(0,0);
				--mouseHold;
			}
			data|=((mouseMotionCopy.x()>>4)&0x0F);
			break;
		case MOUSESTATE_XLOW:
			data|=((mouseMotionCopy.x()   )&0x0F);
			break;
		case MOUSESTATE_YHIGH:
			data|=((mouseMotionCopy.y()>>4)&0x0F);
			break;
		case MOUSESTATE_YLOW:
			data|=((mouseMotionCopy.y()   )&0x0F);
			// mouseMotion.Set(0,0);  Commenting out because PSY-O-BLADE dummy-reads the data all the time,
			// and mouseMotion set for the mouse integration was cleared.
			break;
		default:
			data|=0x0F;
			break;
		}
		data&=(0xCF|(TRIG<<4));
	}
	else if(GAMEPAD==device)
	{
		bool button[4]=
		{
			buttonVirtual[0],buttonVirtual[1],this->run,this->pause
		};
		// Auto Shot
		for(int i=0; i<4; ++i)
		{
			if(0!=autoShotInterval[i])
			{
				auto dt=fm77avTime%autoShotInterval[i];
				if((autoShotInterval[i]/2)<=dt)
				{
					button[i]=false;
				}
			}
		}
		auto run=button[2];
		auto pause=button[3];

		data|=0x3F;
		//if(true==run)  Let's not worry about FMTOWNS game pad for Mutsu.
		//{
		//	data&=0b11110011;
		//}
		if(true==right)
		{
			data&=0b11110111;
		}
		if(true==left)
		{
			data&=0b11111011;
		}

		//if(true==pause)  Let's not worry about FMTOWNS game pad for Mutsu.
		//{
		//	data&=0b11111100;
		//}
		if(true==down)
		{
			data&=0b11111101;
		}
		if(true==up)
		{
			data&=0b11111110;
		}
		if(true==button[0])
		{
			data&=0b11101111;
		}
		if(true==button[1])
		{
			data&=0b11011111;
		}
		data&=(0xCF|(TRIG<<4));
	}
	else if(CYBERSTICK==device)
	{
		if(CYBERSTICK_BOOT_IDLE_TIME<fm77avTime)
		{
			unsigned int trig=this->trig;

			// Auto Shot
			for(int i=0; i<MAX_NUM_BUTTONS; ++i)
			{
				if(0!=autoShotInterval[i])
				{
					auto dt=fm77avTime%autoShotInterval[i];
					if((autoShotInterval[i]/2)<=dt)
					{
						trig&=~(1<<i);
					}
				}
			}

			//  +0 Btn high 4 bits
			//  +1 Btn low 4 bits
			//  +2 Y high 4 bits
			//  +3 X high 4 bits
			//  +4 Pow high 4 bits
			//  +5 Trim high 4 bits?
			//  +6 Y low 4 bits
			//  +7 X low 4 bits
			//  +8 Pow low 4 bits
			//  +9 Trim low 4 bits?
			// +10 Btn ? 4 bits
			// +11 Unused?

			// Trigger Bits (Active Low)
			//         0 Select
			//         1 Start
			//         2 E1 on Throttle
			//         3 E2 on Throttle
			//         4 D Button on Throttle
			//         5 C Button on the Main Body
			//         6
			//         7
			//         8 B Button on the Main Body
			//         9 A Button on the Main Body
			//        10 B Button on the Stick
			//        11 A Button on the Stick

			// What's reasonable mapping?
			//   Physical Trigger 0 -> A Button on the Stick (Bit 11)
			//   Physical Trigger 1 -> B Button on the Stick (Bit 10)
			//   Physical Trigger 2 -> C Button on the Main Body (Bit 5)
			//   Physical Trigger 3 -> D Button on Throttle (Bit 4)
			//   Physical Trigger 4 -> A Button on the Main Body (Bit 9)
			//   Physical Trigger 5 -> B Button on the Main Body (Bit 8)
			//   Physical Trigger 6 -> E2
			//   Physical Trigger 7 -> E1
			//   Physical Trigger 8 -> Select (Bit 0)
			//   Physical Trigger 9 -> Start  (Bit 1)
			unsigned int cyberTrig=0;
			cyberTrig|=((trig&0x001) ? 0x800 : 0);
			cyberTrig|=((trig&0x002) ? 0x400 : 0);
			cyberTrig|=((trig&0x004) ? 0x020 : 0);
			cyberTrig|=((trig&0x008) ? 0x010 : 0);
			cyberTrig|=((trig&0x010) ? 0x200 : 0);
			cyberTrig|=((trig&0x020) ? 0x100 : 0);
			cyberTrig|=((trig&0x040) ? 0x080 : 0);
			cyberTrig|=((trig&0x080) ? 0x040 : 0);
			cyberTrig|=((trig&0x100) ? 0x001 : 0);
			cyberTrig|=((trig&0x200) ? 0x002 : 0);
			cyberTrig|=((trig&0x400) ? 0x004 : 0);
			cyberTrig|=((trig&0x800) ? 0x008 : 0);
			cyberTrig=~cyberTrig;

			switch(state)
			{
			default:
			case CYBERSTICK_00A:	// Idle
				data|=0x3F;
				break;
			case CYBERSTICK_01A:
			case CYBERSTICK_02A:
			case CYBERSTICK_03A:
			case CYBERSTICK_04A:
			case CYBERSTICK_05A:
			case CYBERSTICK_06A:
			case CYBERSTICK_07A:
			case CYBERSTICK_08A:
			case CYBERSTICK_09A:
			case CYBERSTICK_10A:
			case CYBERSTICK_11A:
				data=0x20;
				break;

			case CYBERSTICK_00B:
				data=0x00|((cyberTrig>>4)&0x0F);       // Trigger bit 4 to 7
				break;
			case CYBERSTICK_01B: // I don't remember interval, but I guess 0.1ms separation is good.
				data=0x10|((cyberTrig  )&0x0F);       // Trigger bit 0 to 3
				break;
			case CYBERSTICK_02B:
				data=0x00|(((0x80+mouseMotionCopy.y())>>4)&0x0F);
				break;
			case CYBERSTICK_03B:
				data=0x10|(((0x80+mouseMotionCopy.x())>>4)&0x0F);
				break;
			case CYBERSTICK_04B:
				data=0x00|(((0x80+zAxisCopy)>>4)&0x0F);
				break;
			case CYBERSTICK_05B:
				data=0x10|(((0x80+wAxisCopy)>>4)&0x0F);   // Probably Trim axis high
				break;
			case CYBERSTICK_06B:
				data=0x00|((0x80+mouseMotionCopy.y())&0x0F);
				break;
			case CYBERSTICK_07B:
				data=0x10|((0x80+mouseMotionCopy.x())&0x0F);
				break;
			case CYBERSTICK_08B:
				data=0x00|((0x80+zAxisCopy)&0x0F);
				break;
			case CYBERSTICK_09B:
				data=0x10|((0x80+wAxisCopy)&0x0F);       // Probably Trim axis low
				break;
			case CYBERSTICK_10B:
				data=0x00|((cyberTrig>>8)&0x0F);         // Trigger bits 8 to 11
				break;
			case CYBERSTICK_11B:
				data=0x1F;
				break;
			}

			if(lastStateChangeTime+CYBERSTICK_TIMEOUT<fm77avTime)
			{
				state=CYBERSTICK_00A;
			}
			else if(lastStateChangeTime+CYBERSTICK_READ_INTERVAL<fm77avTime)
			{
				++state;
				lastStateChangeTime=fm77avTime;
			}
		}
		else
		{
			data=0xFF;
		}
	}
	else if(CAPCOMCPSF==device)
	{
		unsigned int trig=this->trig;

		// Auto Shot
		for(int i=0; i<MAX_NUM_BUTTONS; ++i)
		{
			if(0!=autoShotInterval[i])
			{
				auto dt=fm77avTime%autoShotInterval[i];
				if((autoShotInterval[i]/2)<=dt)
				{
					trig&=~(1<<i);
				}
			}
		}

		// Source: https://gamesx.com/wiki/doku.php?id=controls:x686button
		// Pinout questionable.  The wiki tells there might be errors.
		//COM=High
		//  FWD  =bit0=UP
		//  BACK =bit1=DOWN
		//  LEFT =bit2=LEFT
		//  RIGHT=bit3=RIGHT
		//  TRIG1=bit4=L
		//  TRIG2=bit5=START
		//COM=Low
		//  FWD  =bit0=R
		//  BACK =bit1=Y
		//  LEFT =bit2=X
		//  RIGHT=bit3=SELECT
		//  TRIG1=bit4=A
		//  TRIG2=bit5=B

		bool A=(0!=(trig & 1));       //B1 Fall -> Correct
		bool B=(0!=(trig & 2));       //B2 Jump -> Correct
		bool X=(0!=(trig & 4));       //B3
		bool Y=(0!=(trig & 8));       //B4 Attack -> Correct
		bool L=(0!=(trig & 16));      //B5
		bool R=(0!=(trig & 32));      //B6
		bool start=(0!=(trig & 64));  //B7 PAUSE -> Correct
		bool select=(0!=(trig & 128));//B8
		unsigned int flags;
		if(true!=COM)
		{
			flags=(up ?      1 : 0)|
			      (down ?    2 : 0)|
			      (left ?    4 : 0)|
			      (right ?   8 : 0)|
			      (A ?      16 : 0)|
			      (B ?      32 : 0);
		}
		else
		{
			flags=(R ?       1 : 0)|
			      (Y ?       2 : 0)|
			      (X ?       4 : 0)|
			      (select ?  8 : 0)|
			      (L ?      16 : 0)|
			      (start ?  32 : 0);
		}
		data|=((~flags)&0x3F);
		data&=(0xCF|(TRIG<<4));
	}
	else // if(NONE==device)
	{
		data|=0x3F;
		data&=(0xCF|(TRIG<<4));
	}
	lastAccessTime=fm77avTime;
	return data;
}

void FM77AVGamePort::Port::SetGamePadState(bool Abutton,bool Bbutton,bool left,bool right,bool up,bool down,bool run,bool pause,long long int fm77avTime)
{
	if(true!=this->button[0] && this->button[0]!=Abutton)
	{
		autoShotBaseTime[0]=fm77avTime;
	}
	if(true!=this->button[1] && this->button[1]!=Abutton)
	{
		autoShotBaseTime[1]=fm77avTime;
	}

	this->button[0]=Abutton;
	this->button[1]=Bbutton;
	this->left =left;
	this->right=right;
	this->up   =up;
	this->down =down;
	this->run  =run;
	this->pause=pause;
}
void FM77AVGamePort::Port::SetCyberStickState(int x,int y,int z,int w,unsigned int trig,long long int fm77avTime)
{
	SetAutoShotTimer(this->trig,trig,fm77avTime);
	this->mouseMotion.Set(x,y);
	this->zAxis=z;
	this->wAxis=w;
	this->trig=trig;
}
void FM77AVGamePort::Port::SetCAPCOMCPSFState(bool left,bool right,bool up,bool down,bool A,bool B,bool X,bool Y,bool L,bool R, bool start,bool select,long long int fm77avTime)
{
	auto prevTrig=this->trig;
	this->left=left;
	this->right=right;
	this->up=up;
	this->down=down;
	this->trig=(A ? 1 : 0)|
	           (B ? 2 : 0)|
	           (X ? 4 : 0)|
	           (Y ? 8 : 0)|
	           (L ? 16 : 0)|
	           (R ? 32 : 0)|
	           (start ? 64 : 0)|
	           (select ? 128 : 0);
	SetAutoShotTimer(this->trig,trig,fm77avTime);
}
void FM77AVGamePort::Port::SetAutoShotTimer(unsigned int prevTrig,unsigned int trig,long long int fm77avTime)
{
	unsigned int pressed=(~prevTrig)&trig;
	for(unsigned int i=0; i<MAX_NUM_BUTTONS; ++i)
	{
		if(0!=(pressed&1))
		{
			autoShotBaseTime[i]=fm77avTime;
		}
		pressed>>=1;
	}
}

////////////////////////////////////////////////////////////

FM77AVGamePort::FM77AVGamePort(class FM77AV *fm77avPtr) : Device(fm77avPtr)
{
	state.ports[0].device=GAMEPAD;
	state.ports[1].device=MOUSE;
	state.PowerOn();
}

void FM77AVGamePort::State::PowerOn(void)
{
	for(auto &p : ports)
	{
		for(auto &i : p.autoShotInterval)
		{
			i=0;
		}
		for(auto &t : p.autoShotBaseTime)
		{
			t=0;
		}
	}
}
void FM77AVGamePort::State::Reset(void)
{
	for(auto &p : ports)
	{
		p.state=0;
		p.button[0]=false;
		p.button[1]=false;
		p.run=false;
		p.pause=false;
		p.left=false;
		p.right=false;
		p.up=false;
		p.down=false;
		p.COM=false;
		p.mouseMotion.Set(0,0);
		p.lastAccessTime=0;
	}
}


////////////////////////////////////////////////////////////


/* virtual */ void FM77AVGamePort::PowerOn(void)
{
	state.PowerOn();
}
/* virtual */ void FM77AVGamePort::Reset(void)
{
	state.Reset();
}
/* virtual */ uint32_t FM77AVGamePort::SerializeVersion(void) const
{
	// Version 2:
	//   mouseHold
	return 2;
}
/* virtual */ void FM77AVGamePort::SpecificSerialize(std::vector <unsigned char> &data,std::string stateFName) const
{
	for(auto &p : state.ports)
	{
		PushInt32(data,p.device);
		PushInt32(data,p.state);
		PushBool(data,p.COM);
		PushUint16(data,p.TRIG);
		PushInt64(data,p.lastAccessTime);
		PushInt64(data,p.lastStateChangeTime);
		PushUint16(data,p.mouseHold);
	}
}
/* virtual */ bool FM77AVGamePort::SpecificDeserialize(const unsigned char *&data,std::string stateFName,uint32_t version)
{
	for(auto &p : state.ports)
	{
		p.device=ReadInt32(data);
		p.state=ReadInt32(data);
		p.COM=ReadBool(data);
		p.TRIG=ReadUint16(data);
		p.lastAccessTime=ReadInt64(data);

		p.lastStateChangeTime=0;
		if(1<=version)
		{
			p.lastStateChangeTime=ReadInt64(data);
		}
		p.mouseHold=0;
		if(2<=version)
		{
			p.mouseHold=ReadUint16(data);
		}
	}
	return true;
}
