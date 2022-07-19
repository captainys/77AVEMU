#ifndef FM77AVTHREAD_IS_INCLUDED
#define FM77AVTHREAD_IS_INCLUDED
/* { */

#include <thread>
#include <mutex>
#include "fm77av.h"
#include "fm77avrenderthread.h"
#include "fm77aveventlog.h"

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
	FM77AV *fm77avPtr;
	std::unique_ptr <FM77AVRenderingThread> renderingThread;
	int runMode=RUNMODE_RUN;
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

	class OutputControl
	{
	public:
		class CPUOutputControl
		{
		public:
			bool mute=false;
			mutable uint16_t lastPC=0;
		};
		CPUOutputControl main,sub;
	};

	OutputControl output;
	FM77AVEventLog eventLog;

public:
	FM77AVThread();
	~FM77AVThread();

	void SetRunMode(unsigned int runMode);
	/*! If one of main- or sub-CPUs is muted, returns the one NOT muted.
	*/
	unsigned int OnlyOneCPUIsUnmuted(void) const;

	void VMStart(FM77AV *fm77avPtr,class Outside_World *outside_world,FM77AVUIThread *uiThread);
	void VMMainLoop(FM77AV *fm77avPtr,class Outside_World *outside_world,FM77AVUIThread *uiThread);
	void VMEnd(FM77AV *fm77avPtr,class Outside_World *outside_world,FM77AVUIThread *uiThread);
private:
	void AdjustRealTime(FM77AV *fm77avPtr,long long int cpuTimePassed,std::chrono::time_point<std::chrono::high_resolution_clock> time0,class Outside_World *outside_world);

public:
	void PrintStatus(FM77AV &fm77av) const;
	void PrintStatus(FM77AV &fm77av,bool muteMain,bool muteSub) const;
	void PrintCPUState(FM77AV &fm77av,MC6809 &cpu,MemoryAccess &mem,unsigned int mainOrSub) const;
};

/* } */
#endif
