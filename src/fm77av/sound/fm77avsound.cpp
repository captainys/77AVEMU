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
	for(auto &b : ym2203cRegisterMonitor)
	{
		b=0;
	}
	for(auto &b : ay38910RegisterMonitor)
	{
		b=0;
	}
	state.ym2203c.useScheduling=true;
	state.ay38910.useScheduling=true;
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
				state.ay38910.WriteRegisterSchedule(state.ay38910AddrLatch,state.ay38910LastData,fm77avPtr->state.fm77avTime);
				if(0!=ay38910RegisterMonitor[state.ay38910AddrLatch])
				{
					std::cout << "AY38910 Reg[$"+cpputil::Ubtox(state.ay38910AddrLatch)+"]=$"+cpputil::Ubtox(state.ay38910LastData) << " at " << fm77avPtr->state.fm77avTime << std::endl;
					if(MC6809::Debugger::BRKPNT_FLAG_BREAK==ay38910RegisterMonitor[state.ay38910AddrLatch])
					{
						fm77avPtr->mainCPU.debugger.stop=true;
					}
				}
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
			break;
		case 1: // Data Read
			if(state.ym2203cAddrLatch<=0x0F)
			{
				state.ym2203cDataRead=state.ay38910.ReadRegister(state.ym2203cAddrLatch);
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
				// Pre-scaler also influences SSG part, which is done by AY-3-8910 emulation.
				if(YM2612::REG_PRESCALER_0==state.ym2203cAddrLatch)
				{
					state.ay38910.state.preScaler=4;
				}
				if(YM2612::REG_PRESCALER_1==state.ym2203cAddrLatch)
				{
					state.ay38910.state.preScaler=2;
				}
				if(YM2612::REG_PRESCALER_2==state.ym2203cAddrLatch)
				{
					state.ay38910.state.preScaler=1;
				}
			}
			break;
		case 3: // Address Latch
			state.ym2203cAddrLatch=state.ym2203cDataWrite;
			break;
		case 2: // Data Write
			if(0!=ym2203cRegisterMonitor[state.ym2203cAddrLatch])
			{
				std::cout << "YM2203C Reg[$"+cpputil::Ubtox(state.ym2203cAddrLatch)+"]=$"+cpputil::Ubtox(state.ym2203cDataWrite) << " at " << fm77avPtr->state.fm77avTime << std::endl;
				if(MC6809::Debugger::BRKPNT_FLAG_BREAK==ym2203cRegisterMonitor[state.ym2203cAddrLatch])
				{
					fm77avPtr->mainCPU.debugger.stop=true;
				}
			}
			if(state.ym2203cAddrLatch<=0x0F)
			{
				state.ay38910.WriteRegisterSchedule(state.ym2203cAddrLatch,state.ym2203cDataWrite,fm77avPtr->state.fm77avTime);
				if(REG_GAMEPORTENABLE==state.ym2203cAddrLatch)
				{
					// Question: Should I care?
				}
			}
			else
			{
				if(0x2D<=state.ym2203cAddrLatch && state.ym2203cAddrLatch<=0x2F)
				{
					std::cout << "Pre-Scaler [" << cpputil::Ubtox(state.ym2203cAddrLatch) << "]=" << cpputil::Ubtox(state.ym2203cDataWrite) << std::endl;
				}

				// YM2203C does not have additional 3 channels. Channel base is always 0.
				state.ym2203c.WriteRegister(0,state.ym2203cAddrLatch,state.ym2203cDataWrite,fm77avPtr->state.fm77avTime);
				if(REG_PORTB==state.ym2203cAddrLatch)
				{
					fm77avPtr->gameport.state.ports[0].Write(fm77avPtr->state.fm77avTime,0!=(data&0x10),data&3);
					fm77avPtr->gameport.state.ports[1].Write(fm77avPtr->state.fm77avTime,0!=(data&0x20),(data>>2)&3);
				}
				// Pre-scaler also influences SSG part, which is done by AY-3-8910 emulation.
				if(YM2612::REG_PRESCALER_0==state.ym2203cAddrLatch)
				{
					state.ay38910.state.preScaler=4;
				}
				if(YM2612::REG_PRESCALER_1==state.ym2203cAddrLatch)
				{
					state.ay38910.state.preScaler=2;
				}
				if(YM2612::REG_PRESCALER_2==state.ym2203cAddrLatch)
				{
					state.ay38910.state.preScaler=1;
				}
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
		default:
			std::cout << "Warning!  Undefined YM2203C I/O command!  " << cpputil::Ubtox(data&0x0F) << std::endl;
			break;
		}
		state.ym2203cCommand=data&0x0F;
		break;
	case FM77AVIO_YM2203C_DATA://=            0xFD16,
		state.ym2203cDataWrite=data;
		// Apparently it is normal.
		//if(0!=state.ym2203cCommand)
		//{
		//	std::cout << "Warning!  YM2203C Data Register Written while not High-Impedance state!" << std::endl;
		//	break;
		//}
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
			return state.ay38910.ReadRegister(state.ay38910AddrLatch);
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
		auto fm77avPtr=(FM77AV *)vmPtr;
		auto lastWaveGenTime=nextWaveGenTime-MILLISEC_PER_WAVE_GENERATION*1000000;
		if(nextWaveGenTime<=fm77avPtr->state.fm77avTime)
		{
			const unsigned int WAVE_OUT_SAMPLING_RATE=AY38910::WAVE_SAMPLING_RATE; // Must be same for AY-3-8910 and YM2612.
			const uint32_t numSamplesPerWave=MILLISEC_PER_WAVE*WAVE_OUT_SAMPLING_RATE/1000;
			if(true==nextWave.empty())
			{
				nextWaveFilledInMillisec=0;
				nextWave.resize(numSamplesPerWave*4);
				memset(nextWave.data(),0,nextWave.size());
			}

			if(nextWaveFilledInMillisec<MILLISEC_PER_WAVE)
			{
				uint32_t fillBegin=nextWaveFilledInMillisec*numSamplesPerWave/MILLISEC_PER_WAVE;
				uint32_t fillEnd=(nextWaveFilledInMillisec+MILLISEC_PER_WAVE_GENERATION)*numSamplesPerWave/MILLISEC_PER_WAVE;
				fillEnd=std::min(fillEnd,numSamplesPerWave);

				auto fillNumSamples=fillEnd-fillBegin;
				auto fillPtr=nextWave.data()+fillBegin*4;

				if(true==IsFMPlaying())
				{
					state.ym2203c.MakeWaveForNSamples(fillPtr,fillNumSamples,lastWaveGenTime);
				}
				if(true==state.ay38910.IsPlaying())
				{
					unsigned int numSamples=0;
					state.ay38910.AddWaveAllChannelsForNumSamples(fillPtr,fillNumSamples,lastWaveGenTime);
				}
				if(BEEP_OFF!=state.beepState)
				{
					// 1200Hz Square Wave.  Sign flips at 2400 times per sec.
					const uint32_t periodX100=AY38910::WAVE_SAMPLING_RATE*100/2400;
					for(unsigned int i=0; i<fillNumSamples; ++i)
					{
						state.beepTimeBalance+=100;
						if(periodX100<=state.beepTimeBalance)
						{
							state.beepTimeBalance-=periodX100;
							state.beepWaveOut=255-state.beepWaveOut;
						}
						auto dataPtr=fillPtr+i*4;
						auto waveOut=(0==state.beepWaveOut ? -BEEP_SOUND_AMPLITUDE : BEEP_SOUND_AMPLITUDE);
						WordOp_Add(dataPtr,  waveOut);
						WordOp_Add(dataPtr+2,waveOut);
					}
				}
				nextWaveFilledInMillisec+=MILLISEC_PER_WAVE_GENERATION;

				if(BEEP_ONE_SHOT==state.beepState)
				{
					auto fm77avPtr=(FM77AV *)vmPtr;
					if(state.beepStopTime<=fm77avPtr->state.fm77avTime)
					{
						state.beepState=BEEP_OFF;
					}
				}
			}

			nextWaveGenTime+=MILLISEC_PER_WAVE_GENERATION*1000000;
			// Maybe it was not playing a while?
			if(nextWaveGenTime<fm77avPtr->state.fm77avTime)
			{
				nextWaveGenTime=fm77avPtr->state.fm77avTime+MILLISEC_PER_WAVE_GENERATION*1000000;
			}
		}
	}
	else if(true!=nextWave.empty())
	{
		// Sound stopped, but still something in the buffer.
		// And, the sound may resume before the end of the buffer.
		nextWaveFilledInMillisec+=MILLISEC_PER_WAVE_GENERATION;
	}
	else
	{
		// Not playing, no leftover buffer.
		nextWaveGenTime=0;
	}

	if(true!=outside_world->FMPSGChannelPlaying() && MILLISEC_PER_WAVE<=nextWaveFilledInMillisec)
	{
		// Hope wave playback is done before nextWaveGenTime.
		if(true==recordAudio)
		{
			audioRecording.insert(audioRecording.end(),nextWave.begin(),nextWave.end());
		}
		outside_world->FMPSGPlay(nextWave);
		nextWave.clear(); // It was supposed to be cleared in FMPlay.  Just in case.
		// state.ym2612.CheckToneDoneAllChannels();
		nextWaveFilledInMillisec=0;
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

// Difference from Tsugaru is it includes preScaler.
void FM77AVSound::SerializeYM2203CFMPart(std::vector <unsigned char> &data) const
{
	auto ym2203c=state.ym2203c; // Make a copy to flush register schedule.

	ym2203c.FlushRegisterSchedule();

	PushUint32(data,ym2203c.state.preScaler);

	PushBool(data,ym2203c.state.LFO);
	PushUint32(data,ym2203c.state.FREQCTRL);
	PushUint64(data,ym2203c.state.deviceTimeInNS);
	PushUint64(data,ym2203c.state.lastTickTimeInNS);
	for(auto &ch : ym2203c.state.channels)
	{
		PushUint32(data,ch.F_NUM);
		PushUint32(data,ch.BLOCK);
		PushUint32(data,ch.FB);
		PushUint32(data,ch.CONNECT);
		PushUint32(data,ch.L);
		PushUint32(data,ch.R);
		PushUint32(data,ch.AMS);
		PushUint32(data,ch.PMS);
		PushUint32(data,ch.usingSlot);

		for(auto &sl : ch.slots)
		{
			PushUint32(data,sl.DT);
			PushUint32(data,sl.MULTI);
			PushUint32(data,sl.TL);
			PushUint32(data,sl.KS);
			PushUint32(data,sl.AR);
			PushUint32(data,sl.AM);
			PushUint32(data,sl.DR);
			PushUint32(data,sl.SR);
			PushUint32(data,sl.SL);
			PushUint32(data,sl.RR);
			PushUint32(data,sl.SSG_EG);

			// Cache for wave-generation >>
			PushUint64(data,sl.microsecS12);
			PushUint64(data,sl.toneDurationMicrosecS12);
			PushUint32(data,sl.phaseS12);
			PushUint32(data,sl.phaseS12Step);
			PushUint32(data,sl.env[0]);
			PushUint32(data,sl.env[1]);
			PushUint32(data,sl.env[2]);
			PushUint32(data,sl.env[3]);
			PushUint32(data,sl.env[4]);
			PushUint32(data,sl.env[5]);
			PushUint32(data,sl.envDurationCache);
			PushBool(data,sl.InReleasePhase);
			PushUint32(data,sl.ReleaseStartTime);
			PushUint32(data,sl.ReleaseEndTime);
			PushUint32(data,sl.ReleaseStartDbX100);
			PushUint32(data,sl.lastDbX100Cache);
			// Cache for wave-generation <<
		}

		PushUint32(data,ch.playState);
		PushInt32(data,ch.lastSlot0Out[0]);
		PushInt32(data,ch.lastSlot0Out[1]);
	}
	PushUint32(data,ym2203c.state.F_NUM_3CH[0]);
	PushUint32(data,ym2203c.state.F_NUM_3CH[1]);
	PushUint32(data,ym2203c.state.F_NUM_3CH[2]);
	PushUint32(data,ym2203c.state.BLOCK_3CH[0]);
	PushUint32(data,ym2203c.state.BLOCK_3CH[1]);
	PushUint32(data,ym2203c.state.BLOCK_3CH[2]);
	PushUint32(data,ym2203c.state.F_NUM_6CH[0]);
	PushUint32(data,ym2203c.state.F_NUM_6CH[1]);
	PushUint32(data,ym2203c.state.F_NUM_6CH[2]);
	PushUint32(data,ym2203c.state.BLOCK_6CH[0]);
	PushUint32(data,ym2203c.state.BLOCK_6CH[1]);
	PushUint32(data,ym2203c.state.BLOCK_6CH[2]);
	PushUcharArray(data,256,ym2203c.state.regSet[0]);
	PushUcharArray(data,256,ym2203c.state.regSet[1]);
	PushUint64(data,ym2203c.state.timerCounter[0]);
	PushUint64(data,ym2203c.state.timerCounter[1]);
	PushBool(data,ym2203c.state.timerUp[0]);
	PushBool(data,ym2203c.state.timerUp[1]);
	PushUint32(data,ym2203c.state.playingCh);
	PushInt32(data,ym2203c.state.volume);
}
void FM77AVSound::DeserializeYM2203CFMPart(const unsigned char *&data,unsigned int version)
{
	auto &ym2203c=state.ym2203c;

	ym2203c.FlushRegisterSchedule();

	for(auto &ch : ym2203c.state.channels)
	{
		for(auto &sl : ch.slots)
		{
			for(auto &e : sl.env)
			{
				e=0;
			}
		}
	}

	ym2203c.state.preScaler=ReadUint32(data);

	ym2203c.state.LFO=ReadBool(data);
	ym2203c.state.FREQCTRL=ReadUint32(data);
	ym2203c.state.deviceTimeInNS=ReadUint64(data);
	ym2203c.state.lastTickTimeInNS=ReadUint64(data);
	for(auto &ch : ym2203c.state.channels)
	{
		ch.F_NUM=ReadUint32(data);
		ch.BLOCK=ReadUint32(data);
		ch.FB=ReadUint32(data);
		ch.CONNECT=ReadUint32(data);
		ch.L=ReadUint32(data);
		ch.R=ReadUint32(data);
		ch.AMS=ReadUint32(data);
		ch.PMS=ReadUint32(data);
		ch.usingSlot=ReadUint32(data);

		for(auto &sl : ch.slots)
		{
			sl.DT=ReadUint32(data);
			sl.MULTI=ReadUint32(data);
			sl.TL=ReadUint32(data);
			sl.KS=ReadUint32(data);
			sl.AR=ReadUint32(data);
			sl.AM=ReadUint32(data);
			sl.DR=ReadUint32(data);
			sl.SR=ReadUint32(data);
			sl.SL=ReadUint32(data);
			sl.RR=ReadUint32(data);
			sl.SSG_EG=ReadUint32(data);

			// Cache for wave-generation >>
			sl.microsecS12=ReadUint64(data);
			sl.toneDurationMicrosecS12=ReadUint64(data);
			sl.phaseS12=ReadUint32(data);
			sl.phaseS12Step=ReadUint32(data);
			sl.env[0]=ReadUint32(data);
			sl.env[1]=ReadUint32(data);
			sl.env[2]=ReadUint32(data);
			sl.env[3]=ReadUint32(data);
			sl.env[4]=ReadUint32(data);
			sl.env[5]=ReadUint32(data);
			sl.envDurationCache=ReadUint32(data);
			sl.InReleasePhase=ReadBool(data);
			sl.ReleaseStartTime=ReadUint32(data);
			sl.ReleaseEndTime=ReadUint32(data);
			sl.ReleaseStartDbX100=ReadUint32(data);
			sl.lastDbX100Cache=ReadUint32(data);
			// Cache for wave-generation <<
		}

		ch.playState=ReadUint32(data);
		ch.lastSlot0Out[0]=ReadInt32(data);
		ch.lastSlot0Out[1]=ReadInt32(data);
	}
	ym2203c.state.F_NUM_3CH[0]=ReadUint32(data);
	ym2203c.state.F_NUM_3CH[1]=ReadUint32(data);
	ym2203c.state.F_NUM_3CH[2]=ReadUint32(data);
	ym2203c.state.BLOCK_3CH[0]=ReadUint32(data);
	ym2203c.state.BLOCK_3CH[1]=ReadUint32(data);
	ym2203c.state.BLOCK_3CH[2]=ReadUint32(data);
	ym2203c.state.F_NUM_6CH[0]=ReadUint32(data);
	ym2203c.state.F_NUM_6CH[1]=ReadUint32(data);
	ym2203c.state.F_NUM_6CH[2]=ReadUint32(data);
	ym2203c.state.BLOCK_6CH[0]=ReadUint32(data);
	ym2203c.state.BLOCK_6CH[1]=ReadUint32(data);
	ym2203c.state.BLOCK_6CH[2]=ReadUint32(data);
	ReadUcharArray(data,256,ym2203c.state.regSet[0]);
	ReadUcharArray(data,256,ym2203c.state.regSet[1]);
	ym2203c.state.timerCounter[0]=ReadUint64(data);
	ym2203c.state.timerCounter[1]=ReadUint64(data);
	ym2203c.state.timerUp[0]=ReadBool(data);
	ym2203c.state.timerUp[1]=ReadBool(data);
	ym2203c.state.playingCh=ReadUint32(data);
	ym2203c.state.volume=ReadInt32(data);
}
void FM77AVSound::SerializeAY38910(std::vector <unsigned char> &data) const
{
	PushUcharArray(data,AY38910::NUM_REGS,state.ay38910.state.regs);
	for(int i=0; i<AY38910::NUM_CHANNELS; ++i)
	{
		PushUint32(data,state.ay38910.state.ch[i].toneSign);
		PushUint32(data,state.ay38910.state.ch[i].tonePeriodBalance);
	}
	PushUint32(data,state.ay38910.state.envPhase);
	PushUint32(data,state.ay38910.state.envOut);
	PushUint32(data,state.ay38910.state.envPeriodBalance);
	PushUint32(data,state.ay38910.state.envPatternSeg);
	PushUint32(data,state.ay38910.state.preScaler);
	PushUint32(data,state.ay38910.state.LFSR);
	PushUint32(data,state.ay38910.state.noisePeriodBalance);

}
void FM77AVSound::DeserializeAY38910(const unsigned char *&data,unsigned int version)
{
	ReadUcharArray(data,AY38910::NUM_REGS,state.ay38910.state.regs);
	for(int i=0; i<AY38910::NUM_CHANNELS; ++i)
	{
		state.ay38910.state.ch[i].toneSign=ReadUint32(data);
		state.ay38910.state.ch[i].tonePeriodBalance=ReadUint32(data);
	}
	state.ay38910.state.envPhase=ReadUint32(data);
	state.ay38910.state.envOut=ReadUint32(data);
	state.ay38910.state.envPeriodBalance=ReadUint32(data);
	state.ay38910.state.envPatternSeg=ReadUint32(data);
	state.ay38910.state.preScaler=ReadUint32(data);
	state.ay38910.state.LFSR=ReadUint32(data);
	state.ay38910.state.noisePeriodBalance=ReadUint32(data);
}

/* virtual */ uint32_t FM77AVSound::SerializeVersion(void) const
{
	return 0;
}
/* virtual */ void FM77AVSound::SpecificSerialize(std::vector <unsigned char> &data,std::string stateFName) const
{
	SerializeYM2203CFMPart(data);
	PushUint16(data,state.ym2203cCommand);
	PushUint16(data,state.ym2203cDataRead);
	PushUint16(data,state.ym2203cDataWrite);
	PushUint32(data,state.ym2203cAddrLatch);

	SerializeAY38910(data);
	PushUint16(data,state.ay38910regMode);
	PushUint16(data,state.ay38910AddrLatch);
	PushUint16(data,state.ay38910LastControl);
	PushUint16(data,state.ay38910LastData);

	PushUint16(data,state.beepState);
	PushUint64(data,state.beepStopTime);
	PushUint32(data,state.beepTimeBalance);
	PushUint16(data,state.beepWaveOut);
}
/* virtual */ bool FM77AVSound::SpecificDeserialize(const unsigned char *&data,std::string stateFName,uint32_t version)
{
	DeserializeYM2203CFMPart(data,version);
	state.ym2203cCommand=ReadUint16(data);
	state.ym2203cDataRead=ReadUint16(data);
	state.ym2203cDataWrite=ReadUint16(data);
	state.ym2203cAddrLatch=ReadUint32(data);

	DeserializeAY38910(data,version);
	state.ay38910regMode=ReadUint16(data);
	state.ay38910AddrLatch=ReadUint16(data);
	state.ay38910LastControl=ReadUint16(data);
	state.ay38910LastData=ReadUint16(data);

	state.beepState=ReadUint16(data);
	state.beepStopTime=ReadUint64(data);
	state.beepTimeBalance=ReadUint32(data);
	state.beepWaveOut=ReadUint16(data);

	nextWaveGenTime=0;
	return true;
}
