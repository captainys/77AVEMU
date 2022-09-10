#ifndef FM77AVRENDER_IS_INCLUDED
#define FM77AVRENDER_IS_INCLUDED
/* { */

#include "fm77av.h"
#include "fm77avdef.h"
#include "fm77avcrtc.h"

class FM77AVRender
{
private:
	unsigned int wid,hei;
	std::vector <unsigned char> rgba;

	bool CRTEnabled;

	unsigned int scrnMode;
	unsigned int VRAMOffset[FM77AV40_NUM_VRAM_BANKS];
	unsigned int VRAMAccessMask;

	unsigned char VRAM[FM77AV40_VRAM_SIZE];

public:
	class Image
	{
	public:
		unsigned int wid,hei;
		const unsigned char *rgba;
	};

	FM77AVRender();

	/*!
	*/
	Image GetImage(void) const;

	/*! Create a bitmap image.
	*/
	void Create(int wid,int hei);

	/*!
	*/
	void Crop(unsigned int x0,unsigned int y0,unsigned int wid,unsigned int hei);

	/*! 
	*/
	void Prepare(const FM77AV &fm77av);

	/*! 
	*/
	void BuildImage(const class FM77AVCRTC::Palette &palette);

	/*!
	*/
	void FlipUpsideDown(void);
};

/* } */
#endif
