#include "fm77avdef.h"



static inline void Capitalize(std::string &str)
{
	for(auto &c : str)
	{
		if('a'<=c && c<='z')
		{
			c=c+'A'-'a';
		}
	}
}

std::string MachineTypeToStr(unsigned int machineType)
{
	switch(machineType)
	{
	case MACHINETYPE_FM7:
		return "FM-7";
	case MACHINETYPE_FM77:
		return "FM-77";
	case MACHINETYPE_FM77AV:
		return "FM77AV";
	case MACHINETYPE_FM77AV40:
		return "FM77AV40";
	}
	return "UNKNOWN";
}
unsigned int StrToMachineType(std::string str)
{
	Capitalize(str);
	if("FM7"==str || "FM-7"==str)
	{
		return MACHINETYPE_FM7;
	}
	if("FM77"==str || "FM-77"==str)
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
	Capitalize(str);
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

std::string ScrnModeToStr(int scrnMode)
{
	switch(scrnMode)
	{
	case SCRNMODE_640X200:
		return "640X200";
	case SCRNMODE_320X200_4096COL:
		return "320X200_4096COL";
	case SCRNMODE_320X200_260KCOL:
		return "320X200_260KCOL";
	case SCRNMODE_640X400:
		return "640X400";
	}
	return "SCRNMODE_?";
}

unsigned int StrToGamePortEmu(std::string str)
{
	Capitalize(str);
	if("NONE"==str)
	{
		return FM77AV_GAMEPORTEMU_NONE;
	}
	if("MOUSE"==str)
	{
		return FM77AV_GAMEPORTEMU_MOUSE;
	}
	if("KEY"==str)
	{
		return FM77AV_GAMEPORTEMU_KEYBOARD;
	}
	if("PHYS0"==str)
	{
		return FM77AV_GAMEPORTEMU_PHYSICAL0;
	}
	if("PHYS1"==str)
	{
		return FM77AV_GAMEPORTEMU_PHYSICAL1;
	}
	if("PHYS2"==str)
	{
		return FM77AV_GAMEPORTEMU_PHYSICAL2;
	}
	if("PHYS3"==str)
	{
		return FM77AV_GAMEPORTEMU_PHYSICAL3;
	}
	if("PHYS4"==str)
	{
		return FM77AV_GAMEPORTEMU_PHYSICAL4;
	}
	if("PHYS5"==str)
	{
		return FM77AV_GAMEPORTEMU_PHYSICAL5;
	}
	if("PHYS6"==str)
	{
		return FM77AV_GAMEPORTEMU_PHYSICAL6;
	}
	if("PHYS7"==str)
	{
		return FM77AV_GAMEPORTEMU_PHYSICAL7;
	}
	if("ANA0"==str)
	{
		return FM77AV_GAMEPORTEMU_ANALOG0;
	}
	if("ANA1"==str)
	{
		return FM77AV_GAMEPORTEMU_ANALOG1;
	}
	if("ANA2"==str)
	{
		return FM77AV_GAMEPORTEMU_ANALOG2;
	}
	if("ANA3"==str)
	{
		return FM77AV_GAMEPORTEMU_ANALOG3;
	}
	if("ANA4"==str)
	{
		return FM77AV_GAMEPORTEMU_ANALOG4;
	}
	if("ANA5"==str)
	{
		return FM77AV_GAMEPORTEMU_ANALOG5;
	}
	if("ANA6"==str)
	{
		return FM77AV_GAMEPORTEMU_ANALOG6;
	}
	if("ANA7"==str)
	{
		return FM77AV_GAMEPORTEMU_ANALOG7;
	}
	if("PHYS0CYB"==str || "PHYS0CYBER"==str)
	{
		return FM77AV_GAMEPORTEMU_PHYSICAL0_AS_CYBERSTICK;
	}
	if("PHYS1CYB"==str || "PHYS1CYBER"==str)
	{
		return FM77AV_GAMEPORTEMU_PHYSICAL1_AS_CYBERSTICK;
	}
	if("PHYS2CYB"==str || "PHYS2CYBER"==str)
	{
		return FM77AV_GAMEPORTEMU_PHYSICAL2_AS_CYBERSTICK;
	}
	if("PHYS3CYB"==str || "PHYS3CYBER"==str)
	{
		return FM77AV_GAMEPORTEMU_PHYSICAL3_AS_CYBERSTICK;
	}
	if("PHYS4CYB"==str || "PHYS4CYBER"==str)
	{
		return FM77AV_GAMEPORTEMU_PHYSICAL4_AS_CYBERSTICK;
	}
	if("PHYS5CYB"==str || "PHYS5CYBER"==str)
	{
		return FM77AV_GAMEPORTEMU_PHYSICAL5_AS_CYBERSTICK;
	}
	if("PHYS6CYB"==str || "PHYS6CYBER"==str)
	{
		return FM77AV_GAMEPORTEMU_PHYSICAL6_AS_CYBERSTICK;
	}
	if("PHYS7CYB"==str || "PHYS7CYBER"==str)
	{
		return FM77AV_GAMEPORTEMU_PHYSICAL7_AS_CYBERSTICK;
	}

	if("PHYS0CPSF"==str || "PHYS0CAPCOM"==str)
	{
		return FM77AV_GAMEPORTEMU_CAPCOM_BY_PHYSICAL0;
	}
	if("PHYS1CPSF"==str || "PHYS1CAPCOM"==str)
	{
		return FM77AV_GAMEPORTEMU_CAPCOM_BY_PHYSICAL1;
	}
	if("PHYS2CPSF"==str || "PHYS2CAPCOM"==str)
	{
		return FM77AV_GAMEPORTEMU_CAPCOM_BY_PHYSICAL2;
	}
	if("PHYS3CPSF"==str || "PHYS3CAPCOM"==str)
	{
		return FM77AV_GAMEPORTEMU_CAPCOM_BY_PHYSICAL3;
	}
	if("PHYS4CPSF"==str || "PHYS4CAPCOM"==str)
	{
		return FM77AV_GAMEPORTEMU_CAPCOM_BY_PHYSICAL4;
	}
	if("PHYS5CPSF"==str || "PHYS5CAPCOM"==str)
	{
		return FM77AV_GAMEPORTEMU_CAPCOM_BY_PHYSICAL5;
	}
	if("PHYS6CPSF"==str || "PHYS6CAPCOM"==str)
	{
		return FM77AV_GAMEPORTEMU_CAPCOM_BY_PHYSICAL6;
	}
	if("PHYS7CPSF"==str || "PHYS7CAPCOM"==str)
	{
		return FM77AV_GAMEPORTEMU_CAPCOM_BY_PHYSICAL7;
	}

	if("KEYMOUSE"==str || "keymouse"==str)
	{
		return FM77AV_GAMEPORTEMU_MOUSE_BY_KEY;
	}
	if("NUMPADMOUSE"==str || "numpadmouse"==str)
	{
		return FM77AV_GAMEPORTEMU_MOUSE_BY_NUMPAD;
	}


	if("PHYS0MOUSE"==str || "phys0mouse"==str)
	{
		return FM77AV_GAMEPORTEMU_MOUSE_BY_PHYSICAL0;
	}
	if("PHYS1MOUSE"==str || "phys1mouse"==str)
	{
		return FM77AV_GAMEPORTEMU_MOUSE_BY_PHYSICAL1;
	}
	if("PHYS2MOUSE"==str || "phys2mouse"==str)
	{
		return FM77AV_GAMEPORTEMU_MOUSE_BY_PHYSICAL2;
	}
	if("PHYS3MOUSE"==str || "phys3mouse"==str)
	{
		return FM77AV_GAMEPORTEMU_MOUSE_BY_PHYSICAL3;
	}
	if("PHYS4MOUSE"==str || "phys4mouse"==str)
	{
		return FM77AV_GAMEPORTEMU_MOUSE_BY_PHYSICAL4;
	}
	if("PHYS5MOUSE"==str || "phys5mouse"==str)
	{
		return FM77AV_GAMEPORTEMU_MOUSE_BY_PHYSICAL5;
	}
	if("PHYS6MOUSE"==str || "phys6mouse"==str)
	{
		return FM77AV_GAMEPORTEMU_MOUSE_BY_PHYSICAL6;
	}
	if("PHYS7MOUSE"==str || "phys7mouse"==str)
	{
		return FM77AV_GAMEPORTEMU_MOUSE_BY_PHYSICAL7;
	}

	if("ANA0MOUSE"==str || "ana0mouse"==str)
	{
		return FM77AV_GAMEPORTEMU_MOUSE_BY_ANALOG0;
	}
	if("ANA1MOUSE"==str || "ana1mouse"==str)
	{
		return FM77AV_GAMEPORTEMU_MOUSE_BY_ANALOG1;
	}
	if("ANA2MOUSE"==str || "ana2mouse"==str)
	{
		return FM77AV_GAMEPORTEMU_MOUSE_BY_ANALOG2;
	}
	if("ANA3MOUSE"==str || "ana3mouse"==str)
	{
		return FM77AV_GAMEPORTEMU_MOUSE_BY_ANALOG3;
	}
	if("ANA4MOUSE"==str || "ana4mouse"==str)
	{
		return FM77AV_GAMEPORTEMU_MOUSE_BY_ANALOG4;
	}
	if("ANA5MOUSE"==str || "ana5mouse"==str)
	{
		return FM77AV_GAMEPORTEMU_MOUSE_BY_ANALOG5;
	}
	if("ANA6MOUSE"==str || "ana6mouse"==str)
	{
		return FM77AV_GAMEPORTEMU_MOUSE_BY_ANALOG6;
	}
	if("ANA7MOUSE"==str || "ana7mouse"==str)
	{
		return FM77AV_GAMEPORTEMU_MOUSE_BY_ANALOG7;
	}

	if("CYBER"==str || "cyber"==str || "CYBERSTICK"==str || "cyberstick"==str)
	{
		return FM77AV_GAMEPORTEMU_CYBERSTICK;
	}

	return FM77AV_GAMEPORTEMU_NONE;
}

std::string GamePortEmuToStr(unsigned int emu)
{
	switch(emu)
	{
	case FM77AV_GAMEPORTEMU_NONE:
		return "NONE";
	case FM77AV_GAMEPORTEMU_MOUSE:
		return "MOUSE";
	case FM77AV_GAMEPORTEMU_KEYBOARD:
		return "KEY";
	case FM77AV_GAMEPORTEMU_PHYSICAL0:
		return "PHYS0";
	case FM77AV_GAMEPORTEMU_PHYSICAL1:
		return "PHYS1";
	case FM77AV_GAMEPORTEMU_PHYSICAL2:
		return "PHYS2";
	case FM77AV_GAMEPORTEMU_PHYSICAL3:
		return "PHYS3";
	case FM77AV_GAMEPORTEMU_PHYSICAL4:
		return "PHYS4";
	case FM77AV_GAMEPORTEMU_PHYSICAL5:
		return "PHYS5";
	case FM77AV_GAMEPORTEMU_PHYSICAL6:
		return "PHYS6";
	case FM77AV_GAMEPORTEMU_PHYSICAL7:
		return "PHYS7";
	case FM77AV_GAMEPORTEMU_ANALOG0:
		return "ANA0";
	case FM77AV_GAMEPORTEMU_ANALOG1:
		return "ANA1";
	case FM77AV_GAMEPORTEMU_ANALOG2:
		return "ANA2";
	case FM77AV_GAMEPORTEMU_ANALOG3:
		return "ANA3";
	case FM77AV_GAMEPORTEMU_ANALOG4:
		return "ANA4";
	case FM77AV_GAMEPORTEMU_ANALOG5:
		return "ANA5";
	case FM77AV_GAMEPORTEMU_ANALOG6:
		return "ANA6";
	case FM77AV_GAMEPORTEMU_ANALOG7:
		return "ANA7";
	case FM77AV_GAMEPORTEMU_PHYSICAL0_AS_CYBERSTICK:
		return "PHYS0CYB";
	case FM77AV_GAMEPORTEMU_PHYSICAL1_AS_CYBERSTICK:
		return "PHYS1CYB";
	case FM77AV_GAMEPORTEMU_PHYSICAL2_AS_CYBERSTICK:
		return "PHYS2CYB";
	case FM77AV_GAMEPORTEMU_PHYSICAL3_AS_CYBERSTICK:
		return "PHYS3CYB";
	case FM77AV_GAMEPORTEMU_PHYSICAL4_AS_CYBERSTICK:
		return "PHYS4CYB";
	case FM77AV_GAMEPORTEMU_PHYSICAL5_AS_CYBERSTICK:
		return "PHYS5CYB";
	case FM77AV_GAMEPORTEMU_PHYSICAL6_AS_CYBERSTICK:
		return "PHYS6CYB";
	case FM77AV_GAMEPORTEMU_PHYSICAL7_AS_CYBERSTICK:
		return "PHYS7CYB";

	case FM77AV_GAMEPORTEMU_MOUSE_BY_KEY:
		return "KEYMOUSE";
	case FM77AV_GAMEPORTEMU_MOUSE_BY_NUMPAD:
		return "NUMPADMOUSE";
	case FM77AV_GAMEPORTEMU_MOUSE_BY_PHYSICAL0:
		return "PHYS0MOUSE";
	case FM77AV_GAMEPORTEMU_MOUSE_BY_PHYSICAL1:
		return "PHYS1MOUSE";
	case FM77AV_GAMEPORTEMU_MOUSE_BY_PHYSICAL2:
		return "PHYS2MOUSE";
	case FM77AV_GAMEPORTEMU_MOUSE_BY_PHYSICAL3:
		return "PHYS3MOUSE";
	case FM77AV_GAMEPORTEMU_MOUSE_BY_PHYSICAL4:
		return "PHYS4MOUSE";
	case FM77AV_GAMEPORTEMU_MOUSE_BY_PHYSICAL5:
		return "PHYS5MOUSE";
	case FM77AV_GAMEPORTEMU_MOUSE_BY_PHYSICAL6:
		return "PHYS6MOUSE";
	case FM77AV_GAMEPORTEMU_MOUSE_BY_PHYSICAL7:
		return "PHYS7MOUSE";

	case FM77AV_GAMEPORTEMU_MOUSE_BY_ANALOG0:
		return "ANA0MOUSE";
	case FM77AV_GAMEPORTEMU_MOUSE_BY_ANALOG1:
		return "ANA1MOUSE";
	case FM77AV_GAMEPORTEMU_MOUSE_BY_ANALOG2:
		return "ANA2MOUSE";
	case FM77AV_GAMEPORTEMU_MOUSE_BY_ANALOG3:
		return "ANA3MOUSE";
	case FM77AV_GAMEPORTEMU_MOUSE_BY_ANALOG4:
		return "ANA4MOUSE";
	case FM77AV_GAMEPORTEMU_MOUSE_BY_ANALOG5:
		return "ANA5MOUSE";
	case FM77AV_GAMEPORTEMU_MOUSE_BY_ANALOG6:
		return "ANA6MOUSE";
	case FM77AV_GAMEPORTEMU_MOUSE_BY_ANALOG7:
		return "ANA7MOUSE";

	case FM77AV_GAMEPORTEMU_CYBERSTICK:
		return "CYBERSTICK";

	case FM77AV_GAMEPORTEMU_CAPCOM_BY_PHYSICAL0:
		return "PHYS0CPSF";
	case FM77AV_GAMEPORTEMU_CAPCOM_BY_PHYSICAL1:
		return "PHYS1CPSF";
	case FM77AV_GAMEPORTEMU_CAPCOM_BY_PHYSICAL2:
		return "PHYS2CPSF";
	case FM77AV_GAMEPORTEMU_CAPCOM_BY_PHYSICAL3:
		return "PHYS3CPSF";
	case FM77AV_GAMEPORTEMU_CAPCOM_BY_PHYSICAL4:
		return "PHYS4CPSF";
	case FM77AV_GAMEPORTEMU_CAPCOM_BY_PHYSICAL5:
		return "PHYS5CPSF";
	case FM77AV_GAMEPORTEMU_CAPCOM_BY_PHYSICAL6:
		return "PHYS6CPSF";
	case FM77AV_GAMEPORTEMU_CAPCOM_BY_PHYSICAL7:
		return "PHYS7CPSF";
	}
	return "NONE";
}

std::string KeyboardModeToStr(unsigned int keyboardMode)
{
	switch(keyboardMode)
	{
	case FM77AV_KEYBOARD_MODE_DIRECT:
		return "DIRECT";
	case FM77AV_KEYBOARD_MODE_TRANSLATION1:
		return "TRANS1";
	case FM77AV_KEYBOARD_MODE_TRANSLATION2:
		return "TRANS2";
	case FM77AV_KEYBOARD_MODE_TRANSLATION3:
		return "TRANS3";
	}
	return "DEFAULT";
}
unsigned int StrToKeyboardMode(std::string str)
{
	Capitalize(str);
	if(str=="DIRECT")
	{
		return FM77AV_KEYBOARD_MODE_DIRECT;
	}
	else if(str=="TRANS1")
	{
		return FM77AV_KEYBOARD_MODE_TRANSLATION1;
	}
	else if(str=="TRANS2")
	{
		return FM77AV_KEYBOARD_MODE_TRANSLATION2;
	}
	else if(str=="TRANS3")
	{
		return FM77AV_KEYBOARD_MODE_TRANSLATION3;
	}
	return FM77AV_KEYBOARD_MODE_DEFAULT;
}
