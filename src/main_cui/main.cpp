#include <memory>
#include <iostream>
#include <thread>
#include "fm77avargv.h"
#include "fm77av.h"
#include "fm77avthread.h"
#include "fm77avcuithread.h"
#include "outside_world.h"

#include "fssimplewindow_connection.h"



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
	std::unique_ptr <Outside_World> outside_world(new FsSimpleWindowConnection);

	if(true!=fm77av->SetUp(fm77avargv,outside_world.get()))
	{
		return 1;
	}

	FM77AVCUIThread cui;
	std::thread cuiThread(&FM77AVCUIThread::Run,&cui);

	static FM77AVThread vm;
	vm.VMStart(fm77av.get(),outside_world.get(),&cui);
	std::thread vmThread(&FM77AVThread::VMMainLoop,&vm,fm77av.get(),outside_world.get(),&cui);
	vmThread.join();
	vm.VMEnd(fm77av.get(),outside_world.get(),&cui);

	cuiThread.join();

	return 0;
}
