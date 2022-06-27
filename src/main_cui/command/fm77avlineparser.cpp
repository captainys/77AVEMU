#include "mc6809.h"
#include "fm77avlineparser.h"
#include "cpputil.h"

FM77AVLineParser::FM77AVLineParser(const class MC6809 *cpuPtr)
{
	this->cpuPtr=cpuPtr;
}
/* virtual */ long long int FM77AVLineParser::EvaluateRawNumber(const std::string &str) const
{
	std::string cap=str;
	cpputil::Capitalize(cap);
	auto reg=cpuPtr->StrToReg(cap);
	if(MC6809::REG_NULL!=reg)
	{
		return cpuPtr->GetRegisterValue(reg);
	}
	else
	{
		return cpputil::Atoi(str.c_str());
	}
}

FM77AVLineParserHexadecimal::FM77AVLineParserHexadecimal(const class MC6809 *cpuPtr)
{
	this->cpuPtr=cpuPtr;
}
/* virtual */ long long int FM77AVLineParserHexadecimal::EvaluateRawNumber(const std::string &str) const
{
	std::string cap=str;
	cpputil::Capitalize(cap);
	auto reg=cpuPtr->StrToReg(cap);
	if(MC6809::REG_NULL!=reg)
	{
		return cpuPtr->GetRegisterValue(reg);
	}
	else
	{
		return cpputil::Xtoi(str.c_str());
	}
}
