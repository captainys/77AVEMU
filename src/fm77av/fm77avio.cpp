#include <iostream>
#include "fm77av.h"



void FM77AV::IOWrite(uint16_t ioAddr,uint8_t value)
{
	if((0xFD00==(ioAddr&0xFF00) && true==var.monitorIOWriteMain[ioAddr&0xFF]) ||
	   (0xD400==(ioAddr&0xFF00) && true==var.monitorIOWriteSub[ioAddr&0xFF]))
	{
		std::cout << "IOWRITE ";
		if(0xFD00==(ioAddr&0xFF00))
		{
			std::cout << "MAIN:" << cpputil::Ustox(mainCPU.state.PC) << " ";
		}
		else
		{
			std::cout << "SUB: " << cpputil::Ustox(subCPU.state.PC) << " ";
		}
		std::cout << "IO:" << cpputil::Ustox(ioAddr) << " ";
		std::cout << "VALUE:" << cpputil::Ubtox(value) << std::endl;
	}

	switch(ioAddr)
	{
	// Main-CPU I/O
	case FM77AVIO_KEYCODE_PRINTER_CASSETTE://=0xFD00,
		dataRecorder.WriteFD00(state.fm77avTime,value);
		break;
	case FM77AVIO_CASSETTE_IRQMASK://=        0xFD02,
		state.main.irqEnableBits=value|SystemState::MAIN_IRQ_ENABLE_ALWAYS_ON;
		if(0!=(value&1))
		{
			state.keyboardIRQHandler=CPU_MAIN;
		}
		else
		{
			state.keyboardIRQHandler=CPU_SUB;
		}
		break;

	case FM77AVIO_IRQ_BEEP://=                0xFD03,
		// Write to FD03 is taken by sound.
		sound.IOWriteByte(ioAddr,value);
		break;

	case FM77AVIO_SUBSYS_BUSY_HALT: // 0xFD05
		if(0!=(0x80&value))
		{
			if(true==state.subSysBusy)
			{
				std::cout << "Warning!  Sub-Sys Halt request while sub-CPU is busy." << std::endl;
			}

			state.subSysHalt=true;
			state.subSysBusy=true;
			// Question: What's going to happen if the main CPU writes $80 to $FD05
			//           while the busy flag is true?  Does it halt the sub-CPU anyway?
			//           Or, is it ignored?
			// F-BASIC Analysis Manual Phase III Sub-System, pp.38 tells,
			//   (2) BUSY Flag
			//   This flag tells main CPU that sub-system is not in a good timing to halt
			//   eg. executing a command.  It is controlled by sub-system I/O $D40A read/write.
			//   It also goes HI when sub-CPU accepted HALT request from main CPU automatically
			//   (by hardware level).  It serves as HALT acknowledge as well.
			//   So, there are two things: Sub-System BUSY and Sub-System Halt.
			//   Should I make sub-system busy upon 0x80 write to 0xFD05?

			// This description is questionable.  FM-7 Schematic on FM-7/8 Utilization Research
			// does not indicate that BUSY flag has anything to do with HALT status.

			// BUSY signal comes out of M44 8pin (pp.300) then comes into M72 15pin and goes 
			// out to DB7 from 11pin (pp.294).

			// BUSY signal latches to SRWB on *SBUSYSET on M44.  SRWB is read/write signal
			// from the sub-CPU (6809), which makes sense.  However, BUSY signal doesn't seem
			// to have anything to do with sub-CPU HALT status.

			// If my understanding is correct, subSysBusy should not be set here.

			// OK. my bad.  *SHALTAC & *SRESET is given to CLR pin of M44.
			// So, HALTAC (presumably Halt Acknowledge) has same effect as RESET, which should
			// raise the busy flag.  Then, the description of F-BASIC Analysis Manual Phase III
			// should be correct.
		}
		else
		{
			if(true==state.subSysHalt && true==var.monitorSubSysCmd)
			{
				PrintSubSystemCommand();
			}
			if(true==state.subSysHalt && true==var.breakOnUnhaltSubCPU)
			{
				mainCPU.debugger.ExternalBreak("Sub-CPU Unhalt.");
			}
			auto subCmd=physMem.state.data[PhysicalMemory::SUBSYS_SHARED_RAM_BEGIN+2];
			if(subCmd<FM7_MAX_SUB_CMD && 0!=(var.breakOnSubCmd[subCmd]&MC6809::Debugger::BRKPNT_FLAG_BREAK))
			{
				std::string str;
				str="Sub-CPU Command";
				str+=SubCmdToStr(subCmd);
				mainCPU.debugger.ExternalBreak(str);
			}
			state.subSysHalt=false;
		}
		if(0!=(0x40&value))
		{
			state.sub.irqSource|=SystemState::SUB_IRQ_SOURCE_CANCEL_REQ;
		}
		break;
	case FM77AVIO_RS232C_ENABLE_AV40: //=      0xFD0B,
		// In FM-7, $FD0B works same as write to $FD0F.
		// Confirmed on actual hardware.
		// See schematic on FM-7/8 Utilization Research pp.306.  RFD0F and WFD0F (WFD0F is missing last F) are unaware of AB2.
		// I encountered this problem when I was writing an RS-232C utility, and wanted to make sure to enable
		// on-board RS232C of FM77AV40, when the program stopped working on FM-7.  The way FM-7 stopped just looked like
		// accidental enabling of shadow RAM.
		// Then I confirmed on the schematic.
		if(MACHINETYPE_FM7==state.machineType)
		{
			EnableShadowRAM();
		}
		break;

	case FM77AVIO_INITIATOR_ROM://=           0xFD10,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			physMem.IOWriteByte(ioAddr,value);
		}
		break;

	case FM77AVIO_YM2203C_CONTROL://         0xFD15,
	case FM77AVIO_YM2203C_DATA://            0xFD16,
	case FM77AVIO_PSG_CONTROL://             0xFD0D,
	case FM77AVIO_PSG_DATA://                0xFD0E,
		sound.IOWriteByte(ioAddr,value);
		break;

	case FM77AVIO_SHADOW_RAM: //=              0xFD0F,
		EnableShadowRAM();
		break;

	case FM77AVIO_SUBSYS_MODE://=             0xFD12,
		crtc.WriteFD12(value);
		break;
	case FM77AVIO_SUBSYS_BANK://=             0xFD13,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			physMem.WriteFD13(value);
			if(true==physMem.state.subROMSwitch)
			{
				subCPU.Reset();
				subCPU.state.PC=subMemAcc.FetchWord(0xFFFE);
			}
		}
		break;

	case FM77AVIO_FDC_STATUS_COMMAND://      0xFD18,
	case FM77AVIO_FDC_TRACK://               0xFD19,
	case FM77AVIO_FDC_SECTOR://              0xFD1A,
	case FM77AVIO_FDC_DATA://                0xFD1B,
	case FM77AVIO_FDC_SIDE://                0xFD1C,
	case FM77AVIO_FDC_MOTOR_DRIVE://         0xFD1D,
	case FM77AVIO_FDC_DRQ_IRQ://             0xFD1F,
		fdc.IOWriteByte(ioAddr,value);
		break;

	case FM77AVIO_KANJI_ADDR_HIGH://=         0xFD20,
		physMem.WriteFD20(value);
		break;
	case FM77AVIO_KANJI_ADDR_LOW://=          0xFD21,
		physMem.WriteFD21(value);
		break;

	case FM77AVIO_ANALOG_PALETTE_NUM_HIGH://= 0xFD30,
		crtc.WriteFD30(value);
		break;
	case FM77AVIO_ANALOG_PALETTE_NUM_LOW://=  0xFD31,
		crtc.WriteFD31(value);
		break;
	case FM77AVIO_ANALOG_PALETTE_BLUE://=     0xFD32,
		crtc.WriteFD32(value);
		break;
	case FM77AVIO_ANALOG_PALETTE_RED://=      0xFD33,
		crtc.WriteFD33(value);
		break;
	case FM77AVIO_ANALOG_PALETTE_GREEN://=    0xFD34,
		crtc.WriteFD34(value);
		break;

	case FM77AVIO_DISPLAY_PAGE://=            0xFD37,
		crtc.state.VRAMAccessMask=((value>>4)&7);
		physMem.state.VRAMAccessMask=(value&7);
		break;

	case FM77AVIO_DIGITAL_PALETTE_0: //=       0xFD38,
	case FM77AVIO_DIGITAL_PALETTE_1: //=       0xFD39,
	case FM77AVIO_DIGITAL_PALETTE_2: //=       0xFD3A,
	case FM77AVIO_DIGITAL_PALETTE_3: //=       0xFD3B,
	case FM77AVIO_DIGITAL_PALETTE_4: //=       0xFD3C,
	case FM77AVIO_DIGITAL_PALETTE_5: //=       0xFD3D,
	case FM77AVIO_DIGITAL_PALETTE_6: //=       0xFD3E,
	case FM77AVIO_DIGITAL_PALETTE_7: //=       0xFD3F,
		crtc.state.palette.digitalPalette[ioAddr-FM77AVIO_DIGITAL_PALETTE_0]=(value&7);
		break;

	case FM77AVIO_MMR_0://=                   0xFD80,
	case FM77AVIO_MMR_1://=                   0xFD81,
	case FM77AVIO_MMR_2://=                   0xFD82,
	case FM77AVIO_MMR_3://=                   0xFD83,
	case FM77AVIO_MMR_4://=                   0xFD84,
	case FM77AVIO_MMR_5://=                   0xFD85,
	case FM77AVIO_MMR_6://=                   0xFD86,
	case FM77AVIO_MMR_7://=                   0xFD87,
	case FM77AVIO_MMR_8://=                   0xFD88,
	case FM77AVIO_MMR_9://=                   0xFD89,
	case FM77AVIO_MMR_A://=                   0xFD8A,
	case FM77AVIO_MMR_B://=                   0xFD8B,
	case FM77AVIO_MMR_C://=                   0xFD8C,
	case FM77AVIO_MMR_D://=                   0xFD8D,
	case FM77AVIO_MMR_E://=                   0xFD8E,
	case FM77AVIO_MMR_F://=                   0xFD8F,
	case FM77AVIO_MMR_SEG://=                 0xFD90,
	case FM77AVIO_WINDOW_OFFSET://=           0xFD92,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			mainMemAcc.IOWriteByte(ioAddr,value);
		}
		break;
	case FM77AVIO_MEMORY_MODE://=             0xFD93,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			mainMemAcc.IOWriteByte(ioAddr,value);
			if(0==(value&1))
			{
				physMem.state.FE00ROMMode=true;
			}
			else
			{
				physMem.state.FE00ROMMode=false;
			}
		}
		break;
	case FM77AVIO_AV40_EXTMMR://=             0xFD94,
		if(MACHINETYPE_FM77AV40<=state.machineType)
		{
			mainMemAcc.IOWriteByte(ioAddr,value);
		}
		break;



	// Sub-CPU I/O
	case FM77AVIO_ACK_CANCEL_IRQ: //=          0xD402,
		// Same as $D404.  Is WD402 also clear main-to-sub IRQ?
		ClearMainToSubIRQFlag();
		break;
	case FM77AVIO_BEEP:// =                    0xD403,
		sound.IOWriteByte(ioAddr,value);
		break;
	case FM77AVIO_IRQ_TO_MAINCPU: //=          0xD404,
		// FM-Techknow pp. pp.483 and Oh!FM May 1985 pp.47 tells Read D404 to send attnention IRQ to main CPU.
		// F-BASIC Analysys Manual Phase II pp.36 tells reqd/write to send attention IRQ to main CPU.
		// Which is true?
		SetSubToMainFIRQFlag();
		break;

	case FM77AVIO_CRT_ON_OFF://=              0xD408,
		crtc.WriteD408();
		break;

	case FM77AVIO_SUBCPU_BUSY: // =             0xD40A,
		state.subSysBusy=true;
		break;
	case FM77AVIO_VRAM_OFFSET_HIGH:// =        0xD40E,
		crtc.WriteD40E(value);
		break;
	case FM77AVIO_VRAM_OFFSET_LOW: // =         0xD40F,
		if(state.machineType<MACHINETYPE_FM77AV)
		{
			value&=~0x1F;
		}
		crtc.WriteD40F(value);
		break;


	case FM77AVIO_LOGICAL_OP_MODE://         0xD410,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			crtc.WriteD410(value);
		}
		break;
	case FM77AVIO_LOGICAL_OP_COLOR://        0xD411,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			crtc.WriteD411(value);
		}
		break;
	case FM77AVIO_LOGICAL_OP_MASK://         0xD412,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			crtc.WriteD412(value);
		}
		break;
	case FM77AVIO_LOGICAL_OP_COMPARE_0://    0xD413,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			crtc.WriteD413(value);
		}
		break;
	case FM77AVIO_LOGICAL_OP_COMPARE_1://    0xD414,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			crtc.WriteD414(value);
		}
		break;
	case FM77AVIO_LOGICAL_OP_COMPARE_2://    0xD415,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			crtc.WriteD415(value);
		}
		break;
	case FM77AVIO_LOGICAL_OP_COMPARE_3://    0xD416,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			crtc.WriteD416(value);
		}
		break;
	case FM77AVIO_LOGICAL_OP_COMPARE_4://    0xD417,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			crtc.WriteD417(value);
		}
		break;
	case FM77AVIO_LOGICAL_OP_COMPARE_5://    0xD418,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			crtc.WriteD418(value);
		}
		break;
	case FM77AVIO_LOGICAL_OP_COMPARE_6://    0xD419,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			crtc.WriteD419(value);
		}
		break;
	case FM77AVIO_LOGICAL_OP_COMPARE_7://    0xD41A,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			crtc.WriteD41A(value);
		}
		break;
	case FM77AVIO_LOGICAL_OP_BANK_DISABLE:// 0xD41B,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			crtc.WriteD41B(value);
		}
		break;
	case FM77AVIO_LOGICAL_OP_TILING_BLUE://  0xD41C,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			crtc.WriteD41C(value);
		}
		break;
	case FM77AVIO_LOGICAL_OP_TILING_RED://   0xD41D,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			crtc.WriteD41D(value);
		}
		break;
	case FM77AVIO_LOGICAL_OP_TILING_GREEN:// 0xD41E,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			crtc.WriteD41E(value);
		}
		break;

	case FM77AVIO_LINE_DRAW_OFFSET_HIGH://   0xD420,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			crtc.WriteD420(value);
		}
		break;
	case FM77AVIO_LINE_DRAW_OFFSET_LOW://    0xD421,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			crtc.WriteD421(value);
		}
		break;
	case FM77AVIO_LINE_DRAW_PATTERN_HIGH://  0xD422,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			crtc.WriteD422(value);
		}
		break;
	case FM77AVIO_LINE_DRAW_PATTERN_LOW://   0xD423,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			crtc.WriteD423(value);
		}
		break;
	case FM77AVIO_LINE_DRAW_X0_HIGH://       0xD424,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			crtc.WriteD424(value);
		}
		break;
	case FM77AVIO_LINE_DRAW_X0_LOW://        0xD425,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			crtc.WriteD425(value);
		}
		break;
	case FM77AVIO_LINE_DRAW_Y0_HIGH://       0xD426,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			crtc.WriteD426(value);
		}
		break;
	case FM77AVIO_LINE_DRAW_Y0_LOW://        0xD427,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			crtc.WriteD427(value);
		}
		break;
	case FM77AVIO_LINE_DRAW_X1_HIGH://       0xD428,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			crtc.WriteD428(value);
		}
		break;
	case FM77AVIO_LINE_DRAW_X1_LOW://        0xD429,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			crtc.WriteD429(value);
		}
		break;
	case FM77AVIO_LINE_DRAW_Y1_HIGH://       0xD42A,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			crtc.WriteD42A(value);
		}
		break;
	case FM77AVIO_LINE_DRAW_Y1_LOW://        0xD42B,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			crtc.WriteD42B(value);
		}
		break;


	case FM77AVIO_LINE_DRAW_STATUS://=        0xD430,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			crtc.WriteD430(value);
			physMem.WriteD430(value);
			state.subNMIMask=(0!=(value&0x80));
		}
		break;

	case FM77AVIO_ENCODER_DATA://=            0xD431,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			keyboard.WriteD431(value);
		}
		break;
	case FM77AVIO_ENCODER_STATUS://=          0xD432,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			keyboard.WriteD432(value);
		}
		break;
	}
}
uint8_t FM77AV::IORead(uint16_t ioAddr)
{
	uint8_t byteData=NonDestructiveIORead(ioAddr);

	if((0xFD00==(ioAddr&0xFF00) && true==var.monitorIOReadMain[ioAddr&0xFF]) ||
	   (0xD400==(ioAddr&0xFF00) && true==var.monitorIOReadSub[ioAddr&0xFF]))
	{
		std::cout << "IOREAD  ";
		if(0xFD00==(ioAddr&0xFF00))
		{
			std::cout << "MAIN:" << cpputil::Ustox(mainCPU.state.PC) << " ";
		}
		else
		{
			std::cout << "SUB: " << cpputil::Ustox(subCPU.state.PC) << " ";
		}
		std::cout << "IO:" << cpputil::Ustox(ioAddr) << " ";
		std::cout << "VALUE:" << cpputil::Ubtox(byteData) << std::endl;
	}

	switch(ioAddr)
	{
	// Main-CPU I/O
	case FM77AVIO_KEYCODE_PRINTER: //=         0xFD01,
		ClearKeyIRQFlag();
		keyboard.ProcessKeyCodeInQueue();
		break;
	case FM77AVIO_CASSETTE_IRQMASK://=        0xFD02,
		dataRecorder.Move(state.fm77avTime);
		byteData=NonDestructiveIORead(ioAddr);  // This one needs to be read after Move.
		break;
	case FM77AVIO_IRQ_BEEP: // =                0xFD03,
		// From FM-7 schematic, RFD03 seems to clear timer and printer irq.
		state.main.irqSource&=~(SystemState::MAIN_IRQ_SOURCE_TIMER|SystemState::MAIN_IRQ_SOURCE_PRINTER);
		break;
	case FM77AVIO_FIRQ_SUBSYS_INTERFACE: // =   0xFD04,
		// FIRQ flag must be cleared upon reading $FD04
		// F-BASIC Analysis Manual Phase III Sub-System, Shuwa System Trading, pp.45
		// Probably break-key FIRQ not.
		state.main.firqSource&=~SystemState::MAIN_FIRQ_SOURCE_ATTENTION;
		break;
	case FM77AVIO_RS232C_ENABLE_AV40: //=      0xFD0B,
		// In FM-7, $FD0B works same as write to $FD0F.
		// Confirmed on actual hardware.
		// See schematic on FM-7/8 Utilization Research pp.306.  RFD0F and WFD0F (WFD0F is missing last F) are unaware of AB2.
		// I encountered this problem when I was writing an RS-232C utility, and wanted to make sure to enable
		// on-board RS232C of FM77AV40, when the program stopped working on FM-7.  The way FM-7 stopped just looked like
		// accidental enabling of shadow RAM.
		// Then I confirmed on the schematic.
		if(MACHINETYPE_FM7==state.machineType)
		{
			DisableShadowRAM();
		}
		break;

	case FM77AVIO_YM2203C_CONTROL://         0xFD15,
	case FM77AVIO_YM2203C_DATA://            0xFD16,
	case FM77AVIO_PSG_CONTROL://             0xFD0D,
	case FM77AVIO_PSG_DATA://                0xFD0E,
		// Probably Reading sound I/O won't change anything.
		break;
	case FM77AVIO_INT_YM2203C_MOUSE://=       0xFD17,
		// Am I supposed to clear OPN and MOUSE interrupt?
		break;


	case FM77AVIO_SHADOW_RAM: //=              0xFD0F,
		DisableShadowRAM();
		break;

	case FM77AVIO_FDC_STATUS_COMMAND://      0xFD18,
	case FM77AVIO_FDC_TRACK://               0xFD19,
	case FM77AVIO_FDC_SECTOR://              0xFD1A,
	case FM77AVIO_FDC_DATA://                0xFD1B,
	case FM77AVIO_FDC_SIDE://                0xFD1C,
	case FM77AVIO_FDC_MOTOR_DRIVE://         0xFD1D,
	case FM77AVIO_FDC_DRQ_IRQ://             0xFD1F,
		byteData=fdc.IOReadByte(ioAddr);
		break;


	// Sub-CPU I/O
	case FM77AVIO_KEY_LOW: //=                 0xD401,
		ClearKeyIRQFlag();
		keyboard.ProcessKeyCodeInQueue();
		break;
	case FM77AVIO_ACK_CANCEL_IRQ: //=          0xD402,
		ClearMainToSubIRQFlag();
		break;
	case FM77AVIO_BEEP:// =                    0xD403,
		sound.IOReadByte(ioAddr);
		break;
	case FM77AVIO_IRQ_TO_MAINCPU: //=          0xD404,
		SetSubToMainFIRQFlag();
		break;
	case FM77AVIO_CRT_ON_OFF://=              0xD408,
		crtc.ReadD408();
		break;
	case FM77AVIO_SUBCPU_BUSY: // =             0xD40A,
		state.subSysBusy=false;
		break;
	case FM77AVIO_LINE_DRAW_STATUS://=        0xD430,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			physMem.state.subROMSwitch=false;
		}
		break;
	case FM77AVIO_ENCODER_DATA://=            0xD431,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			keyboard.AfterReadD431();
		}
		break;
	case FM77AVIO_ENCODER_STATUS://=          0xD432,
		break;
	}
	return byteData;
}

uint8_t FM77AV::NonDestructiveIORead(uint16_t ioAddr) const
{
	uint8_t byteData=0xFF;
	switch(ioAddr)
	{
	// Main-CPU I/O
	case FM77AVIO_KEYCODE_PRINTER_CASSETTE: // =0xFD00,
		byteData=0x7F;
		if(keyboard.state.lastKeyCode&0x100)
		{
			byteData|=0x80;
		}
		break;
	case FM77AVIO_KEYCODE_PRINTER: // =         0xFD01,
		byteData=keyboard.state.lastKeyCode&0xFF;
		break;
	case FM77AVIO_CASSETTE_IRQMASK://=        0xFD02,
		byteData=0x7F;
		if(true==dataRecorder.Read())
		{
			byteData|=0x80;
		}
		break;
	case FM77AVIO_IRQ_BEEP: // =                0xFD03,
		{
			uint8_t irqSource=(state.main.irqSource&0x07);
			if(0!=(state.main.irqSource&~0x0f))
			{
				irqSource|=SystemState::MAIN_IRQ_SOURCE_EXT;
			}
			byteData=~irqSource; // Active-Low
		}
		break;
	case FM77AVIO_FIRQ_SUBSYS_INTERFACE: // =   0xFD04,
		byteData=~state.main.firqSource; // Active-Low
		break;
	case FM77AVIO_SUBSYS_BUSY_HALT:
		byteData=0x7E; // Bit0=0 means has disk, RS232C, or ext device.
		if(true!=ExternalDevicePresent())
		{
			byteData|=1;
		}
		if(true==state.subSysBusy)
		{
			byteData|=0x80;
		}
		break;
	case FM77AVIO_RS232C_ENABLE_AV40: //=      0xFD0B,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			if(true==physMem.state.DOSMode)
			{
				byteData|=0x01;
			}
			else
			{
				byteData&=0xFE;
			}
		}
		break;

	case FM77AVIO_SUBSYS_MODE://=             0xFD12,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			byteData=crtc.NonDestructiveReadFD12();
		}
		break;
	case FM77AVIO_SUBSYS_BANK://=             0xFD13,
		break;

	case FM77AVIO_YM2203C_CONTROL://         0xFD15,
	case FM77AVIO_YM2203C_DATA://            0xFD16,
	case FM77AVIO_PSG_CONTROL://             0xFD0D,
	case FM77AVIO_PSG_DATA://                0xFD0E,
		byteData=sound.NonDestructiveIOReadByte(ioAddr);
		break;
	case FM77AVIO_INT_YM2203C_MOUSE://=       0xFD17,
		if(0!=(state.main.irqSource&SystemState::MAIN_IRQ_SOURCE_YM2203C))
		{
			byteData&=0xF7;
		}
		break;



	case FM77AVIO_FDC_STATUS_COMMAND://      0xFD18,
	case FM77AVIO_FDC_TRACK://               0xFD19,
	case FM77AVIO_FDC_SECTOR://              0xFD1A,
	case FM77AVIO_FDC_DATA://                0xFD1B,
	case FM77AVIO_FDC_SIDE://                0xFD1C,
	case FM77AVIO_FDC_MOTOR_DRIVE://         0xFD1D,
	case FM77AVIO_FDC_DRQ_IRQ://             0xFD1F,
		byteData=fdc.NonDestructiveIORead(ioAddr);
		break;

	case FM77AVIO_KANJI_DATA_L://=          0xFD22,
		byteData=physMem.ReadFD22();
		break;
	case FM77AVIO_KANJI_DATA_R://=         0xFD23,
		byteData=physMem.ReadFD23();
		break;


	case FM77AVIO_DIGITAL_PALETTE_0: //=       0xFD38,
	case FM77AVIO_DIGITAL_PALETTE_1: //=       0xFD39,
	case FM77AVIO_DIGITAL_PALETTE_2: //=       0xFD3A,
	case FM77AVIO_DIGITAL_PALETTE_3: //=       0xFD3B,
	case FM77AVIO_DIGITAL_PALETTE_4: //=       0xFD3C,
	case FM77AVIO_DIGITAL_PALETTE_5: //=       0xFD3D,
	case FM77AVIO_DIGITAL_PALETTE_6: //=       0xFD3E,
	case FM77AVIO_DIGITAL_PALETTE_7: //=       0xFD3F,
		byteData=0xF8|crtc.state.palette.digitalPalette[ioAddr-FM77AVIO_DIGITAL_PALETTE_0];
		break;

	case FM77AVIO_MMR_0://=                   0xFD80,
	case FM77AVIO_MMR_1://=                   0xFD81,
	case FM77AVIO_MMR_2://=                   0xFD82,
	case FM77AVIO_MMR_3://=                   0xFD83,
	case FM77AVIO_MMR_4://=                   0xFD84,
	case FM77AVIO_MMR_5://=                   0xFD85,
	case FM77AVIO_MMR_6://=                   0xFD86,
	case FM77AVIO_MMR_7://=                   0xFD87,
	case FM77AVIO_MMR_8://=                   0xFD88,
	case FM77AVIO_MMR_9://=                   0xFD89,
	case FM77AVIO_MMR_A://=                   0xFD8A,
	case FM77AVIO_MMR_B://=                   0xFD8B,
	case FM77AVIO_MMR_C://=                   0xFD8C,
	case FM77AVIO_MMR_D://=                   0xFD8D,
	case FM77AVIO_MMR_E://=                   0xFD8E,
	case FM77AVIO_MMR_F://=                   0xFD8F,
	case FM77AVIO_MMR_SEG://=                 0xFD90,
	case FM77AVIO_WINDOW_OFFSET://=           0xFD92,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			byteData=mainMemAcc.NonDestructiveIOReadByte(ioAddr);
		}
		break;
	case FM77AVIO_MEMORY_MODE://=             0xFD93,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			byteData=mainMemAcc.NonDestructiveIOReadByte(ioAddr);
			if(true==physMem.state.FE00ROMMode)
			{
				byteData&=0xFE;
			}
			else
			{
				byteData|=1;
			}
		}
		break;
	case FM77AVIO_AV40_EXTMMR://=             0xFD94,
		if(MACHINETYPE_FM77AV40<=state.machineType)
		{
			// byteData=mainMemAcc.NonDestructiveIOReadByte(ioAddr);
		}
		break;



	// Sub-CPU I/O
	case FM77AVIO_KEY_HIGH: // =                0xD400,
		byteData=0x7F;
		if(keyboard.state.lastKeyCode&0x100)
		{
			byteData|=0x80;
		}
		break;
	case FM77AVIO_KEY_LOW: //=                 0xD401,
		byteData=keyboard.state.lastKeyCode&0xFF;
		break;


	case FM77AVIO_LOGICAL_OP_MODE://         0xD410,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			byteData=crtc.NonDestructiveReadD410();
		}
		break;
	case FM77AVIO_LOGICAL_OP_COLOR://        0xD411,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			byteData=crtc.NonDestructiveReadD411();
		}
		break;
	case FM77AVIO_LOGICAL_OP_MASK://         0xD412,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			byteData=crtc.NonDestructiveReadD412();
		}
		break;
	case FM77AVIO_LOGICAL_OP_COMPARE_0://    0xD413,://         0xD413,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			byteData=crtc.NonDestructiveReadD413();
		}
		break;
	case FM77AVIO_LOGICAL_OP_BANK_DISABLE:// 0xD41B,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			byteData=crtc.NonDestructiveReadD41B();
		}
		break;


	case FM77AVIO_LINE_DRAW_STATUS://=        0xD430,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			byteData=crtc.NonDestructiveReadD430();
			if(true!=physMem.state.subROMSwitch)
			{
				byteData&=0xFE;
			}
		}
		break;
	case FM77AVIO_ENCODER_DATA://=            0xD431,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			byteData=keyboard.NonDestructiveReadD431();
		}
		break;
	case FM77AVIO_ENCODER_STATUS://=          0xD432,
		if(MACHINETYPE_FM77AV<=state.machineType)
		{
			byteData=keyboard.NonDestructiveReadD432();
		}
		break;
	}
	return byteData;
}

void FM77AV::EnableShadowRAM(void)
{
	physMem.state.shadowRAMEnabled=true;
}
void FM77AV::DisableShadowRAM(void)
{
	physMem.state.shadowRAMEnabled=false;
}
