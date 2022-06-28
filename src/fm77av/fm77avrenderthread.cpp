#include <iostream>
#include <thread>
#include <chrono>

#include <stdlib.h>
#include <string.h>

#include "fm77avrenderthread.h"
#include "fm77avrender.h"
#include "outside_world.h"



FM77AVRenderingThread::FM77AVRenderingThread()
{
	command=NO_COMMAND;
	std::thread thr(&FM77AVRenderingThread::ThreadFunc,this);
	workerThread.swap(thr);
}
FM77AVRenderingThread::~FM77AVRenderingThread()
{
	std::cout << "Ending Rendering Thread." << std::endl;
	{
		std::unique_lock <std::mutex> mainLock(mainMutex);
		command=QUIT;
	}
	cond.notify_one();
	workerThread.join();
	std::cout << "." << std::endl;
}

void FM77AVRenderingThread::ThreadFunc(void)
{
	std::unique_lock <std::mutex> mainLock(mainMutex);
	for(;;)
	{
		cond.wait(mainLock,[&]{return NO_COMMAND!=command;});
		if(QUIT==command)
		{
			break;
		}
		else if(RENDER==command)
		{
			rendererPtr->BuildImage(VRAMCopy,paletteCopy);

			if(true==imageNeedsFlip)
			{
				rendererPtr->FlipUpsideDown();
			}

			{
				std::unique_lock <std::mutex> statusLock(statusMutex);
				command=NO_COMMAND;
				imageReady=true;
			}
		}
	}
}

void FM77AVRenderingThread::WaitIdle(void)
{
	// Being able to lock means the thread is either just started, or waiting on condition variable.
	std::unique_lock <std::mutex> mainLock(mainMutex);
}

void FM77AVRenderingThread::CheckRenderingTimer(FM77AV &fm77av,FM77AVRender &render)
{
	if(STATE_IDLE==state && 
	   fm77av.state.nextRenderingTime<=fm77av.state.fm77avTime && 
	   true!=fm77av.crtc.InVSYNC(fm77av.state.fm77avTime))
	{
		render.Prepare(fm77av.crtc);
		this->rendererPtr=&render;
		memcpy(this->VRAMCopy,fm77av.physMem.GetVRAMBank(0),fm77av.physMem.GetVRAMBankSize(0)); // Bank 0 always.
		// switch(screen mode)
		//{
		//case SCRNMODE_640X480_SINGLE:
		//	break;
		//}
		this->paletteCopy=fm77av.crtc.GetPalette();

		state=STATE_RENDERING;
		fm77av.state.nextRenderingTime=fm77av.state.fm77avTime+FM77AV_RENDERING_FREQUENCY;

		checkImageAfterThisTIme=fm77av.state.fm77avTime+3000000; // Give sub-thread some time.

		{
			std::unique_lock <std::mutex> statusLock(statusMutex);
			command=RENDER;
			imageReady=false;
		}
		cond.notify_one();
	}
}

void FM77AVRenderingThread::CheckImageReady(FM77AV &fm77av,Outside_World &world)
{
	if(STATE_RENDERING==state && checkImageAfterThisTIme<fm77av.state.fm77avTime)
	{
		bool imageReady;
		{
			std::unique_lock <std::mutex> statusLock(statusMutex);
			imageReady=this->imageReady;
		}
		if(true==imageReady)
		{
			world.Render(rendererPtr->GetImage(),fm77av);
			world.UpdateStatusBitmap(fm77av);
			state=STATE_IDLE;
		}
	}
}

void FM77AVRenderingThread::DiscardRunningRenderingTask(void)
{
	WaitIdle();
	state=STATE_IDLE;
	{
		std::unique_lock <std::mutex> statusLock(statusMutex);
		command=NO_COMMAND;
		imageReady=false;
	}
}
