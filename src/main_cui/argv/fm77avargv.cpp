#include <iostream>
#include "fm77avargv.h"
#include "cpputil.h"
#include "d77.h"



void FM77AVArgv::Help(void)
{
	std::cout << "Mutsu_CUI ROM-PATH <options>" << std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << "-IMGPATH dir" << std::endl;
	std::cout << "  Image search path." << std::endl;
	std::cout << "-T77|-TAPE t77file.t77" << std::endl;
	std::cout << "  Set T77 cassette data recorder dump." << std::endl;
	std::cout << "-FD0 filename" << std::endl;
	std::cout << "  Floppy disk image file name for Drive A." << std::endl;
	std::cout << "-FD1 filename" << std::endl;
	std::cout << "  Floppy disk image file name for Drive B." << std::endl;
	std::cout << "-FD0WP,-FD1WP" << std::endl;
	std::cout << "  Write protect floppy disk." << std::endl;
	std::cout << "-FD0UP,-FD1UP" << std::endl;
	std::cout << "  Write un-protect floppy disk." << std::endl;
	std::cout << "-GENFD filename.d77 320|640" << std::endl;
	std::cout << "  Generate an empty disk. Need to specify 320KB disk (2D) or 640KB disk (2DD)" << std::endl;
	std::cout << "  Specify 400 as size to make 2D disk with 1024-byte per sector, 5 sectors per track." << std::endl;
	std::cout << "-NOWAIT" << std::endl;
	std::cout << "  Run VM without adjusting time for the wall-clock time." << std::endl;
}
bool FM77AVArgv::AnalyzeCommandParameter(int argc,char *argv[])
{
	if(argc<2)
	{
		std::cout << "ERROR: Need ROM Path." << std::endl;
		return false;
	}
	ROMPath=argv[1];

	bool dontStart=false;
	for(int i=2; i<argc; ++i)
	{
		std::string arg=argv[i];
		auto ARG=arg;
		cpputil::Capitalize(ARG);

		if("-H"==ARG || "-HELP"==ARG)
		{
			Help();
			dontStart=true;
		}
		else if("-NOWAIT"==ARG)
		{
			noWait=true;
		}
		else if(("-T77"==ARG || "-TAPE"==ARG) && i+1<argc)
		{
			t77Path=argv[i+1];
			++i;
		}
		else if("-IMGPATH"==ARG && i+1<argc)
		{
			imgSearchPaths.push_back(argv[i+1]);
			++i;
		}
		else if(("-FD0"==ARG || "-FD1"==ARG || "-FD2"==ARG || "-FD3"==ARG) && i+1<argc)
		{
			int drv=ARG[3]-'0';
			fdImgFName[drv]=argv[i+1];
			++i;
		}
		else if("-FD0WP"==ARG || "-FD1WP"==ARG || "-FD2WP"==ARG || "-FD3WP"==ARG)
		{
			int drv=ARG[3]-'0';
			fdImgWriteProtect[drv]=true;
		}
		else if("-FD0UP"==ARG || "-FD1UP"==ARG || "-FD2UP"==ARG || "-FD3UP"==ARG)
		{
			int drv=ARG[3]-'0';
			fdImgWriteProtect[drv]=false;
		}
		else if("-GENFD"==ARG && i+2<argc)
		{
			std::string fName=argv[i+1];
			std::string size=argv[i+2];
			unsigned int KB=cpputil::Atoi(argv[i+2]);
			if(640==KB || 320==KB || 400==KB)
			{
				D77File d77;
				switch(KB)
				{
				case 320:
				case 400:
					d77.CreateUnformatted(80,"FM7DISK");
					break;
				case 640:
					d77.CreateUnformatted(160,"FM7DISK");
					break;
				}

				auto diskPtr=d77.GetDisk(0);
				int mediaType=0;
				int numTracks;
				int sectorsPerTrack;
				int sectorSize;
				D77File::D77Disk::D77Sector sec[16];

				if(320==KB)
				{
					mediaType=0;
					numTracks=80;
					sectorsPerTrack=16;
					sectorSize=256;
				}
				else if(400==KB)
				{
					mediaType=0;
					numTracks=80;
					sectorsPerTrack=5;
					sectorSize=1024;
				}
				else if(640==KB)
				{
					mediaType=0x10;
					numTracks=160;
					sectorsPerTrack=8;
					sectorSize=512;
				}

				diskPtr->header.mediaType=mediaType;
				for(int i=0; i<numTracks; ++i)
				{
					for(int j=0; j<sectorsPerTrack; ++j)
					{
						sec[j].Make(i/2,i%2,j+1,sectorSize);
						sec[j].nSectorTrack=sectorsPerTrack;
					}
					diskPtr->WriteTrack(i/2,i%2,sectorsPerTrack,sec);
				}


				std::vector <unsigned char> img;

				auto ext=cpputil::GetExtension(fName.c_str());
				auto EXT=cpputil::Capitalize(ext);
				if(".D77"==EXT)
				{
					std::cout << "Making D77 disk image." << std::endl;
					img=d77.MakeD77Image();
				}
				else
				{
					std::cout << "Unsupported data type." << std::endl;
					return false;
				}

				if(true!=cpputil::WriteBinaryFile(fName,img.size(),img.data()))
				{
					std::cout << "Failed to write file: " << fName << std::endl;
					return false;
				}
				else
				{
					std::cout << "Created FD Image: " << fName << std::endl;
				}
			}
			else
			{
				std::cout << "Unsupported floppy-disk size: " << KB << std::endl;
				std::cout << "Must be 640 or 320" << std::endl;
				return false;
			}
			i+=2;
		}
		else
		{
			std::cout << "Unknown option " << argv[i] << std::endl;
			return false;
		}
	}

	if(true==dontStart)
	{
		return false;
	}
	return true;
}
