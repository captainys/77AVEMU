#include "runvm.h"
#include "fssimplewindow_connection.h"


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
	fm77avThreadPtr->SetRunMode(FM77AVThread::RUNMODE_RUN);
	fm77avThreadPtr->SetReturnOnPause(true);
}
void FM77AVVM::Free(void)
{
	delete fm77avThreadPtr;
	delete fm77avPtr;
	delete cmdQueuePtr;
	delete outsideWorldPtr;
	fm77avPtr=nullptr;
	fm77avThreadPtr=nullptr;
	cmdQueuePtr=nullptr;
	outsideWorldPtr=nullptr;
}

void FM77AVVM::Run(void)
{
	if(nullptr==fm77avPtr ||
	   FM77AVThread::RUNMODE_EXIT==fm77avThreadPtr->GetRunMode())
	{
		Free();
		Alloc();

		fm77avPtr->SetUp(profile,outsideWorldPtr);
		fm77avThreadPtr->SetRunMode(FM77AVThread::RUNMODE_RUN);
		fm77avThreadPtr->VMStart(fm77avPtr,outsideWorldPtr,cmdQueuePtr);
	}
	else
	{
		fm77avThreadPtr->SetRunMode(FM77AVThread::RUNMODE_RUN);
	}

	fm77avThreadPtr->VMMainLoop(fm77avPtr,outsideWorldPtr,cmdQueuePtr);

	if(FM77AVThread::RUNMODE_EXIT==fm77avThreadPtr->GetRunMode())
	{
		fm77avThreadPtr->VMEnd(fm77avPtr,outsideWorldPtr,cmdQueuePtr);
		Free();
	}
	else
	{
		fm77avPtr->ForceRender(lastImage,outsideWorldPtr);
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
