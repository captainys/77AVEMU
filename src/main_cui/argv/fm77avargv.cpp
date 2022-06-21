#include <iostream>
#include "fm77avargv.h"



bool FM77AVArgv::AnalyzeCommandParameter(int argc,char *argv[])
{
	if(argc<2)
	{
		std::cout << "ERROR: Need ROM Path." << std::endl;
		return false;
	}
	ROMPath=argv[1];

	for(int i=2; i<argc; ++i)
	{
	}

	return true;
}
