#include <iostream>
#include "fm77avargv.h"
#include "cpputil.h"



void FM77AVArgv::Help(void)
{
	std::cout << "Mutsu_CUI ROM-PATH <options>" << std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << "-T77|-TAPE t77file.t77" << std::endl;
	std::cout << "  Set T77 cassette data recorder dump." << std::endl;
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
