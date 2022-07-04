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
AY38910::AY38910()
{
	std::cout << "AY-3-8910 Emulator for Mutsu - DONDANZ" << std::endl;
	std::cout << "by CaptainYS" << std::endl;
	std::cout << "http://www.ysflight.com" << std::endl;
	Reset();
}

void AY38910::Reset(void)
{
	for(auto &r : state.regs)
	{
		r=0;
	}

	state.envPhase=0;
	state.envOut=0;
	state.envPeriodBalance=0;
	state.envPatternSeg=0;

}
uint8_t AY38910::Read(uint8_t reg) const
{
	return state.regs[reg];
}
void AY38910::Write(uint8_t reg,uint8_t value)
{
	state.regs[reg]=value;
	if(REG_ENV_PATTERN==reg)
	{
		state.envPatternSeg=0;
		StartEnvelopeSegment();
	}
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
		volume=state.envOut;
		return volume*MAX_AMPLITUDE/ENV_OUT_MAX;
	}
	else
	{
		volume&=0x0F;
		return volume*MAX_AMPLITUDE/16;
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
inline unsigned int AY38910::EnvelopeFreqX1000(void) const
{
	unsigned int D=state.regs[REG_ENV_FREQ_HIGH];
	D<<=8;
	D|=state.regs[REG_ENV_FREQ_LOW];
	if(0<D)
	{
		unsigned int freq=FREQ_CONST*1000/256;
		freq/=D;
		return freq;
	}
	return 0;
}

const uint8_t AY38910::envPtn[16][4]
{
	{ENV_DOWN,ENV_ZERO,ENV_KEEP,ENV_KEEP},
	{ENV_DOWN,ENV_ZERO,ENV_KEEP,ENV_KEEP},
	{ENV_DOWN,ENV_ZERO,ENV_KEEP,ENV_KEEP},
	{ENV_DOWN,ENV_ZERO,ENV_KEEP,ENV_KEEP},

	{ENV_UP,  ENV_ZERO,ENV_KEEP,ENV_KEEP},
	{ENV_UP,  ENV_ZERO,ENV_KEEP,ENV_KEEP},
	{ENV_UP,  ENV_ZERO,ENV_KEEP,ENV_KEEP},
	{ENV_UP,  ENV_ZERO,ENV_KEEP,ENV_KEEP},

	{ENV_DOWN,ENV_REPT,ENV_KEEP,ENV_KEEP},

	{ENV_DOWN,ENV_ZERO,ENV_KEEP,ENV_KEEP},

	{ENV_DOWN,ENV_UP,  ENV_REPT,ENV_KEEP},

	{ENV_DOWN,ENV_UP,  ENV_REPT,ENV_KEEP},

	{ENV_DOWN,ENV_ONE, ENV_KEEP,ENV_KEEP},

	{ENV_UP,  ENV_REPT,ENV_KEEP,ENV_KEEP},

	{ENV_UP,  ENV_ONE, ENV_KEEP,ENV_KEEP},

	{ENV_UP,  ENV_ZERO,ENV_KEEP,ENV_KEEP},
};

void AY38910::StartEnvelopeSegment(void)
{
	state.envPhase=0;
	state.envPeriodBalance=0;
	switch(envPtn[GetEnvelopePatternType()][state.envPatternSeg])
	{
	case ENV_UP:
	case ENV_ZERO:
		state.envOut=0;
		break;
	case ENV_DOWN:
	case ENV_ONE:
		state.envOut=ENV_OUT_MAX;
		break;
	}
}

inline unsigned int AY38910::GetEnvelopePatternType(void) const
{
	return state.regs[REG_ENV_PATTERN]&0x0F;
}

void AY38910::AddWaveAllChannelsForNumSamples(unsigned char data[],unsigned long long int numSamples)
{
	unsigned int halfTonePeriodX10[3]={0,0,0};
	for(int ch=0; ch<3; ++ch)
	{
		if(0==(state.regs[REG_MIXER]&(1<<ch)))
		{
			unsigned int toneFreqX1000=ChannelFrequencyX1000(ch);
			if(0<toneFreqX1000)
			{
				unsigned int tonePeriodX10=WAVE_SAMPLING_RATE*10000/toneFreqX1000;
				halfTonePeriodX10[ch]=tonePeriodX10/2;
			}
		}
	}

	unsigned int envPeriodX10=0;
	unsigned int envType=GetEnvelopePatternType();
	{
		unsigned int envFreqX1000=EnvelopeFreqX1000();
		if(0<envFreqX1000)
		{
			envPeriodX10=WAVE_SAMPLING_RATE*10000/envFreqX1000;
		}
	}

	for(unsigned long long int i=0; i<numSamples; ++i)
	{
		auto dataPtr=data+i*4; // 16-bit stereo
		for(int ch=0; ch<3; ++ch)
		{
			if(0<halfTonePeriodX10[ch])
			{
				if(0==(state.regs[REG_MIXER]&(1<<ch)))
				{
					if(state.ch[ch].tonePeriodBalance<10)
					{
						state.ch[ch].toneSign=1-state.ch[ch].toneSign;
						state.ch[ch].tonePeriodBalance+=halfTonePeriodX10[ch];
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
				}
			}
		}

		// Like drawing a line from (0,0)-(envPeriod,ENV_OUT_MAX) with DDA.
		state.envPeriodBalance+=(ENV_OUT_MAX*10);
		if(envPeriodX10<=state.envPeriodBalance)
		{
			state.envPeriodBalance-=envPeriodX10;
			if(ENV_UP==envPtn[envType][state.envPatternSeg])
			{
				++state.envOut;
			}
			if(ENV_DOWN==envPtn[envType][state.envPatternSeg] && 0<state.envOut)
			{
				--state.envOut;
			}
		}
		state.envPhase+=10;
		if(envPeriodX10<=state.envPhase)
		{
			if(ENV_KEEP!=envPtn[envType][state.envPatternSeg])
			{
				state.envPatternSeg=(state.envPatternSeg+1)&3;
				if(ENV_REPT==envPtn[envType][state.envPatternSeg])
				{
					state.envPatternSeg=0;
				}
				StartEnvelopeSegment();
			}
		}

		*(uint16_t *)(dataPtr+2)=*(uint16_t *)(dataPtr);
	}
}
