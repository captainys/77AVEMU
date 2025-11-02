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
#include "vgmrecorder.h"
#include "outside_world.h"

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
		// OK, now I introduced Register-Write Scheduling, which should solve the problem.
		MILLISEC_PER_WAVE_GENERATION=5,

		RINGBUFFER_CLEAR_TIME=1000000000, // Play this long after the last play-back to make sure ring buffer is clear.
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

	class YM_PLUS_AY
	{
	public:
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

		uint8_t vgm_reg=VGMRecorder::REG_YM2203;

		void Reset(void);
	};

	class State
	{
	public:
		bool mute=false;
		bool enable_whg=false;

		YM_PLUS_AY ym,whg;

		uint8_t beepState=BEEP_OFF;
		uint64_t beepStopTime=0;
		uint32_t beepTimeBalance=0;
		uint8_t beepWaveOut=0;

		void PowerOn(void);
		void Reset(void);
		void ResetVariables(void);
	};
	State state;


	class Variable
	{
	public:
		bool vgmRecordingArmed=false;
		VGMRecorder vgmRecorder;
	};
	Variable var;



	bool recordAudio=false;
	std::vector <unsigned char> audioRecording;
	uint8_t ym2203cRegisterMonitor[256];
	uint8_t ay38910RegisterMonitor[256];

	int64_t ringBufferClearTimeLeft=0;
	uint64_t nextWaveFilledInMillisec=0;
	uint64_t nextWaveGenTime=0;
	std::vector <unsigned char> nextWave;

	inline bool IsFMPlaying(void) const
	{
		return 0!=state.ym.ym2203c.state.playingCh || 0<state.ym.ym2203c.regWriteSched.size();
	}
	inline bool IsPSGPlaying(void) const
	{
		return state.ym.ay38910.IsPlaying();
	}
	inline bool IsWHG_FMPlaying(void) const
	{
		return 0!=state.whg.ym2203c.state.playingCh || 0<state.whg.ym2203c.regWriteSched.size();
	}
	inline bool IsWHG_SSGPlaying(void) const
	{
		return state.whg.ay38910.IsPlaying();
	}

	FM77AVSound(class FM77AV *fm77avPtr);

	virtual void PowerOn(void);
	virtual void Reset(void);

	void YMWriteControl(YM_PLUS_AY &ym,uint8_t data);

	void IOWrite(unsigned int ioport,unsigned int data);
	void ProcessFMWrite(YM_PLUS_AY &ym,unsigned char reg,unsigned char value);
	void ProcessPSGWrite(unsigned char reg,unsigned char value);

	unsigned int IORead(unsigned int ioport);
	uint8_t NonDestructiveIOReadByte(unsigned int ioport) const;

	/*! Called from FM77AV::RunFastDevicePolling.
	*/
	void SoundPolling(uint64_t FM77AVTime);

	std::vector <std::string> GetStatusText(void) const;

	/*! Call this function periodically to continue sound playback.
	*/
	void ProcessSound(Outside_World::Sound *soundPtr);

	/*! Call this function while VM is paused.
	*/
	void ProcessSilence(class Outside_World::Sound *outside_world);

	void StartRecording(void);
	void EndRecording(void);
	void SaveRecording(std::string fName) const;

	void ArmVGMRecording(void);
	void StartVGMRecording(void);
	void EndVGMRecording(void);
	void TrimVGMRecording(void);
	bool SaveVGMRecording(std::string fName) const;

	void SerializeYM2203CFMPart(std::vector <unsigned char> &data,const YM_PLUS_AY &ym) const;
	void DeserializeYM2203CFMPart(const unsigned char *&data,unsigned int version,YM_PLUS_AY &ym);
	void SerializeAY38910(std::vector <unsigned char> &data,const YM_PLUS_AY &ym) const;
	void DeserializeAY38910(const unsigned char *&data,unsigned int version,YM_PLUS_AY &ym);

	virtual uint32_t SerializeVersion(void) const;
	virtual void SpecificSerialize(std::vector <unsigned char> &data,std::string stateFName) const;
	virtual bool SpecificDeserialize(const unsigned char *&data,std::string stateFName,uint32_t version);
};


/* } */
#endif
