#include "sound.h"



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
	state.PSG.Reset();
	state.PSGAddrLatch=0;

}
/* virtual */ void FM77AVSound::IOWriteByte(unsigned int ioport,unsigned int data)
{
}
/* virtual */ unsigned int FM77AVSound::IOReadByte(unsigned int ioport)
{
	uint8_t byteData=NonDestructiveIOReadByte(ioport);
	return byteData;
}
uint8_t FM77AVSound::NonDestructiveIOReadByte(unsigned int ioport)
{
	return 0;
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
}
