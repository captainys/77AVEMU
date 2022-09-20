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
		PRESCALING_DEFAULT=2,

		NUM_CHANNELS=3,
		NUM_REGS=16,
		REG_MASK=NUM_REGS-1,
		FREQ_CONST=1228800,  // FM-Techknow pp.230, 200000 for PC88?
		MAX_AMPLITUDE=4096,

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

	class RegWriteLog
	{
	public:
		uint64_t t;
		uint8_t reg,value;
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

		unsigned int preScaler=2;

		uint32_t LFSR=1;
		unsigned int noisePeriodBalance=0;
	};
	State state;

	bool channelMute[NUM_CHANNELS]={false,false,false};

	static const uint8_t envPtn[16][4];
	static const unsigned int DAConvOut[16];

	bool takeRegisterLog=false;
	std::vector <RegWriteLog> registerLog;

	AY38910();
	void Reset(void);
	uint8_t ReadRegister(uint8_t reg) const;
	void WriteRegister(uint8_t reg,uint8_t value,uint64_t vmTime);

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
	inline void MoveLFSR(void);  // Turned out 8-bit was too short for high-frequency noise.
	inline uint32_t LFSR24(uint32_t lfsr) const;
	void StartEnvelopeSegment(void);

	std::vector <unsigned char> MakeWaveAllChannels(unsigned long long int millisec,uint64_t lastWaveGentime);
	void AddWaveAllChannelsForNumSamples(unsigned char data[],unsigned long long int numSamples,uint64_t lastWaveGentime);
private:
	class WithScheduler;
	class WithoutScheduler;
	template <class SCHEDULER>
	void AddWaveAllChannelsForNumSamplesTemplate(unsigned char data[],unsigned long long int numSamples,uint64_t lastWaveGentime);

public:
	std::vector <std::string> GetStatusText(void) const;

	std::vector <std::string> FormatRegisterLog(void) const;



	/* Some PSG magicians bangs registers faster than the wave-generation interval.  In such situation,
	   AY38910 class needs to remember register-write log, and update as it generates wave.

	   Schedule must be flushed:
	     After every wave generation,
	     Before saving state.

	   To enable scheduling, 
	      (1) useScheduling=true;
	      (2) Use WriteRegisterSchedule instead of WriteRegister.
	      (3) VM needs to remember when the wave was generated for the last time, and pass it to MakeWaveForNSamples.
	*/
	bool useScheduling=false;
	uint8_t regCache[NUM_REGS];
	// AY registers are readable.  Some may be using register read to identify FM-7 and FM-8.
	// Therefore, register values need to be written to the registers immediately, but to
	// delay writing, it needs to remember the state of the registers at the end of last wave generation.
	std::vector <RegWriteLog> regWriteSched;
	void WriteRegisterSchedule(unsigned int reg,unsigned int value,uint64_t vmTime);
	void FlushRegisterSchedule(void);
};

/* } */
#endif
