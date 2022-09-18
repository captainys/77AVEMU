#ifndef FM77AVSOUND_IS_INCLUDED
#define FM77AVSOUND_IS_INCLUDED
/* { */



#include <stdint.h>
#include <vector>
#include <string>
#include "device.h"
#include "ay38910.h"
#include "ym2612.h"
#include "fm77avdef.h"

class FM77AVSound : public Device
{
public:
	enum
	{
#if !defined(__linux__) && !defined(__linux)
		MILLISEC_PER_WAVE=20, // Looks like time resolution of Wave Playback of Direct Sound is 10ms.
		// And it needs to be 10 times integer.
#else
		MILLISEC_PER_WAVE=40, // Maybe because I am developing on VirtualBox, I am getting outrageously slow latency of 80ms (40ms*2).
#endif
		// 2ms resolution is needed for "What?" sound of Xanadu.
		// Silpheed "Presented by Game Arts" requires higher time resolution.  Will need to go down to 500us.
		MILLISEC_PER_WAVE_GENERATION=1,
	};

	virtual const char *DeviceName(void) const{return "SOUND";}

	enum
	{
		BEEP_OFF,
		BEEP_ONE_SHOT,
		BEEP_CONTINUOUS,

		SINGLE_BEEP_DURATION=210000000, // 0.21 seconds from FM-7.
		BEEP_SOUND_AMPLITUDE=2048,
	};
	enum
	{
		REG_GAMEPORTENABLE=7,
		REG_PORTA=14,
		REG_PORTB=15,
	};

	class State
	{
	public:
		bool mute=false;

		YM2612 ym2203c;  // Will use Tsugaru-Ben for emulate YM2203C.
		uint8_t ym2203cCommand=0;
		uint8_t ym2203cDataRead=0;
		uint8_t ym2203cDataWrite=0;
		unsigned int ym2203cAddrLatch=0;

		AY38910 ay38910;
		uint8_t ay38910regMode=0; // 0:High Impedance  1:Data Read  2:Data Write  3:AddrLatch
		uint8_t ay38910AddrLatch=0;
		uint8_t ay38910LastControl=0;
		uint8_t ay38910LastData=0;

		uint8_t beepState=BEEP_OFF;
		uint64_t beepStopTime=0;
		uint32_t beepTimeBalance=0;
		uint8_t beepWaveOut=0;

		void PowerOn(void);
		void Reset(void);
		void ResetVariables(void);
	};
	State state;



	bool recordAudio=false;
	std::vector <unsigned char> audioRecording;
	uint8_t ym2203cRegisterMonitor[256];
	uint8_t ay38910RegisterMonitor[AY38910::NUM_REGS];

	uint64_t nextWaveFilledInMillisec=0;
	uint64_t nextWaveGenTime=0;
	std::vector <unsigned char> nextWave;

	inline bool IsFMPlaying(void) const
	{
		return 0!=state.ym2203c.state.playingCh;
	}
	inline bool IsPSGPlaying(void) const
	{
		return state.ay38910.IsPlaying();
	}

	FM77AVSound(class FM77AV *fm77avPtr);

	virtual void PowerOn(void);
	virtual void Reset(void);

	virtual void IOWriteByte(unsigned int ioport,unsigned int data);

	virtual unsigned int IOReadByte(unsigned int ioport);
	uint8_t NonDestructiveIOReadByte(unsigned int ioport) const;

	/*! Called from FM77AV::RunFastDevicePolling.
	*/
	void SoundPolling(uint64_t FM77AVTime);

	std::vector <std::string> GetStatusText(void) const;

	/*! Call this function periodically to continue sound playback.
	*/
	void ProcessSound(class Outside_World *outside_world);

	void StartRecording(void);
	void EndRecording(void);
	void SaveRecording(std::string fName) const;


	void SerializeYM2203CFMPart(std::vector <unsigned char> &data) const;
	void DeserializeYM2203CFMPart(const unsigned char *&data,unsigned int version);
	void SerializeAY38910(std::vector <unsigned char> &data) const;
	void DeserializeAY38910(const unsigned char *&data,unsigned int version);

	virtual uint32_t SerializeVersion(void) const;
	virtual void SpecificSerialize(std::vector <unsigned char> &data,std::string stateFName) const;
	virtual bool SpecificDeserialize(const unsigned char *&data,std::string stateFName,uint32_t version);
};


/* } */
#endif
