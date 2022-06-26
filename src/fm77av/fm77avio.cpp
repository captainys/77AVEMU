#include <iostream>
#include "fm77av.h"



void FM77AV::IOWrite(uint16_t ioAddr,uint8_t value)
{
	if((0xFD00==(ioAddr&0xFF00) && true==var.monitorIOWriteMain[ioAddr&0xFF]) ||
	   (0xD400==(ioAddr&0xFF00) && true==var.monitorIOWriteSub[ioAddr&0xFF]))
	{
		std::cout << "IOWRITE ";
		if(0xFD00==(ioAddr&0xFF00))
		{
			std::cout << "MAIN:" << cpputil::Ustox(mainCPU.state.PC) << " ";
		}
		else
		{
			std::cout << "SUB: " << cpputil::Ustox(subCPU.state.PC) << " ";
		}
		std::cout << "IO:" << cpputil::Ustox(ioAddr) << " ";
		std::cout << "VALUE:" << cpputil::Ubtox(value) << std::endl;
	}

	switch(ioAddr)
	{
	// Main-CPU I/O
	case FM77AVIO_SUBSYS_BUSY_HALT: // 0xFD05
		if(0x80==value)
		{
			state.subSysHalt=true;
			//state.subSysBusy=true;
			// Question: What's going to happen if the main CPU writes $80 to $FD05
			//           while the busy flag is true?  Does it halt the sub-CPU anyway?
			//           Or, is it ignored?
			// F-BASIC Analysis Manual Phase III Sub-System, pp.38 tells,
			//   (2) BUSY Flag
			//   This flag tells main CPU that sub-system is not in a good timing to halt
			//   eg. executing a command.  It is controlled by sub-system I/O $D40A read/write.
			//   It also goes HI when sub-CPU accepted HALT request from main CPU automatically
			//   (by hardware level).  It serves as HALT acknowledge as well.
			//   So, there are two things: Sub-System BUSY and Sub-System Halt.
			//   Should I make sub-system busy upon 0x80 write to 0xFD05?

			// This description is questionable.  FM-7 Schematic on FM-7/8 Utilization Research
			// does not indicate that BUSY flag has anything to do with HALT status.

			// BUSY signal comes out of M44 8pin (pp.300) then comes into M72 15pin and goes 
			// out to DB7 from 11pin (pp.294).

			// BUSY signal latches to SRWB on *SBUSYSET on M44.  SRWB is read/write signal
			// from the sub-CPU (6809), which makes sense.  However, BUSY signal doesn't seem
			// to have anything to do with sub-CPU HALT status.

			// If my understanding is correct, subSysBusy should not be set here.
		}
		else
		{
			if(true==state.subSysHalt && true==var.monitorSubSysCmd)
			{
				PrintSubSystemCommand();
			}
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

	if((0xFD00==(ioAddr&0xFF00) && true==var.monitorIOReadMain[ioAddr&0xFF]) ||
	   (0xD400==(ioAddr&0xFF00) && true==var.monitorIOReadSub[ioAddr&0xFF]))
	{
		std::cout << "IOREAD  ";
		if(0xFD00==(ioAddr&0xFF00))
		{
			std::cout << "MAIN:" << cpputil::Ustox(mainCPU.state.PC) << " ";
		}
		else
		{
			std::cout << "SUB: " << cpputil::Ustox(subCPU.state.PC) << " ";
		}
		std::cout << "IO:" << cpputil::Ustox(ioAddr) << " ";
		std::cout << "VALUE:" << cpputil::Ubtox(byteData) << std::endl;
	}

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
