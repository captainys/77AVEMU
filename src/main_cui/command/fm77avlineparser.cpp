#include "mc6809.h"
#include "fm77avlineparser.h"
#include "cpputil.h"

FM77AVLineParserBase::FM77AVLineParserBase(const class MC6809 *main,const class MC6809 *sub,const class MC6809 *def)
{
	this->mainCPU=main;
	this->subCPU=sub;
	this->defaultCPU=def;
}
/* virtual */ long long int FM77AVLineParserBase::EvaluateRawNumber(const std::string &str) const
{
	auto cpu=defaultCPU;
	std::string cap=str;
	cpputil::Capitalize(cap);

	if("MAIN."==cap.substr(0,5))
	{
		cpu=mainCPU;
		cap=cap.substr(5);
	}
	else if("M."==cap.substr(0,2))
	{
		cpu=mainCPU;
		cap=cap.substr(2);
	}
	else if("SUB."==cap.substr(0,4))
	{
		cpu=subCPU;
		cap=cap.substr(4);
	}
	else if("S."==cap.substr(0,2))
	{
		cpu=subCPU;
		cap=cap.substr(2);
	}
	else if('\''==str[0] && 0!=str[1] && '\''==str[2])
	{
		return int(str[1]);
	}

	auto reg=MC6809::StrToReg(cap);
	if(MC6809::REG_NULL!=reg)
	{
		if(nullptr!=cpu)
		{
			return cpu->GetRegisterValue(reg);
		}
		evaluationError=true;
		return 0;
	}
	else
	{
		return EvaluateNotARegister(str);
	}
}
long long int FM77AVLineParserBase::Evaluate(void) const
{
	evaluationError=false;
	return LineParser::Evaluate();
}
long long int FM77AVLineParser::EvaluateNotARegister(const std::string &str) const
{
	return cpputil::Atoi(str.c_str());
}
long long int FM77AVLineParserHexadecimal::EvaluateNotARegister(const std::string &str) const
{
	return cpputil::Xtoi(str.c_str());
}
