#ifndef FM77AVSOUND_IS_INCLUDED
#define FM77AVSOUND_IS_INCLUDED
/* { */



#include <stdint.h>
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
		MILLISEC_PER_WAVE=10, // Looks like time resolution of Wave Playback of Direct Sound is 10ms.
#else
		MILLISEC_PER_WAVE=40, // Maybe because I am developing on VirtualBox, I am getting outrageously slow latency of 80ms (40ms*2).
#endif
	};

	virtual const char *DeviceName(void) const{return "SOUND";}

	class State
	{
	public:
		YM2612 ym2203c;  // Will use Tsugaru-Ben for emulate YM2203C.
		unsigned int ym2203cAddrLatch[2];

		AY38910 ay38910;
		uint8_t ay38910regMode=0; // 0:High Impedance  1:Data Read  2:Data Write  3:AddrLatch
		uint8_t ay38910AddrLatch=0;
		uint8_t ay38910LastControl=0;
		uint8_t ay38910LastData=0;

		void PowerOn(void);
		void Reset(void);
		void ResetVariables(void);
	};
	State state;



	bool recordAudio=false;
	std::vector <unsigned char> audioRecording;

	std::vector <unsigned char> nextWave;

	inline bool IsFMPlaying(void) const
	{
		return false; // Will come 0!=state.ym2612.state.playingCh;
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


	void SerializeYM2612(std::vector <unsigned char> &data) const;
	void DeserializeYM2612(const unsigned char *&data,unsigned int version);
	void SerializeAY3910(std::vector <unsigned char> &data) const;
	void DeserializeAY3910(const unsigned char *&data);

	virtual uint32_t SerializeVersion(void) const;
	virtual void SpecificSerialize(std::vector <unsigned char> &data,std::string stateFName) const;
	virtual bool SpecificDeserialize(const unsigned char *&data,std::string stateFName,uint32_t version);
};


/* } */
#endif
