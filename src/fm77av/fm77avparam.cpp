#include <string>
#include <sstream>
#include "fm77avparam.h"
#include "cpputil.h"



void FM77AVParam::CleanUp(void)
{
	ROMPath="";

	for(auto &f : fdImgFName)
	{
		f="";
	}
	for(auto &wp : fdImgWriteProtect)
	{
		wp=false;
	}

	t77Path="";
	t77SavePath="";

	gamePort[0]=FM77AV_GAMEPORTEMU_PHYSICAL0;
	gamePort[1]=FM77AV_GAMEPORTEMU_PHYSICAL1;

	maxButtonHoldTime[0][0]=0;
	maxButtonHoldTime[0][1]=0;
	maxButtonHoldTime[1][0]=0;
	maxButtonHoldTime[1][1]=0;

	scaling=100;
	appSpecificSetting=FM77AV_APPSPECIFIC_NONE;

	// memSizeInKB=192; Will be supported

	machineType=MACHINETYPE_AUTO;

	pauseOnStart=false;
	keyboardMode=FM77AV_KEYBOARD_MODE_DIRECT;

	virtualKeys.clear();

	bool unitTest=false; // If true, CUI will not be attached.  Implies termination condition.

	quickScrnShotDir="";
	hostShortCutKeys.clear();

	//quickStateSaveFName="";

	pauseResumeKeyLabel="SCROLLLOCK";
}

////////////////////////////////////////////////////////////

FM77AVProfile::FM77AVProfile()
{
	CleanUp();
}
void FM77AVProfile::CleanUp(void)
{
	FM77AVParam::CleanUp();
	autoStart=false;

	virtualKeys.resize(MAX_NUM_VIRTUALKEYS);
	for(auto &vk : virtualKeys)
	{
		vk.fm77avKey="";
		vk.physicalId=-1;
	}
}
std::vector <std::string> FM77AVProfile::Serialize(void) const
{
	std::ostringstream sstream;
	std::vector <std::string> text;

	text.push_back("ROMDIR__ ");
	text.back().push_back('\"');
	text.back()+=ROMPath;
	text.back().push_back('\"');

	text.push_back("TAPEIMG_ ");
	text.back().push_back('\"');
	text.back()+=t77Path;
	text.back().push_back('\"');

	text.push_back("TAPESAVE ");
	text.back().push_back('\"');
	text.back()+=t77SavePath;
	text.back().push_back('\"');

	for(int i=0; i<NUM_FDDRIVES; ++i)
	{
		text.push_back("FDIMG___ ");
		text.back().push_back('0'+i);
		text.back().push_back(' ');
		text.back().push_back('0');
		text.back().push_back(' ');
		text.back().push_back('\"');
		text.back()+=fdImgFName[i];
		text.back().push_back('\"');

		text.push_back("FDWPROT_ ");
		text.back().push_back('0'+i);
		text.back().push_back(' ');
		text.back().push_back('0');
		text.back().push_back(' ');
		text.back().push_back(fdImgWriteProtect[i] ? '1' : '0');
	}

	text.push_back("GAMEPORT 0 ");
	text.back()+=GamePortEmuToStr(gamePort[0]);
	text.push_back("GAMEPORT 1 ");
	text.back()+=GamePortEmuToStr(gamePort[1]);

	for(int i=0; i<2; ++i)
	{
		for(int j=0; j<2; ++j)
		{
			text.push_back("BTNHOLDT 0 0 ");
			text.back()[9]+=i;
			text.back()[11]+=j;

			sstream.str("");
			sstream << maxButtonHoldTime[i][j];
			text.back()+=sstream.str();
		}
	}

	text.push_back("AUTOSTAR ");
	text.back()+=(autoStart ? "1" : "0");

	sstream.str("");
	sstream << "SCALING_ " << scaling;
	text.push_back(sstream.str());

	sstream.str("");
	sstream << "AUTOSCAL " << (true==autoScaling ? 1 : 0);
	text.push_back(sstream.str());

	switch(windowModeOnStartUp)
	{
	case WINDOW_NORMAL:
		text.push_back("WNDWMODE NORMAL");
		break;
	case WINDOW_MAXIMIZE:
		text.push_back("WNDWMODE MAXIMIZE");
		break;
	case WINDOW_FULLSCREEN:
		text.push_back("WNDWMODE FULLSCREEN");
		break;
	}

	sstream.str("");
	sstream << "APPSPEC_ " << FM77AVAppToStr(appSpecificSetting);
	text.push_back(sstream.str());

	if(FM77AV_KEYBOARD_MODE_DEFAULT!=keyboardMode)
	{
		text.push_back("KYBDMODE ");
		text.back()+=KeyboardModeToStr(keyboardMode);
	}

	for(auto vk : virtualKeys)
	{
		if(""!=vk.fm77avKey && 0<=vk.physicalId)
		{
			sstream.str("");
			sstream << "VIRTUKEY " << vk.fm77avKey << " " << vk.physicalId << " " << vk.button;
			text.push_back(sstream.str());
		}
	}

	// if(""!=startUpStateFName)
	// {
	// 	sstream.str("");
	// 	sstream << "LOADSTAT " << startUpStateFName;
	// 	text.push_back(sstream.str());
	// }

	text.push_back("QSSDIREC ");
	text.back().push_back('\"');
	text.back()+=quickScrnShotDir;
	text.back().push_back('\"');

	for(auto hsc : hostShortCutKeys)
	{
		text.push_back("HOSTSCUT ");
		text.back()+=hsc.hostKey;
		text.back()+=" ";
		text.back()+=(hsc.ctrl ? "1" : "0");
		text.back()+=" ";
		text.back()+=(hsc.shift ? "1" : "0");
		text.back()+=" ";
		text.back().push_back('\"');
		text.back()+=hsc.cmdStr;
		text.back().push_back('\"');
	}

	//text.push_back("QSTASAVE ");
	//text.back().push_back('\"');
	//text.back()+=quickStateSaveFName;
	//text.back().push_back('\"');

	text.push_back("PAUSEKEY ");
	text.back()+=pauseResumeKeyLabel;

	if(MACHINETYPE_UNKNOWN!=machineType)
	{
		text.push_back("FM77AVTYP ");
		text.back()+=MachineTypeToStr(machineType);
	}

	return text;
}
bool FM77AVProfile::Deserialize(const std::vector <std::string> &text)
{
	bool useThrottleAxis=false;

	CleanUp();
	unsigned int nVirtualKey=0;
	for(auto &cppstr : text)
	{
		auto argv=cpputil::Parser(cppstr.data());

		if(0==argv.size())
		{
			continue;
		}

		auto ARGV0=argv[0];
		cpputil::Capitalize(ARGV0);
		if(ARGV0=="ROMDIR__")
		{
			if(2<=argv.size())
			{
				ROMPath=argv[1];
			}
		}
		else if(ARGV0=="TAPEIMG_")
		{
			if(2<=argv.size())
			{
				t77Path=argv[1];
			}
		}
		else if(ARGV0=="TAPESAVE")
		{
			if(2<=argv.size())
			{
				t77SavePath=argv[1];
			}
		}
		else if(ARGV0=="FDIMG___")
		{
			if(4<=argv.size())
			{
				int drive=cpputil::Atoi(argv[1].c_str());
				int fileNum=cpputil::Atoi(argv[2].c_str());
				if(0<=drive && drive<NUM_FDDRIVES && 0<=fileNum && 0==fileNum)
				{
					fdImgFName[drive]=argv[3].c_str();
				}
			}
		}
		else if(ARGV0=="FDWPROT_")
		{
			if(4<=argv.size())
			{
				int drive=cpputil::Atoi(argv[1].c_str());
				int fileNum=cpputil::Atoi(argv[2].c_str());
				if(0<=drive && drive<2 && 0<=fileNum && 0==fileNum)
				{
					fdImgWriteProtect[drive]=(0!=cpputil::Atoi(argv[3].c_str()));
				}
			}
		}
		else if(ARGV0=="AUTOSTAR")
		{
			if(2<=argv.size())
			{
				autoStart=(0!=cpputil::Atoi(argv[1].c_str()));
			}
		}
		else if(ARGV0=="GAMEPORT")
		{
			if(3<=argv.size())
			{
				int port=cpputil::Atoi(argv[1].c_str());
				if(0==port || 1==port)
				{
					gamePort[port]=StrToGamePortEmu(argv[2].c_str());
				}
			}
		}
		else if(ARGV0=="BTNHOLDT")
		{
			if(4<=argv.size())
			{
				int port=cpputil::Atoi(argv[1].c_str())&1;
				int btn=cpputil::Atoi(argv[2].c_str())&1;
				int nanosec=cpputil::Atoi(argv[3].c_str());
				maxButtonHoldTime[port][btn]=nanosec;
			}
		}
		else if(ARGV0=="SCALING_")
		{
			if(2<=argv.size())
			{
				scaling=cpputil::Atoi(argv[1].c_str());
			}
		}
		else if(ARGV0=="AUTOSCAL")
		{
			if(2<=argv.size())
			{
				autoScaling=(0!=cpputil::Atoi(argv[1].c_str()));
			}
		}
		else if(ARGV0=="WNDWMODE")
		{
			if(2<=argv.size())
			{
				if(argv[1]=="NORMAL")
				{
					windowModeOnStartUp=WINDOW_NORMAL;
				}
				else if(argv[1]=="MAXIMIZE")
				{
					windowModeOnStartUp=WINDOW_MAXIMIZE;
				}
				else if(argv[1]=="FULLSCREEN")
				{
					windowModeOnStartUp=WINDOW_FULLSCREEN;
				}
			}
		}
		else if(ARGV0=="APPSPEC_")
		{
			if(2<=argv.size())
			{
				appSpecificSetting=FM77AVStrToApp(argv[1].c_str());
			}
		}
		else if(ARGV0=="VIRTUKEY")
		{
			if(3<=argv.size() && nVirtualKey<MAX_NUM_VIRTUALKEYS)
			{
				virtualKeys[nVirtualKey].fm77avKey=argv[1];
				virtualKeys[nVirtualKey].physicalId=cpputil::Atoi(argv[2].c_str());
				virtualKeys[nVirtualKey].button=cpputil::Atoi(argv[3].c_str());
				++nVirtualKey;
			}
		}
		else if(ARGV0=="KYBDMODE")
		{
			if(2<=argv.size())
			{
				keyboardMode=StrToKeyboardMode(argv[1].c_str());
			}
		}
		else if(ARGV0=="LOADSTAT")
		{
			if(2<=argv.size())
			{
				startUpStateFName=argv[1];
			}
		}
		else if(ARGV0=="QSSDIREC")
		{
			if(2<=argv.size())
			{
				quickScrnShotDir=argv[1].c_str();
			}
		}
		else if(ARGV0=="HOSTSCUT")
		{
			if(5<=argv.size())
			{
				HostShortCut hsc;
				hsc.hostKey=argv[1].c_str();
				hsc.ctrl=(0!=cpputil::Atoi(argv[2].c_str()));
				hsc.shift=(0!=cpputil::Atoi(argv[3].c_str()));
				hsc.cmdStr=argv[4].c_str();
				hostShortCutKeys.push_back(hsc);
			}
		}
		//else if(ARGV0=="QSTASAVE")
		//{
		//	if(2<=argv.size())
		//	{
		//		quickStateSaveFName=argv[1].c_str();
		//	}
		//}
		else if(ARGV0=="PAUSEKEY")
		{
			if(2<=argv.size())
			{
				pauseResumeKeyLabel=argv[1].c_str();
			}
		}
		else if(ARGV0=="FM77AVTYP")
		{
			if(2<=argv.size())
			{
				machineType=StrToMachineType(argv[1].c_str());
			}
		}
		else
		{
			errorMsg="Unrecognized keyword:";
			errorMsg+=argv[0];
			return false;
		}
	}

	errorMsg="";
	return true;
}
