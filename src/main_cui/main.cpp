#include <memory>
#include <iostream>
#include <thread>
#include "fm77avargv.h"
#include "fm77av.h"
#include "fm77avthread.h"
#include "fm77avcuithread.h"
#include "outside_world.h"

#include "fssimplewindow_connection.h"



#ifdef _WIN32
#include <timeapi.h>
#else
static void timeBeginPeriod(int)
{
}
static void timeEndPeriod(int)
{
}
#endif


void RunWindowThreadLoop(Outside_World::WindowInterface *window)
{
	auto t0=std::chrono::high_resolution_clock::now();
	window->ClearVMClosedFlag();
	while(true!=window->CheckVMClosed())
	{
		window->Interval();
		auto t=std::chrono::high_resolution_clock::now();
		auto dt=t-t0;
		if(50<=std::chrono::duration_cast<std::chrono::milliseconds>(dt).count() || true==window->winThr.newImageRendered)
		{
			window->Render(true);
			t0=t;
			window->winThr.newImageRendered=false;
		}
		else
		{
			timeBeginPeriod(1);
			std::this_thread::sleep_for(std::chrono::milliseconds(8));
			timeEndPeriod(1);
		}
	}
}

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
	auto window=outside_world->CreateWindowInterface();

	if(true!=fm77av->SetUp(fm77avargv,outside_world.get(),window))
	{
		return 1;
	}

	static FM77AVThread vm;
	if(true==fm77avargv.pauseOnStart)
	{
		vm.SetRunMode(FM77AVThread::RUNMODE_PAUSE);
	}

	window->Start();

	if(true!=fm77avargv.unitTest)
	{
		FM77AVCUIThread cui;
		std::thread cuiThread(&FM77AVCUIThread::Run,&cui);
		auto sound=outside_world->CreateSound();
		std::thread vmThread([&]
		{
			window->ClearVMClosedFlag();
			vm.VMStart(fm77av.get(),outside_world.get(),window,&cui);
			vm.VMMainLoop(fm77av.get(),outside_world.get(),window,sound,&cui);
			vm.VMEnd(fm77av.get(),outside_world.get(),window,&cui);
		});

		RunWindowThreadLoop(window);

		vmThread.join();
		cuiThread.join();

		window->Stop();

		outside_world->DeleteSound(sound);
		outside_world->DeleteWindowInterface(window);
	}
	else
	{
		FM77AVUIThread noUI;
		auto sound=outside_world->CreateSound();
		std::thread vmThread([&]
		{
			window->ClearVMClosedFlag();
			vm.VMStart(fm77av.get(),outside_world.get(),window,&noUI);
			vm.VMMainLoop(fm77av.get(),outside_world.get(),window,sound,&noUI);
			vm.VMEnd(fm77av.get(),outside_world.get(),window,&noUI);
		});

		RunWindowThreadLoop(window);

		vmThread.join();

		window->Stop();

		outside_world->DeleteSound(sound);
		outside_world->DeleteWindowInterface(window);

		return fm77av->TestSuccess();
	}

	return 0;
}
