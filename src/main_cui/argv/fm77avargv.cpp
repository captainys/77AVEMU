#include <stdio.h>
#include <iostream>
#include "fm77avargv.h"
#include "fm77avkey.h"
#include "fm77avkeyboard.h"
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
	std::cout << "-AUTOSTARTTAPE" << std::endl;
	std::cout << "  Auto start the first tape program." << std::endl;
	std::cout << "-T77AUTOSAVE|-AUTOSAVETAPE t77file.t77" << std::endl;
	std::cout << "  In old days, we were saving programs/data in audio cassette tapes.  I had to press REC" << std::endl;
	std::cout << "  button before writing, and must release REC button before reading.  If I forgot pressing" << std::endl;
	std::cout << "  REC and thought I was saving data, it was not saved, and gone as soon as I turned off" << std::endl;
	std::cout << "  the PC.  If I forgot releasing the REC button before reading, I accidentally erased" << std::endl;
	std::cout << "  the tape instead of reading from it.  Not to repeat such tragedy, you can specify" << std::endl;
	std::cout << "  auto-save tape image.  The file does not have to exist.  The program will create it." << std::endl;
	std::cout << "  If you don't virtually press REC button in the emulator, or if you write-protect the" << std::endl;
	std::cout << "  primary tape image, data will be saved to this auto-save image.  You can later mount it" << std::endl;
	std::cout << "  for reading." << std::endl;
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
	std::cout << "-DIVD77 filename.d77" << std::endl;
	std::cout << "  Divides multi-disk D77 file into single-disk D77 files." << std::endl;
	std::cout << "  Suffix  _1, _2, _3, ... will be added after each file name." << std::endl;
	std::cout << "  Does nothing if the file is already a single-disk image." << std::endl;
	std::cout << "-NOWAIT" << std::endl;
	std::cout << "  Run VM without adjusting time for the wall-clock time." << std::endl;
	std::cout << "-ALIAS aliasLabel filename" << std::endl;
	std::cout << "  Define file-name alias.  Alias can later be used as a parameter to FDxLOAD, TAPELOAD commands." << std::endl;
	std::cout << "  eg. You can use -ALIAS DISKA \"full-path-to-game-diskA\" to ease disk swap from command." << std::endl;
	std::cout << "-KEYBOARD DIRECT/TRANS/TRANS2/TRANS3" << std::endl;
	std::cout << "  Keyboard mode.  DIRECT is good for gaming.  TRANS/TRANS2/TRANS3 is good for typing." << std::endl;
	std::cout << "-AUTOSTOP autoStopType" << std::endl;
	std::cout << "  FM-7 seriew before FM77AV could not detect key-release.  Therefore, game programs" << std::endl;
	std::cout << "  could start moving a character by a key stroke, but did not know when the key was" << std::endl;
	std::cout << "  released and should stop.  Therefore, we had to press typically Num-5 top stop a" << std::endl;
	std::cout << "  character.  In the emulator, this problem can be solved by virtually typing a" << std::endl;
	std::cout << "  different key when num keys or arrow keys are released.  You can choose from" << std::endl;
	std::cout << "  AFTER_NUM_RELEASE, AFTER_ARROW_RELEASE, AFTER_NUM_RELEASE_AND_RETYPE," << std::endl;
	std::cout << "  AFTER_ARROW_RELEASE_AND_RETYPE, and AFTER_ANY_KEY_RELEASE." << std::endl;
	std::cout << "-AUTOSTOPKEY keyCode" << std::endl;
	std::cout << "  Specify which key should virtually be pressed after num keys or arrow keys are" << std::endl;
	std::cout << "  released." << std::endl;
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
	std::cout << "-VIRTKEY townsKey gamePadPhysicalId button" << std::endl;
	std::cout << "  Assign a virtual key to a gamepad button." << std::endl;
	std::cout << "-COM0, -COM1, -COM2, -COM3" << std::endl;
	std::cout << "  Enable COM port." << std::endl;
	std::cout << "-QUICKSSDIR dir" << std::endl;
	std::cout << "  Specify quick screen shot directory." << std::endl;
	std::cout << "-SSCROP x0 y0 wid hei" << std::endl;
	std::cout << "  Crop screen shot.  x0 and wid is FM-7's screen coordinate." << std::endl;
	std::cout << "  y0 and hei is FM-7's screen coordinate in 320x200 and 640x400 mode," << std::endl;
	std::cout << "  FM-7's screen coordinate times 2 in 640x200 mode." << std::endl;
	std::cout << "  In other words, it is all in screen-shot coordinate." << std::endl;
	std::cout << "-HOSTSHORTCUT hostKey ctrl shift \"command\"" << std::endl;
	std::cout << "  Assign host short cut key (hot key)." << std::endl;
	std::cout << "-INITCMD cmd" << std::endl;
	std::cout << "  Specify initial command." << std::endl;
	std::cout << "-EVTLOG filename.evt" << std::endl;
	std::cout << "  Specify event-log file to play back." << std::endl;
	std::cout << "-SCALE X" << std::endl;
	std::cout << "  Screen scaling X percent." << std::endl;
	std::cout << "-AUTOSCALE" << std::endl;
	std::cout << "  Auto scaleing screen to match the window size." << std::endl;
	std::cout << "-MAXIMIZE" << std::endl;
	std::cout << "  Maximize the window on start up" << std::endl;
	std::cout << "-FULLSCREEN" << std::endl;
	std::cout << "  Fullscreen on start up" << std::endl;
	std::cout << "-SYM symbol-file" << std::endl;
	std::cout << "  Load symbol file, and auto-save when modified." << std::endl;
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
	for(int i=1; i<argc; ++i)
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
			autoScaling=true;
		}
		else if("-FULLSCREEN"==ARG)
		{
			windowModeOnStartUp=WINDOW_FULLSCREEN;
			autoScaling=true;
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
		else if("-AUTOSTARTTAPE"==ARG)
		{
			autoLoadTapeFile=true;
		}
		else if(("-T77AUTOSAVE"==ARG || "-AUTOSAVETAPE"==ARG) && i+1<argc)
		{
			t77SavePath=argv[i+1];
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
		else if("-DIVD77"==ARG && i+1<argc)
		{
			if(true!=DivideD77(argv[i+1]))
			{
				return false;
			}
			++i;
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
		else if("-AUTOSTOP"==ARG && i+1<argc)
		{
			autoStopType=FM77AVKeyboard::StrToAutoStop(argv[i+1]);
			if(FM77AVKeyboard::AUTOSTOP_NONE==autoStopType)
			{
				std::cout << "Auto-stop type can be one of:" << std::endl;
				std::cout << "  AFTER_NUM_RELEASE" << std::endl;
				std::cout << "  AFTER_NUM_RELEASE_AND_RETYPE" << std::endl;
				std::cout << "  AFTER_ARROW_RELEASE_AND_RETYPE" << std::endl;
				std::cout << "  AFTER_ARROW_RELEASE_AND_RETYPE" << std::endl;
				std::cout << "  AFTER_ANY_KEY_RELEASE" << std::endl;
				return false;
			}
			++i;
		}
		else if("-AUTOSTOPKEY"==ARG && i+1<argc)
		{
			autoStopKey=FM77AVKeyLabelToKeyCode(argv[i+1]);
			if(AVKEY_NULL==autoStopKey)
			{
				std::cout << "Key code can be one of:" << std::endl;
				for(int i=0; i<AVKEY_NUM_KEYCODE; ++i)
				{
					if(AVKEY_NULL!=i)
					{
						std::cout << FM77AVKeyCodeToKeyLabel(i) << std::endl;
					}
				}
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
		else if("-POWEROFFAT"==ARG && i+2<argc)
		{
			powerOffAtCPUType=StrToCPU(argv[i+1]);
			powerOffAtAddr=cpputil::Xtoi(argv[i+2]);
			i+=2;
		}
		else if("-UNITTEST"==ARG)
		{
			unitTest=true;
		}
		else if("-QUICKSSDIR"==ARG && i+1<argc)
		{
			quickScrnShotDir=argv[i+1];
			++i;
		}
		else if("-SSCROP"==ARG && i+4<argc)
		{
			scrnShotX0=cpputil::Atoi(argv[i+1]);
			scrnShotY0=cpputil::Atoi(argv[i+2]);
			scrnShotWid=cpputil::Atoi(argv[i+3]);
			scrnShotHei=cpputil::Atoi(argv[i+4]);
			i+=4;
		}
		else if("-HOSTSHORTCUT"==ARG && i+4<argc)
		{
			HostShortCut hsc;
			hsc.hostKey=argv[i+1];
			hsc.ctrl=(0!=cpputil::Atoi(argv[i+2]));
			hsc.shift=(0!=cpputil::Atoi(argv[i+3]));
			hsc.cmdStr=argv[i+4];
			hostShortCutKeys.push_back(hsc);
			i+=4;
		}
		else if("-INITCMD"==ARG && i+1<argc)
		{
			initialCmd.push_back(argv[i+1]);
			++i;
		}
		else if("-EVTLOG"==ARG && i+1<argc)
		{
			playbackEventLogFName=argv[i+1];
			++i;
		}
		else if("-COM0"==ARG || "-COM1"==ARG || "-COM2"==ARG || "-COM3"==ARG)
		{
			enableCOM[ARG[4]-'0']=true;
		}
		else if("-ALIAS"==ARG && i+2<argc)
		{
			auto LABEL=argv[i+1];
			cpputil::Capitalize(LABEL);
			fileNameAlias[LABEL]=argv[i+2];
			i+=2;
		}
		else if(("-VIRTKEY"==ARG || "-VIRTUALKEY"==ARG) && i+3<argc)
		{
			VirtualKey vk;
			vk.fm77avKey=argv[i+1];
			vk.physicalId=cpputil::Atoi(argv[i+2]);
			vk.button=cpputil::Atoi(argv[i+3]);
			virtualKeys.push_back(vk);
			i+=3;
		}
		else if("-SYM"==ARG && i+1<argc)
		{
			symTableFName=argv[i+1];
			++i;
		}
		else
		{
			if(1!=i) // First arg probably was a ROM dir, if not an option.
			{
				std::cout << "Unknown option " << argv[i] << std::endl;
				return false;
			}
		}
	}

	if(true==dontStart)
	{
		return false;
	}
	return true;
}

/* static */ bool FM77AVArgv::DivideD77(std::string d77FName)
{
	auto d77Bin=cpputil::ReadBinaryFile(d77FName);

	D77File d77;
	if(0<d77Bin.size())
	{
		d77.SetData(d77Bin);

		if(d77.GetNumDisk()<=1)
		{
			// Already a single-disk image.
			return true;
		}

		auto fNameBase=cpputil::RemoveExtension(d77FName.c_str());

		for(int diskId=0; diskId<d77.GetNumDisk(); ++diskId)
		{
			auto diskPtr=d77.GetDisk(diskId);
			std::cout << "Disk Label: " << diskPtr->header.diskName << std::endl;

			char suffix[8];
			sprintf(suffix,"_%d.D77",diskId+1);

			auto outFName=fNameBase+suffix;
			std::cout << "Saved as:" << outFName << std::endl;

			auto d77Img=diskPtr->MakeD77Image();
			if(true!=cpputil::WriteBinaryFile(outFName,d77Img.size(),d77Img.data()))
			{
				std::cout << "Could not write file." << std::endl;
				return false;
			}
		}
	}
	else
	{
		std::cout << "Could not read the source file." << std::endl;
		return false;
	}
}
