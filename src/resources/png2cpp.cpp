#include "yspng.h"
#include "cpputil.h"

#include <iostream>
#include <fstream>
#include <string>

// 16x16 icons to C++ source.

std::string files[]=
{
	"FD_BUSY",
	"FD_IDLE",
	"TAPE_IDLE",
	"TAPE_LOADING",
	"TAPE_SAVING",
};

std::string nonSquareFiles[]=
{
	"MENU",
	"PAUSE",
	"CAPS",
	"KANA",
	"INS",
};

int main(void)
{
	// Check all of them are 16x16
	for(auto fName : files)
	{
		fName+=".png";
		std::cout << "Checking:"<<fName<<std::endl;

		YsRawPngDecoder png;
		png.Decode(fName.c_str());
		if(png.wid!=16 || png.hei!=16)
		{
			std::cout << fName << " is not 16x16." << std::endl;
			return 1;
		}
	}

	// Save as .cpp
	std::ofstream ofs;
	ofs.open("../outside_world/icons.cpp");

	ofs << "#include" << '"' << "icons.h" << '"' << std::endl;

	for(auto fName : files)
	{
		ofs << "const unsigned char " << fName << "[]={" << std::endl;

		YsRawPngDecoder png;
		png.Decode((fName+".png").c_str());

		for(int i=0; i<png.wid*png.hei; ++i)
		{
			ofs<<"0x"<<cpputil::Ubtox(png.rgba[i*4  ])<<",";
			ofs<<"0x"<<cpputil::Ubtox(png.rgba[i*4+1])<<",";
			ofs<<"0x"<<cpputil::Ubtox(png.rgba[i*4+2])<<",";
			ofs<<"0x"<<cpputil::Ubtox(png.rgba[i*4+3])<<",";
			if((i+1)%8==0 || i+1==png.wid*png.hei)
			{
				ofs<<std::endl;
			}
		}

		ofs << "};" << std::endl;
	}

	for(auto fName : nonSquareFiles)
	{
		YsRawPngDecoder png;
		png.Decode((fName+".png").c_str());

		ofs << "const int " << fName << "_wid=" << png.wid << ";" << std::endl;
		ofs << "const int " << fName << "_hei=" << png.hei << ";" << std::endl;

		ofs << "const unsigned char " << fName << "[]={" << std::endl;

		for(int i=0; i<png.wid*png.hei; ++i)
		{
			ofs<<"0x"<<cpputil::Ubtox(png.rgba[i*4  ])<<",";
			ofs<<"0x"<<cpputil::Ubtox(png.rgba[i*4+1])<<",";
			ofs<<"0x"<<cpputil::Ubtox(png.rgba[i*4+2])<<",";
			ofs<<"0x"<<cpputil::Ubtox(png.rgba[i*4+3])<<",";
			if((i+1)%8==0 || i+1==png.wid*png.hei)
			{
				ofs<<std::endl;
			}
		}

		ofs << "};" << std::endl;
	
	}

	ofs.close();


	ofs.open("../outside_world/icons.h");

	std::cout << "#ifndef FM77AV_OUTSIDE_WORLD_ICONS_IS_INCLUDED" << std::endl;
	std::cout << "#define FM77AV_OUTSIDE_WORLD_ICONS_IS_INCLUDED" << std::endl;

	std::cout << "//16x16 icons" << std::endl;
	for(auto fName : files)
	{
		ofs << "extern const unsigned char " << fName << "[];" << std::endl;
	}

	std::cout << "//Non-square icons" << std::endl;
	for(auto fName : nonSquareFiles)
	{
		YsRawPngDecoder png;
		png.Decode((fName+".png").c_str());

		ofs << "extern const int " << fName << "_wid;" << std::endl;
		ofs << "extern const int " << fName << "_hei;" << std::endl;
		ofs << "extern const unsigned char " << fName << "[];" << std::endl;
	}
	ofs.close();


	std::cout << "#endif" << std::endl;

	return 0;
}
