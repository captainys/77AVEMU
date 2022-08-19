#ifndef RUNVM_IS_INCLUDED
#define RUNVM_IS_INCLUDED
/* { */



#ifdef OUT
#undef OUT
#endif

#include <queue>
#include <vector>
#include <string>

#include "fm77av.h"
#include "fm77avthread.h"
#include "outside_world.h"
#include "fm77avcommand.h"
#include "fm77avparam.h"
#include "fssimplewindow_connection.h"

class FM77AVCommandQueue : public FM77AVUIThread
{
public:
	using FM77AVUIThread::uiLock;
	std::queue <std::string> cmdqueue;
	FM77AVCommandInterpreter cmdInterpreter;

	virtual void Main(FM77AVThread &vmThread,FM77AV &fm77av,const FM77AVParam &argv,Outside_World &outside_world);
	virtual void ExecCommandQueue(FM77AVThread &vmThread,FM77AV &fm77av,Outside_World *outside_world);

	void SendCommand(std::string cmd);
};

class FM77AVVM
{
public:
	FM77AVProfile profile;
	FM77AVRender lastImage;
	FM77AV *fm77avPtr=nullptr;
	FM77AVThread *fm77avThreadPtr=nullptr;
	FM77AVCommandQueue *cmdQueuePtr=nullptr;
	FsSimpleWindowConnection *outsideWorldPtr=nullptr;

	FM77AVVM();
	~FM77AVVM();

	void Alloc(void);
	void Free(void);

	std::vector <std::string> GetMissingROMFiles(void) const;
	void Run(void);
	bool IsRunning(void) const;
	void SendCommand(std::string cmd);
};



/* } */
#endif
