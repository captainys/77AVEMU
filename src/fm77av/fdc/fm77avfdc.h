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
	};
	class FM77AV *fm77avPtr;

	bool debugBreakOnCommandWrite=false;
	bool monitorFDC=false;

	virtual const char *DeviceName(void) const{return "FDC";}

	FM77AVFDC(class FM77AV *fm77avPtr);

	void SendCommand(unsigned int cmd);

	/*! Turns off BUSY flag.  Also if IRQ is not masked it raises IRR flag of PIC.
	*/
	void MakeReady(void);

	virtual void RunScheduledTask(unsigned long long int townsTime);
	virtual void IOWriteByte(unsigned int ioport,unsigned int data);
	virtual unsigned int IOReadByte(unsigned int ioport);

	unsigned int NonDestructiveIORead(unsigned int ioport) const;

	virtual void Reset(void);
	void WriteTrack(const std::vector <uint8_t> &data);
};

/* } */
#endif
