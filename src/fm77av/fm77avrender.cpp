#include <algorithm>
#include "fm77av.h"
#include "fm77avrender.h"
#include "string.h"



FM77AVRender::FM77AVRender()
{
	Create(640,400);
}

void FM77AVRender::Create(int wid,int hei)
{
	rgba.resize(wid*hei*4);
	this->wid=wid;
	this->hei=hei;
}

void FM77AVRender::Crop(unsigned int x0,unsigned int y0,unsigned int newWid,unsigned int newHei)
{
	std::vector <unsigned char> newRGBA;
	newRGBA.resize(newWid*newHei*4);

	memset(newRGBA.data(),0,newRGBA.size());

	unsigned char *dstRow=newRGBA.data();
	unsigned char *srcRow=rgba.data()+(wid*y0+x0)*4;
	unsigned int copySizePerRow=0;
	if(x0+newWid<=wid)
	{
		copySizePerRow=newWid*4;
	}
	else if(x0<wid)
	{
		copySizePerRow=(wid-x0)*4;
	}
	for(unsigned int y=0; y<newHei && y0+y<hei; ++y)
	{
		memcpy(dstRow,srcRow,copySizePerRow);
		dstRow+=newWid*4;
		srcRow+=wid*4;
	}

	std::swap(rgba,newRGBA);
	wid=newWid;
	hei=newHei;
}

FM77AVRender::Image FM77AVRender::GetImage(void) const
{
	Image image;
	image.wid=wid;
	image.hei=hei;
	image.rgba=rgba.data();
	return image;
}
FM77AVRender::ImageCopy FM77AVRender::GetImageCopy(void)
{
	ImageCopy image;
	std::swap(image.rgba,rgba);
	image.wid=wid;
	image.hei=hei;

	wid=0;
	hei=0;
	rgba.clear();
	return image;
}
void FM77AVRender::Prepare(const FM77AV &fm77av)
{
	this->CRTEnabled=fm77av.crtc.state.CRTEnabled;
	if(true==fm77av.crtc.state.CRTEnabled)
	{
		this->scrnMode=fm77av.crtc.state.scrnMode;
		this->VRAMOffset[0]=fm77av.crtc.state.VRAMOffset[0];
		this->VRAMOffset[1]=fm77av.crtc.state.VRAMOffset[1];
		this->VRAMOffset[2]=fm77av.crtc.state.VRAMOffset[2];
		this->VRAMAccessMask=fm77av.crtc.state.VRAMAccessMask;

		switch(fm77av.crtc.state.scrnMode)
		{
		case SCRNMODE_640X200:
			memcpy(this->VRAM,
			       fm77av.physMem.GetVRAMBank(fm77av.crtc.state.displayPage),
			       fm77av.physMem.GetVRAMBankSize(fm77av.crtc.state.displayPage));
			this->VRAMOffset[0]=this->VRAMOffset[fm77av.crtc.state.displayPage];
			break;
		case SCRNMODE_320X200_4096COL:
			memcpy(this->VRAM,fm77av.physMem.GetVRAMBank(0),fm77av.physMem.GetVRAMBankSize(0));
			memcpy(this->VRAM+fm77av.physMem.GetVRAMBankSize(0),fm77av.physMem.GetVRAMBank(1),fm77av.physMem.GetVRAMBankSize(1));
			break;
		case SCRNMODE_640X400:
			memcpy(this->VRAM                        ,fm77av.physMem.GetVRAMBank(0),fm77av.physMem.GetVRAMBankSize(0));
			memcpy(this->VRAM+FM77AV_VRAM_BANK_SIZE  ,fm77av.physMem.GetVRAMBank(1),fm77av.physMem.GetVRAMBankSize(1));
			memcpy(this->VRAM+FM77AV_VRAM_BANK_SIZE*2,fm77av.physMem.GetVRAMBank(2),fm77av.physMem.GetVRAMBankSize(2));
			break;
		}
	}
}

void FM77AVRender::BuildImage(const class FM77AVCRTC::Palette &palette)
{
	switch(this->scrnMode)
	{
	case SCRNMODE_640X200:
	case SCRNMODE_640X400:
		Create(640,400);
		break;
	case SCRNMODE_320X200_4096COL:
	case SCRNMODE_320X200_260KCOL:
		Create(320,200);
		break;
	}

	if(true!=this->CRTEnabled)
	{
		memset(rgba.data(),0,rgba.size());
		return;
	}

	switch(scrnMode)
	{
	case SCRNMODE_640X200:
		{
			for(unsigned int y=0; y<200; ++y)
			{
				auto rgba0=rgba.data()+(640*4)*(y*2);
				auto rgba1=rgba.data()+(640*4)*(y*2+1);

				for(unsigned int x=0; x<640; x+=8)
				{
					unsigned int addr=y*80+(x>>3);
					addr=FM77AVCRTC::TransformVRAMAddress(addr,scrnMode,VRAMOffset[0]);
					unsigned int B=VRAM[addr];
					unsigned int R=VRAM[addr+0x4000];
					unsigned int G=VRAM[addr+0x8000];

					if(0!=(VRAMAccessMask&1))
					{
						B=0;
					}
					if(0!=(VRAMAccessMask&2))
					{
						R=0;
					}
					if(0!=(VRAMAccessMask&4))
					{
						G=0;
					}

					for(int dx=0; dx<8; ++dx)
					{
						uint8_t code=
							(0!=(G&0x80) ? 4 : 0) |
							(0!=(R&0x80) ? 2 : 0) |
							(0!=(B&0x80) ? 1 : 0);
						code=palette.digitalPalette[code];

						rgba0[0]=((code&2) ? 0xFF : 0);
						rgba0[1]=((code&4) ? 0xFF : 0);
						rgba0[2]=((code&1) ? 0xFF : 0);
						rgba0[3]=0xFF;
						rgba1[0]=rgba0[0];
						rgba1[1]=rgba0[1];
						rgba1[2]=rgba0[2];
						rgba1[3]=rgba0[3];
						rgba0+=4;
						rgba1+=4;
						R<<=1;
						G<<=1;
						B<<=1;
					}
				}
			}
		}
		break;
	case SCRNMODE_320X200_4096COL:
		{
			for(unsigned int y=0; y<200; ++y)
			{
				auto rgba0=rgba.data()+(320*4)*y;

				for(unsigned int x=0; x<320; x+=8)
				{
					unsigned int addrBase=y*40+(x>>3);
					unsigned int addrBank0=FM77AVCRTC::TransformVRAMAddress(addrBase,scrnMode,VRAMOffset[0]);
					unsigned int addrBank1=FM77AVCRTC::TransformVRAMAddress(addrBase,scrnMode,VRAMOffset[1]);

					unsigned int B3=VRAM[addrBank0];
					unsigned int B2=VRAM[addrBank0+ 0x2000];
					unsigned int B1=VRAM[addrBank1+ 0xC000];
					unsigned int B0=VRAM[addrBank1+ 0xE000];

					unsigned int R3=VRAM[addrBank0+ 0x4000];
					unsigned int R2=VRAM[addrBank0+ 0x6000];
					unsigned int R1=VRAM[addrBank1+0x10000];
					unsigned int R0=VRAM[addrBank1+0x12000];

					unsigned int G3=VRAM[addrBank0+ 0x8000];
					unsigned int G2=VRAM[addrBank0+ 0xA000];
					unsigned int G1=VRAM[addrBank1+0x14000];
					unsigned int G0=VRAM[addrBank1+0x16000];

					if(0!=(VRAMAccessMask&1)) // Is this flag valid in 4096-color mode?
					{
						B3=0;
						B2=0;
						B1=0;
						B0=0;
					}
					if(0!=(VRAMAccessMask&2)) // Is this flag valid in 4096-color mode?
					{
						R3=0;
						R2=0;
						R1=0;
						R0=0;
					}
					if(0!=(VRAMAccessMask&4)) // Is this flag valid in 4096-color mode?
					{
						G3=0;
						G2=0;
						G1=0;
						G0=0;
					}

					for(int dx=0; dx<8; ++dx)
					{
						uint16_t code=
							(0!=(G3&0x80) ? 2048 : 0) |
							(0!=(G2&0x80) ? 1024 : 0) |
							(0!=(G1&0x80) ? 512 : 0) |
							(0!=(G0&0x80) ? 256 : 0) |
							(0!=(R3&0x80) ? 128 : 0) |
							(0!=(R2&0x80) ? 64 : 0) |
							(0!=(R1&0x80) ? 32 : 0) |
							(0!=(R0&0x80) ? 16 : 0) |
							(0!=(B3&0x80) ? 8 : 0) |
							(0!=(B2&0x80) ? 4 : 0) |
							(0!=(B1&0x80) ? 2 : 0) |
							(0!=(B0&0x80) ? 1 : 0);
						rgba0[0]=palette.analogPalette[code][0];
						rgba0[1]=palette.analogPalette[code][1];
						rgba0[2]=palette.analogPalette[code][2];
						rgba0[3]=0xFF;
						rgba0+=4;
						R3<<=1;
						R2<<=1;
						R1<<=1;
						R0<<=1;
						G3<<=1;
						G2<<=1;
						G1<<=1;
						G0<<=1;
						B3<<=1;
						B2<<=1;
						B1<<=1;
						B0<<=1;
					}
				}
			}
		}
		break;
	case SCRNMODE_640X400:
		{
			for(unsigned int y=0; y<400; ++y)
			{
				auto pixel=rgba.data()+(640*4)*y;

				for(unsigned int x=0; x<640; x+=8)
				{
					unsigned int addr=y*80+(x>>3);
					addr=FM77AVCRTC::TransformVRAMAddress(addr,scrnMode,VRAMOffset[addr&1]);
					unsigned int B=VRAM[addr];
					unsigned int R=VRAM[addr+FM77AV_VRAM_BANK_SIZE];
					unsigned int G=VRAM[addr+FM77AV_VRAM_BANK_SIZE*2];

					if(0!=(VRAMAccessMask&1))
					{
						B=0;
					}
					if(0!=(VRAMAccessMask&2))
					{
						R=0;
					}
					if(0!=(VRAMAccessMask&4))
					{
						G=0;
					}

					for(int dx=0; dx<8; ++dx)
					{
						uint8_t code=
							(0!=(G&0x80) ? 4 : 0) |
							(0!=(R&0x80) ? 2 : 0) |
							(0!=(B&0x80) ? 1 : 0);
						code=palette.digitalPalette[code];

						pixel[0]=((code&2) ? 0xFF : 0);
						pixel[1]=((code&4) ? 0xFF : 0);
						pixel[2]=((code&1) ? 0xFF : 0);
						pixel[3]=0xFF;
						pixel+=4;
						R<<=1;
						G<<=1;
						B<<=1;
					}
				}
			}
		}
		break;
	}
}
void FM77AVRender::FlipUpsideDown(void)
{
	std::vector <unsigned char> flip;
	flip.resize(wid*4);

	auto upsideDown=rgba.data();
	auto rightSideUp=rgba.data()+(hei-1)*wid*4;
	for(unsigned int y=0; y<hei/2; ++y)
	{
		memcpy(flip.data(),upsideDown ,wid*4);
		memcpy(upsideDown ,rightSideUp,wid*4);
		memcpy(rightSideUp,flip.data(),wid*4);
		upsideDown+=wid*4;
		rightSideUp-=wid*4;
	}
}
