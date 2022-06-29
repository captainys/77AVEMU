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
		unsigned int dataPtr=16;           // First byte=16.
		unsigned int microSecLeft=0; // Micro seconds left for this byte pair.
		uint64_t fm77avTime=0;   // In nano sec.
		bool eot=false;
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

class FM77AVDataRecorder
{
public:
	virtual const char *DeviceName(void) const{return "FMDataRecorder";}

	class State
	{
	public:
		FM77AVTape t77;
		FM77AVTape::TapePointer ptr;
		bool motor;
	};
	State state;

	void Reset(void);  // Don't rewind
	bool LoadT77(std::string fName);
	void MotorOn(uint64_t fm77avTime);
	void MotorOff(void);
	void Move(uint64_t fm77avTime);
	bool Read(void) const;
};



/* } */
#endif
