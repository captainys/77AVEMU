#include "fm77av.h"
#include "fm77avdef.h"
#include "fm77avsound.h"



FM77AVSound::FM77AVSound(class FM77AV *fm77avPtr) : Device(fm77avPtr)
{
	PowerOn();
}
void FM77AVSound::SetOutsideWorld(class Outside_World *outside_world)
{
	this->outside_world=outside_world;
}
/* virtual */ void FM77AVSound::PowerOn(void)
{
	Reset();
}
/* virtual */ void FM77AVSound::Reset(void)
{
	state.ay38910.Reset();
	state.ay38910AddrLatch=0;

}
/* virtual */ void FM77AVSound::IOWriteByte(unsigned int ioport,unsigned int data)
{
	switch(ioport)
	{
	case FM77AVIO_PSG_CONTROL://             0xFD0D,
		{
			auto control=data&3;
			if(3==state.ay38910LastControl && 0==control) // Latch Address
			{
				state.ay38910AddrLatch=state.ay38910LastData&AY38910::REG_MASK;
			}
			if(1==control) // Read Data
			{
				// Probably I don't have anything to do here.
			}
			if(2==control) // Write Data
			{
				state.ay38910.Write(state.ay38910AddrLatch,state.ay38910LastData);
			}
			state.ay38910LastControl=control;
		}
		break;
	case FM77AVIO_PSG_DATA://                0xFD0E,
		if(0==state.ay38910LastControl)
		{
			state.ay38910LastData=data;
		}
		break;
	}
}
/* virtual */ unsigned int FM77AVSound::IOReadByte(unsigned int ioport)
{
	uint8_t byteData=NonDestructiveIOReadByte(ioport);
	return byteData;
}
uint8_t FM77AVSound::NonDestructiveIOReadByte(unsigned int ioport) const
{
	switch(ioport)
	{
	case FM77AVIO_PSG_CONTROL://             0xFD0D,
		break;
	case FM77AVIO_PSG_DATA://                0xFD0E,
		if(1==state.ay38910LastControl) // Read
		{
			return state.ay38910.Read(state.ay38910AddrLatch);
		}
		break;
	}
	return 0xFF;
}
void FM77AVSound::SoundPolling(uint64_t FM77AVTime)
{
}
std::vector <std::string> FM77AVSound::GetStatusText(void) const
{
	std::vector <std::string> text;
	return text;
}
void FM77AVSound::ProcessSound(void)
{
}
/* virtual */ uint32_t FM77AVSound::SerializeVersion(void) const
{
	return 0;
}
/* virtual */ void FM77AVSound::SpecificSerialize(std::vector <unsigned char> &data,std::string stateFName) const
{
}
/* virtual */ bool FM77AVSound::SpecificDeserialize(const unsigned char *&data,std::string stateFName,uint32_t version)
{
	return true;
}
