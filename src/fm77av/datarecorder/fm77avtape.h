#ifndef FM77AVTAPE_IS_INCLUDED
#define FM77AVTAPE_IS_INCLUDED
/* { */



#include <stdint.h>
#include <string>
#include <fstream>
#include <vector>
#include "device.h"

class FM77AVTape
{
public:
	std::string fName;
	char header[16];
	std::vector <unsigned char> data;
	bool writeProtect=false;

	bool Load(std::string fName);
	bool Save(void) const;
	bool SaveAs(std::string fName) const;

	enum
	{
		MICROSEC_PER_T77_ONE=9
	};

	class TapePointer
	{
	public:
		unsigned int dataPtr=0;
		uint64_t fm77avTime=0;   // In nano sec.
		bool eot=false;
		bool writeProtect=false;
	};

	// T77 format that I remembered from my utility:
	//   - Data contains two-byte pairs.
	//       Level Duration(ms)
	//   - For long silence, it can be
	//       0x7F 0xFF
	void Rewind(TapePointer &ptr) const;
	void Seek(TapePointer &ptr,unsigned int dataPtr) const;
	void MotorOn(TapePointer &pointer,uint64_t fm77avTimeNanoSec) const; // First byte=16
	void MoveTapePointer(TapePointer &pointer,uint64_t fm77avTimeNanoSec) const;
	uint8_t GetLevel(TapePointer tracker) const;
};

class FM77AVDataRecorder : public Device
{
public:
	virtual const char *DeviceName(void) const{return "FMDataRecorder";}

	enum
	{
		HEAD_SILENT_TIME=1000000000, // 1.0sec
	};

	class State
	{
	public:
		FM77AVTape t77;
		FM77AVTape::TapePointer ptr;
		bool motor;
		bool recButton=false;
		bool writeData=false;

		uint64_t motorOnTime=0;
		uint64_t lastBitFlipTime=0;
	};
	State state;

	FM77AVDataRecorder(VMBase *vmPtr) : Device(vmPtr){}

	void Reset(void);  // Don't rewind
	bool LoadT77(std::string fName);
	void MotorOn(uint64_t fm77avTime);
	void MotorOff(uint64_t fm77avTime);
	void Move(uint64_t fm77avTime);
	bool Read(void) const;

	void Rewind(void);

	void WriteBit(uint64_t fm77avTime);

	void WriteFD00(uint64_t fm77avTime,uint8_t data);

	std::vector <std::string> GetStatusText(uint64_t fm77avTime) const;
};



/* } */
#endif
