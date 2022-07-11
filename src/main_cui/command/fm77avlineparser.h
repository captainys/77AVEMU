#ifndef FM77AVLINEPARSER_IS_INCLUDED
#define FM77AVLINEPARSER_IS_INCLUDED
/* { */

#include "lineparser.h"

class FM77AVLineParserBase : public LineParser
{
public:
	mutable bool evaluationError=false;
	const class MC6809 *mainCPU=nullptr,*subCPU=nullptr,*defaultCPU=nullptr;

	FM77AVLineParserBase(const class MC6809 *main,const class MC6809 *sub,const class MC6809 *def);
	long long int Evaluate(void) const;
	virtual long long int EvaluateRawNumber(const std::string &str) const;

	virtual long long int EvaluateNotARegister(const std::string &str) const=0;
};

class FM77AVLineParser : public FM77AVLineParserBase
{
public:
	FM77AVLineParser(const class MC6809 *main,const class MC6809 *sub,const class MC6809 *def) : FM77AVLineParserBase(main,sub,def){}
	long long int EvaluateNotARegister(const std::string &str) const;
};


class FM77AVLineParserHexadecimal : public FM77AVLineParserBase
{
public:
	FM77AVLineParserHexadecimal(const class MC6809 *main,const class MC6809 *sub,const class MC6809 *def) : FM77AVLineParserBase(main,sub,def){}
	long long int EvaluateNotARegister(const std::string &str) const;
};

/* } */
#endif
