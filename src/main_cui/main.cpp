#include <memory>
#include <iostream>
#include <thread>
#include "fm77avargv.h"
#include "fm77av.h"
#include "fm77avthread.h"
#include "fm77avcuithread.h"



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

	FM77AVCUIThread cui;
	std::thread cuiThread(&FM77AVCUIThread::Run,&cui);


	class Outside_World *outside_world=nullptr; // Tentative

	FM77AVThread vm;
	vm.VMStart(fm77av.get(),outside_world,&cui);
	std::thread vmThread(&FM77AVThread::VMMainLoop,&vm,fm77av.get(),outside_world,&cui);
	vmThread.join();
	vm.VMEnd(fm77av.get(),outside_world,&cui);

	cuiThread.join();

	return 0;
}
