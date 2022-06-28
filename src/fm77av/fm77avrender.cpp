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

void FM77AVRender::Prepare(const FM77AV::CRTC &crtc)
{
}

void FM77AVRender::BuildImage(const unsigned char VRAM[],const class FM77AV::Palette &palette)
{
}
