#ifndef FM77AVFDC_IS_INCLUDED
#define FM77AVFDC_IS_INCLUDED
/* { */

#include "diskdrive.h"

class FM77AVFDC : public DiskDrive
{
public:
	enum
	{
		// 2D/2DD 300rpm.  Confirmed on actual FM77AV.  (300 index-hole detection for 60.95 seconds)
		// 5 rotation per seconds.
		// 200 ms per rotation.
		// Track Read returns 0x17BB bytes (6075 bytes) on actual hardwarre.
		// 200 ms/6075=32 microseconds per byte.
		NANOSEC_PER_BYTE=32000,
		NANOSEC_BETWEEN_MULTI_SECTOR_READ=3200000, // Just multiplied by 100.  Let's see how it works.

		FORMAT_WRITE_LENGTH_2D_2DD=6144,
		FORMAT_WRITE_LENGTH_2HD_1232KB=10416,
		FORMAT_WRITE_LENFTH_2HD_1440KB=12934,

		INDEXHOLE_INTERVAL=200000000, // 300rpm.  5 rotations per second.  200ms per index hole.
		INDEXHOLE_DURATION=20000,
	};
	class FM77AV *fm77avPtr;

	bool debugBreakOnCommandWrite=false;
	bool monitorFDC=false;

	enum
	{
		FDD_DRIVE_TYPE_2DD=0,
		FDD_DRIVE_TYPE_2D=1
	};

	class FM7SpecificState
	{
	public:
		uint8_t driveMode=FDD_DRIVE_TYPE_2DD;
		bool enableDriveMap=false;
		uint8_t driveMapping[4] {0,1,2,3};
		uint8_t currentDS=0;
		uint8_t lastLogicalDriveWritten=0;
	};
	FM7SpecificState state7;

	virtual const char *DeviceName(void) const{return "FDC";}

	FM77AVFDC(class FM77AV *fm77avPtr);

	void SendCommand(unsigned int cmd);

	/*! Turns off BUSY flag.  Also if IRQ is not masked it raises IRR flag of PIC.
	*/
	void MakeReady(void);

	virtual void RunScheduledTask(unsigned long long int fm77avTime);
	virtual void IOWriteByte(unsigned int ioport,unsigned int data);
	virtual unsigned int IOReadByte(unsigned int ioport);

	unsigned int NonDestructiveIORead(unsigned int ioport) const;

	bool IndexHole(unsigned long long fm77avTime) const;

	void Reset(void) override;
	void WriteTrack(const std::vector <uint8_t> &data);
	inline bool has2DD(void) const;
	unsigned int compensateTrackNumber(unsigned int trackPos);
	inline unsigned int mapDrive(unsigned int logicalDrive) const;


	uint32_t SerializeVersion(void) const override;
	void SpecificSerialize(std::vector <unsigned char> &data,std::string stateFName) const override;
	bool SpecificDeserialize(const unsigned char *&data,std::string stateFName,uint32_t version) override;
};

/* } */
#endif
