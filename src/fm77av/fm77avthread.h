#ifndef FM77AVTHREAD_IS_INCLUDED
#define FM77AVTHREAD_IS_INCLUDED
/* { */

#include <thread>
#include <mutex>
#include "fm77av.h"

class FM77AVUIThread
{
public:
	std::mutex uiLock;
	bool vmTerminated=false;
	virtual void ExecCommandQueue(class FM77AVThread &fm77avThread,FM77AV &fm77avPtr,class Outside_World *outside_world){};
};

class FM77AVThread
{
private:
	FM77AV *townsPtr;
	// std::unique_ptr <TownsRenderingThread> renderingThread;
	int runMode=RUNMODE_PAUSE;
	bool returnOnPause=false;

	// This will be used for virtually slwoing down CPU when VM is lagging.
	int64_t timeDeficit=0;

public:
	enum
	{
		NANOSECONDS_PER_TIME_SYNC=    1000000, // 1ms
	};

	enum
	{
		RUNMODE_PAUSE,
		RUNMODE_RUN,
		RUNMODE_ONE_INSTRUCTION,
		RUNMODE_EXIT,
	};

public:
	FM77AVThread();
	~FM77AVThread();

	void VMStart(FM77AV *fm77avPtr,class Outside_World *outside_world,FM77AVUIThread *uiThread);
	void VMMainLoop(FM77AV *fm77avPtr,class Outside_World *outside_world,FM77AVUIThread *uiThread);
	void VMEnd(FM77AV *fm77avPtr,class Outside_World *outside_world,FM77AVUIThread *uiThread);
private:
	void AdjustRealTime(FM77AV *fm77avPtr,long long int cpuTimePassed,std::chrono::time_point<std::chrono::high_resolution_clock> time0,class Outside_World *outside_world);

public:
	void PrintStatus(FM77AV &fm77av) const;
};

/* } */
#endif
