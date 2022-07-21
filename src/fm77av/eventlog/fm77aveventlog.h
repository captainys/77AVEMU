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
		EVENTTYPE_NULL,
		EVENTTYPE_KEYPRESS,
		EVENTTYPE_KEYRELEASE,
		EVENTTYPE_GOTO,
		EVEYTTYPE_TYPE_COMMAND,
	};
	static std::string EventTypeToStr(unsigned int eventType);
	static unsigned int StrToEventType(std::string str);

	class Event
	{
	public:
		unsigned int type=EVENTTYPE_NULL;
		std::string label; // For GOTO
		uint64_t time=0;

		unsigned int code=0;
		// AVKEY_? for KEYPRESS and KEYRELEASE

		std::string str;
		// Label for GOTO
	};

	unsigned int state=STATE_IDLE;
	std::vector <Event> eventLog;
	uint64_t timeOrigin=0;
	unsigned int playbackPointer=0;


	void CleanUp(void);

	bool LoadEventLog(std::string fName);

	static uint64_t GetTime(std::string timeStr);

	void BeginPlayback(uint64_t fm77avTime);

	void RunOneStep(class FM77AV &fm77av);
	void Playback(class FM77AV &fm77av);

	void StopPlayback(void);
};

/* } */
#endif
