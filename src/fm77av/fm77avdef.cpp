#include "fm77avdef.h"
#include "cpputil.h"



std::string MachineTypeToStr(unsigned int machineType)
{
	switch(machineType)
	{
	case MACHINETYPE_FM7:
		return "FM-7";
	case MACHINETYPE_FM77AV:
		return "FM77AV";
	case MACHINETYPE_FM77AV40:
		return "FM77AV40";
	}
	return "UNKNOWN";
}
unsigned int StrToMachineType(std::string str)
{
	if("FM7"==str || "FM-7"==str)
	{
		return MACHINETYPE_FM7;
	}
	if("FM77AV"==str)
	{
		return MACHINETYPE_FM77AV;
	}
	if("FM77AV40"==str)
	{
		return MACHINETYPE_FM77AV40;
	}
	return MACHINETYPE_UNKNOWN;
}
std::string CPUToStr(unsigned int cpuMainOrSub)
{
	if(CPU_MAIN==cpuMainOrSub)
	{
		return "MAIN";
	}
	if(CPU_SUB==cpuMainOrSub)
	{
		return "SUB";
	}
	return "?";
}
unsigned int StrToCPU(std::string str)
{
	cpputil::Capitalize(str);
	if("MAIN"==str || "M"==str)
	{
		return CPU_MAIN;
	}
	else if("SUB"==str || "S"==str)
	{
		return CPU_SUB;
	}
	return CPU_UNKNOWN;
}

std::string SubCmdToStr(unsigned int subCmd)
{
	switch(subCmd)
	{
	case FM7_SUBCMD_INKEY:
		return "INKEY";
	case FM7_SUBCMD_DEFINE_PFKEY:
		return "DEFINE_PFKEY";
	case FM7_SUBCMD_GET_PFKEY_STR:
		return "GET_PFKEY_STR";
	case FM7_SUBCMD_INT_CTRL:
		return "INT_CTRL";
	case FM7_SUBCMD_SET_TIMER:
		return "SET_TIMER";
	case FM7_SUBCMD_READ_TIMER:
		return "READ_TIMER";
	case FM7_SUBCMD_TEST:
		return "TEST";
	case FM7_SUBCMD_INIT:
		return "INIT";
	case FM7_SUBCMD_ERASE:
		return "ERASE";
	case FM7_SUBCMD_PUT:
		return "PUT";
	case FM7_SUBCMD_GET:
		return "GET";
	case FM7_SUBCMD_GETC:
		return "GETC";
	case FM7_SUBCMD_GET_CHAR_BLOCK1:
		return "GET_CHAR_BLOCK1";
	case FM7_SUBCMD_PUT_CHAR_BLOCK1:
		return "PUT_CHAR_BLOCK1";
	case FM7_SUBCMD_GET_CHAR_BLOCK2:
		return "GET_CHAR_BLOCK2";
	case FM7_SUBCMD_PUT_CHAR_BLOCK2:
		return "PUT_CHAR_BLOCK2";
	case FM7_SUBCMD_GET_BUFFER_ADDR:
		return "GET_BUFFER_ADDR";
	case FM7_SUBCMD_TABSET:
		return "TABSET";
	case FM7_SUBCMD_CONSOLE_CONTROL:
		return "CONSOLE_CONTROL";
	case FM7_SUBCMD_ERASE2:
		return "ERASE2";
	case FM7_SUBCMD_LINE:
		return "LINE";
	case FM7_SUBCMD_CHAIN:
		return "CHAIN";
	case FM7_SUBCMD_POINT:
		return "POINT";
	case FM7_SUBCMD_PAINT:
		return "PAINT";
	case FM7_SUBCMD_SYMBOL:
		return "SYMBOL";
	case FM7_SUBCMD_CHANGE_COLOR:
		return "CHANGE_COLOR";
	case FM7_SUBCMD_GET_BLOCK1:
		return "GET_BLOCK1";
	case FM7_SUBCMD_PUT_BLOCK1:
		return "PUT_BLOCK1";
	case FM7_SUBCMD_GET_BLOCK2:
		return "GET_BLOCK2";
	case FM7_SUBCMD_PUT_BLOCK2:
		return "PUT_BLOCK2";
	case FM7_SUBCMD_GCURSOR:
		return "GCURSOR";
	case FM7_SUBCMD_CHAR_LINE:
		return "CHAR_LINE";
	}
	return "?";
}

std::string BIOSCmdToStr(unsigned int biosCmd)
{
	switch(biosCmd)
	{
	case FM7_BIOSCMD_ANALOGP: //0x00,
		return "ANALOGP";
	case FM7_BIOSCMD_MOTOR: //0x01,
		return "MOTOR";
	case FM7_BIOSCMD_CTBWRT: //0x02,
		return "CTBWRT";
	case FM7_BIOSCMD_CTBRED: //0x03,
		return "CTBRED";
	case FM7_BIOSCMD_INTBBL: //0x04,
		return "INTBBL";
	case FM7_BIOSCMD_SCREEN: //0x05,
		return "SCREEN";
	case FM7_BIOSCMD_WRTBBL: //0x06,
		return "WRTBBL";
	case FM7_BIOSCMD_REDBBL: //0x07,
		return "REDBBL";
	case FM7_BIOSCMD_RESTOR: //0x08,
		return "RESTOR";
	case FM7_BIOSCMD_DWRITE: //0x09,
		return "DWRITE";
	case FM7_BIOSCMD_DREAD: //0x0A,
		return "DREAD";
	case FM7_BIOSCMD_UNUSED1: //0x0B,
		return "UNUSED1";
	case FM7_BIOSCMD_BEEPON: //0x0C,
		return "BEEPON";
	case FM7_BIOSCMD_BEEPOF: //0x0D,
		return "BEEPOF";
	case FM7_BIOSCMD_LPOUT: //0x0E,
		return "LPOUT";
	case FM7_BIOSCMD_HDCOPY: //0x0F,
		return "HDCOPY";
	case FM7_BIOSCMD_SUBOUT: //0x10,
		return "SUBOUT";
	case FM7_BIOSCMD_SUBIN: //0x11,
		return "SUBIN";
	case FM7_BIOSCMD_INPUT: //0x12,
		return "INPUT";
	case FM7_BIOSCMD_INPUTC: //0x13,
		return "INPUTC";
	case FM7_BIOSCMD_OUTPUT: //0x14,
		return "OUTPUT";
	case FM7_BIOSCMD_KEYIN: //0x15,
		return "KEYIN";
	case FM7_BIOSCMD_KANJIR: //0x16,
		return "KANJIR";
	case FM7_BIOSCMD_LPCHK: //0x17,
		return "LPCHK";
	case FM7_BIOSCMD_BIINIT: //0x18,
		return "BIINIT";
	case FM7_BIOSCMD_UNUSED2: //0x19,
		return "UNUSED2";
	case FM7_BIOSCMD_UNUSED3: //0x1A,
		return "UNUSED3";
	case FM7_BIOSCMD_UNUSED4: //0x1B,
		return "UNUSED4";
	}
	return "?";
}
