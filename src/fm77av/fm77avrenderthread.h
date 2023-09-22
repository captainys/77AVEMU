#ifndef FM77AVRENDERTHREAD_IS_INCLUDED
#define FM77AVRENDERTHREAD_IS_INCLUDED
/* { */

#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <cstdint>

#include "fm77avrender.h"
#include "fm77av.h"
#include "fm77avdef.h"



class FM77AVRenderingThread
{
public:
	enum
	{
		NO_COMMAND,
		RENDER,
		QUIT
	};

	std::mutex mainMutex,statusMutex;
	std::thread workerThread;
	std::condition_variable cond;
	bool imageNeedsFlip=false;

	FM77AVRender *rendererPtr;
	FM77AVCRTC::Palette paletteCopy;

	bool imageReady=false;
	unsigned int command=NO_COMMAND;

	enum
	{
		STATE_IDLE,
		STATE_RENDERING,
	};
	unsigned int state=STATE_IDLE;
	int64_t checkImageAfterThisTIme;

private:
	void ThreadFunc(void);

public:
	FM77AVRenderingThread();
	~FM77AVRenderingThread();

	void CheckRenderingTimer(FM77AV &fm77av,class FM77AVRender &render);
	void CheckImageReady(FM77AV &fm77av,class Outside_World::WindowInterface &window);
	void WaitIdle(void);

	void DiscardRunningRenderingTask(void);
};


/* } */
#endif
