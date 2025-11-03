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

	{
		std::unordered_map <std::string,std::string> empty;
		fileNameAlias.swap(empty);
	}

	virtualKeys.clear();

	bool unitTest=false; // If true, CUI will not be attached.  Implies termination condition.

	quickScrnShotDir="";
	scrnShotX0=0;
	scrnShotY0=0;
	scrnShotWid=0;
	scrnShotHei=0;

	mapXYExpression[0]="";
	mapXYExpression[1]="";

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
	autoStartOnLoad=false;

	// Expect GUI environment.
	autoScaling=true;
	windowModeOnStartUp=WINDOW_MAXIMIZE;

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
	text.back()+=(autoStartOnLoad ? "1" : "0");

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

	text.push_back("AUTOTAPE ");
	text.back()+=(autoLoadTapeFile ? "1" : "0");

	if(MACHINETYPE_UNKNOWN!=machineType)
	{
		text.push_back("FM77AVTYP ");
		text.back()+=MachineTypeToStr(machineType);
	}

	sstream.str("");
	sstream << "SCRNCROP " << scrnShotX0 << " " << scrnShotY0 << " " << scrnShotWid << " " << scrnShotHei;
	text.push_back(sstream.str());

	text.push_back("MAPLOC_X ");
	text.back().push_back('\"');
	text.back()+=mapXYExpression[0];
	text.back().push_back('\"');

	text.push_back("MAPLOC_Y ");
	text.back().push_back('\"');
	text.back()+=mapXYExpression[1];
	text.back().push_back('\"');

	for(auto iter=fileNameAlias.begin(); fileNameAlias.end()!=iter; ++iter)
	{
		if(""!=iter->first)
		{
			text.push_back("IMGALIAS ");
			text.back()+=iter->first;
			text.back()+=" \"";
			text.back()+=iter->second;
			text.back()+="\"";
		}
	}

	text.push_back("ENBL_WHG ");
	text.back()+=(enable_whg ? "1" : "0");

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
				autoStartOnLoad=(0!=cpputil::Atoi(argv[1].c_str()));
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
		else if(ARGV0=="AUTOTAPE")
		{
			if(2<=argv.size())
			{
				autoLoadTapeFile=(0!=cpputil::Atoi(argv[1].c_str()));
			}
		}
		else if(ARGV0=="IMGALIAS")
		{
			fileNameAlias[argv[1]]=argv[2];
		}
		else if(ARGV0=="SCRNCROP")
		{
			if(5<=argv.size())
			{
				scrnShotX0=cpputil::Atoi(argv[1].c_str());
				scrnShotY0=cpputil::Atoi(argv[2].c_str());
				scrnShotWid=cpputil::Atoi(argv[3].c_str());
				scrnShotHei=cpputil::Atoi(argv[4].c_str());
			}
		}
		else if(ARGV0=="MAPLOC_X")
		{
			if(2<=argv.size())
			{
				mapXYExpression[0]=argv[1];
			}
		}
		else if(ARGV0=="MAPLOC_Y")
		{
			if(2<=argv.size())
			{
				mapXYExpression[1]=argv[1];
			}
		}
		else if(ARGV0=="ENBL_WHG")
		{
			if(2<=argv.size())
			{
				enable_whg=(0!=cpputil::Atoi(argv[1].c_str()));
			}
		}
		else if(ARGV0=="ENBL_THG")
		{
			if(2<=argv.size())
			{
				enable_thg=(0!=cpputil::Atoi(argv[1].c_str()));
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

void FM77AVProfile::MakeRelativePath(std::string baseDir,std::string alias)
{
	MakeRelative(startUpStateFName,baseDir,alias);

	MakeRelative(ROMPath,baseDir,alias);
	MakeRelative(quickStateSaveFName,baseDir,alias);
	MakeRelative(quickScrnShotDir,baseDir,alias);;

	for(auto &p : imgSearchPaths)
	{
		MakeRelative(p,baseDir,alias);
	}
	for(auto p : fileNameAlias)
	{
		MakeRelative(p.second,baseDir,alias);
		fileNameAlias[p.first]=p.second;
	}
	MakeRelative(t77Path,baseDir,alias);
	MakeRelative(t77SavePath,baseDir,alias);
	for(auto &f : fdImgFName)
	{
		MakeRelative(f,baseDir,alias);
	}
}

void FM77AVProfile::MakeRelative(std::string &fName,std::string baseDir,std::string alias)
{
	auto src=fName;
	for(auto &c : src)
	{
		if('\\'==c)
		{
			c='/';
		}
	}
	for(auto &c : baseDir)
	{
		if('\\'==c)
		{
			c='/';
		}
	}

	if(0<baseDir.size() && '/'==baseDir.back())
	{
		baseDir.pop_back();
	}

	// Special case baseDir==src
	if(""!=src && ""!=baseDir && (baseDir==src || (baseDir+"/")==src))
	{
		fName=alias;
		return;
	}

	// (1) src is longer or equal to than baseDir.
	// (2) baseDir matches the first part of src, and
	// (3) subsequent char is '/'.

	// (1)
	if(src.size()<=baseDir.size())
	{
		return;
	}

	// (2)
	auto chopOff=src;
	chopOff.resize(baseDir.size());
	if(baseDir!=chopOff)
	{
		return;
	}

	// (3)
	if(src[baseDir.size()]!='/')
	{
		return;
	}

	if(0<=alias.size() && alias.back()=='/')
	{
		alias.pop_back();
	}

	alias.insert(alias.end(),src.begin()+baseDir.size(),src.end());
	std::swap(fName,alias);
}
