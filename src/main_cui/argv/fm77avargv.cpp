#include <iostream>
#include "fm77avargv.h"
#include "cpputil.h"
#include "d77.h"



void FM77AVArgv::Help(void)
{
	std::cout << "Mutsu_CUI ROM-PATH <options>" << std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << "-PAUSE" << std::endl;
	std::cout << "  Pause on start." << std::endl;
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
	std::cout << "-GAMEPORT0 KEY|PHYSx|ANAx|NONE" << std::endl;
	std::cout << "-GAMEPORT1 KEY|PHYSx|ANAx|NONE" << std::endl;
	std::cout << "  Specify game-port emulation.  By keyboard (Arrow,Z,X,A,S), or physical gamepad." << std::endl;
	std::cout << "  PHYS0,PHYS1,PHYS2,PHYS3 use physical game pad direction button (or hat switch) as up/down/left/right." << std::endl;
	std::cout << "  ANA0,ANA1,ANA2,ANA3 use physical game pad analog stick as up/down/left/right." << std::endl;
	std::cout << "  KEYMOUSE use arrow keys and ZX keys for mouse cursor and buttons." << std::endl;
	std::cout << "  NUMPADMOUSE use NUMPAD number keys and /* keys for mouse cursor and buttons." << std::endl;
	std::cout << "  PHYS0MOUSE,PHYS1MOUSE,PHYS2MOUSE,PHYS3MOUSE use physical game pad digital axis for mouse." << std::endl;
	std::cout << "  ANA0MOUSE,ANA1MOUSE,ANA2MOUSE,ANA3MOUSE use physical game pad analog axis for mouse." << std::endl;
	std::cout << "-BUTTONHOLDTIME0 0|1 time_in_millisec" << std::endl;
	std::cout << "-BUTTONHOLDTIME1 0|1 time_in_millisec" << std::endl;
	std::cout << "  In some games, when you click on a menu or a button, you end up selecting the next menu" << std::endl;
	std::cout << "  or the button that happens to appear in the next options.  Daikoukai Jidai (KOEI)" << std::endl;
	std::cout << "  is virtually unplayable.  Super Daisenryaku keeps scrolling to the end when you want" << std::endl;
	std::cout << "  to scroll only by one screen width.  You really had to press the button and release" << std::endl;
	std::cout << "  immediately.  How patient we were!" << std::endl;
	std::cout << "  It is due to the bad programming.  When the program reads button state as DOWN, it must" << std::endl;
	std::cout << "  wait until the state changes to UP before making a next selection.  But, we cannot go" << std::endl;
	std::cout << "  in to all of the programs and write patches.  Instead, this option let you limit" << std::endl;
	std::cout << "  maximum time that the button is sent as DOWN to the virtual machine even when" << std::endl;
	std::cout << "  you keep it down for 100 seconds." << std::endl;
	std::cout << "-SCALE X" << std::endl;
	std::cout << "  Screen scaling X percent." << std::endl;
	std::cout << "-AUTOSCALE" << std::endl;
	std::cout << "  Auto scaleing screen to match the window size." << std::endl;
	std::cout << "-MAXIMIZE" << std::endl;
	std::cout << "  Maximize the window on start up" << std::endl;
	std::cout << "-FULLSCREEN" << std::endl;
	std::cout << "  Fullscreen on start up" << std::endl;
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
		else if("-SCALE"==ARG && i+1<argc)
		{
			scaling=atoi(argv[i+1]);
			if(scaling<SCALING_MIN)
			{
				scaling=SCALING_MIN;
			}
			else if(SCALING_MAX<scaling)
			{
				scaling=SCALING_MAX;
			}
			++i;
		}
		else if("-AUTOSCALE"==ARG)
		{
			autoScaling=true;
		}
		else if("-MAXIMIZE"==ARG)
		{
			windowModeOnStartUp=WINDOW_MAXIMIZE;
		}
		else if("-FULLSCREEN"==ARG)
		{
			windowModeOnStartUp=WINDOW_FULLSCREEN;
		}
		else if("-PAUSE"==ARG)
		{
			pauseOnStart=true;
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
		else if("-KEYBOARD"==ARG && i+1<argc)
		{
			std::string MODE=argv[i+1];
			cpputil::Capitalize(MODE);
			if("DIRECT"==MODE)
			{
				keyboardMode=FM77AV_KEYBOARD_MODE_DIRECT;
			}
			else if("TRANS"==MODE || "TRANSLATION"==MODE || "TRANS1"==MODE || "TRANSLATION1"==MODE)
			{
				keyboardMode=FM77AV_KEYBOARD_MODE_TRANSLATION1;
			}
			else if("TRANS2"==MODE || "TRANSLATION2"==MODE)
			{
				keyboardMode=FM77AV_KEYBOARD_MODE_TRANSLATION2;
			}
			else if("TRANS3"==MODE || "TRANSLATION3"==MODE)
			{
				keyboardMode=FM77AV_KEYBOARD_MODE_TRANSLATION3;
			}
			else
			{
				std::cout << "Undefined keyboard emulation." << std::endl;
				return false;
			}
			++i;
		}
		else if(("-GAMEPORT0"==ARG || "-GAMEPORT1"==ARG) && i+1<argc)
		{
			int portId=(ARG.back()-'0')&1;
			std::string DEV=argv[i+1];
			cpputil::Capitalize(DEV);
			gamePort[portId]=StrToGamePortEmu(DEV);
			if(FM77AV_GAMEPORTEMU_NONE==gamePort[portId])
			{
				std::cout << "Undefined game-port device." << std::endl;
				std::cout << "Possible options:" << std::endl;
				for(int i=0; i<FM77AV_GAMEPORTEMU_NUM_DEVICES; ++i)
				{
					std::cout << GamePortEmuToStr(i) << std::endl;
				}
			}
			++i;
		}
		else if(("-BUTTONHOLDTIME0"==ARG || "-BUTTONHOLDTIME1"==ARG) && i+2<argc)
		{
			int portId=(ARG.back()-'0')&1;
			int button=cpputil::Atoi(argv[i+1])&1;
			long long int holdTime=cpputil::Atoi(argv[i+2]);
			maxButtonHoldTime[portId][button]=holdTime*1000000; // Make it nano sec.
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
