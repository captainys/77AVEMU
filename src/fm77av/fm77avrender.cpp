#include "fm77avrender.h"



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

FM77AVRender::Image FM77AVRender::GetImage(void) const
{
	Image image;
	image.wid=wid;
	image.hei=hei;
	image.rgba=rgba.data();
	return image;
}
void FM77AVRender::Prepare(const FM77AVCRTC &crtc)
{
	this->scrnMode=crtc.state.scrnMode;
	this->VRAMOffset=crtc.state.VRAMOffset;
	switch(crtc.state.scrnMode)
	{
	case SCRNMODE_640X200_SINGLE:
	case SCRNMODE_640X200_DOUBLE:
	case SCRNMODE_640X400_SINGLE:
		Create(640,400);
		break;
	case SCRNMODE_320X200_4096COL:
	case SCRNMODE_320X200_260KCOL:
		Create(320,200);
		break;
	}
}

void FM77AVRender::BuildImage(const unsigned char VRAM[],const class FM77AVCRTC::Palette &palette)
{
	switch(scrnMode)
	{
	case SCRNMODE_640X200_SINGLE:
		{
			for(unsigned int y=0; y<200; ++y)
			{
				auto rgba0=rgba.data()+(640*4)*(y*2);
				auto rgba1=rgba.data()+(640*4)*(y*2+1);

				for(unsigned int x=0; x<640; x+=8)
				{
					unsigned int addr=y*80+(x>>3);
					addr=FM77AVCRTC::TransformVRAMAddress(addr,scrnMode,VRAMOffset);
					unsigned int B=VRAM[addr];
					unsigned int R=VRAM[addr+0x4000];
					unsigned int G=VRAM[addr+0x8000];

					for(int dx=0; dx<8; ++dx)
					{
						// Tentative.  Must consider palette.
						rgba0[0]=((R&0x80) ? 0xFF : 0);
						rgba0[1]=((G&0x80) ? 0xFF : 0);
						rgba0[2]=((B&0x80) ? 0xFF : 0);
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
