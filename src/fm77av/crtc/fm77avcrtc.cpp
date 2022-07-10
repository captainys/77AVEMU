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
	for(int i=0; i<8; ++i)
	{
		tilePtnCache[i]=0;
	}

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
	data&=0x0f;
	state.palette.analogPalette[state.palette.analogPaletteLatch][2]=data|(data<<4);
}
void FM77AVCRTC::WriteFD33(uint8_t data)
{
	data&=0x0f;
	state.palette.analogPalette[state.palette.analogPaletteLatch][0]=data|(data<<4);
}
void FM77AVCRTC::WriteFD34(uint8_t data)
{
	data&=0x0f;
	state.palette.analogPalette[state.palette.analogPaletteLatch][1]=data|(data<<4);
}
void FM77AVCRTC::VRAMDummyRead(uint16_t VRAMAddrIn)
{
	if(true==state.hardDraw.enabled)
	{
		auto fm77avPtr=(FM77AV *)vmPtr;
		auto VRAM=fm77avPtr->physMem.GetCurrentVRAMBank();

		VRAMAddrIn&=VRAM_PLANE_MASK;
		uint16_t VRAMAddr[3]=
		{
			VRAMAddrIn,
			VRAMAddrIn+VRAM_PLANE_SIZE,
			VRAMAddrIn+VRAM_PLANE_SIZE*2,
		};

		if(0!=(state.hardDraw.condition&2))
		{
			state.hardDraw.compareResult=0;
		}

		uint8_t bit=0x80;
		uint8_t writeBits=0;
		for(int i=0; i<8; ++i)
		{
			if(0!=(state.hardDraw.maskBits&bit))
			{
				goto NEXT;
			}

			if(0!=(state.hardDraw.condition&2) && 0==state.hardDraw.compareColor[i]&0x80) // Or should it be compareColor[7-i]&0x80?
			{
				uint8_t srcColor=0;
				srcColor|=(0!=(VRAM[VRAMAddr[0]]&bit) ? 1 : 0);
				srcColor|=(0!=(VRAM[VRAMAddr[1]]&bit) ? 2 : 0);
				srcColor|=(0!=(VRAM[VRAMAddr[2]]&bit) ? 4 : 0);

				if((0==(state.hardDraw.condition&1) && state.hardDraw.compareColor[i]==srcColor) ||
				   (0!=(state.hardDraw.condition&1) && state.hardDraw.compareColor[i]!=srcColor))
				{
					// Pass
					state.hardDraw.compareResult|=bit;
				}
				else
				{
					goto NEXT;
				}
			}
			writeBits|=bit;

		NEXT:
			bit>>=1;
		}

		uint8_t bankMask=state.hardDraw.bankMask;
		uint8_t col=state.hardDraw.color;
		for(int i=0; i<3; ++i,bankMask>>=1,col>>=1)
		{
			if(0!=(bankMask&1))
			{
				continue;
			}

			uint8_t rgb=(0!=(col&1) ? 255 : 0);
			uint8_t src=VRAM[VRAMAddr[i]];

			switch(state.hardDraw.cmd)
			{
			case HD_CMD_TILE://6
				VRAM[VRAMAddr[i]]&=~writeBits;
				VRAM[VRAMAddr[i]]|=(state.hardDraw.tile[i]&writeBits);
				break;
			case HD_CMD_PSET://0
				VRAM[VRAMAddr[i]]&=~writeBits;
				VRAM[VRAMAddr[i]]|=(rgb&writeBits);
				break;
			case HD_CMD_OR://2
				VRAM[VRAMAddr[i]]|=(rgb&writeBits);
				break;
			case HD_CMD_AND://3
				VRAM[VRAMAddr[i]]=(src&~writeBits)|(src&rgb&writeBits);
				break;
			case HD_CMD_XOR://4,
				VRAM[VRAMAddr[i]]^=(writeBits&rgb);
				break;
			case HD_CMD_NOT://5,
				VRAM[VRAMAddr[i]]&=~writeBits;
				VRAM[VRAMAddr[i]]|=((~rgb)&writeBits);
				break;
			case HD_CMD_CMP://7,
				break;
			}
		}
	}
}
void FM77AVCRTC::WriteD410(uint8_t data)
{
	state.hardDraw.enabled=(0!=(0x80&data));
	state.hardDraw.condition=(data>>5)&3;
	state.hardDraw.cmd=data&7;
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
	state.hardDraw.color=data&7;
}
uint8_t FM77AVCRTC::NonDestructiveReadD411(void) const
{
	return 0xF8|state.hardDraw.color;
}
void FM77AVCRTC::WriteD412(uint8_t data)
{
	state.hardDraw.maskBits=data;
}
uint8_t FM77AVCRTC::NonDestructiveReadD412(void) const
{
	return state.hardDraw.maskBits;
}
void FM77AVCRTC::WriteD413(uint8_t data)
{
	state.hardDraw.compareColor[0]=data&0x87;
}
uint8_t FM77AVCRTC::NonDestructiveReadD413(void) const
{
	// FM77AV40 Hardware Manual pp. 157 tells it is comparison result.
	return state.hardDraw.compareResult;
}
void FM77AVCRTC::WriteD414(uint8_t data)
{
	state.hardDraw.compareColor[1]=data&0x87;
}
void FM77AVCRTC::WriteD415(uint8_t data)
{
	state.hardDraw.compareColor[2]=data&0x87;
}
void FM77AVCRTC::WriteD416(uint8_t data)
{
	state.hardDraw.compareColor[3]=data&0x87;
}
void FM77AVCRTC::WriteD417(uint8_t data)
{
	state.hardDraw.compareColor[4]=data&0x87;
}
void FM77AVCRTC::WriteD418(uint8_t data)
{
	state.hardDraw.compareColor[5]=data&0x87;
}
void FM77AVCRTC::WriteD419(uint8_t data)
{
	state.hardDraw.compareColor[6]=data&0x87;
}
void FM77AVCRTC::WriteD41A(uint8_t data)
{
	state.hardDraw.compareColor[7]=data&0x87;
}
void FM77AVCRTC::WriteD41B(uint8_t data)
{
	state.hardDraw.bankMask=data&7;
}
uint8_t FM77AVCRTC::NonDestructiveReadD41B(void) const
{
	return 0xF8|state.hardDraw.bankMask;
}
void FM77AVCRTC::WriteD41C(uint8_t data)
{
	state.hardDraw.tile[0]=data;
	for(int i=0; i<8; ++i)
	{
		state.hardDraw.tilePtnCache[i]&=0xFE;
		if(0!=(data&0x80))
		{
			state.hardDraw.tilePtnCache[i]|=1;
		}
		data<<=1;
	}
}
void FM77AVCRTC::WriteD41D(uint8_t data)
{
	state.hardDraw.tile[1]=data;
	for(int i=0; i<8; ++i)
	{
		state.hardDraw.tilePtnCache[i]&=0xFD;
		if(0!=(data&0x80))
		{
			state.hardDraw.tilePtnCache[i]|=2;
		}
		data<<=1;
	}
}
void FM77AVCRTC::WriteD41E(uint8_t data)
{
	state.hardDraw.tile[2]=data;
	for(int i=0; i<8; ++i)
	{
		state.hardDraw.tilePtnCache[i]&=0xFB;
		if(0!=(data&0x80))
		{
			state.hardDraw.tilePtnCache[i]|=4;
		}
		data<<=1;
	}
}
void FM77AVCRTC::WriteD420(uint8_t data)
{
	uint16_t orPtn=data&0x1F;
	orPtn<<9;
	state.hardDraw.addrOffset&=0x1FF;
	state.hardDraw.addrOffset|=orPtn;
}
void FM77AVCRTC::WriteD421(uint8_t data)
{
	uint16_t orPtn=data;
	orPtn<<1;
	state.hardDraw.addrOffset&=0xFE00;
	state.hardDraw.addrOffset|=orPtn;
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

std::vector <std::string> FM77AVCRTC::GetStatusText(void) const
{
	std::vector <std::string> text;

	text.push_back("MODE:");
	text.back()+=ScrnModeToStr(state.scrnMode);
	text.back()+=" VRAMOffset:";
	text.back()+=cpputil::Ustox(state.VRAMOffset);
	if(0xFFFF==state.VRAMOffsetMask)
	{
		text.back()+=" VRAMOffsetLowBits:Enabled";
	}
	else
	{
		text.back()+=" VRAMOffsetLowBits:Disabled";
	}

	text.push_back("");
	text.back()+="VRAMAccessMask:";
	text.back()+=cpputil::Ubtox(state.VRAMAccessMask);
	text.back()+=" DisplayPage:";
	text.back().push_back('0'+state.displayPage);
	text.back()+=" ActivePage:";
	text.back().push_back('0'+state.activePage);

	text.push_back("HWDrawing:");
	text.back()+=(true==state.hardDraw.enabled ? "Ena " : "Dis ");
	text.back()+="Cond:";
	switch(state.hardDraw.condition)
	{
	case 0:
		text.back()+="Always(0) ";
		break;
	case 1:
		text.back()+="Always(1) ";
		break;
	case 2:
		text.back()+="If Equal(2) ";
		break;
	case 3:
		text.back()+="If Not Equal(3) ";
		break;
	}
	text.back()+="OP:";
	switch(state.hardDraw.cmd)
	{
	case HD_CMD_PSET://0,
		text.back()+="PSET";
		break;
	case HD_CMD_OR://2,
		text.back()+="OR";
		break;
	case HD_CMD_AND://3,
		text.back()+="AND";
		break;
	case HD_CMD_XOR://4,
		text.back()+="XOR";
		break;
	case HD_CMD_NOT://5,
		text.back()+="NOT";
		break;
	case HD_CMD_TILE://6,
		text.back()+="TILE";
		break;
	case HD_CMD_CMP://7,
		text.back()+="CMP";
		break;
	default:
		text.back()+="?(1)";
		break;
	}
	text.back()+=" COL:";
	text.back().push_back('0'+state.hardDraw.color);

	text.back()+=" MaskBits:";
	text.back()+=cpputil::Ubtox(state.hardDraw.maskBits);

	text.push_back("");
	text.back()+="CMPCOL:";
	for(int i=0; i<8; ++i)
	{
		text.back()+=cpputil::Ubtox(state.hardDraw.compareColor[i]&0x87);
		text.back().push_back(' ');
	}

	text.push_back("");
	text.back()+="BankMask:";
	text.back()+=cpputil::Ubtox(state.hardDraw.bankMask);

	text.back()+="TilePtn:";
	text.back()+=cpputil::Ubtox(state.hardDraw.tile[0]);
	text.back().push_back(' ');
	text.back()+=cpputil::Ubtox(state.hardDraw.tile[1]);
	text.back().push_back(' ');
	text.back()+=cpputil::Ubtox(state.hardDraw.tile[2]);

	return text;
}
