#ifndef FM77AVARGV_IS_INCLUDED
#define FM77AVARGV_IS_INCLUDED
/* { */

#include "fm77avparam.h"

class FM77AVArgv : public FM77AVParam
{
public:
	bool AnalyzeCommandParameter(int argc,char *argv[]);
};

/* } */
#endif
