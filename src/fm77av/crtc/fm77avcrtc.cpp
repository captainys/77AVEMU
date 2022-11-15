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
	lineBusyBy=0;
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

unsigned int FM77AVCRTC::GetBytesPerLine(void) const
{
	switch(state.scrnMode)
	{
	case SCRNMODE_640X200:
	case SCRNMODE_640X400:
		return 80;
	case SCRNMODE_320X200_4096COL:
	case SCRNMODE_320X200_260KCOL:
		return 40;
	}
	return 80; // Unknown screen mode.
}
unsigned int FM77AVCRTC::GetPlaneVRAMMask(void) const
{
	switch(state.scrnMode)
	{
	case SCRNMODE_640X200:
		return 0x3FFF;
	case SCRNMODE_640X400:
		return 0x7FFF;
	case SCRNMODE_320X200_4096COL:
	case SCRNMODE_320X200_260KCOL:
		return 0x1FFF;
	}
	return 0x3FFF; // Unknown screen mode.
}
void FM77AVCRTC::AddBreakOnHardwareVRAMWriteType(uint8_t opType,bool justMonitorDontBreak)
{
	breakOnHardwareVRAMWriteOpBits|=(1<<opType);
	justMonitorDontBreakOnHardwareVRAMWrite=justMonitorDontBreak;
}
void FM77AVCRTC::ClearBreakOnHardwareVRAMWriteType(uint8_t opType)
{
	breakOnHardwareVRAMWriteOpBits&=~(1<<opType);
}

void FM77AVCRTC::BreakOnHardwareLineDrawing(bool justMonitorDontBreak)
{
	breakOnHardwareLineDrawing=true;
	justMonitorDontBreakOnHardwareLineDrawing=justMonitorDontBreak;
}
void FM77AVCRTC::ClearBreakOnHardwareLineDrawing(void)
{
	breakOnHardwareLineDrawing=false;
}

void FM77AVCRTC::Reset(void)
{
	Device::Reset();
	state.palette.Reset();
	state.hardDraw.Reset();
	state.scrnMode=SCRNMODE_640X200;
	for(auto &o : state.VRAMOffset)
	{
		o=0;
	}
	state.VRAMOffsetMask=0xFFE0;
	state.VRAMAccessMask=0;
	state.displayPage=0;
	state.activePage=0;
	state.CRTEnabled=true;
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
bool FM77AVCRTC::InVBLANK(const uint64_t fm77avTime) const
{
	unsigned int intoFrame=((unsigned long long)fm77avTime)%VSYNC_CYCLE;
	return  (CRT_VERTICAL_DURATION<intoFrame);
}
bool FM77AVCRTC::InVSYNC(uint64_t fm77avTime) const
{
	unsigned int intoFrame=((unsigned long long)fm77avTime)%VSYNC_CYCLE;
	if(CRT_VERTICAL_DURATION<intoFrame)
	{
		unsigned int intoVBLANK=intoFrame-CRT_VERTICAL_DURATION;
		if(CRT_VBLANK_TO_VSYNC<=intoVBLANK)
		{
			return (intoVBLANK-CRT_VBLANK_TO_VSYNC)<CRT_VSYNC_DURATION;
		}
	}
	return false;
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

void FM77AVCRTC::WriteD408(void)
{
	state.CRTEnabled=false;
}
void FM77AVCRTC::ReadD408(void)
{
	state.CRTEnabled=true;
}

void FM77AVCRTC::ReadD409(void)
{
	state.VRAMAccessFlag=true;
}
void FM77AVCRTC::WriteD409(void)
{
	state.VRAMAccessFlag=false;
}

void FM77AVCRTC::WriteFD04(uint8_t data)
{
	if(0==(data&8))
	{
		state.scrnMode=SCRNMODE_640X400;
	}
	else if(0!=(data&0x10))
	{
		state.scrnMode=SCRNMODE_320X200_260KCOL;
	}
	else
	{
		state.scrnMode=state.avScrnMode;
	}
}
void FM77AVCRTC::WriteFD12(uint8_t data)
{
	if(0==(data&0x40))
	{
		state.avScrnMode=SCRNMODE_640X200;
	}
	else
	{
		state.avScrnMode=SCRNMODE_320X200_4096COL;
	}

	if(SCRNMODE_640X200==state.scrnMode ||
	   SCRNMODE_320X200_4096COL==state.scrnMode)
	{
		state.scrnMode=state.avScrnMode;
	}
}
const int FM77AVCRTC::NonDestructiveReadFD12(void) const
{
	auto fm77avPtr=(const FM77AV *)vmPtr;

	uint8_t byteData=0xBF; // Bit4 should be clear in 640x200 mode.
	if(SCRNMODE_320X200_4096COL==state.scrnMode)
	{
		byteData|=0x40;
	}

	if(true!=InVSYNC(fm77avPtr->state.fm77avTime)) // Better getting VSYNC and BLANK together.  Same calculation.
	{
		byteData&=0xFE;
	}
	if(true==InBlank(fm77avPtr->state.fm77avTime))
	{
		byteData&=0xFD;
	}
	return byteData;
}

void FM77AVCRTC::WriteD40E(uint8_t value)
{
	state.VRAMOffset[state.activePage]&=0xFF;
	state.VRAMOffset[state.activePage]|=(value<<8);
}
void FM77AVCRTC::WriteD40F(uint8_t value)
{
	state.VRAMOffset[state.activePage]&=0xFF00;
	state.VRAMOffset[state.activePage]|=value;
	state.VRAMOffset[state.activePage]&=state.VRAMOffsetMask;
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
	if(fm77avPtr->state.fm77avTime<=state.hardDraw.lineBusyBy)
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

		if(0!=((1<<state.hardDraw.cmd)&breakOnHardwareVRAMWriteOpBits))
		{
			fm77avPtr->mainCPU.debugger.ExternalBreak("CRTC Hardware VRAM Write");
		}

		uint8_t *VRAMByte[3]={nullptr,nullptr,nullptr};
		if(SCRNMODE_640X400!=state.scrnMode)
		{
			auto VRAM=fm77avPtr->physMem.GetCurrentVRAMBank();
			VRAMAddrIn&=VRAM_PLANE_MASK;
			VRAMByte[0]=&VRAM[VRAMAddrIn];
			VRAMByte[1]=&VRAM[VRAMAddrIn+VRAM_PLANE_SIZE];
			VRAMByte[2]=&VRAM[VRAMAddrIn+VRAM_PLANE_SIZE*2];
		}
		else
		{
			VRAMAddrIn&=VRAM_PLANE_MASK_640X400;
			VRAMByte[0]=fm77avPtr->physMem.GetVRAMBank(0)+(VRAMAddrIn&0x7FFF);
			VRAMByte[1]=fm77avPtr->physMem.GetVRAMBank(1)+(VRAMAddrIn&0x7FFF);
			VRAMByte[2]=fm77avPtr->physMem.GetVRAMBank(2)+(VRAMAddrIn&0x7FFF);
		}

		if(0!=(state.hardDraw.condition&2) || HD_CMD_CMP==state.hardDraw.cmd)
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

			if(0!=(state.hardDraw.condition&2) || HD_CMD_CMP==state.hardDraw.cmd)
			{
				uint8_t srcColor=0;
				srcColor|=(0!=((*VRAMByte[0])&bit) ? 1 : 0);
				srcColor|=(0!=((*VRAMByte[1])&bit) ? 2 : 0);
				srcColor|=(0!=((*VRAMByte[2])&bit) ? 4 : 0);

				srcColor|=fm77avPtr->physMem.state.VRAMAccessMask;

				bool match=false;
				for(auto cmpColor : state.hardDraw.compareColor) // Should it work this way?
				{
					if(cmpColor==srcColor)
					{
						match=true;
						break;
					}
				}

				if((0==(state.hardDraw.condition&1) && true==match) ||
				   (0!=(state.hardDraw.condition&1) && true!=match))
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

		// 2022/08/05 Experiment on a real FM77AV showed that the hardware-drawing is affected by multi-page ($FD37), which is stored as VRAMAccessMask.
		uint8_t bankMask=state.hardDraw.bankMask|fm77avPtr->physMem.state.VRAMAccessMask;
		uint8_t col=state.hardDraw.color;
		for(int i=0; i<3; ++i,bankMask>>=1,col>>=1)
		{
			if(0!=(bankMask&1))
			{
				continue;
			}

			uint8_t rgb=(0!=(col&1) ? 255 : 0);
			uint8_t src=*VRAMByte[i];

			switch(state.hardDraw.cmd)
			{
			case HD_CMD_TILE://6
				*VRAMByte[i]&=~writeBits;
				*VRAMByte[i]|=(state.hardDraw.tile[i]&writeBits);
				break;
			case HD_CMD_PSET://0
				*VRAMByte[i]&=~writeBits;
				*VRAMByte[i]|=(rgb&writeBits);
				break;
			case HD_CMD_OR://2
				*VRAMByte[i]|=(rgb&writeBits);
				break;
			case HD_CMD_AND://3
				*VRAMByte[i]=(src&~writeBits)|(src&rgb&writeBits);
				break;
			case HD_CMD_XOR://4,
				*VRAMByte[i]^=(writeBits&rgb);
				break;
			case 1: // FM77AV Demo uses 1.  My guess is it is for erasing a line.
			case HD_CMD_NOT://5,
				*VRAMByte[i]&=~(rgb&writeBits);
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
	orPtn<<=9;
	state.hardDraw.addrOffset&=0x1FF;
	state.hardDraw.addrOffset|=orPtn;
}
void FM77AVCRTC::WriteD421(uint8_t data)
{
	uint16_t orPtn=data;
	orPtn<<=1;
	state.hardDraw.addrOffset&=0xFE00;
	state.hardDraw.addrOffset|=orPtn;
}

void FM77AVCRTC::WriteD422(uint8_t data)
{
	uint16_t data16=data;
	state.hardDraw.lineStipple&=0x00FF;
	state.hardDraw.lineStipple|=(data16<<8);
}
void FM77AVCRTC::WriteD423(uint8_t data)
{
	state.hardDraw.lineStipple&=0xFF00;
	state.hardDraw.lineStipple|=data;
}
void FM77AVCRTC::WriteD424(uint8_t data)
{
	uint16_t data16=data;
	state.hardDraw.x0&=0x00FF;
	state.hardDraw.x0|=(data16<<8);
}
void FM77AVCRTC::WriteD425(uint8_t data)
{
	state.hardDraw.x0&=0xFF00;
	state.hardDraw.x0|=data;
}
void FM77AVCRTC::WriteD426(uint8_t data)
{
	uint16_t data16=data;
	state.hardDraw.y0&=0x00FF;
	state.hardDraw.y0|=(data16<<8);
}
void FM77AVCRTC::WriteD427(uint8_t data)
{
	state.hardDraw.y0&=0xFF00;
	state.hardDraw.y0|=data;
}
void FM77AVCRTC::WriteD428(uint8_t data)
{
	uint16_t data16=data;
	state.hardDraw.x1&=0x00FF;
	state.hardDraw.x1|=(data16<<8);
}
void FM77AVCRTC::WriteD429(uint8_t data)
{
	state.hardDraw.x1&=0xFF00;
	state.hardDraw.x1|=data;
}
void FM77AVCRTC::WriteD42A(uint8_t data)
{
	uint16_t data16=data;
	state.hardDraw.y1&=0x00FF;
	state.hardDraw.y1|=(data16<<8);
}
void FM77AVCRTC::WriteD42B(uint8_t data)
{
	auto fm77avPtr=(FM77AV *)vmPtr;
	if(true==breakOnHardwareLineDrawing)
	{
		fm77avPtr->mainCPU.debugger.ExternalBreak("CRTC Hardware VRAM Write");
	}
	state.hardDraw.y1&=0xFF00;
	state.hardDraw.y1|=data;
	DrawLine();

	int32_t dx=state.hardDraw.x1-state.hardDraw.x0;
	int32_t dy=state.hardDraw.y1-state.hardDraw.y0;
	if(dx<0)
	{
		dx=-dx;
	}
	if(dy<0)
	{
		dy=-dy;
	}

	state.hardDraw.lineBusyBy=fm77avPtr->state.fm77avTime+(std::max(dx,dy)+1)*HD_LINE_TIME_PER_PIXEL;
}

void FM77AVCRTC::DrawLine(void)
{
	auto fm77avPtr=(FM77AV *)vmPtr;

	int vx,vy,dx,dy;
	dx=state.hardDraw.x1-state.hardDraw.x0;
	dy=state.hardDraw.y1-state.hardDraw.y0;
	if(0<=dx)
	{
		vx=1;
	}
	else
	{
		vx=-1;
		dx=-dx;
	}
	if(0<=dy)
	{
		vy=1;
	}
	else
	{
		vy=-1;
		dy=-dy;
	}

	unsigned int bytesPerLine=GetBytesPerLine();
	int VRAMVy=bytesPerLine*vy;

	uint8_t *VRAMPlane[3]={nullptr,nullptr,nullptr};
	if(SCRNMODE_640X400!=state.scrnMode)
	{
		auto VRAM=fm77avPtr->physMem.GetCurrentVRAMBank();
		VRAMPlane[0]=VRAM;
		VRAMPlane[1]=VRAM+0x4000;
		VRAMPlane[2]=VRAM+0x8000;
	}
	else
	{
		VRAMPlane[0]=fm77avPtr->physMem.GetVRAMBank(0);
		VRAMPlane[1]=fm77avPtr->physMem.GetVRAMBank(1);
		VRAMPlane[2]=fm77avPtr->physMem.GetVRAMBank(2);
	}
	auto planeMask=GetPlaneVRAMMask();

	unsigned int VRAMLayerBaseAddr=state.hardDraw.addrOffset&~planeMask;
	unsigned int VRAMInLayerAddr[2];
	if(SCRNMODE_640X400!=state.scrnMode)
	{
		VRAMInLayerAddr[0]=(state.hardDraw.addrOffset+GetActiveVRAMOffset())&planeMask;
		VRAMInLayerAddr[1]=VRAMInLayerAddr[0];
	}
	else
	{
		// WTF! WTF! WTF! WTF! WTF! WTF!
		// 640x400 mode uses VRAM offset register 0 for even address, and register 1 odd address.
		VRAMInLayerAddr[0]=(state.hardDraw.addrOffset+state.VRAMOffset[0]*2)&planeMask;
		VRAMInLayerAddr[1]=(state.hardDraw.addrOffset+state.VRAMOffset[1]*2)&planeMask;
	}

	unsigned int lineStippleBit=0x8000;

	unsigned int balance=0,count=0;;
	int x=state.hardDraw.x0;
	int y=state.hardDraw.y0;

	VRAMInLayerAddr[0]+=y*bytesPerLine+(x>>3);
	VRAMInLayerAddr[1]+=y*bytesPerLine+(x>>3);
	if(state.hardDraw.lineStipple&lineStippleBit)
	{
		PutDot(VRAMPlane,VRAMLayerBaseAddr+(VRAMInLayerAddr[(x>>3)&1]&planeMask),x&7,count++);
	}
	lineStippleBit>>=1;
	if(0==dx && 0==dy)
	{
		// Done.  Nothing to do.
	}
	else if(0==dx) // Vertical
	{
		for(;;)
		{
			y+=vy;
			VRAMInLayerAddr[0]+=VRAMVy;
			VRAMInLayerAddr[1]+=VRAMVy;
			if(state.hardDraw.lineStipple&lineStippleBit)
			{
				PutDot(VRAMPlane,VRAMLayerBaseAddr+(VRAMInLayerAddr[(x>>3)&1]&planeMask),x&7,count++);
			}
			lineStippleBit>>=1;
			if(0==lineStippleBit)
			{
				lineStippleBit=0x8000;
			}
			if(y==state.hardDraw.y1)
			{
				break;
			}
		}
	}
	else if(0==dy) // Horizontal
	{
		for(;;)
		{
			auto prevX=x;
			x+=vx;
			if((prevX>>3)!=(x>>3))
			{
				VRAMInLayerAddr[0]+=vx;
				VRAMInLayerAddr[1]+=vx;
			}
			if(state.hardDraw.lineStipple&lineStippleBit)
			{
				PutDot(VRAMPlane,VRAMLayerBaseAddr+(VRAMInLayerAddr[(x>>3)&1]&planeMask),x&7,count++);
			}
			lineStippleBit>>=1;
			if(0==lineStippleBit)
			{
				lineStippleBit=0x8000;
			}

			if(x==state.hardDraw.x1)
			{
				break;
			}
		}
	}
	else if(dy<dx) // Long in X
	{
		for(;;)
		{
			auto prevX=x;
			x+=vx;
			balance+=dy;
			if((prevX>>3)!=(x>>3))
			{
				VRAMInLayerAddr[0]+=vx;
				VRAMInLayerAddr[1]+=vx;
			}
			if(dx<=balance)
			{
				y+=vy;
				VRAMInLayerAddr[0]+=VRAMVy;
				VRAMInLayerAddr[1]+=VRAMVy;
				balance-=dx;
			}
			if(state.hardDraw.lineStipple&lineStippleBit)
			{
				PutDot(VRAMPlane,VRAMLayerBaseAddr+(VRAMInLayerAddr[(x>>3)&1]&planeMask),x&7,count++);
			}
			lineStippleBit>>=1;
			if(0==lineStippleBit)
			{
				lineStippleBit=0x8000;
			}

			if(x==state.hardDraw.x1 && y==state.hardDraw.y1)
			{
				break;
			}
		}
	}
	else // if(dx<dy) // Long in Y
	{
		for(;;)
		{
			y+=vy;
			VRAMInLayerAddr[0]+=VRAMVy;
			VRAMInLayerAddr[1]+=VRAMVy;
			balance+=dx;
			if(dy<=balance)
			{
				auto prevX=x;
				x+=vx;
				if((prevX>>3)!=(x>>3))
				{
					VRAMInLayerAddr[0]+=vx;
					VRAMInLayerAddr[1]+=vx;
				}
				balance-=dy;
			}
			if(state.hardDraw.lineStipple&lineStippleBit)
			{
				PutDot(VRAMPlane,VRAMLayerBaseAddr+(VRAMInLayerAddr[(x>>3)&1]&planeMask),x&7,count++);
			}
			lineStippleBit>>=1;
			if(0==lineStippleBit)
			{
				lineStippleBit=0x8000;
			}

			if(x==state.hardDraw.x1 && y==state.hardDraw.y1)
			{
				break;
			}
		}
	}
}

void FM77AVCRTC::PutDot(uint8_t *VRAMPlane[3],unsigned int VRAMAddr,uint8_t bit,unsigned int count)
{
	if(0!=(state.hardDraw.condition&2))
	{
		uint8_t srcColor=0;
		srcColor|=(0!=(VRAMPlane[0][VRAMAddr]&bit) ? 1 : 0);
		srcColor|=(0!=(VRAMPlane[1][VRAMAddr]&bit) ? 2 : 0);
		srcColor|=(0!=(VRAMPlane[2][VRAMAddr]&bit) ? 4 : 0);

		bool match=false;
		for(auto cmpColor : state.hardDraw.compareColor) // Should it work this way?
		{
			if(cmpColor==srcColor)
			{
				match=true;
				break;
			}
		}

		if((0==(state.hardDraw.condition&1) && true==match) ||
		   (0!=(state.hardDraw.condition&1) && true!=match))
		{
			// Pass
		}
		else
		{
			return;
		}
	}

	auto fm77avPtr=(const FM77AV *)vmPtr;

	uint8_t writeBit=(1<<(7-bit));
	// 2022/08/05 Experiment on a real FM77AV showed that the hardware-drawing is affected by multi-page ($FD37), which is stored as VRAMAccessMask.
	uint8_t bankMask=state.hardDraw.bankMask|fm77avPtr->physMem.state.VRAMAccessMask;
	uint8_t col=state.hardDraw.color;
	for(int i=0; i<3; ++i,bankMask>>=1,col>>=1)
	{
		if(0!=(bankMask&1))
		{
			continue;
		}

		uint8_t rgb=(0!=(col&1) ? 255 : 0);
		uint8_t src=VRAMPlane[i][VRAMAddr];

		switch(state.hardDraw.cmd)
		{
		case HD_CMD_TILE://6
			// Should I take bit from X? Or should I take bit from count?
			// F-BASIC V3.4 L20 Tile Box function implies X.
			VRAMPlane[i][VRAMAddr]&=~writeBit;
			VRAMPlane[i][VRAMAddr]|=(state.hardDraw.tile[i]&writeBit);
			break;
		case HD_CMD_PSET://0
			VRAMPlane[i][VRAMAddr]&=~writeBit;
			VRAMPlane[i][VRAMAddr]|=(rgb&writeBit);
			break;
		case HD_CMD_OR://2
			VRAMPlane[i][VRAMAddr]|=(rgb&writeBit);
			break;
		case HD_CMD_AND://3
			VRAMPlane[i][VRAMAddr]=(src&~writeBit)|(src&rgb&writeBit);
			break;
		case HD_CMD_XOR://4,
			VRAMPlane[i][VRAMAddr]^=(writeBit&rgb);
			break;
		case 1: // FM77AV Demo uses 1.  My guess is it is for erasing a line.
		case HD_CMD_NOT://5
			VRAMPlane[i][VRAMAddr]&=~(rgb&writeBit);
			break;
		case HD_CMD_CMP://7,
			break;
		}
	}
}

std::vector <std::string> FM77AVCRTC::GetStatusText(void) const
{
	std::vector <std::string> text;

	text.push_back("MODE:");
	text.back()+=ScrnModeToStr(state.scrnMode);
	text.back()+=" VRAMOffset:";
	text.back()+=cpputil::Ustox(state.VRAMOffset[0]);
	text.back()+="/";
	text.back()+=cpputil::Ustox(state.VRAMOffset[1]);
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

	text.back()+=" TilePtn:";
	text.back()+=cpputil::Ubtox(state.hardDraw.tile[0]);
	text.back().push_back(' ');
	text.back()+=cpputil::Ubtox(state.hardDraw.tile[1]);
	text.back().push_back(' ');
	text.back()+=cpputil::Ubtox(state.hardDraw.tile[2]);

	text.push_back("");
	text.back()+="LINE AddrOffset="+cpputil::Ustox(state.hardDraw.addrOffset);
	text.back()+=" Style=";
	text.back()+=cpputil::Ustox(state.hardDraw.lineStipple);
	text.back()+=" (";
	text.back()+=cpputil::Itoa(state.hardDraw.x0);
	text.back()+=",";
	text.back()+=cpputil::Itoa(state.hardDraw.y0);
	text.back()+=")-(";
	text.back()+=cpputil::Itoa(state.hardDraw.x1);
	text.back()+=",";
	text.back()+=cpputil::Itoa(state.hardDraw.y1);
	text.back()+=")";

	return text;
}

/* virtual */ uint32_t FM77AVCRTC::SerializeVersion(void) const
{
	return 0;
}
/* virtual */ void FM77AVCRTC::SpecificSerialize(std::vector <unsigned char> &data,std::string stateFName) const
{
	PushUcharArray(data,8,state.palette.digitalPalette);
	PushUint16(data,state.palette.analogPaletteLatch);
	PushUcharArray(data,4096*3,&state.palette.analogPalette[0][0]);

	PushBool(data,state.hardDraw.enabled);
	PushUint16(data,state.hardDraw.condition);
	PushUint16(data,state.hardDraw.cmd);
	PushUint16(data,state.hardDraw.color);
	PushUint16(data,state.hardDraw.maskBits);
	PushUint16(data,state.hardDraw.compareResult);
	PushUcharArray(data,8,state.hardDraw.compareColor);
	PushUint16(data,state.hardDraw.bankMask);
	PushUcharArray(data,3,state.hardDraw.tile);
	PushUcharArray(data,8,state.hardDraw.tilePtnCache);

	PushUint64(data,state.hardDraw.lineBusyBy);
	PushUint16(data,state.hardDraw.addrOffset);
	PushUint16(data,state.hardDraw.lineStipple);
	PushUint16(data,state.hardDraw.x0);
	PushUint16(data,state.hardDraw.y0);
	PushUint16(data,state.hardDraw.x1);
	PushUint16(data,state.hardDraw.y1);

	PushUint32(data,state.scrnMode);
	PushUint32(data,state.avScrnMode);

	PushBool(data,state.VRAMAccessFlag);

	for(int i=0; i<FM77AV40_NUM_VRAM_BANKS; ++i)
	{
		PushUint16(data,state.VRAMOffset[i]);
	}
	PushUint16(data,state.VRAMOffsetMask);
	PushUint16(data,state.VRAMAccessMask);
	PushUint16(data,state.displayPage);
	PushUint16(data,state.activePage);
	PushBool(data,state.CRTEnabled);
}
/* virtual */ bool FM77AVCRTC::SpecificDeserialize(const unsigned char *&data,std::string stateFName,uint32_t version)
{
	ReadUcharArray(data,8,state.palette.digitalPalette);
	state.palette.analogPaletteLatch=ReadUint16(data);
	ReadUcharArray(data,4096*3,&state.palette.analogPalette[0][0]);

	state.hardDraw.enabled=ReadBool(data);
	state.hardDraw.condition=ReadUint16(data);
	state.hardDraw.cmd=ReadUint16(data);
	state.hardDraw.color=ReadUint16(data);
	state.hardDraw.maskBits=ReadUint16(data);
	state.hardDraw.compareResult=ReadUint16(data);
	ReadUcharArray(data,8,state.hardDraw.compareColor);
	state.hardDraw.bankMask=ReadUint16(data);
	ReadUcharArray(data,3,state.hardDraw.tile);
	ReadUcharArray(data,8,state.hardDraw.tilePtnCache);

	state.hardDraw.lineBusyBy=ReadUint64(data);
	state.hardDraw.addrOffset=ReadUint16(data);
	state.hardDraw.lineStipple=ReadUint16(data);
	state.hardDraw.x0=ReadUint16(data);
	state.hardDraw.y0=ReadUint16(data);
	state.hardDraw.x1=ReadUint16(data);
	state.hardDraw.y1=ReadUint16(data);

	state.scrnMode=ReadUint32(data);
	state.avScrnMode=ReadUint32(data);

	state.VRAMAccessFlag=ReadBool(data);

	for(int i=0; i<FM77AV40_NUM_VRAM_BANKS; ++i)
	{
		state.VRAMOffset[i]=ReadUint16(data);
	}
	state.VRAMOffsetMask=ReadUint16(data);
	state.VRAMAccessMask=ReadUint16(data);
	state.displayPage=ReadUint16(data);
	state.activePage=ReadUint16(data);
	state.CRTEnabled=ReadBool(data);

	return true;
}
