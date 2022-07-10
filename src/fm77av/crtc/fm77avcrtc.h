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
		bool lineBusy=false;
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
		// VSYNC_CYCLE is 1670000, but it is close enough to 0x1000000(16777216)
		VSYNC_CYCLE=         0x1000000,
		// Measurement taken from actual FM TOWNS II MX hardware tells VSYNC lasts for 60us.
		CRT_VERTICAL_DURATION=0x1000000-60000, // Time CRTC spends for drawing.  VSYNC_CYCLE-CRT_VERTICAL_DURATION gives duration of VSYNC.
		// HSYNC_CYCLE should be 32000, but it is close enough to 0x8000(32768)
		HSYNC_CYCLE=            0x8000, // Not accurate.  Fixed at 31K
		CRT_HORIZONTAL_DURATION= 30000,

		VRAM_PLANE_SIZE=0x4000,
		VRAM_PLANE_MASK=0x3FFF,
	};

	class State
	{
	public:
		Palette palette;
		HardwareDrawing hardDraw;
		unsigned int scrnMode=SCRNMODE_640X200_SINGLE;
		uint16_t VRAMOffset[FM77AV40_NUM_VRAM_BANKS]={0,0,0},VRAMOffsetMask=0xffe0;
		uint8_t VRAMAccessMask=0;
		uint8_t displayPage=0;
		uint8_t activePage=0;
	};

	uint8_t breakOnHardwareVRAMWriteOpBits=0;

	FM77AVCRTC(VMBase *vmBase);
	void Reset(void);
	State state;
	Palette &GetPalette(void);
	const Palette &GetPalette(void) const;
	bool InVSYNC(uint64_t fm77avTime) const;
	bool InHSYNC(uint64_t fm77avTime) const;
	inline bool InBlank(uint64_t fm77avTime) const
	{
		return InVSYNC(fm77avTime)||InHSYNC(fm77avTime);
	}
	inline uint16_t GetActiveVRAMOffset(void) const
	{
		return state.VRAMOffset[state.activePage];
	}

	void AddBreakOnHardwareVRAMWriteType(uint8_t opType);
	void ClearBreakOnHardwareVRAMWriteType(uint8_t opType);

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
	// Graphics Accelerator <<


	static inline uint32_t TransformVRAMAddress(uint32_t addr,unsigned int scrnMode,uint16_t VRAMOffset)
	{
		uint32_t addrHI,addrLO;
		switch(scrnMode)
		{
		case SCRNMODE_640X200_SINGLE:
		case SCRNMODE_640X200_DOUBLE:
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
		return TransformVRAMAddress(addr,state.scrnMode,GetActiveVRAMOffset()&state.VRAMOffsetMask);
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
