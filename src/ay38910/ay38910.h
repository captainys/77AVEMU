#ifndef AY38910_IS_INCLUDED
#define AY38910_IS_INCLUDED
/* { */



#include <stdint.h>
#include <vector>

class AY38910
{
public:
	enum
	{
		NUM_CHANNELS=3,
		NUM_REGS=16,
		REG_MASK=NUM_REGS-1,
		FREQ_CONST=1228800,  // FM-Techknow pp.230, 200000 for PC88?
		MAX_AMPLITUDE=1024,

		ENV_OUT_MAX=256,
		LFSR_CYCLE=255,

		WAVE_SAMPLING_RATE=44100,  // Must be equal from ym2612.h

		REG_CH0_FREQ_LOW=0,
		REG_CH0_FEEQ_HIGH=1,
		REG_CH1_FREQ_LOW=2,
		REG_CH1_FEEQ_HIGH=3,
		REG_CH2_FREQ_LOW=4,
		REG_CH2_FEEQ_HIGH=5,
		REG_NOISE_FREQ=6,
		REG_MIXER=7,
		REG_CH0_VOL=8,
		REG_CH1_VOL=9,
		REG_CH2_VOL=10,
		REG_ENV_FREQ_LOW=11,
		REG_ENV_FREQ_HIGH=12,
		REG_ENV_PATTERN=13,
		REG_PORTA=14,
		REG_PORTB=15,
	};
	enum
	{
		ENV_UP,
		ENV_DOWN,
		ENV_ZERO,
		ENV_ONE,
		ENV_REPT,
		ENV_KEEP
	};

	class Channel
	{
	public:
		unsigned int toneSign=0;
		unsigned int tonePeriodBalance=0;
	};
	class State
	{
	public:
		uint8_t regs[NUM_REGS];
		Channel ch[NUM_CHANNELS];
		unsigned int envPhase=0;
		unsigned int envOut=0;
		unsigned int envPeriodBalance=0;
		unsigned int envPatternSeg=0;  // 0 to 3.  Index to envPtn[PTN].

		unsigned char LFSR=1;
		unsigned int noisePeriodBalance=0;
	};
	State state;

	static const uint8_t envPtn[16][4];

	AY38910();
	void Reset(void);
	uint8_t Read(uint8_t reg) const;
	void Write(uint8_t reg,uint8_t value);

	inline bool IsPlaying(void) const
	{
		return (state.regs[REG_MIXER]&0x3F)!=0x3F;
	}

	inline unsigned int GetF_NUM(int ch) const;
	inline unsigned int ChannelFrequencyX1000(int ch) const;
	inline unsigned int GetAmplitude(int ch) const;
	inline unsigned int EnvelopeFreqX1000(void) const;
	inline unsigned int GetEnvelopePatternType(void) const;
	inline unsigned int NoiseFreqX1000(void) const;
	inline void MoveLFSR(void);
	void StartEnvelopeSegment(void);

	std::vector <unsigned char> MakeWaveAllChannels(unsigned long long int millisec);
	void AddWaveAllChannelsForNumSamples(unsigned char data[],unsigned long long int numSamples);
};

/* } */
#endif
