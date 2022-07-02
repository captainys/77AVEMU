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
		if(0!=(value&2)) // MOTOR ON
		{
			dataRecorder.MotorOn(state.fm77avTime);
		}
		else
		{
			dataRecorder.MotorOff();
		}
		break;
	case FM77AVIO_CASSETTE_IRQMASK://=        0xFD02,
		state.main.irqEnableBits=value;
		if(0!=(value&1))
		{
			state.keyboardIRQHandler=CPU_MAIN;
		}
		else
		{
			state.keyboardIRQHandler=CPU_SUB;
		}
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
	case FM77AVIO_SHADOW_RAM: //=              0xFD0F,
		EnableShadowRAM();
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




	// Sub-CPU I/O
	case FM77AVIO_ACK_CANCEL_IRQ: //=          0xD402,
		// Same as $D404.  Is WD402 also clear main-to-sub IRQ?
		ClearMainToSubIRQFlag();
		break;
	case FM77AVIO_IRQ_TO_MAINCPU: //=          0xD404,
		// FM-Techknow pp. pp.483 and Oh!FM May 1985 pp.47 tells Read D404 to send attnention IRQ to main CPU.
		// F-BASIC Analysys Manual Phase II pp.36 tells reqd/write to send attention IRQ to main CPU.
		// Which is true?
		SetSubToMainFIRQFlag();
		break;
	case FM77AVIO_SUBCPU_BUSY: // =             0xD40A,
		state.subSysBusy=true;
		break;
	case FM77AVIO_VRAM_OFFSET_HIGH:// =        0xD40E,
		crtc.state.VRAMOffset&=0xFF;
		crtc.state.VRAMOffset|=(value<<8);
		break;
	case FM77AVIO_VRAM_OFFSET_LOW: // =         0xD40F,
		crtc.state.VRAMOffset&=0xFF00;
		crtc.state.VRAMOffset|=value;
		if(state.machineType<MACHINETYPE_FM77AV)
		{
			crtc.state.VRAMOffset&=~0x1F;
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
	case FM77AVIO_SHADOW_RAM: //=              0xFD0F,
		DisableShadowRAM();
		break;





	// Sub-CPU I/O
	case FM77AVIO_KEY_LOW: //=                 0xD401,
		ClearKeyIRQFlag();
		keyboard.ProcessKeyCodeInQueue();
		break;
	case FM77AVIO_ACK_CANCEL_IRQ: //=          0xD402,
		ClearMainToSubIRQFlag();
		break;
	case FM77AVIO_IRQ_TO_MAINCPU: //=          0xD404,
		SetSubToMainFIRQFlag();
		break;
	case FM77AVIO_SUBCPU_BUSY: // =             0xD40A,
		state.subSysBusy=false;
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
		byteData=~state.main.irqSource; // Active-Low
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
