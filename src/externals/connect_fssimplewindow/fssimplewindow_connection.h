/* LICENSE>>
Copyright 2020 Soji Yamakawa (CaptainYS, http://www.ysflight.com)

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

<< LICENSE */
#ifndef FSSIMPLEWINDOW_CONNECTION_IS_INCLUDED
#define FSSIMPLEWINDOW_CONNECTION_IS_INCLUDED
/* { */



/*
The original plan was entirely transition to the streaming mode.  However, it turned out DirectSound gives an shameful 60ms latency (practically 80ms), and ALSA apalling 80ms latency.
macOS's AVAudioEngine easily gave a 40ms latency.

DirectSound does better job if I poll play-back state every 1ms and start new 20ms segment, which yields maximum 40ms latency.  The draw back is when you move window or click on 
the window frame, you may hear zapping noise, but I believe latency is more important for games.

ALSA has no choice.  Behind the scenes ALSA code does streaming.

So, my best decision is to use streaming mode in macOS, which should work perfectly.  I have no choice with ALSA.  But, for Windows, I keep it polling-based playback.
*/
#ifndef _WIN32
#define AUDIO_USE_STREAMING
#else
// Opposite is use polling
#endif



#include "outside_world.h"
#include "yssimplesound.h"
#include "ysgamepad.h"
#include "fssimplewindow.h"
#include <vector>
#include <stdint.h>

class FsSimpleWindowConnection : public Outside_World
{
public:
	class HostShortCut
	{
	public:
		bool inUse=false;
		bool ctrl=false,shift=false,graph=false;
		std::string cmdStr;
	};

	#define DEFAULT_PAUSE_KEY_CODE FSKEY_SCROLLLOCK

	enum
	{
		STATUS_TAPEPOS_X=320,
	};

	GLuint mainTexId,statusTexId,pauseIconTexId,menuIconTexId;

	HostShortCut hostShortCut[FSKEY_NUM_KEYCODE];
	unsigned int PAUSE_KEY_CODE=DEFAULT_PAUSE_KEY_CODE;
	unsigned int GRAPH_KEY_CODE=FSKEY_NULL;

	bool gamePadInitialized=false;
	unsigned int *FSKEYtoFM77AVKEY=nullptr;
	unsigned int *FSKEYState=nullptr;
	FsSimpleWindowConnection();
	~FsSimpleWindowConnection();

	GLuint GenTexture(void);
	void UpdateTexture(GLuint texId,int wid,int hei,const unsigned char *rgba) const;
	void DrawTextureRect(int x0,int y0,int x1,int y1) const;

	std::vector <unsigned char> PAUSEicon,MENUicon,FD_IDLEicon,FD_BUSYicon,TAPE_IDLEicon,TAPE_LOADINGicon,TAPE_SAVINGicon,CAPSicon,KANAicon,INSicon;

	std::vector <struct YsGamePadReading> gamePads,prevGamePads;

	// For mouse emulation by pad digital axes.
	int mouseDX=0,mouseDY=0;

	unsigned int lastPressedFsKey=0;
	uint64_t nextKeyRepeatTime=0;

	int winWid=640,winHei=480;
	unsigned int sinceLastResize=0;
	unsigned int prevTapePosition=0;


	virtual std::vector <std::string> MakeKeyMappingText(void) const;
	virtual void LoadKeyMappingFromText(const std::vector <std::string> &text);

	virtual void Start(void);
	virtual void Stop(void);
	virtual void DevicePolling(class FM77AV &fm77av);
	void PollGamePads(void);
	virtual void UpdateStatusBitmap(class FM77AV &fm77av);
	virtual void Render(const FM77AVRender::Image &img,const class FM77AV &fm77av);
	void RenderBeforeSwapBuffers(const FM77AVRender::Image &img,const class FM77AV &fm77av);
	virtual bool ImageNeedsFlip(void);

	virtual void SetKeyboardLayout(unsigned int layout);

	virtual void RegisterHostShortCut(std::string hostKeyLabel,bool ctrl,bool shift,std::string cmdStr);
	virtual void RegisterPauseResume(std::string hostKeyLabel);



	YsSoundPlayer soundPlayer;

#ifdef AUDIO_USE_STREAMING
	YsSoundPlayer::Stream FMPSGStream;
#else
	YsSoundPlayer::SoundData FMPSGChannel;
#endif
	virtual void FMPSGPlay(std::vector <unsigned char > &wave);
	virtual void FMPSGPlayStop(void);
	virtual bool FMPSGChannelPlaying(void);
};

/* } */
#endif
