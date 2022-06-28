#include "fm77avcrtc.h"
#include "fm77av.h"



FM77AVCRTC::FM77AVCRTC(VMBase *vmBase) : Device(vmBase)
{
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

