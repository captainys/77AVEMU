#include <iostream>
#include "fm77av.h"
#include "fm77avdef.h"
#include "fm77avsound.h"
#include "outside_world.h"



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
	state.ay38910.Reset();
	state.ay38910regMode=0; // 0:High Impedance  1:Data Read  2:Data Write  3:AddrLatch
	state.ay38910AddrLatch=0;
	state.ay38910LastControl=0;
	state.ay38910LastData=0;


}
/* virtual */ void FM77AVSound::IOWriteByte(unsigned int ioport,unsigned int data)
{
	switch(ioport)
	{
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
	}
}
/* virtual */ unsigned int FM77AVSound::IOReadByte(unsigned int ioport)
{
	uint8_t byteData=NonDestructiveIOReadByte(ioport);
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
	}
	return 0xFF;
}
void FM77AVSound::SoundPolling(uint64_t FM77AVTime)
{
}
std::vector <std::string> FM77AVSound::GetStatusText(void) const
{
	std::vector <std::string> text;
	return text;
}
void FM77AVSound::ProcessSound(Outside_World *outside_world)
{
	if((true==state.ay38910.IsPlaying() /* or FMPlying or Beep Playing */) && nullptr!=outside_world)
	{
		if(true==nextWave.empty())
		{
			/*if(true==IsFMPlaying() && 0!=(state.muteFlag&2))
			{
				nextWave=state.ym2612.MakeWaveAllChannels(FM_PCM_MILLISEC_PER_WAVE);
			} */
			if(true==state.ay38910.IsPlaying())
			{
				const unsigned int WAVE_OUT_SAMPLING_RATE=AY38910::WAVE_SAMPLING_RATE; // Must be same for AY-3-8910 and YM2612.
				unsigned int numSamples=0;
				if(true==nextWave.empty()) // YM2612 not playing.
				{
					numSamples=MILLISEC_PER_WAVE*WAVE_OUT_SAMPLING_RATE/1000;
					nextWave.resize(numSamples*4);
					std::memset(nextWave.data(),0,numSamples*4);
				}
				else
				{
					numSamples=(unsigned int)(nextWave.size()/4);
				}

				state.ay38910.AddWaveAllChannelsForNumSamples(nextWave.data(),numSamples);
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