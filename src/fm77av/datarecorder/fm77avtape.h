#ifndef FM77AVTAPE_IS_INCLUDED
#define FM77AVTAPE_IS_INCLUDED
/* { */



#include <stdint.h>
#include <string>
#include <fstream>
#include <vector>
#include "device.h"
#include "t77.h"
#include "fm7lib.h"

class FM77AVTape
{
public:
	std::string fName;
	char header[18];
	std::vector <unsigned char> data;
	bool writeProtect=false;

	mutable bool modified=false;
	uint64_t lastModifiedTime=0;

	bool Load(std::string fName);
	bool Save(void) const;
	bool SaveAs(std::string fName) const;

	enum
	{
		NANOSEC_PER_T77_ONE=9000
	};

	class
	{
	};

	class TapeFile
	{
	public:
		std::string fName;
		unsigned int fType;
		uint64_t ptr;
	};

	class TapePointer
	{
	public:
		unsigned int dataPtr=0;
		uint64_t fm77avTime=0;   // In nano sec.
		bool eot=false;
	};

	void Eject(void);
	// T77 format that I remembered from my utility:
	//   - Data contains two-byte pairs.
	//       Level Duration(ms)
	//   - For long silence, it can be
	//       0x7F 0xFF
	void Rewind(TapePointer &ptr) const;
	void Dniwer(TapePointer &ptr) const;
	void Seek(TapePointer &ptr,unsigned int dataPtr) const;
	void MotorOn(TapePointer &pointer,uint64_t fm77avTimeNanoSec) const; // First byte=16
	void MoveTapePointer(TapePointer &pointer,uint64_t fm77avTimeNanoSec) const;
	uint8_t GetLevel(TapePointer tracker) const;

	std::vector <TapeFile> Files(void) const;
};

class FM77AVDataRecorder : public Device
{
public:
	virtual const char *DeviceName(void) const{return "FMDataRecorder";}

	enum
	{
		HEAD_SILENT_TIME=1000000000, // 1.0sec
	};

	class TapePointerPair
	{
	public:
		FM77AVTape t77;
		FM77AVTape::TapePointer ptr;

		void Rewind(void);
		void Dniwer(void);
	};

	class State
	{
	public:
		TapePointerPair primary,toSave;
		bool motor;
		bool recButton=false;
		bool writeData=false;

		uint64_t motorOnTime=0;
		uint64_t lastBitFlipTime=0;
	};
	State state;
	class Outside_World *outside_world=nullptr;

	FM77AVDataRecorder(VMBase *vmPtr) : Device(vmPtr){}

	void Reset(void);  // Don't rewind
	bool LoadT77(std::string fName);
	bool LoadAutoSaveT77(std::string fName);
	void Eject(void);
	void MotorOn(uint64_t fm77avTime);
	void MotorOff(uint64_t fm77avTime);
	void Move(uint64_t fm77avTime);
	bool Read(void) const;

	void Rewind(void);
	void FastForwardAllTheWay(void);

	void WriteBit(TapePointerPair &tape,uint64_t fm77avTime);

	void WriteFD00(uint64_t fm77avTime,uint8_t data);

	void SaveModifiedTapeImagesAfterOneSecond(uint64_t fm77avTime);
	void SaveModifiedTapeImages(void);

	std::vector <std::string> GetStatusText(uint64_t fm77avTime) const;

	/* virtual */ uint32_t SerializeVersion(void) const;
	/* virtual */ void SpecificSerialize(std::vector <unsigned char> &data,std::string stateFName) const;
	/* virtual */ bool SpecificDeserialize(const unsigned char *&data,std::string stateFName,uint32_t version);
};



/* } */
#endif
