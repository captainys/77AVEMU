#include "fm77av.h"



void FM77AV::IOWrite(uint16_t ioAddr,uint8_t value)
{
}
uint8_t FM77AV::IORead(uint16_t ioAddr)
{
	uint8_t byteData=NonDestructiveIORead(ioAddr);
	switch(ioAddr)
	{
	case FM77AVIO_SUBSYS_INTERFACE: // =        0xFD04,
		byteData=NonDestructiveIORead_FM77AVIO_SUBSYS_INTERFACE();
		// FIRQ flag must be cleared upon reading $FD04
		// F-BASIC Analysis Manual Phase III Sub-System, Shuwa System Trading, pp.45
		state.main.firq=0;
		break;
	}
	return byteData;
}

uint8_t FM77AV::NonDestructiveIORead(uint16_t ioAddr) const
{
	uint8_t byteData=0xFF;
	switch(ioAddr)
	{
	case FM77AVIO_SUBSYS_INTERFACE: // =        0xFD04,
		byteData=NonDestructiveIORead_FM77AVIO_SUBSYS_INTERFACE();
		break;
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
