#include "fm77av.h"



void FM77AV::IOWrite(uint16_t ioAddr,uint8_t value)
{
	switch(ioAddr)
	{
	// Main-CPU I/O
	case FM77AVIO_SUBSYS_BUSY_HALT: // 0xFD05
		if(0x80==value)
		{
			state.subSysHalt=true;
			state.subSysBusy=true;
			// Question: What's going to happen if the main CPU writes $80 to $FD05
			//           while the busy flag is true?  Does it halt the sub-CPU anyway?
			//           Or, is it ignored?
		}
		else
		{
			state.subSysHalt=false;
		}
		break;

	// Sub-CPU I/O
	case FM77AVIO_SUBCPU_BUSY: // =             0xD40A,
		state.subSysBusy=true;
		break;
	}
}
uint8_t FM77AV::IORead(uint16_t ioAddr)
{
	uint8_t byteData=NonDestructiveIORead(ioAddr);
	switch(ioAddr)
	{
	// Main-CPU I/O
	case FM77AVIO_SUBSYS_INTERFACE: // =        0xFD04,
		byteData=NonDestructiveIORead_FM77AVIO_SUBSYS_INTERFACE();
		// FIRQ flag must be cleared upon reading $FD04
		// F-BASIC Analysis Manual Phase III Sub-System, Shuwa System Trading, pp.45
		state.main.firq=0;
		break;


	// Sub-CPU I/O
	case FM77AVIO_SUBCPU_BUSY: // =             0xD40A,
		state.subSysBusy=false;
		break;
	}
	return byteData;
}

uint8_t FM77AV::NonDestructiveIORead(uint16_t ioAddr) const
{
	uint8_t byteData=0xFF;
	switch(ioAddr)
	{
	// Main-CPU I/O
	case FM77AVIO_SUBSYS_BUSY_HALT:
		byteData=0x7E; // Bit0=0 means has disk, RS232C, or ext device.
		if(true!=ExternalDevicePresent())
		{
			byteData|=1;
		}
		if(true==state.subSysBusy)
		{
			byteData|=0x80;
		}
		break;
	case FM77AVIO_SUBSYS_INTERFACE: // =        0xFD04,
		byteData=NonDestructiveIORead_FM77AVIO_SUBSYS_INTERFACE();
		break;


	// Sub-CPU I/O
	}
	return byteData;
}

uint8_t FM77AV::NonDestructiveIORead_FM77AVIO_SUBSYS_INTERFACE(void) const
{
	uint8_t byteData=0xFF;
	//if(MACHINETYPE_FM77AV40<=state.machineType)
	//{
	//	
	//}
	//if(breakKeyDown)
	//{
	//	byteData&=(~2);
	//}
	if(0!=(state.main.firq&FIRQ_SUBSYS_ATTENTION))
	{
		byteData&=(~1);
	}
	return byteData;
}
