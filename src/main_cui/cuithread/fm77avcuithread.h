#ifndef FM77AVCUITHREAD_IS_INCLUDED
#define FM77AVCUITHREAD_IS_INCLUDED
/* { */

#include <queue>
#include "fm77avthread.h"
#include "fm77avcommand.h"

class FM77AVCUIThread : public FM77AVUIThread, public FM77AVCommand
{
public:
	std::queue <std::string> cmdQueue;
	void Run(void);
};

/* } */
#endif
