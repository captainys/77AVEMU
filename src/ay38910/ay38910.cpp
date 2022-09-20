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
	for(auto &b : regCache)
	{
		b=0;
	}
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
	state.LFSR=1;
	state.preScaler=2;

}
uint8_t AY38910::ReadRegister(uint8_t reg) const
{
	return state.regs[reg];
}
void AY38910::WriteRegister(uint8_t reg,uint8_t value,uint64_t vmTime)
{
	state.regs[reg]=value;
	if(REG_ENV_PATTERN==reg)
	{
		state.envPatternSeg=0;
		StartEnvelopeSegment();
	}

	if(true==takeRegisterLog)
	{
		RegWriteLog l;
		l.t=vmTime;
		l.reg=reg;
		l.value=value;
		registerLog.push_back(l);
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
		freq=freq*PRESCALING_DEFAULT/state.preScaler;
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
		volume=state.envOut*16/ENV_OUT_MAX;
		if(16<=volume)
		{
			volume=15;
		}
		return DAConvOut[volume];
	}
	else
	{
		return DAConvOut[volume&0x0F];
	}
}

std::vector <unsigned char> AY38910::MakeWaveAllChannels(unsigned long long int millisec,uint64_t lastWaveGenTime)
{
	std::vector <unsigned char> wave;

	unsigned long long int numSamples=(millisec<<12);
	numSamples*=WAVE_SAMPLING_RATE;
	numSamples/=1000;
	numSamples>>=12;

	wave.resize(4*numSamples);
	memset(wave.data(),0,wave.size());

	AddWaveAllChannelsForNumSamples(wave.data(),numSamples,lastWaveGenTime);
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
		freq=freq*PRESCALING_DEFAULT/state.preScaler;
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

	{ENV_DOWN,ENV_ONE, ENV_KEEP,ENV_KEEP},

	{ENV_UP,  ENV_REPT,ENV_KEEP,ENV_KEEP},

	{ENV_UP,  ENV_ONE, ENV_KEEP,ENV_KEEP},

	{ENV_UP  ,ENV_DOWN,ENV_REPT,ENV_KEEP},

	{ENV_UP,  ENV_ZERO,ENV_KEEP,ENV_KEEP},
};

const unsigned int AY38910::DAConvOut[16]=
{
	// Fig.3 of AY-3-8910 data sheet implies it is C*pow((sqrt(2)),linear)
	// Values obtained by the following code.
	//#include <math.h>
	//#include <stdio.h>
	//int main(void)
	//{
	//	double C=1.0/pow(sqrt(2.0),15);
	//	for(int i=0; i<16; ++i)
	//	{
	//		double o=C*pow(sqrt(2),i);
	//		printf("%d %lf\n",i,65535.0*o);
	//	}
	//	return 0;
	//}
	AY38910::MAX_AMPLITUDE*0/65535,
	AY38910::MAX_AMPLITUDE*511/65535,
	AY38910::MAX_AMPLITUDE*724/65535,
	AY38910::MAX_AMPLITUDE*1023/65535,
	AY38910::MAX_AMPLITUDE*1448/65535,
	AY38910::MAX_AMPLITUDE*2047/65535,
	AY38910::MAX_AMPLITUDE*2896/65535,
	AY38910::MAX_AMPLITUDE*4095/65535,
	AY38910::MAX_AMPLITUDE*5792/65535,
	AY38910::MAX_AMPLITUDE*8191/65535,
	AY38910::MAX_AMPLITUDE*11585/65535,
	AY38910::MAX_AMPLITUDE*16383/65535,
	AY38910::MAX_AMPLITUDE*23170/65535,
	AY38910::MAX_AMPLITUDE*32767/65535,
	AY38910::MAX_AMPLITUDE*46340/65535,
	AY38910::MAX_AMPLITUDE*65535/65535,
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

inline unsigned int AY38910::NoiseFreqX1000(void) const
{
	unsigned int F=1+(state.regs[REG_NOISE_FREQ]&0x1F);
	unsigned int freq=FREQ_CONST*1000/16;
	freq/=F;
	return freq;
}

inline void AY38910::MoveLFSR(void)
{
	// Experiment showed the following taps give 255 period for 8-bit LFSR.
	// I don't know which one is used by AY-3-8910.
	// I just randomly pick one.
	// 142
	// 149
	// 150
	// 166
	// 175
	// 177
	// 178
	// 180
	// 184
	// 195
	// 198
	// 212
	// 225
	// 231
	// 243
	// 250
	// Implementation based on wikipedia.
	const unsigned char taps=142;
	state.LFSR=(state.LFSR>>1)^(-(state.LFSR&1)&taps);
}

inline uint32_t AY38910::LFSR24(uint32_t lfsr) const
{
	unsigned int bit=((lfsr>>(24-24))^(lfsr>>(24-23))^(lfsr>>(24-22))^(lfsr>>(24-17)))&1;
	lfsr=(lfsr>>1)|(bit<<23);
	return lfsr;
}

class AY38910::PlaybackParam
{
public:
	uint32_t halfTonePeriodX1000[3]={0,0,0};
	uint32_t envPeriodX1000=0;
	uint32_t envType=0;
	uint32_t noisePeriodX1000=0;

};

class AY38910::WithScheduler
{
public:
	unsigned int schedPtr=0;
	unsigned int balance=0;
	unsigned int numSamples=0;
	unsigned int totalNanosec=0;

	inline void BeginMakeWave(AY38910 *ay,uint64_t numSamples)
	{
		for(int i=0; i<AY38910::NUM_REGS; ++i)
		{
			ay->state.regs[i]=ay->regCache[i];
		}
		totalNanosec=1000000000*numSamples/AY38910::WAVE_SAMPLING_RATE;
		this->numSamples=numSamples;
	}
	inline void Increment(AY38910 *ay,PlaybackParam &pp,uint64_t count,uint64_t lastWaveGenTime)
	{
		// 44100Hz.  1sample=22675nanosec=23us.
		// Divide it rather than DDA it.
		if(schedPtr<ay->regWriteSched.size() && 0<numSamples)
		{
			auto nanosec=count;
			nanosec*=totalNanosec;
			nanosec/=numSamples;
			nanosec+=lastWaveGenTime;
			while(schedPtr<ay->regWriteSched.size() && ay->regWriteSched[schedPtr].t<=nanosec)
			{
				auto &sched=ay->regWriteSched[schedPtr];
				ay->WriteRegister(sched.reg,sched.value,sched.t);
				ay->RecalculatePlaybackParam(pp);
				++schedPtr;
			}
		}
	}
	inline void EndMakeWave(AY38910 *ay)
	{
		while(schedPtr<ay->regWriteSched.size())
		{
			auto &sched=ay->regWriteSched[schedPtr];
			ay->WriteRegister(sched.reg,sched.value,sched.t);
			++schedPtr;
		}
		ay->regWriteSched.clear();
		for(int i=0; i<AY38910::NUM_REGS; ++i)
		{
			ay->regCache[i]=ay->state.regs[i];
		}
	}
};

class AY38910::WithoutScheduler
{
public:
	static inline void BeginMakeWave(AY38910 *ay,uint64_t numSamples)
	{
	}
	static inline void Increment(AY38910 *ay,PlaybackParam &pp,uint64_t count,uint64_t lastWaveGenTime)
	{
	}
	static inline void EndMakeWave(AY38910 *ay)
	{
	}
};

void AY38910::AddWaveAllChannelsForNumSamples(unsigned char data[],unsigned long long int numSamples,uint64_t systemTimeInNS)
{
	if(true==useScheduling)
	{
		AddWaveAllChannelsForNumSamplesTemplate<WithScheduler>(data,numSamples,systemTimeInNS);
	}
	else
	{
		AddWaveAllChannelsForNumSamplesTemplate<WithoutScheduler>(data,numSamples,systemTimeInNS);
	}
}

template <class SCHEDULER>
void AY38910::AddWaveAllChannelsForNumSamplesTemplate(unsigned char data[],unsigned long long int numSamples,uint64_t lastWaveGenTime)
{
	SCHEDULER scheduler;
	PlaybackParam pp;
	scheduler.BeginMakeWave(this,numSamples);
	RecalculatePlaybackParam(pp);

	for(unsigned long long int i=0; i<numSamples; ++i)
	{
		scheduler.Increment(this,pp,i,lastWaveGenTime);

		auto dataPtr=data+i*4; // 16-bit stereo
		for(int ch=0; ch<3; ++ch)
		{
			if(0<pp.halfTonePeriodX1000[ch])
			{
				if(0==(state.regs[REG_MIXER]&(1<<ch)))
				{
					state.ch[ch].tonePeriodBalance+=1000;
					if(pp.halfTonePeriodX1000[ch]<=state.ch[ch].tonePeriodBalance)
					{
						state.ch[ch].toneSign=1-state.ch[ch].toneSign;
						state.ch[ch].tonePeriodBalance-=pp.halfTonePeriodX1000[ch];
					}

					int ampl=GetAmplitude(ch);
					if(0==state.ch[ch].toneSign)
					{
						ampl=-ampl;
					}

					if(true!=channelMute[ch])
					{
						WordOp_Add(dataPtr  ,ampl);
						WordOp_Add(dataPtr+2,ampl);
					}
				}
			}
			if(0==(state.regs[REG_MIXER]&(8<<ch)) && 0<pp.noisePeriodX1000)
			{
				if(true!=channelMute[ch])
				{
					int ampl=GetAmplitude(ch);
					if(0!=(state.LFSR&1))
					{
						ampl=-ampl;
					}
					WordOp_Add(dataPtr  ,ampl);
					WordOp_Add(dataPtr+2,ampl);
				}
			}
		}

		// Like drawing a line from (0,0)-(envPeriod,ENV_OUT_MAX) with DDA.
		state.envPeriodBalance+=(ENV_OUT_MAX*1000);
		if(pp.envPeriodX1000<=state.envPeriodBalance)
		{
			state.envPeriodBalance-=pp.envPeriodX1000;
			if(ENV_UP==envPtn[pp.envType][state.envPatternSeg])
			{
				++state.envOut;
			}
			if(ENV_DOWN==envPtn[pp.envType][state.envPatternSeg] && 0<state.envOut)
			{
				--state.envOut;
			}
		}

		state.envPhase+=1000;
		if(pp.envPeriodX1000<=state.envPhase)
		{
			if(ENV_KEEP!=envPtn[pp.envType][state.envPatternSeg])
			{
				state.envPatternSeg=(state.envPatternSeg+1)&3;
				if(ENV_REPT==envPtn[pp.envType][state.envPatternSeg])
				{
					state.envPatternSeg=0;
				}
				StartEnvelopeSegment();
			}
		}

		if(0!=pp.noisePeriodX1000)
		{
			state.noisePeriodBalance+=1000;
			while(pp.noisePeriodX1000<=state.noisePeriodBalance)
			{
				state.noisePeriodBalance-=pp.noisePeriodX1000;
				state.LFSR=LFSR24(state.LFSR);
			}
		}
	}

	scheduler.EndMakeWave(this);
}

void AY38910::RecalculatePlaybackParam(PlaybackParam &pp)
{
	for(int ch=0; ch<3; ++ch)
	{
		pp.halfTonePeriodX1000[ch]=0;
		if(0==(state.regs[REG_MIXER]&(1<<ch)))
		{
			uint64_t toneFreqX1000=ChannelFrequencyX1000(ch);
			if(0<toneFreqX1000)
			{
				uint64_t tonePeriodX1000=WAVE_SAMPLING_RATE;
				tonePeriodX1000*=1000000;
				tonePeriodX1000/=toneFreqX1000;
				pp.halfTonePeriodX1000[ch]=uint32_t(tonePeriodX1000/2);
			}
		}
	}

	pp.envPeriodX1000=0;
	pp.envType=GetEnvelopePatternType();
	{
		uint64_t envFreqX1000=EnvelopeFreqX1000();
		if(0<envFreqX1000)
		{
			uint64_t periodX1000=WAVE_SAMPLING_RATE;
			periodX1000*=1000000;
			periodX1000/=envFreqX1000;
			pp.envPeriodX1000=uint32_t(periodX1000);
		}
	}

	pp.noisePeriodX1000=0;
	if(0b00111000!=(state.regs[REG_MIXER]&0b00111000))
	{
		uint64_t noiseFreqX1000=NoiseFreqX1000();
		if(0<noiseFreqX1000)
		{
			uint64_t periodX1000=WAVE_SAMPLING_RATE;
			periodX1000*=1000000;
			periodX1000/=noiseFreqX1000;
			pp.noisePeriodX1000=uint32_t(periodX1000);
		}
	}
}

std::vector <std::string> AY38910::GetStatusText(void) const
{
	std::vector <std::string> text;

	text.push_back("AY-3-8910");

	text.push_back("    ");
	for(int i=0; i<16; ++i)
	{
		text.back()+=cpputil::Ubtox(i);
		text.back()+=" ";
	}

	text.push_back("REG ");
	for(auto r : state.regs)
	{
		text.back()+=cpputil::Ubtox(r);
		text.back()+=" ";
	}

	for(int ch=0; ch<NUM_CHANNELS; ++ch)
	{
		text.push_back("");
		text.back()+="F_NUM ";
		text.back()+=cpputil::Ustox(GetF_NUM(ch));
	}

	return text;
}

std::vector <std::string> AY38910::FormatRegisterLog(void) const
{
	std::vector <std::string> text;
	if(0<registerLog.size())
	{
		auto t0=registerLog[0].t;
		for(auto l : registerLog)
		{
			std::cout << (l.t-t0) << " $" << cpputil::Ubtox(l.reg) << " $" << cpputil::Ubtox(l.value) << std::endl;
		}
	}
	return text;
}

////////////////////////////////////////////////////////////

void AY38910::WriteRegisterSchedule(unsigned int reg,unsigned int value,uint64_t systemTimeInNS)
{
	RegWriteLog rwl;
	rwl.reg=reg;
	rwl.value=value;
	rwl.t=systemTimeInNS;
	regWriteSched.push_back(rwl);

	// FM-8 games may be checking presence of AY38910 by writing to the register and reading it back.
	// The change of the value needs to be visible immediately.
	state.regs[reg]=value;
}
void AY38910::FlushRegisterSchedule(void)
{
	for(auto sched : regWriteSched)
	{
		WriteRegister(sched.reg,sched.value,sched.t);
	}
	regWriteSched.clear();
}
