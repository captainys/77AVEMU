#include <fstream>
#include <iostream>
#include "fm77av.h"
#include "fm77aveventlog.h"
#include "fm77avkey.h"
#include "cpputil.h"

/* static */ std::string FM77AVEventLog::EventTypeToStr(unsigned int eventType)
{
	switch(eventType)
	{
	case EVT_NULL:
		return "NULL";
	case EVT_KEYPRESS:
		return "KEYPRESS";
	case EVT_KEYRELEASE:
		return "KEYRELEASE";
	case EVT_GOTO:
		return "GOTO";
	case EVT_PAD0_A_DOWN:
		return "PAD0ADOWN";
	case EVT_PAD0_A_UP:
		return "PAD0AUP";
	}
	return "NULL";
}
/* static */ unsigned int FM77AVEventLog::StrToEventType(std::string str)
{
	auto STR=str;
	cpputil::Capitalize(STR);
	if("KEYPRESS"==STR)
	{
		return EVT_KEYPRESS;
	}
	else if("KEYRELEASE"==STR)
	{
		return EVT_KEYRELEASE;
	}
	else if("GOTO"==STR)
	{
		return EVT_GOTO;
	}
	else if("PAD0ADOWN"==STR)
	{
		return EVT_PAD0_A_DOWN;
	}
	else if("PAD0AUP"==STR)
	{
		return EVT_PAD0_A_UP;
	}
	return EVT_NULL;
}

void FM77AVEventLog::CleanUp(void)
{
	state=STATE_IDLE;
	eventLog.clear();
	timeOrigin=0;
	playbackPointer=0;
}

bool FM77AVEventLog::LoadEventLog(std::string fName)
{
	std::ifstream ifp(fName);
	if(true==ifp.is_open())
	{
		CleanUp();
		uint64_t prevTime=0;
		while(true!=ifp.eof())
		{
			std::string str;
			std::getline(ifp,str);

			auto STR=str;
			cpputil::Capitalize(STR);

			for(int i=0; i<STR.size(); ++i)
			{
				if('#'==STR[i])
				{
					STR.resize(i);
					break;
				}
			}

			std::cout << STR << std::endl;

			auto ARGV=cpputil::Parser(STR.c_str());
			if(0<ARGV.size())
			{
				if("EVENT"==ARGV[0] && 2<=ARGV.size())
				{
					Event newEvent;
					newEvent.time=GetTime(ARGV[1],prevTime);
					prevTime=newEvent.time;
					eventLog.push_back(newEvent);
				}
				else if("KEYPRESS"==ARGV[0] && 2<=ARGV.size() && 0<eventLog.size())
				{
					eventLog.back().type=EVT_KEYPRESS;
					if(ARGV[1].substr(0,7)=="AVKEY_")
					{
						ARGV[1]=ARGV[1].substr(7);
					}
					eventLog.back().code=FM77AVKeyLabelToKeyCode(ARGV[1]);
				}
				else if("KEYRELEASE"==ARGV[0] && 2<=ARGV.size() && 0<eventLog.size())
				{
					eventLog.back().type=EVT_KEYRELEASE;
					if(ARGV[1].substr(0,7)=="AVKEY_")
					{
						ARGV[1]=ARGV[1].substr(7);
					}
					eventLog.back().code=FM77AVKeyLabelToKeyCode(ARGV[1]);
				}
				else if("PAD0ADOWN"==ARGV[0])
				{
					eventLog.back().type=EVT_PAD0_A_DOWN;
				}
				else if("PAD0AUP"==ARGV[0])
				{
					eventLog.back().type=EVT_PAD0_A_UP;
				}
				else if("TYPECOMMAND"==ARGV[0] && 0<eventLog.size())
				{
					eventLog.back().type=EVEYTTYPE_TYPE_COMMAND;
					bool start=false;
					eventLog.back().str="";
					for(auto c : str)
					{
						if(true==start)
						{
							eventLog.back().str.push_back(c);
						}
						else if(' '==c || '\t'==c)
						{
							start=true;
						}
					}
				}
				else if("GOTO"==ARGV[0] && 2<=ARGV.size() && 0<eventLog.size())
				{
					eventLog.back().type=EVT_GOTO;
					eventLog.back().str=ARGV[1];
				}
				else if("LABEL"==ARGV[0] && 2<=ARGV.size() && 0<eventLog.size())
				{
					eventLog.back().label=ARGV[1];
				}
				else
				{
					CleanUp();
					return false;
				}
			}
		}
		return true;
	}
	return false;
}
void FM77AVEventLog::BeginPlayback(uint64_t fm77avTime)
{
	timeOrigin=fm77avTime;
	state=STATE_PLAYBACK;
}
void FM77AVEventLog::RunOneStep(class FM77AV &fm77av)
{
	if(STATE_PLAYBACK==state)
	{
		Playback(fm77av);
	}
}
void FM77AVEventLog::Playback(FM77AV &fm77av)
{
	unsigned int nStep=0;
	while(playbackPointer<eventLog.size() &&
	      timeOrigin+eventLog[playbackPointer].time<fm77av.state.fm77avTime &&
	      nStep<eventLog.size()) // Prevent from running more than one loop of all events..
	{
		auto &evt=eventLog[playbackPointer];
		switch(evt.type)
		{
		case EVT_KEYPRESS:
			fm77av.keyboard.Press(0,evt.code); // Keyflags=0
			break;
		case EVT_KEYRELEASE:
			fm77av.keyboard.Release(0,evt.code); // Keyflags=0
			break;
		case EVT_PAD0_A_DOWN:
			fm77av.gameport.state.ports[0].button[0]=true;
			break;
		case EVT_PAD0_A_UP:
			fm77av.gameport.state.ports[0].button[0]=false;
			break;
		case EVT_GOTO:
			for(int i=0; i<eventLog.size(); ++i)
			{
				if(eventLog[i].label==evt.str)
				{
					playbackPointer=i;
					timeOrigin=fm77av.state.fm77avTime-eventLog[i].time; // timeOrigin+eventLog[i].time==fm77avTime
					goto NO_INCREMENT;
				}
			}
			break;
		case EVEYTTYPE_TYPE_COMMAND:
			for(auto c : evt.str)
			{
				fm77av.keyboard.Type(c);
			}
			fm77av.keyboard.Type(0x0D);
			break;
		}

		++playbackPointer;

	NO_INCREMENT:
		++nStep;
	}
}
/* static */ uint64_t FM77AVEventLog::GetTime(std::string timeStr,uint64_t prevTime)
{
	const char *cstr=timeStr.c_str();
	uint64_t baseTime=0;
	if('+'==*cstr)
	{
		baseTime=prevTime;
		++cstr;
	}
	uint64_t t=cpputil::Atoi(cstr);
	if(2<timeStr.size() && ("MS"==timeStr.substr(timeStr.size()-2) || "ms"==timeStr.substr(timeStr.size()-2)))
	{
		return baseTime+t*1000000;
	}
	if(2<timeStr.size() && ("US"==timeStr.substr(timeStr.size()-2) || "us"==timeStr.substr(timeStr.size()-2)))
	{
		return baseTime+t*1000;
	}
	if(2<timeStr.size() && ("NS"==timeStr.substr(timeStr.size()-2) || "ns"==timeStr.substr(timeStr.size()-2)))
	{
		return baseTime+t;
	}
	if(3<timeStr.size() && ("SEC"==timeStr.substr(timeStr.size()-3) || "sec"==timeStr.substr(timeStr.size()-3)))
	{
		return baseTime+t*1000000000;
	}
	return 0;

}
void FM77AVEventLog::StopPlayback(void)
{
	state=STATE_IDLE;
}
