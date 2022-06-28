#ifndef FM77AVRENDER_IS_INCLUDED
#define FM77AVRENDER_IS_INCLUDED
/* { */

#include "fm77av.h"

class FM77AVRender
{
private:
	unsigned int wid,hei;
	std::vector <unsigned char> rgba;

public:
	class Image
	{
	public:
		unsigned int wid,hei;
		unsigned char *rgba;
	};

	FM77AVRender();

	/*! Create a bitmap image.
	*/
	void Create(int wid,int hei);

	/*! 
	*/
	void Prepare(const FM77AV::CRTC &crtc);

	/*! 
	*/
	void BuildImage(const unsigned char VRAM[],const class FM77AV::Palette &palette);
};

/* } */
#endif
