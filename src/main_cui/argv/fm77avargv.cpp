#include <iostream>
#include "fm77avargv.h"
#include "cpputil.h"



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
