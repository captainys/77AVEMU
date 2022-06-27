#ifndef FM77AVLINEPARSER_IS_INCLUDED
#define FM77AVLINEPARSER_IS_INCLUDED
/* { */

#include "lineparser.h"

class FM77AVLineParser : public LineParser
{
public:
	const class MC6809 *cpuPtr;
	FM77AVLineParser(const class MC6809 *);
	virtual long long int EvaluateRawNumber(const std::string &str) const;
};

class FM77AVLineParserHexadecimal : public LineParser
{
public:
	const class MC6809 *cpuPtr;
	FM77AVLineParserHexadecimal(const class MC6809 *);
	virtual long long int EvaluateRawNumber(const std::string &str) const;
};

/* } */
#endif
