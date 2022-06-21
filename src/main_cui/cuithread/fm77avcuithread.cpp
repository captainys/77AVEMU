#include <iostream>
#include "fm77avcuithread.h"



void FM77AVCUIThread::Run(void)
{
	for(;;)
	{
		std::string cmd;
		std::cout << ">";
		std::getline(std::cin,cmd);

		uiLock.lock();
		cmdQueue.push(cmd);
		uiLock.unlock();
	}
}
