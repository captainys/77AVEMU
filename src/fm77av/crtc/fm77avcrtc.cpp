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
		analogPalette[i][0]=0;
		analogPalette[i][1]=0;
		analogPalette[i][2]=0;
	}
	analogPaletteLatch=0;
}

void FM77AVCRTC::HardwareDrawing::Reset(void)
{
	// $D410
	enabled=false;
	condition=0;
	cmd=HD_CMD_PSET;
	// $D411
	color=0;
	// $D412
	maskBits=0;
	// $D413 to $D41A
	compareColor[0]=0;
	compareColor[1]=0;
	compareColor[2]=0;
	compareColor[3]=0;
	compareColor[4]=0;
	compareColor[5]=0;
	compareColor[6]=0;
	compareColor[7]=0;
	// $D41B
	bankMask=0;
	// $D41C to $D41E
	tile[0]=0;
	tile[1]=0;
	tile[2]=0;

	// $D430
	lineBusy=false;
	// $D420,$D421
	addrOffset=0;
	// $D422,$D423
	lineStipple=0xFFFF;
	// $D424,$D425
	x0=0;
	y0=0;
	x1=0;
	y1=0;
}

FM77AVCRTC::FM77AVCRTC(VMBase *vmBase) : Device(vmBase)
{
	Reset();
}
void FM77AVCRTC::Reset(void)
{
	Device::Reset();
	state.palette.Reset();
	state.hardDraw.Reset();
	state.scrnMode=SCRNMODE_640X200_SINGLE;
	state.VRAMOffset=0;
	state.VRAMOffsetMask=0xFFE0;
	state.VRAMAccessMask=0;
	state.displayPage=0;
	state.activePage=0;
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
	if(0!=(data&4))
	{
		state.VRAMOffsetMask=0xffff;
	}
	else
	{
		state.VRAMOffsetMask=0xffe0;
	}
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
	if(true==state.hardDraw.lineBusy)
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

void FM77AVCRTC::WriteFD30(uint8_t data)
{
	uint16_t data16=data&0x0F;
	state.palette.analogPaletteLatch&=0xFF;
	state.palette.analogPaletteLatch|=(data16<<8);
}
void FM77AVCRTC::WriteFD31(uint8_t data)
{
	state.palette.analogPaletteLatch&=0xFF00;
	state.palette.analogPaletteLatch|=data;
}
void FM77AVCRTC::WriteFD32(uint8_t data)
{
	state.palette.analogPalette[state.palette.analogPaletteLatch][2]=data|(data<<4);
}
void FM77AVCRTC::WriteFD33(uint8_t data)
{
	state.palette.analogPalette[state.palette.analogPaletteLatch][0]=data|(data<<4);
}
void FM77AVCRTC::WriteFD34(uint8_t data)
{
	state.palette.analogPalette[state.palette.analogPaletteLatch][1]=data|(data<<4);
}
void FM77AVCRTC::VRAMDummyRead(uint8_t VRAMAddr)
{
	std::cout << cpputil::Ustox(VRAMAddr) << std::endl;
}
void FM77AVCRTC::WriteD410(uint8_t data)
{
}
uint8_t FM77AVCRTC::NonDestructiveReadD410(void) const
{
	uint8_t byteData=0x18;
	if(true==state.hardDraw.enabled)
	{
		byteData|=0x80;
	}
	byteData|=(state.hardDraw.condition<<5);
	byteData|=state.hardDraw.cmd;
	return byteData;
}
void FM77AVCRTC::WriteD411(uint8_t data)
{
}
uint8_t FM77AVCRTC::NonDestructiveReadD411(void) const
{
	return 0xF8|state.hardDraw.color;
}
void FM77AVCRTC::WriteD412(uint8_t data)
{
}
uint8_t FM77AVCRTC::NonDestructiveReadD412(void) const
{
	return state.hardDraw.maskBits;
}
void FM77AVCRTC::WriteD413(uint8_t data)
{
}
uint8_t FM77AVCRTC::NonDestructiveReadD413(void) const
{
	return 0; // Compare Bit according to FM77AV40 Hardware Manual, D0 to D7 according to FM Techknow and Oh!FM 1989-05.  No other explanation.
}
void FM77AVCRTC::WriteD414(uint8_t data)
{
}
void FM77AVCRTC::WriteD415(uint8_t data)
{
}
void FM77AVCRTC::WriteD416(uint8_t data)
{
}
void FM77AVCRTC::WriteD417(uint8_t data)
{
}
void FM77AVCRTC::WriteD418(uint8_t data)
{
}
void FM77AVCRTC::WriteD419(uint8_t data)
{
}
void FM77AVCRTC::WriteD41A(uint8_t data)
{
}
void FM77AVCRTC::WriteD41B(uint8_t data)
{
}
uint8_t FM77AVCRTC::NonDestructiveReadD41B(void) const
{
	return 0xF8|state.hardDraw.bankMask;
}
void FM77AVCRTC::WriteD41C(uint8_t data)
{
}
void FM77AVCRTC::WriteD41D(uint8_t data)
{
}
void FM77AVCRTC::WriteD41E(uint8_t data)
{
}
void FM77AVCRTC::WriteD420(uint8_t data)
{
}
void FM77AVCRTC::WriteD421(uint8_t data)
{
}
void FM77AVCRTC::WriteD422(uint8_t data)
{
}
void FM77AVCRTC::WriteD423(uint8_t data)
{
}
void FM77AVCRTC::WriteD424(uint8_t data)
{
}
void FM77AVCRTC::WriteD425(uint8_t data)
{
}
void FM77AVCRTC::WriteD426(uint8_t data)
{
}
void FM77AVCRTC::WriteD427(uint8_t data)
{
}
void FM77AVCRTC::WriteD428(uint8_t data)
{
}
void FM77AVCRTC::WriteD429(uint8_t data)
{
}
void FM77AVCRTC::WriteD42A(uint8_t data)
{
}
void FM77AVCRTC::WriteD42B(uint8_t data)
{
}
