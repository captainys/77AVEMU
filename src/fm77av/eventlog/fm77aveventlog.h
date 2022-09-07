#ifndef FM77AVEVENTLOG_IS_INCLUDED
#define FM77AVEVENTLOG_IS_INCLUDED
/* { */

#include <string>
#include <vector>

class FM77AVEventLog
{
public:
	enum
	{
		STATE_IDLE,
		STATE_RECORDING,
		STATE_PLAYBACK,
	};

	enum
	{
		EVT_NULL,
		EVT_KEYPRESS,
		EVT_KEYRELEASE,
		EVT_PAD0_A_DOWN,
		EVT_PAD0_A_UP,
		EVT_PAD0_B_DOWN,
		EVT_PAD0_B_UP,
		EVT_PAD0_FWD_DOWN,
		EVT_PAD0_FWD_UP,
		EVT_PAD0_BACK_DOWN,
		EVT_PAD0_BACK_UP,
		EVT_PAD0_LEFT_DOWN,
		EVT_PAD0_LEFT_UP,
		EVT_PAD0_RIGHT_DOWN,
		EVT_PAD0_RIGHT_UP,
		EVT_GOTO,
		EVEYTTYPE_TYPE_COMMAND,
	};
	static std::string EventTypeToStr(unsigned int eventType);
	static unsigned int StrToEventType(std::string str);

	class Event
	{
	public:
		unsigned int type=EVT_NULL;
		std::string label; // For GOTO
		uint64_t time=0;

		unsigned int code=0;
		// AVKEY_? for KEYPRESS and KEYRELEASE
		// |0|0||B|A|Right|Left|Down|Up| for PADPRESS and PADRELEASE

		std::string str;
		// Label for GOTO
	};

	unsigned int state=STATE_IDLE;
	std::vector <Event> eventLog;
	uint64_t timeOrigin=0;
	unsigned int playbackPointer=0;


	void CleanUp(void);

	bool LoadEventLog(std::string fName);

	static uint64_t GetTime(std::string timeStr,uint64_t prevTime);

	void BeginPlayback(uint64_t fm77avTime);

	void RunOneStep(class FM77AV &fm77av);
	void Playback(class FM77AV &fm77av);

	void StopPlayback(void);
};

/* } */
#endif
