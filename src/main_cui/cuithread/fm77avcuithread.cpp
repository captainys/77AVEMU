#include <iostream>
#include "fm77avcuithread.h"



void FM77AVCUIThread::Run(void)
{
	while(true!=vmTerminated && true!=cuiQuit)
	{
		std::string cmd;
		std::cout << ">";
		std::getline(std::cin,cmd);

		uiLock.lock();
		cmdQueue.push(cmd);
		uiLock.unlock();

		auto CMD=Interpret(cmd);
		if(CMD_QUIT==CMD.primaryCmd || CMD_FORCE_QUIT==CMD.primaryCmd)
		{
			break;
		}
	}
}

/* virtual */ void FM77AVCUIThread::ExecCommandQueue(class FM77AVThread &fm77avThread,FM77AV &fm77av,class Outside_World *outside_world)
{
	if(true!=cmdQueue.empty())
	{
		auto cmdStr=cmdQueue.front();
		cmdQueue.pop();
		auto cmd=FM77AVCommandInterpreter::Interpret(cmdStr);
		FM77AVCommandInterpreter::Execute(fm77avThread,fm77av,outside_world,cmd);
	}

}
