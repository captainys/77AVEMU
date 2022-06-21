#include <memory>
#include <iostream>
#include "fm77avargv.h"
#include "fm77av.h"
#include "fm77avthread.h"



int main(int argc,char *argv[])
{
	FM77AVArgv fm77avargv;
	if(true!=fm77avargv.AnalyzeCommandParameter(argc,argv))
	{
		return 1;
	}

	std::cout << "FM7/77AV Emulator Mutsu" << std::endl;
	std::cout << "by CaptainYS" << std::endl;
	std::cout << "http://www.ysflight.com" << std::endl;

	std::unique_ptr <FM77AV> fm77av(new FM77AV);
	if(true!=fm77av->SetUp(fm77avargv))
	{
		return 1;
	}


	return 0;
}
