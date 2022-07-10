#include <iostream>
#include <string.h>
#include "fm77av.h"
#include "fm77avdef.h"
#include "fm77avsound.h"
#include "outside_world.h"

inline void WordOp_Add(unsigned char *ptr,int value)
{
	value+=cpputil::GetSignedWord(ptr);

	if(value<-32767)
	{
		value=-32767;
	}
	else if(32767<value)
	{
		value=32767;
	}
	cpputil::PutWord(ptr,(value&0xFFFF));
}
inline void WordOp_Set(unsigned char *ptr,short value)
{
#ifdef YS_LITTLE_ENDIAN
	if(value<-32767)
	{
		*((short *)ptr)=-32767;
	}
	else if(32767<value)
	{
		*((short *)ptr)=32767;
	}
	else
	{
		*((short *)ptr)=value;
	}
#else
	if(value<-32767)
	{
		value=-32767;
	}
	else if(32767<value)
	{
		value=32767;
	}
	ptr[0]=value&255;
	ptr[1]=(value>>8)&255;
#endif
}



FM77AVSound::FM77AVSound(class FM77AV *fm77avPtr) : Device(fm77avPtr)
{
	PowerOn();
}
/* virtual */ void FM77AVSound::PowerOn(void)
{
	Reset();
}
/* virtual */ void FM77AVSound::Reset(void)
{
	state.mute=false;

	state.ym2203c.Reset();
	state.ym2203cCommand=0;
	state.ym2203cDataRead=0;
	state.ym2203cDataWrite=0;
	state.ym2203cAddrLatch=0;

	state.ay38910.Reset();
	state.ay38910regMode=0; // 0:High Impedance  1:Data Read  2:Data Write  3:AddrLatch
	state.ay38910AddrLatch=0;
	state.ay38910LastControl=0;
	state.ay38910LastData=0;

	state.beepState=BEEP_OFF;
	state.beepStopTime=0;
	state.beepTimeBalance=0;
}
/* virtual */ void FM77AVSound::IOWriteByte(unsigned int ioport,unsigned int data)
{
	FM77AV *fm77avPtr=(FM77AV *)vmPtr;
	switch(ioport)
	{
	case FM77AVIO_IRQ_BEEP://=                0xFD03,
		state.mute=(0==(data&0x01));
		// Confirmed on real FM77AV.
		//    POKE &HFD03,&HC1 will play continuous beep.
		if(0!=(data&0x80))
		{
			state.beepState=BEEP_CONTINUOUS;
		}
		else if(0!=(data&0x40))
		{
			state.beepState=BEEP_ONE_SHOT;
			state.beepStopTime=fm77avPtr->state.fm77avTime+SINGLE_BEEP_DURATION;
		}
		else
		{
			state.beepState=BEEP_OFF;
		}
		break;

	case FM77AVIO_PSG_CONTROL://             0xFD0D,
		{
			auto control=(data&3);
			if(3==state.ay38910LastControl && 0==control) // Latch Address
			{
				state.ay38910AddrLatch=(state.ay38910LastData&AY38910::REG_MASK);
			}
			if(1==control) // Read Data
			{
				// Probably I don't have anything to do here.
			}
			if(2==state.ay38910LastControl && 0==control) // Write Data
			{
				state.ay38910.Write(state.ay38910AddrLatch,state.ay38910LastData);
			}
			state.ay38910LastControl=control;
		}
		break;
	case FM77AVIO_PSG_DATA://                0xFD0E,
		if(0==state.ay38910LastControl)
		{
			state.ay38910LastData=data;
		}
		break;
	case FM77AVIO_YM2203C_CONTROL://=         0xFD15,
		switch(data&0x0F)
		{
		case 0: // High impedance
			switch(state.ym2203cCommand)
			{
			case 3: // Address Latch
				state.ym2203cAddrLatch=state.ym2203cDataWrite;
				break;
			case 2: // Data Write
				if(state.ym2203cAddrLatch<=0x0F)
				{
					state.ay38910.Write(state.ym2203cAddrLatch,state.ym2203cDataWrite);
					if(REG_GAMEPORTENABLE==state.ym2203cAddrLatch)
					{
						// Question: Should I care?
					}
				}
				else
				{
					// YM2203C does not have additional 3 channels. Channel base is always 0.
					state.ym2203c.WriteRegister(0,state.ym2203cAddrLatch,state.ym2203cDataWrite);
					if(REG_PORTB==state.ym2203cAddrLatch)
					{
						fm77avPtr->gameport.state.ports[0].Write(fm77avPtr->state.fm77avTime,0!=(data&0x10),data&3);
						fm77avPtr->gameport.state.ports[1].Write(fm77avPtr->state.fm77avTime,0!=(data&0x20),(data>>2)&3);
					}
				}
			}
			break;
		case 1: // Data Read
			if(state.ym2203cAddrLatch<=0x0F)
			{
				state.ym2203cDataRead=state.ay38910.Read(state.ym2203cAddrLatch);
			}
			else if(REG_PORTA==state.ym2203cAddrLatch)
			{
				auto portSel=(state.ym2203c.ReadRegister(0,REG_PORTB)>>6)&1;
				state.ym2203cDataRead=fm77avPtr->gameport.state.ports[portSel].Read(fm77avPtr->state.fm77avTime);
				state.ym2203cDataRead|=0xC0;
			}
			else
			{
				// YM2203C does not have additional 3 channels. Channel base is always 0.
				state.ym2203cDataRead=state.ym2203c.ReadRegister(0,state.ym2203cAddrLatch);
			}
			break;
		case 4: // Status Read
			state.ym2203cDataRead=0b01111100;
			state.ym2203cDataRead|=(true==state.ym2203c.TimerAUp() ? 1 : 0);
			state.ym2203cDataRead|=(true==state.ym2203c.TimerBUp() ? 2 : 0);
			break;
		case 9: // Joystick Read
			{
				auto portSel=(state.ym2203c.ReadRegister(0,REG_PORTB)>>6)&1;
				state.ym2203cDataRead=fm77avPtr->gameport.state.ports[portSel].Read(fm77avPtr->state.fm77avTime);
				state.ym2203cDataRead|=0xC0;
			}
			break;
		}
		state.ym2203cCommand=data&0x0F;
		break;
	case FM77AVIO_YM2203C_DATA://=            0xFD16,
		state.ym2203cDataWrite=data;
		break;

	case FM77AVIO_BEEP://=                    0xD403,
		state.beepState=BEEP_ONE_SHOT;
		state.beepStopTime=fm77avPtr->state.fm77avTime+SINGLE_BEEP_DURATION;
		break;
	}
}
/* virtual */ unsigned int FM77AVSound::IOReadByte(unsigned int ioport)
{
	uint8_t byteData=NonDestructiveIOReadByte(ioport);
	if(FM77AVIO_BEEP==ioport)//=                    0xD403,
	{
		FM77AV *fm77avPtr=(FM77AV *)vmPtr;
		state.beepState=BEEP_ONE_SHOT;
		state.beepStopTime=fm77avPtr->state.fm77avTime+SINGLE_BEEP_DURATION;
	}
	return byteData;
}
uint8_t FM77AVSound::NonDestructiveIOReadByte(unsigned int ioport) const
{
	switch(ioport)
	{
	case FM77AVIO_PSG_CONTROL://             0xFD0D,
		break;
	case FM77AVIO_PSG_DATA://                0xFD0E,
		if(1==state.ay38910LastControl) // Read
		{
			return state.ay38910.Read(state.ay38910AddrLatch);
		}
		break;

	case FM77AVIO_YM2203C_CONTROL://=         0xFD15,
		break;
	case FM77AVIO_YM2203C_DATA://=            0xFD16,
		return state.ym2203cDataRead;
	}
	return 0xFF;
}
void FM77AVSound::SoundPolling(uint64_t fm77avTime)
{
	state.ym2203c.Run(fm77avTime);
	auto fm77avPtr=(FM77AV *)vmPtr;
	if(true==(state.ym2203c.TimerAUp() || state.ym2203c.TimerBUp()))
	{
		fm77avPtr->state.main.irqSource|=FM77AV::SystemState::MAIN_IRQ_SOURCE_YM2203C;
	}
	else
	{
		fm77avPtr->state.main.irqSource&=~FM77AV::SystemState::MAIN_IRQ_SOURCE_YM2203C;
	}
}
std::vector <std::string> FM77AVSound::GetStatusText(void) const
{
	std::vector <std::string> text;
	for(auto str : state.ym2203c.GetStatusText())
	{
		text.push_back(str);
	}
	for(auto str : state.ay38910.GetStatusText())
	{
		text.push_back(str);
	}
	return text;
}

void FM77AVSound::ProcessSound(Outside_World *outside_world)
{
	if((true==state.ay38910.IsPlaying() || true==IsFMPlaying() || BEEP_OFF!=state.beepState) && nullptr!=outside_world)
	{
		if(true==nextWave.empty())
		{
			if(true==IsFMPlaying())
			{
				nextWave=state.ym2203c.MakeWaveAllChannels(MILLISEC_PER_WAVE);
			}
			if(true==state.ay38910.IsPlaying())
			{
				const unsigned int WAVE_OUT_SAMPLING_RATE=AY38910::WAVE_SAMPLING_RATE; // Must be same for AY-3-8910 and YM2612.
				unsigned int numSamples=0;
				if(true==nextWave.empty()) // YM2612 not playing.
				{
					numSamples=MILLISEC_PER_WAVE*WAVE_OUT_SAMPLING_RATE/1000;
					nextWave.resize(numSamples*4);
					memset(nextWave.data(),0,numSamples*4);
				}
				else
				{
					numSamples=(unsigned int)(nextWave.size()/4);
				}

				state.ay38910.AddWaveAllChannelsForNumSamples(nextWave.data(),numSamples);
			}
			if(BEEP_OFF!=state.beepState)
			{
				// 1200Hz Square Wave.  Sign flips at 2400 times per sec.
				const uint32_t periodX100=AY38910::WAVE_SAMPLING_RATE*100/2400;
				if(true==nextWave.empty()) // YM2612 not playing.
				{
					unsigned int numSamples=MILLISEC_PER_WAVE*AY38910::WAVE_SAMPLING_RATE/1000;
					nextWave.resize(numSamples*4);
					memset(nextWave.data(),0,numSamples*4);
					for(unsigned int i=0; i<numSamples; ++i)
					{
						state.beepTimeBalance+=100;
						if(periodX100<=state.beepTimeBalance)
						{
							state.beepTimeBalance-=periodX100;
							state.beepWaveOut=255-state.beepWaveOut;
						}
						auto dataPtr=nextWave.data()+i*4;
						auto waveOut=(0==state.beepWaveOut ? -BEEP_SOUND_AMPLITUDE : BEEP_SOUND_AMPLITUDE);
						WordOp_Set(dataPtr,  waveOut);
						WordOp_Set(dataPtr+2,waveOut);
					}
				}
				else
				{
					unsigned int numSamples=(unsigned int)(nextWave.size()/4);
					for(unsigned int i=0; i<numSamples; ++i)
					{
						state.beepTimeBalance+=100;
						if(periodX100<=state.beepTimeBalance)
						{
							state.beepTimeBalance-=periodX100;
							state.beepWaveOut=255-state.beepWaveOut;
						}
						auto dataPtr=nextWave.data()+i*4;
						auto waveOut=(0==state.beepWaveOut ? -BEEP_SOUND_AMPLITUDE : BEEP_SOUND_AMPLITUDE);
						WordOp_Add(dataPtr,  waveOut);
						WordOp_Add(dataPtr+2,waveOut);
					}
				}
			}
		}

		if(BEEP_ONE_SHOT==state.beepState)
		{
			auto fm77avPtr=(FM77AV *)vmPtr;
			if(state.beepStopTime<=fm77avPtr->state.fm77avTime)
			{
				state.beepState=BEEP_OFF;
			}
		}

		if(true!=outside_world->FMPSGChannelPlaying() && true!=nextWave.empty())
		{
			if(true==recordAudio)
			{
				audioRecording.insert(audioRecording.end(),nextWave.begin(),nextWave.end());
			}
			outside_world->FMPSGPlay(nextWave);
			nextWave.clear(); // It was supposed to be cleared in FMPlay.  Just in case.
			// state.ym2612.CheckToneDoneAllChannels();
		}
	}
}

void FM77AVSound::StartRecording(void)
{
	recordAudio=true;
	audioRecording.clear();
}
void FM77AVSound::EndRecording(void)
{
	recordAudio=false;
}
#include "yssimplesound.h"
void FM77AVSound::SaveRecording(std::string fName) const
{
	YsSoundPlayer::SoundData data;
	auto dataCopy=audioRecording;
	data.CreateFromSigned16bitStereo(44100,dataCopy);
	auto wavFile=data.MakeWavByteData();
	cpputil::WriteBinaryFile(fName,wavFile.size(),wavFile.data());
}

/* virtual */ uint32_t FM77AVSound::SerializeVersion(void) const
{
	return 0;
}
/* virtual */ void FM77AVSound::SpecificSerialize(std::vector <unsigned char> &data,std::string stateFName) const
{
}
/* virtual */ bool FM77AVSound::SpecificDeserialize(const unsigned char *&data,std::string stateFName,uint32_t version)
{
	return true;
}
