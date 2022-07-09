#include "fm77avcrtc.h"
#include "fm77av.h"



void FM77AVCRTC::Palette::Reset(void)
{
	for(int i=0; i<8; ++i)
	{
		digitalPalette[i]=i;
	}
	for(int i=0; i<4096; ++i)
	{
		analogPalette[i]=i;
	}
}

FM77AVCRTC::FM77AVCRTC(VMBase *vmBase) : Device(vmBase)
{
}
void FM77AVCRTC::Reset(void)
{
	Device::Reset();
	state.palette.Reset();
	state.scrnMode=SCRNMODE_640X200_SINGLE;
	state.VRAMOffset=0;
	state.VRAMOffsetLowBitsEnabled=false;
	state.VRAMAccessMask=0;
	state.displayPage=0;
	state.activePage=0;

	state.lineHardwareBusy=false;
}
FM77AVCRTC::Palette &FM77AVCRTC::GetPalette(void)
{
	return state.palette;
}
const FM77AVCRTC::Palette &FM77AVCRTC::GetPalette(void) const
{
	return state.palette;
}
// Let's say 60 frames per sec.
// 1 frame takes 16.7ms.
// Horizontal Scan frequency is say 31KHz.
// 1 line takes 0.032ms.
// 480 lines take 15.36ms.
// Then, VSYNC should be 1.34ms long.
// Will take screenmode into account eventually.
// Also should take HSYNC into account.
bool FM77AVCRTC::InVSYNC(const uint64_t fm77avTime) const
{
	unsigned int intoFrame=((unsigned long long)fm77avTime)%VSYNC_CYCLE;
	return  (CRT_VERTICAL_DURATION<intoFrame);
}
bool FM77AVCRTC::InHSYNC(const uint64_t fm77avTime) const
{
	unsigned int intoFrame=((unsigned long long)fm77avTime)%VSYNC_CYCLE;
	if(intoFrame<CRT_VERTICAL_DURATION)
	{
		auto intoLine=intoFrame%HSYNC_CYCLE;
		return (CRT_HORIZONTAL_DURATION<intoLine);
	}
	return false;
}
uint64_t FM77AVCRTC::NextVSYNCRisingEdge(uint64_t fm77avTime) const
{
	fm77avTime+=(VSYNC_CYCLE-CRT_VERTICAL_DURATION); // Get over to the next cycle if it is already in VSYNC.
	uint64_t mod=fm77avTime%VSYNC_CYCLE;
	return fm77avTime-mod+CRT_VERTICAL_DURATION;
}
uint64_t FM77AVCRTC::NextVSYNCEndTime(uint64_t fm77avTime) const
{
	uint64_t mod=fm77avTime%VSYNC_CYCLE;
	return fm77avTime-mod+VSYNC_CYCLE;
}

void FM77AVCRTC::WriteFD12(uint8_t data)
{
	if(0==(data&0x40))
	{
		state.scrnMode=SCRNMODE_640X200_SINGLE;
	}
	else
	{
		state.scrnMode=SCRNMODE_320X200_4096COL;
	}
}
const int FM77AVCRTC::NonDestructiveReadFD12(void) const
{
	auto fm77avPtr=(const FM77AV *)vmPtr;

	uint8_t byteData=0xBF;
	if(SCRNMODE_320X200_4096COL==state.scrnMode)
	{
		byteData|=0x40;
	}
	// else if(SCRNMODE_640X200_SINGLE==state.scrnMode)
	//{
	//	byteData&=0xBF;
	//}

	if(true==InVSYNC(fm77avPtr->state.fm77avTime))
	{
		byteData&=0xFD; // VSYNC=1, DISPTMG=0
	}
	else
	{
		byteData&=0xFE; // VSYNC=1, DISPTMG=0
	}
	return byteData;
}
void FM77AVCRTC::WriteD430(uint8_t data)
{
	state.displayPage=((0!=(data&0x40)) ? 1 : 0);
	state.activePage=((0!=(data&0x20)) ? 1 : 0);
	state.VRAMOffsetLowBitsEnabled=(0!=(data&4));
	// Katakana/Hiragana is managed by physMem.
}
uint8_t FM77AVCRTC::NonDestructiveReadD430(void) const
{
	auto fm77avPtr=(const FM77AV *)vmPtr;
	uint8_t data=0xFF;
	if(true!=InBlank(fm77avPtr->state.fm77avTime))
	{
		data&=0x7F;
	}
	if(true==state.lineHardwareBusy)
	{
		data&=0xEF;
	}
	if(true!=InVSYNC(fm77avPtr->state.fm77avTime))
	{
		data&=0xFB;
	}
	// Sub-Monitor Change is managed by physMem.
	return data;
}
