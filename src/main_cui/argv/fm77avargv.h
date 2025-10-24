#ifndef FM77AVARGV_IS_INCLUDED
#define FM77AVARGV_IS_INCLUDED
/* { */

#include "fm77avparam.h"

class FM77AVArgv : public FM77AVParam
{
public:
	void Help(void);
	void PrintApplicationList(void) const;
	bool AnalyzeCommandParameter(int argc,char *argv[]);

	bool TryLoadProfile(std::string fileName);

	static bool DivideD77(std::string d77FName);
};

/* } */
#endif
