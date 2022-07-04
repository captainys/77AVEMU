#include <iostream>
#include <string.h>
#include "ay38910.h"
#include "cpputil.h"



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

void AY38910::Reset(void)
{
	for(auto &r : state.regs)
	{
		r=0;
	}
}
uint8_t AY38910::Read(uint8_t reg) const
{
	return state.regs[reg];
}
void AY38910::Write(uint8_t reg,uint8_t value)
{
	state.regs[reg]=value;
}

inline unsigned int AY38910::GetF_NUM(int ch) const
{
	unsigned int reg=REG_CH0_FREQ_LOW+ch*2;
	unsigned int F_NUM=state.regs[reg+1];
	F_NUM<<=8;
	return (F_NUM|state.regs[reg])&0xFFF;
}
inline unsigned int AY38910::ChannelFrequencyX1000(int ch) const
{
	auto F_NUM=GetF_NUM(ch);
	if(0<F_NUM)
	{
		unsigned int freq=FREQ_CONST*1000/16;
		freq/=F_NUM;
		return freq;
	}
	return 0;
}
inline unsigned int AY38910::GetAmplitude(int ch) const
{
	unsigned int volume=state.regs[REG_CH0_VOL+ch];
	if(0!=(0x10&volume)) // Envelope
	{
		return MAX_AMPLITUDE; // Tentative
	}
	else
	{
		volume&=0x10;
		return MAX_AMPLITUDE/16;
	}
}

std::vector <unsigned char> AY38910::MakeWaveAllChannels(unsigned long long int millisec)
{
	std::vector <unsigned char> wave;

	unsigned long long int numSamples=(millisec<<12);
	numSamples*=WAVE_SAMPLING_RATE;
	numSamples/=1000;
	numSamples>>=12;

	wave.resize(4*numSamples);
	memset(wave.data(),0,wave.size());

	AddWaveAllChannelsForNumSamples(wave.data(),numSamples);
	return wave;
}
void AY38910::AddWaveAllChannelsForNumSamples(unsigned char data[],unsigned long long int numSamples)
{
	for(int ch=0; ch<3; ++ch)
	{
		// Let's worry about envelope and noise later.
		unsigned int toneFreqX1000=ChannelFrequencyX1000(0);
		if(0<toneFreqX1000)
		{
			unsigned int tonePeriodX10=WAVE_SAMPLING_RATE*10000/toneFreqX1000;
			unsigned int halfTonePeriodX10=tonePeriodX10/2;

			for(unsigned long long int i=0; i<numSamples; ++i)
			{
				auto dataPtr=data+i*4; // 16-bit stereo

				if(state.ch[ch].tonePeriodBalance<10)
				{
					state.ch[ch].toneSign=1-state.ch[ch].toneSign;
					state.ch[ch].tonePeriodBalance+=halfTonePeriodX10;
				}
				else
				{
					state.ch[ch].tonePeriodBalance-=10;
				}

				int ampl=GetAmplitude(ch);
				if(0==state.ch[ch].toneSign)
				{
					ampl=-ampl;
				}
				WordOp_Add(dataPtr,ampl);
				*(uint16_t *)(dataPtr+2)=*(uint16_t *)(dataPtr);
			}
		}
	}
}
