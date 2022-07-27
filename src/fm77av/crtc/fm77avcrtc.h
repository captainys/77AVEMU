#ifndef FM77AVCRTC_IS_INCLUDED
#define FM77AVCRTC_IS_INCLUDED
/* { */



#include "device.h"
#include "fm77avdef.h"

class FM77AVCRTC : public Device
{
public:
	virtual const char *DeviceName(void) const{return "CRTC";}

	class Palette
	{
	public:
		uint8_t digitalPalette[8];
		uint16_t analogPaletteLatch=0;
		uint8_t analogPalette[4096][3];
		void Reset(void);
	};

	enum
	{
		HD_CMD_PSET=0,
		HD_CMD_OR=2,
		HD_CMD_AND=3,
		HD_CMD_XOR=4,
		HD_CMD_NOT=5,
		HD_CMD_TILE=6,
		HD_CMD_CMP=7, // Compare? What is it?

		// OK.  I should measure in the actual hardware.  But, for the time being,
		// Let's make a wild guess.  1 pixel per clock.
		HD_LINE_TIME_PER_PIXEL=500,
	};

	class HardwareDrawing
	{
	public:
		// $D410
		bool enabled=false; // bit7
		uint8_t condition=0;
		uint8_t cmd=HD_CMD_PSET;
		// $D411
		uint8_t color=0;
		// $D412
		uint8_t maskBits=0;
		// $D413
		uint8_t compareResult=0;
		// $D413 to $D41A
		uint8_t compareColor[8];
		// $D41B
		uint8_t bankMask=0;
		// $D41C to $D41E
		uint8_t tile[3];
		uint8_t tilePtnCache[8];


		// $D430
		uint64_t lineBusyBy=0;
		// $D420,$D421
		uint16_t addrOffset=0;
		// $D422,$D423
		uint16_t lineStipple=0xFFFF;
		// $D424,$D425
		uint16_t x0=0,y0=0,x1=0,y1=0;

		void Reset(void);
	};

	enum
	{
		// VSYNC_CYCLE is 1/60sec.
		VSYNC_CYCLE=         16666667,
		// Measurement taken from actual FM TOWNS II MX hardware tells VSYNC lasts for 60us.
		CRT_VERTICAL_DURATION=16666667-3940000, // FM77AV40 Hardware Manual tells Vertical Blank is 3.94ms
		// Beginning of vertical blank to VSYNC
		CRT_VBLANK_TO_VSYNC=1520000,  // 1.52ms FM77AV40 Hardware Manual pp.182
		//
		CRT_VSYNC_DURATION=510000,    // 0.51ms FM77AV40 Hardware Manual pp.182
		// HSYNC_CYCLE should be 32000, but it is close enough to 0x8000(32768)
		HSYNC_CYCLE=            63500, // 63.5us FM77AV40 Hardware Manual pp.180
		CRT_HORIZONTAL_DURATION= 39700, // 39.7us FM77AV40 Hardware Manual pp.180

		// From FM77AV40 Hardware Manual
		// In 200-line mode
		//   Horizontal cycle is 63.5us.  23.8us blank and then 39.7us display
		//   Vertical cycle is 1/60sec.   3.94ms blank and 12.7ms display    (12.7ms is 63.5us times 200)
		//   VSYNC starts 1.52ms after vertical blank and lasts for 0.51ms
		// In 400-line mode
		//   Horizontal cycle is 41.04us. 10.64us blank and then 30.4us display
		//   Vertical cycle is 18.06ms.   1.64ms blank and then 16.42ms display
		//   VSYNC starts 0.34ms after vertical blank and lasts for 0.32ms

		VRAM_PLANE_SIZE=0x4000,
		VRAM_PLANE_MASK=0x3FFF,
	};

	class State
	{
	public:
		Palette palette;
		HardwareDrawing hardDraw;
		unsigned int scrnMode=SCRNMODE_640X200;
		uint16_t VRAMOffset[FM77AV40_NUM_VRAM_BANKS]={0,0,0},VRAMOffsetMask=0xffe0;
		uint8_t VRAMAccessMask=0;
		uint8_t displayPage=0;
		uint8_t activePage=0;
		bool CRTEnabled=true;
	};

	uint8_t breakOnHardwareVRAMWriteOpBits=0;
	bool breakOnHardwareLineDrawing=false;

	FM77AVCRTC(VMBase *vmBase);
	void Reset(void);
	State state;
	Palette &GetPalette(void);
	const Palette &GetPalette(void) const;
	bool InVSYNC(uint64_t fm77avTime) const;
	bool InVBLANK(uint64_t fm77avTime) const;
	bool InHSYNC(uint64_t fm77avTime) const;
	inline bool InBlank(uint64_t fm77avTime) const
	{
		return InVBLANK(fm77avTime)||InHSYNC(fm77avTime);
	}
	inline uint16_t GetActiveVRAMOffset(void) const
	{
		return state.VRAMOffset[state.activePage];
	}

	/*! Returns 80 for 640x? mode, 40 for 320x? mode.
	*/
	unsigned int GetBytesPerLine(void) const;
	/*! Returns 0x3FFF for 640x? mode, 0x1FFF for 320x? mode.
	*/
	unsigned int GetPlaneVRAMMask(void) const;

	void AddBreakOnHardwareVRAMWriteType(uint8_t opType);
	void ClearBreakOnHardwareVRAMWriteType(uint8_t opType);

	void BreakOnHardwareLineDrawing(void);
	void ClearBreakOnHardwareLineDrawing(void);

	void WriteFD30(uint8_t data);
	void WriteFD31(uint8_t data);
	void WriteFD32(uint8_t data);
	void WriteFD33(uint8_t data);
	void WriteFD34(uint8_t data);

	void WriteFD12(uint8_t data);
	const int NonDestructiveReadFD12(void) const;

	void WriteD40E(uint8_t data);
	void WriteD40F(uint8_t data);

	void WriteD430(uint8_t data);
	uint8_t NonDestructiveReadD430(void) const;

	// Graphics Accelerator >>
	void VRAMDummyRead(uint16_t VRAMAddr);
	void WriteD408(void);
	void ReadD408(void);
	void WriteD410(uint8_t data);
	uint8_t NonDestructiveReadD410(void) const;
	void WriteD411(uint8_t data);
	uint8_t NonDestructiveReadD411(void) const;
	void WriteD412(uint8_t data);
	uint8_t NonDestructiveReadD412(void) const;
	void WriteD413(uint8_t data);
	uint8_t NonDestructiveReadD413(void) const;
	void WriteD414(uint8_t data);
	void WriteD415(uint8_t data);
	void WriteD416(uint8_t data);
	void WriteD417(uint8_t data);
	void WriteD418(uint8_t data);
	void WriteD419(uint8_t data);
	void WriteD41A(uint8_t data);
	void WriteD41B(uint8_t data);
	uint8_t NonDestructiveReadD41B(void) const;
	void WriteD41C(uint8_t data);
	void WriteD41D(uint8_t data);
	void WriteD41E(uint8_t data);
	void WriteD420(uint8_t data);
	void WriteD421(uint8_t data);
	void WriteD422(uint8_t data);
	void WriteD423(uint8_t data);
	void WriteD424(uint8_t data);
	void WriteD425(uint8_t data);
	void WriteD426(uint8_t data);
	void WriteD427(uint8_t data);
	void WriteD428(uint8_t data);
	void WriteD429(uint8_t data);
	void WriteD42A(uint8_t data);
	void WriteD42B(uint8_t data);
	void DrawLine(void);
	void PutDot(uint8_t *VRAM,unsigned int VRAMAddr,uint8_t bit,unsigned int count); // Count is for line stipple or tile.
	// Graphics Accelerator <<


	static inline uint32_t TransformVRAMAddress(uint32_t addr,unsigned int scrnMode,uint16_t VRAMOffset)
	{
		uint32_t addrHI,addrLO;
		switch(scrnMode)
		{
		case SCRNMODE_640X200:
			addrHI=addr&~0x3FFF;
			addrLO=(addr+VRAMOffset)&0x3FFF;
			return addrHI|addrLO;
		case SCRNMODE_320X200_4096COL:
			addrHI=addr&~0x1FFF;
			addrLO=(addr+VRAMOffset)&0x1FFF;
			return addrHI|addrLO;
		}
		return addr;
	}
	inline uint32_t TransformVRAMAddress(uint32_t addr) const
	{
		return TransformVRAMAddress(addr,state.scrnMode,GetActiveVRAMOffset());
	}

	/*
	VSYNC Cycle        |<----------------->|
	Vertical Duration  |<------------->|
	VSYNC              000000000000000001111
                                           |
                                    NextVSYNCEndTime
	*/
	uint64_t NextVSYNCRisingEdge(uint64_t fm77avTime) const;
	uint64_t NextVSYNCEndTime(uint64_t fm77avTime) const;

	std::vector <std::string> GetStatusText(void) const;
};


/* } */
#endif
