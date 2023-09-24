#include <thread>
#include <chrono>
#include "runvm.h"
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

/* virtual */ void FM77AVCommandQueue::Main(FM77AVThread &,FM77AV &,const FM77AVParam &,Outside_World &)
{
}
/* virtual */ void FM77AVCommandQueue::ExecCommandQueue(FM77AVThread &vmThread,FM77AV &fm77av,Outside_World *outside_world)
{
	for(int i=0; i<2; ++i)
	{
		auto &cmdqueue=(i==0 ? this->cmdqueue : outside_world->commandQueue);
		while(true!=cmdqueue.empty())
		{
			auto cmdline=cmdqueue.front();
			cmdqueue.pop();

			std::cout << cmdline << std::endl;

			auto cmd=cmdInterpreter.Interpret(cmdline);
			cmdInterpreter.Execute(vmThread,fm77av,outside_world,cmd);
			if(FM77AVCommandInterpreter::CMD_QUIT==cmd.primaryCmd)
			{
			}
		}
	}
}

void FM77AVCommandQueue::SendCommand(std::string cmd)
{
	uiLock.lock();
	this->cmdqueue.push(cmd);
	uiLock.unlock();
}

FM77AVVM::FM77AVVM()
{
}
FM77AVVM::~FM77AVVM()
{
	Free();
}

void FM77AVVM::Alloc(void)
{
	fm77avPtr=new FM77AV;
	fm77avThreadPtr=new FM77AVThread;
	cmdQueuePtr=new FM77AVCommandQueue;
	outsideWorldPtr=new FsSimpleWindowConnection;
	outsideWorldPtr->lowerRightIcon=Outside_World::LOWER_RIGHT_MENU;
	soundPtr=outsideWorldPtr->CreateSound();
	windowPtr=outsideWorldPtr->CreateWindowInterface();
	fm77avThreadPtr->SetRunMode(FM77AVThread::RUNMODE_RUN);
	fm77avThreadPtr->SetReturnOnPause(true);
}
void FM77AVVM::Free(void)
{
	if(nullptr!=outsideWorldPtr)
	{
		outsideWorldPtr->DeleteSound(soundPtr);
		soundPtr=nullptr;
		outsideWorldPtr->DeleteWindowInterface(windowPtr);
		windowPtr=nullptr;
	}
	delete fm77avThreadPtr;
	delete fm77avPtr;
	delete cmdQueuePtr;
	delete outsideWorldPtr;
	fm77avPtr=nullptr;
	fm77avThreadPtr=nullptr;
	cmdQueuePtr=nullptr;
	outsideWorldPtr=nullptr;
}

std::vector <std::string> FM77AVVM::GetMissingROMFiles(void) const
{
	return fm77avPtr->CheckMissingROMFiles(profile);
}

void FM77AVVM::Run(void)
{
	bool freshStart=false;
	if(nullptr==fm77avPtr ||
	   FM77AVThread::RUNMODE_EXIT==fm77avThreadPtr->GetRunMode())
	{
		Free();
		Alloc();

		fm77avPtr->SetUp(profile,outsideWorldPtr,windowPtr);
		windowPtr->Start();
		freshStart=true;
	}

	windowPtr->ClearVMClosedFlag();

	std::thread VMThr([&]
	{
		if(true==freshStart)
		{
			fm77avThreadPtr->VMStart(fm77avPtr,outsideWorldPtr,windowPtr,cmdQueuePtr);
		}
		fm77avThreadPtr->SetRunMode(FM77AVThread::RUNMODE_RUN);
		fm77avThreadPtr->VMMainLoop(fm77avPtr,outsideWorldPtr,windowPtr,soundPtr,cmdQueuePtr);
		if(FM77AVThread::RUNMODE_EXIT==fm77avThreadPtr->GetRunMode())
		{
			fm77avThreadPtr->VMEnd(fm77avPtr,outsideWorldPtr,windowPtr,cmdQueuePtr);
		}
	});

	auto t0=std::chrono::high_resolution_clock::now();
	while(true!=windowPtr->CheckVMClosed())
	{
		windowPtr->Interval();
		auto t=std::chrono::high_resolution_clock::now();
		auto dt=t-t0;
		if(50<=std::chrono::duration_cast<std::chrono::milliseconds>(dt).count() || true==windowPtr->winThr.newImageRendered)
		{
			windowPtr->Render(true);
			t0=t;
			windowPtr->winThr.newImageRendered=false;
		}
		else
		{
			timeBeginPeriod(1);
			std::this_thread::sleep_for(std::chrono::milliseconds(8));
			timeEndPeriod(1);
		}
	}

	VMThr.join();

	if(FM77AVThread::RUNMODE_EXIT==fm77avThreadPtr->GetRunMode())
	{
		windowPtr->Stop();
		Free();
	}
}
bool FM77AVVM::IsRunning(void) const
{
	return
		(nullptr!=fm77avThreadPtr &&
		 FM77AVThread::RUNMODE_EXIT!=fm77avThreadPtr->GetRunMode());
}
void FM77AVVM::SendCommand(std::string cmd)
{
	if(nullptr!=cmdQueuePtr)
	{
		cmdQueuePtr->SendCommand(cmd);
	}
}
