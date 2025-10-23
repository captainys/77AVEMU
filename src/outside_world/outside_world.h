/* LICENSE>>
Copyright 2020 Soji Yamakawa (CaptainYS, http://www.ysflight.com)

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

<< LICENSE */
#ifndef OUTSIDE_WORLD_IS_INCLUDED
#define OUTSIDE_WORLD_IS_INCLUDED
/* { */

#include <vector>
#include <string>
#include <queue>
#include <thread>
#include <mutex>

#include "fm77avdef.h"
#include "fm77avparam.h"
#include "fm77avrender.h"
#include "fm77avcrtc.h"

class Outside_World
{
public:
	enum
	{
		LOWER_RIGHT_NONE,
		LOWER_RIGHT_PAUSE,
		LOWER_RIGHT_MENU,
	};
	enum
	{
		STATUS_WID=640,
		STATUS_HEI=16
	};

	class VirtualKey
	{
	public:
		unsigned int fm77avKey=0;
		int physicalId=-1;
		unsigned int button=0;
	};

	unsigned int gamePort[FM77AV_NUM_GAMEPORTS];

	// Pause mouse integration until mouse cursor is moved.
	enum
	{
		MOUSE_STATIONARY_COUNT=4
	};
	bool mouseIntegrationActive=false;
	int lastMx,lastMy,mouseStationaryCount=MOUSE_STATIONARY_COUNT;

	bool visualizeAudioOut=false; // Implementation should consider showing output level from audio channels if true.

	/*! Virtual Keys.
	*/
	std::vector <VirtualKey> virtualKeys;

	/*!
	*/
	std::queue <std::string> commandQueue;

	/*! Cache of game-pad indices that needs to be updated in polling.
	*/
	std::vector <unsigned int> gamePadsNeedUpdate;
	bool gameDevsNeedUpdateCached=false;


	class StatusBarInfo
	{
	public:
		bool fdAccessLamp[4]={false,false,false,false};
		bool insLED=false,capsLED=false,kanaLED=false;
		void Update(const class FM77AV &fm77av);
	};
	StatusBarInfo currentStatusBarInfo;


	inline float ApplyZeroZone(float rawInput,float zeroZone)
	{
		if(rawInput<-zeroZone)
		{
			return rawInput+zeroZone;
		}
		else if(zeroZone<=rawInput)
		{
			return rawInput-zeroZone;
		}
		else
		{
			return 0.0;
		}
	}


	unsigned int keyboardMode=FM77AV_KEYBOARD_MODE_DIRECT;

	enum
	{
		KEYBOARD_LAYOUT_US,
		KEYBOARD_LAYOUT_JP,
	};

	bool pauseKey=false;
	unsigned int indicatedTapePosition=0;

	unsigned int dx=0,dy=0;  // Screen (0,0) will be window (dx,dy)
	unsigned int scaling=100; // In Percent

	unsigned int lowerRightIcon=LOWER_RIGHT_NONE;

	Outside_World();
	virtual ~Outside_World();

	// Directories
	/*! This function must return the directory where the executable is saved in Windows and Linux,
	    /Contents/Resources sub-diretory of the application bundle in macOS.
	    The returned string is in the system-encoding, which doesn't matter in macOS and Linux,
	    but may matter in Windows until shift-JIS is eradicated.
	*/
	virtual std::string GetProgramResourceDirectory(void) const=0;

	virtual void Start(void)=0;
	virtual void Stop(void)=0;
	virtual void DevicePolling(class FM77AV &fm77av)=0;

	void SetKeyboardMode(unsigned int mode);
	virtual void SetKeyboardLayout(unsigned int layout)=0;

	void AddVirtualKey(unsigned int fm77avKey,int physicalId,unsigned int button);

	/*! Return pauseKey flag.  The flag is clear after this function.
	*/
	bool PauseKeyPressed(void);

	/*! Implementation should call this function for each inkey for application-specific augmentation to work correctly.
	*/
	void ProcessInkey(class FM77AV &fm77av,int fm77avKey);

	/*! Implementation should call this function for each mouse reading for application-specific augmentation to work correctly.
	*/
	void ProcessMouse(class FM77AV &fm77av,int lb,int mb,int rb,int mx,int my);

	/*! Right now it updates mouse neutral position for Wing Commander 1 if app-specific augumentation is enabled.
	*/
	void ProcessAppSpecific(class FM77AV &fm77av);

	virtual std::vector <std::string> MakeKeyMappingText(void) const;
	virtual void LoadKeyMappingFromText(const std::vector <std::string> &text);


	/*! Cache gamePadsNeedUpdate member.
	    Reading game pad may not be the fastest function to call, and therefore reading same game pad multiple times
	    in one polling should be avoided.
	    This function caches which game pads needs to be updated.
	    If the sub-class overloads this function, call Outside_World::CacheGamePadIndicesThatNeedUpdates, and then
	    add an ID by calling UseGamePad function..
	*/
	virtual void CacheGamePadIndicesThatNeedUpdates(void);

	/*! Call this function to cache game pad index that needs to be updated every polling.
	*/
	void UseGamePad(unsigned int gamePadIndex);


	/*! Host-Key Label is implementation dependent.
	*/
	virtual void RegisterHostShortCut(std::string hostKeyLabel,bool ctrl,bool shift,std::string cmdStr);

	/*! Host-Key Label is implementation dependent.
	*/
	virtual void RegisterPauseResume(std::string hostKeyLabel);



public:
	class WindowInterface
	{
	public:
		std::mutex deviceStateLock;
		std::mutex rendererLock;
		std::mutex newImageLock;

		class SharedVariables
		{
		public:
			// Locked by deviceStateLock
			unsigned int indicatedTapePosition=0; // Written from the VM Thread
			StatusBarInfo currentStatusBarInfo;   // Copied from Outside::World::currentStatusBarInfo in the VM Thread
			bool VMClosedFromVMThread=false;
			std::vector <unsigned int> gamePadsNeedUpdate;  // Copy of Outside_World's gamePadsNeedUpdate.

			// Locked by rendererLock
			unsigned int dx=0,dy=0;  // Screen (0,0) will be window (dx,dy)
			unsigned int scaling=100; // In Percent

			// Locked by newImageLock
			FM77AVRender renderer;
			FM77AVCRTC::Palette paletteCopy;
			bool needRender=false;
			unsigned int lowerRightIcon=LOWER_RIGHT_NONE;
			bool visualizeAudioOut=false; // Implementation should consider showing output level from audio channels if true.
		};
		class VMThreadVariables
		{
		public:
		};
		class WindowThreadVariables
		{
		public:
			bool newImageRendered=false;
			FM77AVRender::ImageCopy mostRecentImage;
			int winWid=640,winHei=480;
			bool VMClosed=false;

			StatusBarInfo statusBarInfo,prevStatusBarInfo;
			std::vector <unsigned int> gamePadsNeedUpdate;  // Copy of Outside_World's gamePadsNeedUpdate.

			unsigned int indicatedTapePosition=0; // Written from the VM Thread
			unsigned int prevTapePosition=0;
		};
		SharedVariables shared;
		VMThreadVariables VMThr;
		WindowThreadVariables winThr;


		bool windowShift=false;

		unsigned char *statusBitmap;
		bool autoScaling=false;
		unsigned int windowModeOnStartUp=FM77AVParam::WINDOW_NORMAL;


		WindowInterface();
		~WindowInterface();

		virtual void Start(void)=0;
		virtual void Stop(void)=0;

		/*! Called in the window thread.
		*/
		virtual void Interval(void)=0;

		/*! Interval implementation must call this function.
		    Called in the window thread.
		*/
		void BaseInterval(void);

		// Called in the VM thread.
		bool SendNewImageInfo(class FM77AV &fm77av);

		void NotifyVMClosed(void);
		bool CheckVMClosed(void) const;
		void ClearVMClosedFlag(void);

		/*! Called in the VM thread.
		*/
		virtual void Communicate(Outside_World *outside_world)=0;

		virtual void Render(bool swapBuffers)=0;

		virtual void Render(const FM77AVRender::Image &img,const class FM77AV &fm77av)=0;

		/*! Implementation should return true if the image needs to be flipped before drawn on the window.
		    The flag is transferred to rendering thread class at the beginning of the FM77AVThread::Start.
		*/
		virtual bool ImageNeedsFlip(void)=0;

		void Put16x16(int x0,int y0,const unsigned char icon16x16[]);
		void Put16x16Invert(int x0,int y0,const unsigned char icon16x16[]);

		void Put16x16Select(int x0,int y0,const unsigned char idleIcon16x16[],const unsigned char busyIcon16x16[],bool busy);
		void Put16x16SelectInvert(int x0,int y0,const unsigned char idleIcon16x16[],const unsigned char busyIcon16x16[],bool busy);

		void PutWx16Invert(int x0,int y0,int W,const unsigned char icon[]);
		void ClearWx16(int x0,int y0,int W);
	};
	virtual WindowInterface *CreateWindowInterface(void) const=0;
	virtual void DeleteWindowInterface(WindowInterface *) const=0;

	class Sound
	{
	public:
		virtual void Start(void)=0;
		virtual void Stop(void)=0;

		virtual void Polling(void)=0;

		virtual void FMPSGPlay(std::vector <unsigned char > &wave)=0;
		virtual void FMPSGPlayStop(void)=0;
		virtual bool FMPSGChannelPlaying(void)=0;
	};
	virtual Sound *CreateSound(void) const=0;
	virtual void DeleteSound(Sound *) const=0;
};


/* } */
#endif
