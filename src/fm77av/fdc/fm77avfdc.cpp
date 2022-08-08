/* LICENSE>>
Copyright 2020 Soji Yamakawa (CaptainYS, http://www.ysflight.com)

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

<< LICENSE */
#include <iostream>

#include "cpputil.h"
#include "fm77avfdc.h"
#include "fm77avdef.h"
#include "fm77av.h"


FM77AVFDC::FM77AVFDC(class FM77AV *fm77avPtr) : DiskDrive(fm77avPtr)
{
	this->fm77avPtr=fm77avPtr;

	Reset();
	debugBreakOnCommandWrite=false;
}

////////////////////////////////////////////////////////////


void FM77AVFDC::SendCommand(unsigned int cmd)
{
	DiskDrive::SendCommand(cmd,fm77avPtr->state.fm77avTime);
}
void FM77AVFDC::MakeReady(void)
{
	state.busy=false;
	fm77avPtr->state.main.irqSource|=FM77AV::SystemState::MAIN_IRQ_SOURCE_FDC;
	state.IRQ=true;
	fm77avPtr->UnscheduleDeviceCallBack(*this);
	state.lastStatus&=0xFE;  // Clear busy-flag.  Don't touch other status.
}

////////////////////////////////////////////////////////////

/* virtual */ void FM77AVFDC::RunScheduledTask(unsigned long long int fm77avTime)
{
	auto &drv=state.drive[DriveSelect()];
	auto imgFilePtr=GetDriveImageFile(DriveSelect());
	auto diskIdx=drv.diskIndex;
	DiskDrive::DiskImage *imgPtr=nullptr;
	if(nullptr!=imgFilePtr)
	{
		imgPtr=&imgFilePtr->img;
	}

	state.recordType=false;
	state.recordNotFound=false;
	state.CRCError=false;
	state.lostData=false;
	state.writeFault=false;

	switch(state.lastCmd&0xF0)
	{
	case 0x00: // Restore
		MakeReady();
		drv.trackPos=0;
		drv.trackReg=0;
		break;
	case 0x10: // Seek
		MakeReady();
		// Seek to dataReg
		if(drv.trackPos<drv.dataReg)
		{
			drv.lastSeekDir=1;
		}
		else if(drv.dataReg<drv.trackPos)
		{
			drv.lastSeekDir=-1;
		}
		drv.trackPos=drv.dataReg;
		if(state.lastCmd&0x10)
		{
			drv.trackReg=drv.trackPos;
		}
		break;
	case 0x20: // Step?
	case 0x30: // Step?
		MakeReady();
		drv.trackPos+=drv.lastSeekDir;
		if(drv.trackPos<0)
		{
			drv.trackPos=0;
		}
		else if(80<drv.trackPos)
		{
			drv.trackPos=80;
		}
		if(state.lastCmd&0x10)
		{
			drv.trackReg=drv.trackPos;
		}
		break;
	case 0x40: // Step In
	case 0x50: // Step In
		MakeReady();
		++drv.trackPos;
		if(80<drv.trackPos)
		{
			drv.trackPos=80;
		}
		if(state.lastCmd&0x10)
		{
			drv.trackReg=drv.trackPos;
		}
		break;
	case 0x60: // Step Out
	case 0x70: // Step Out
		MakeReady();
		--drv.trackPos;
		if(drv.trackPos<0)
		{
			drv.trackPos=0;
		}
		if(state.lastCmd&0x10)
		{
			drv.trackReg=drv.trackPos;
		}
		break;

	case 0x80: // Read Data (Read Sector)
	case 0x90: // Read Data (Read Sector)
		if(0==state.data.size()) // Need to prepare sector data.
		{
			if(nullptr!=imgPtr)
			{
				if(true==CheckMediaTypeAndDriveModeCompatible(drv.mediaType,GetDriveMode()))
				{
					unsigned int nSteps=0;
					auto sector=imgPtr->ReadSectorFrom(diskIdx,drv.trackPos,state.side,GetSectorReg(),state.sectorPositionInTrack,nSteps);
					if(true==sector.exists)
					{
						std::swap(state.data,sector.data);
						state.CRCErrorAfterRead=(0!=sector.crcStatus);
						state.nanosecPerByte=imgPtr->GetNanoSecPerByte(diskIdx,drv.trackPos,state.side,GetSectorReg());
						if(0==state.nanosecPerByte)
						{
							state.nanosecPerByte=NANOSEC_PER_BYTE;
						}
						if(MACHINETYPE_FM77AV40<=fm77avPtr->state.machineType &&
						   true==fm77avPtr->dmac.TxRQ() &&
						   true==fm77avPtr->dmac.FDCtoMEM())
						{
							// DMA Transfer
							fm77avPtr->dmac.SetDMAEnd(false);
							fm77avPtr->dmac.SetIRQ(false);
							fm77avPtr->dmac.SetBusy(true);
							fm77avPtr->ScheduleDeviceCallBack(*this,fm77avTime+state.nanosecPerByte*state.data.size());
						}
						else
						{
							state.dataReadPointer=0;
							state.DRQ=true;
							state.lastDRQTime=fm77avTime;
							// Should I raise IRQ?

							// CPU needs to read a byte from the data register and clear DRQ before this schedule.
							fm77avPtr->ScheduleDeviceCallBack(*this,fm77avTime+state.nanosecPerByte);
						}
					}
					else
					{
						state.recordNotFound=true;
						MakeReady();
					}
				}
				else
				{
					state.recordNotFound=true;
					MakeReady();
				}
			}
			else
			{
				MakeReady(); // Must return drive not ready.  Taken care by MakeUpStatus.
			}
		}
		else
		{
			if(MACHINETYPE_FM77AV40<=fm77avPtr->state.machineType &&
			   true==fm77avPtr->dmac.TxRQ() &&
			   true==fm77avPtr->dmac.FDCtoMEM())
			{
				auto addr0=fm77avPtr->dmac.Address();
				for(unsigned int i=0; i<fm77avPtr->dmac.NumBytes() && i<state.data.size(); ++i)
				{
					fm77avPtr->mainMemAcc.StoreByte(addr0+i,state.data[i]);
				}
				fm77avPtr->dmac.SetDMAEnd(true);
				fm77avPtr->dmac.SetIRQ(true);
				fm77avPtr->dmac.SetBusy(false);
				if(true==fm77avPtr->dmac.IRQEnabled())
				{
					fm77avPtr->state.main.irqSource|=FM77AV::SystemState::MAIN_IRQ_SOURCE_DMA;
				}
				std::cout << "DMA Transfer" << std::endl;
				MakeReady();
			}
			else
			{
				if(true!=state.DRQ && state.dataReadPointer<state.data.size())
				{
					// Pointer is incremented at IORead.
					state.DRQ=true;
					state.lastDRQTime+=state.nanosecPerByte;
					fm77avPtr->ScheduleDeviceCallBack(*this,state.lastDRQTime+state.nanosecPerByte);

					// Memo to myself:
					//   I was scheduling next DRQ as fm77avTime+NANOSEC_PER_BYTE, which was incorrect.
					//   FDC doesn't care when CPU reads a byte, therefore next DRQ will come
					//   one-byte-length on the disk after the last DRQ.
					//   YsII for FM77AV measures time for reading one sector by counting loop between DRQs.
					//   This difference matters.
				}
				else // Data was not read in time.
				{
					state.lostData=true;
					MakeReady();
				}
			}
		}
		break;
	case 0xA0: // Write Data (Write Sector)
	case 0xB0: // Write Data (Write Sector)
		if(state.expectedWriteLength==0)
		{
			if(nullptr!=imgPtr)
			{
				if(true==imgPtr->WriteProtected(diskIdx))
				{
					// Write protected.
				}
				else if(true==CheckMediaTypeAndDriveModeCompatible(drv.mediaType,GetDriveMode()))
				{
					auto sector=imgPtr->ReadSector(diskIdx,drv.trackPos,state.side,GetSectorReg());
					if(true==sector.exists && 0<sector.data.size())
					{
						state.data.resize(sector.data.size());
						state.expectedWriteLength=sector.data.size();
						state.dataReadPointer=0; // Maybe I should call it just data pointer.
						state.DRQ=true;
						state.lastDRQTime=fm77avTime;
						// Should I raise IRQ?
						fm77avPtr->ScheduleDeviceCallBack(*this,fm77avTime+NANOSEC_PER_BYTE);
					}
					else
					{
						state.recordNotFound=true;
						MakeReady();
					}
				}
				else
				{
					state.recordNotFound=true;
					MakeReady();
				}
			}
			else
			{
				MakeReady(); // Must return drive not ready.  Taken care by MakeUpStatus.
			}
		}
		else if(true!=state.DRQ)
		{
			state.DRQ=true;
			// Should I raise IRQ?
			state.lastDRQTime+=NANOSEC_PER_BYTE;
			fm77avPtr->ScheduleDeviceCallBack(*this,state.lastDRQTime+NANOSEC_PER_BYTE);
		}
		else // Data didn't come in time.  In fact, I may need to write partial and mark it as CRC error.
		{
			state.lostData=true;
			MakeReady();
		}
		break;

	case 0xC0: // Read Address
		if(0==state.data.size())
		{
			if(nullptr!=imgPtr)
			{
				if(true==CheckMediaTypeAndDriveModeCompatible(drv.mediaType,GetDriveMode()))
				{
					// Copy CHRN and CRC CRC to DMA.
					state.data=imgPtr->ReadAddress(diskIdx,drv.trackPos,state.side,state.addrMarkReadCount);
					if(0<state.data.size())
					{
						state.dataReadPointer=0;
						state.DRQ=true;
						state.lastDRQTime=fm77avTime;
						// Should I raise IRQ?
						fm77avPtr->ScheduleDeviceCallBack(*this,fm77avTime+NANOSEC_PER_BYTE);
						++state.addrMarkReadCount;
					}
				}
				else
				{
					MakeReady();
					state.recordNotFound=true;
				}
			}
			else
			{
				MakeReady(); // Must return drive not ready.  Taken care by MakeUpStatus.
			}
		}
		else if(true!=state.DRQ)
		{
			state.DRQ=true;
			state.lastDRQTime+=NANOSEC_PER_BYTE;
			// Should I raise IRQ?
			fm77avPtr->ScheduleDeviceCallBack(*this,state.lastDRQTime+NANOSEC_PER_BYTE);
		}
		else
		{
			state.lostData=true;
			MakeReady();
		}
		break;
	case 0xE0: // Read Track
		if(0==state.data.size())
		{
			if(nullptr!=imgPtr)
			{
				if(true==CheckMediaTypeAndDriveModeCompatible(drv.mediaType,GetDriveMode()))
				{
					std::vector <unsigned char> rawRead;
					for(int i=0; i<0x1800; ++i)
					{
						rawRead.push_back(0x4E);
					}

					std::swap(state.data,rawRead);
					state.dataReadPointer=0;
					state.DRQ=true;
					state.lastDRQTime=fm77avTime;
					// Should I raise IRQ?
					fm77avPtr->ScheduleDeviceCallBack(*this,fm77avTime+NANOSEC_PER_BYTE);
				}
				else
				{
					MakeReady();
					state.recordNotFound=true;
				}
			}
			else
			{
				MakeReady(); // Must return drive not ready.  Taken care by MakeUpStatus.
			}
		}
		else if(true!=state.DRQ)
		{
			state.DRQ=true;
			state.lastDRQTime+=NANOSEC_PER_BYTE;
			// Should I raise IRQ?
			fm77avPtr->ScheduleDeviceCallBack(*this,state.lastDRQTime+NANOSEC_PER_BYTE);
		}
		else
		{
			state.lostData=true;
			MakeReady();
		}
		break;
	case 0xF0: // Write Track
		if(state.expectedWriteLength==0)
		{
			if(nullptr!=imgPtr)
			{
				if(true==imgPtr->WriteProtected(diskIdx))
				{
					// Write protected.
				}
				else if(true==CheckMediaTypeAndDriveModeCompatible(drv.mediaType,GetDriveMode()))
				{
					state.expectedWriteLength=FORMAT_WRITE_LENGTH_2D_2DD;
					state.data.resize(state.expectedWriteLength);
					state.dataReadPointer=0;
					state.DRQ=true;
					state.lastDRQTime=fm77avTime;
					// Should I raise IRQ?
					fm77avPtr->ScheduleDeviceCallBack(*this,state.lastDRQTime+NANOSEC_PER_BYTE);
				}
				else
				{
					state.writeFault=true;
					MakeReady();
				}
			}
			else
			{
				MakeReady(); // Must return drive not ready.  Taken care by MakeUpStatus.
			}
		}
		else if(true!=state.DRQ)
		{
			state.DRQ=true;
			state.lastDRQTime+=NANOSEC_PER_BYTE;
			// Should I raise IRQ?
			fm77avPtr->ScheduleDeviceCallBack(*this,state.lastDRQTime+NANOSEC_PER_BYTE);
		}
		else // Data didn't come in time.  In fact, I may need to write partial and mark it as CRC error.
		{
			// In fact, if it ends partial, it should still make sectors, or the sectors have already been created
			// from disk-drive point of view.
			WriteTrack(state.data);
			state.lostData=true;
			MakeReady();
		}
		break;

	default:
	case 0xD0: // Force Interrupt
		if(true==state.busy)
		{
			std::cout << "Terminated FDC command:" << cpputil::Ubtox(state.lastCmd) << std::endl;
		}
		state.busy=false;
		break;
	}
	state.lastStatus=MakeUpStatus(state.lastCmd);
}

/* virtual */ void FM77AVFDC::IOWriteByte(unsigned int ioport,unsigned int data)
{
	auto &drv=state.drive[DriveSelect()];
	auto imgFilePtr=GetDriveImageFile(DriveSelect());
	auto diskIdx=drv.diskIndex;
	DiskDrive::DiskImage *imgPtr=nullptr;
	if(nullptr!=imgFilePtr)
	{
		imgPtr=&imgFilePtr->img;
	}

	switch(ioport)
	{
	case FM77AVIO_FDC_STATUS_COMMAND://=      0xFD18,
		SendCommand(data);
		state.drive[DriveSelect()].diskChange=false; // Is this the right timing to erase diskChange flag?

		if(0xD0==(data&0xF0))
		{
			fm77avPtr->state.main.irqSource|=FM77AV::SystemState::MAIN_IRQ_SOURCE_FDC;
			state.IRQ=true;
		}
		else
		{
			fm77avPtr->state.main.irqSource&=~FM77AV::SystemState::MAIN_IRQ_SOURCE_FDC;
			state.IRQ=false;
		}

		if(true==debugBreakOnCommandWrite)
		{
			fm77avPtr->mainCPU.debugger.ExternalBreak("FDC Command Write "+cpputil::Ubtox(data)+" "+FDCCommandToExplanation(data));
		}
		if(true==monitorFDC)
		{
			std::cout << "At PC=" + cpputil::Ustox(fm77avPtr->mainCPU.state.PC);
			std::cout << " FDC CMD "+cpputil::Ubtox(data)+" "+FDCCommandToExplanation(data);
			std::cout << " DRV " << DriveSelect();
			switch(data&0xF0)
			{
			case 0x00: // Restore
				break;
			case 0x10: // Seek
				std::cout << " To Track " << drv.dataReg;
				break;
			case 0x20: // Step?
			case 0x30: // Step?
				std::cout << " Dir " << drv.lastSeekDir << " To Track " << drv.trackPos;
				break;
			case 0x40: // Step In
			case 0x50: // Step In
				std::cout << " To Track " << drv.trackPos;
				break;
			case 0x60: // Step Out
			case 0x70: // Step Out
				std::cout << " To Track " << drv.trackPos;
				break;

			case 0x80: // Read Data (Read Sector)
			case 0x90: // Read Data (Read Sector)
				std::cout << " C " << drv.trackPos << " H " << state.side << " R " << GetSectorReg();
				{
					if(nullptr!=imgPtr)
					{
						auto sector=imgPtr->ReadSector(diskIdx,drv.trackPos,state.side,GetSectorReg());
						if(true==sector.exists && 0!=sector.crcStatus)
						{
							std::cout << " CRC Error";
						}
						else if(true!=sector.exists)
						{
							std::cout << " Sector Not Exist";
						}
					}
				}
				break;
			case 0xA0: // Write Data (Write Sector)
			case 0xB0: // Write Data (Write Sector)
				std::cout << " C " << drv.trackPos << " H " << state.side << " R " << GetSectorReg();
				{
					if(nullptr!=imgPtr)
					{
						auto sector=imgPtr->ReadSector(diskIdx,drv.trackPos,state.side,GetSectorReg());
						if(true==sector.exists && 0!=sector.crcStatus)
						{
							std::cout << " CRC Error";
						}
						else if(true!=sector.exists)
						{
							std::cout << " Sector Not Exist";
						}
					}
				}
				break;
			case 0xC0: // Read Address
				break;
			case 0xE0: // Read Track
				break;
			case 0xF0: // Write Track
				break;

			default:
			case 0xD0: // Force Interrupt
				break;
			}
			std::cout << std::endl;
		}
		break;
	case FM77AVIO_FDC_TRACK://=               0xFD19,
		state.drive[DriveSelect()].trackReg=data;
		break;
	case FM77AVIO_FDC_SECTOR://=              0xFD1A,
		SetSectorReg(data);
		break;
	case FM77AVIO_FDC_DATA://=                0xFD1B,
		if(true==state.DRQ)
		{
			state.DRQ=false;
			if(state.dataReadPointer<state.data.size())
			{
				state.data[state.dataReadPointer++]=data;
			}
			auto &drv=state.drive[DriveSelect()];

			if(state.expectedWriteLength<=state.dataReadPointer)
			{
				if(0xA0==(state.lastCmd&0xF0) || 0xB0==(state.lastCmd&0xF0)) // Write Data (Write Sector)
				{
					auto len=imgPtr->GetSectorLength(diskIdx,drv.trackPos,state.side,GetSectorReg());
					if(0<len)
					{
						auto writeSize=std::max(state.dataReadPointer,len);
						imgPtr->WriteSector(diskIdx,drv.trackPos,state.side,GetSectorReg(),writeSize,state.data.data());
						if(state.lastCmd&0x10 && 0<imgPtr->GetSectorLength(diskIdx,drv.trackPos,state.side,GetSectorReg()+1)) // Multi Record
						{
							SetSectorReg(GetSectorReg()+1);
							state.expectedWriteLength=0; // Signal new sector
							fm77avPtr->ScheduleDeviceCallBack(*this,fm77avPtr->state.fm77avTime+NANOSEC_BETWEEN_MULTI_SECTOR_READ);
						}
						else
						{
							MakeReady();
						}
					}
					else
					{
						MakeReady();
					}
				}
				else if(0xF0==(state.lastCmd&0xF0)) // // Write Track
				{
					WriteTrack(state.data);
					MakeReady();
				}
			}
			state.lastStatus=MakeUpStatus(state.lastCmd);
		}
		state.drive[DriveSelect()].dataReg=data;
		break;
	case FM77AVIO_FDC_SIDE://=                0xFD1C,
		state.side=(data&1);
		break;
	case FM77AVIO_FDC_MOTOR_DRIVE://=         0xFD1D,
		state.driveSelectBit=(1<<(data&0x03));
		state.drive[DriveSelect()].motor=(0!=(data&0x80));
		break;
	}
}
/* virtual */ unsigned int FM77AVFDC::IOReadByte(unsigned int ioport)
{
	auto &drv=state.drive[DriveSelect()];
	auto imgFilePtr=GetDriveImageFile(DriveSelect());
	auto diskIdx=drv.diskIndex;
	DiskDrive::DiskImage *imgPtr=nullptr;
	if(nullptr!=imgFilePtr)
	{
		imgPtr=&imgFilePtr->img;
	}

	unsigned char data=0xFF;
	if(FM77AVIO_FDC_DATA==ioport)
	{
		if(true==state.DRQ)
		{
			state.DRQ=false;
			state.drive[DriveSelect()].dataReg=state.data[state.dataReadPointer++];
			if(state.data.size()<=state.dataReadPointer)
			{
				state.CRCError=state.CRCErrorAfterRead;
				if(0x90==(state.lastCmd&0xF0) && nullptr!=imgPtr && 0<imgPtr->GetSectorLength(diskIdx,drv.trackPos,state.side,GetSectorReg()+1)) // Read Sector + MultiRecordFlag
				{
					state.data.clear();
					state.dataReadPointer=0;
					SetSectorReg(GetSectorReg()+1);
					// Question.  Should I raise IRQ here?
				}
				else
				{
					MakeReady();
				}
			}
		}
		state.lastStatus=MakeUpStatus(state.lastCmd);
	}
	else if(FM77AVIO_FDC_STATUS_COMMAND==ioport)//=      0xFD18,
	{
		fm77avPtr->state.main.irqSource&=~FM77AV::SystemState::MAIN_IRQ_SOURCE_FDC;
		state.IRQ=false;
	}
	return NonDestructiveIORead(ioport);
}
unsigned int FM77AVFDC::NonDestructiveIORead(unsigned int ioport) const
{
	unsigned char data=0xFF;
	switch(ioport)
	{
	case FM77AVIO_FDC_STATUS_COMMAND://=      0xFD18,
		// This status byte is supposed to be a command status.  My original interpretation was it updates
		// only when FDC command was issued.
		// However, FM-OASYS reads this byte and checks bit 7 for drive-not ready without issuing a command.
		// Presumably, bit 7 of this byte returns current drive-ready state.
		// I assume FM-7 FDC works the same.  It's Fujitsu-made chip.
		data=state.lastStatus;
		if(true==DriveReady())
		{
			data&=0x7F;
		}

		switch(state.lastCmd&0xF0)
		{
		case 0x00: // Restore
		case 0x10: // Seek
		case 0x20: // Step?
		case 0x30: // Step?
		case 0x40: // Step In
		case 0x50: // Step In
		case 0x60: // Step Out
		case 0x70: // Step Out
		case 0xD0: // Force Interrupt
			data|=(true==IndexHole(fm77avPtr->state.fm77avTime) ? 0x02 : 0);
			break;

		case 0x80: // Read Data (Read Sector)
		case 0x90: // Read Data (Read Sector)
		case 0xA0: // Write Data (Write Sector)
		case 0xB0: // Write Data (Write Sector)
		case 0xC0: // Read Address
		case 0xE0: // Read Track
		case 0xF0: // Write Track
			break;
		}

		return data;
	case FM77AVIO_FDC_TRACK://=               0xFD19,
		return state.drive[DriveSelect()].trackReg;
	case FM77AVIO_FDC_SECTOR://=              0xFD1A,
		return GetSectorReg();
	case FM77AVIO_FDC_DATA://=                0xFD1B,
	return state.drive[DriveSelect()].dataReg;
	case FM77AVIO_FDC_SIDE://=                0xFD1C,
		return (state.side&1)|0xFE;
	case FM77AVIO_FDC_MOTOR_DRIVE://=         0xFD1D,
		data=(true==state.drive[DriveSelect()].motor ? 0x80 : 0);
		data|=DriveSelect();
		return data;
	case FM77AVIO_FDC_DRQ_IRQ://=             0xFD1F,
		data=0x3F;
		if(true==state.DRQ)
		{
			data|=0x80;
		}
		if(true==state.IRQ)
		{
			data|=0x40;
		}
		return data;
	}
	return data;
}
/* virtual */ void FM77AVFDC::Reset(void)
{
	DiskDrive::Reset();
	state.HISPD=false;
}

void FM77AVFDC::WriteTrack(const std::vector <uint8_t> &formatData)
{
	auto &drv=state.drive[DriveSelect()];
	auto imgFilePtr=GetDriveImageFile(DriveSelect());
	auto diskIdx=drv.diskIndex;
	DiskDrive::DiskImage *imgPtr=nullptr;
	if(nullptr!=imgFilePtr)
	{
		imgPtr=&imgFilePtr->img;
	}

	if(nullptr!=imgPtr)
	{
		if(true==imgPtr->WriteProtected(diskIdx))
		{
			// Write protected.
		}
		else if(true==CheckMediaTypeAndDriveModeCompatibleForFormat(drv.mediaType,GetDriveMode()))
		{
			// What's the length?
			// Looks like fm77av MENU makes 2DE0H bytes of data.
			// [10]
			//   1232KB format 1024 bytes per sector,  8 sectors per track, 77 tracks
			//   1440KB format  512 bytes per sector, 18 sectors per track, 80 tracks
			//    640KB format  512 bytes per sector,  8 sectors per track, 80 tracks
			//    720KB format  512 bytes per sector,  9 sectors per track, 80 tracks
			// [2] pp. 250
			// From the index hole:
			//    GAP        80 bytes 0x4E
			//    SYNC       12 bytes 0x00
			//    INDEX MARK  4 bytes 0xC2,0xC2,0xC2,0xFC (or 0xF6,0xF6,0xF6,0xFC)
			//    GAP        50 bytes 0x4E
			//    {
			//    SYNC       12 bytes 0x00
			//    ADDR MARK   4 bytes 0xA1,0xA1,0xA1,0xFE (or 0xF5,0xF5,0xF5,0xFE)
			//    CHRN        4 bytes
			//    CRC         2 bytes 0xF7 (Write 1 byte will become 2 bytes of CRC code)
			//    GAP        22 bytes
			//    SYNC       12 bytes
			//    DATA Mark   4 bytes 0xA1,0xA1,0xA1,0xFB (or 0xF5,0xF5,0xF5,0xFB)
			//    DATA        x bytes (x=128*(2^N))
			//    CRC         2 bytes 0xF7 (Write 1 byte will become 2 bytes of CRC code)
			//    GAP        54/84/116/108 bytes.
			//    } times sectors
			//    GAP        598/400/654/? bytes
			// 1232KB format -> 80+12+4+50+(12+4+4+2+22+12+4+1024+2+116)*8+654=10416 (28B0H)
			// 1440KB format -> 80+12+4+50+(12+4+4+2+22+12+4+512+2+108)*18+?=12422+? (3086H+?)
			//  640KB format -> 80+12+4+50+(12+4+4+2+22+12+4+512+2+84)*8+598=6008 (1778H)
			//  720KB format -> 80+12+4+50+(12+4+4+2+22+12+4+512+2+54)*9+400=6198 (1836H)

			// 2HD -> Read 0x3286 bytes
			// 2DD -> Read 0x1836 bytes

			unsigned int C=0,H=0,R=0,N=0,trackCapacity=0;
			std::vector <D77File::D77Disk::D77Sector> sectors;
			for(unsigned int ptr=0; ptr<formatData.size()-4; ++ptr)
			{
				// FM-OASYS writes 00 00 00 FE, 00 00 00 FB for formatting the track 0 side 0.
				if((0xA1==formatData[ptr] &&
				    0xA1==formatData[ptr+1] &&
				    0xA1==formatData[ptr+2] &&
				    0xFE==formatData[ptr+3]) ||
				   (0xF5==formatData[ptr] &&
				    0xF5==formatData[ptr+1] &&
				    0xF5==formatData[ptr+2] &&
				    0xFE==formatData[ptr+3]) ||
				   (0x00==formatData[ptr] &&
				    0x00==formatData[ptr+1] &&
				    0x00==formatData[ptr+2] &&
				    0xFE==formatData[ptr+3])
				    ) // Address Mark
				{
					C=formatData[ptr+4];
					H=formatData[ptr+5];
					R=formatData[ptr+6];
					N=formatData[ptr+7];
					std::cout << "CHRN:" << C << " " << H << " " << R << " " << N << std::endl;
					ptr+=7;
				}
				else if((0xA1==formatData[ptr] &&
				         0xA1==formatData[ptr+1] &&
				         0xA1==formatData[ptr+2] &&
				         0xFB==formatData[ptr+3]) ||
				        (0xF5==formatData[ptr] &&
				         0xF5==formatData[ptr+1] &&
				         0xF5==formatData[ptr+2] &&
				         0xFB==formatData[ptr+3]) ||
				        (0x00==formatData[ptr] &&
				         0x00==formatData[ptr+1] &&
				         0x00==formatData[ptr+2] &&
				         0xFB==formatData[ptr+3])
				         ) // Data Mark
				{
					auto dataPtr=formatData.data()+ptr+4;
					unsigned int sectorSize=128*(1<<N);
					if(0xF7==dataPtr[sectorSize]) // CRC
					{
						std::cout << "Sector Data" << std::endl;
						D77File::D77Disk::D77Sector sector;
						sector.Make(C,H,R,sectorSize);
						for(unsigned int i=0; i<sectorSize; ++i)
						{
							sector.sectorData[i]=dataPtr[i];
						}
						sectors.push_back(sector);
						trackCapacity+=sectorSize;
					}
				}
			}

			auto writeSize=std::max<unsigned int>(state.data.size(),state.dataReadPointer);
			state.data.resize(writeSize);
			auto newDiskMediaType=imgPtr->WriteTrack(diskIdx,drv.trackPos,state.side,state.data);

			if(MEDIA_UNKNOWN!=newDiskMediaType)
			{
				if(drv.mediaType==MEDIA_2D)
				{
					// Don't change media type from 2D to 2DD.
				}
				else if((drv.mediaType==MEDIA_2DD_640KB || drv.mediaType==MEDIA_2DD_720KB) &&
				        (newDiskMediaType==MEDIA_2DD_640KB || newDiskMediaType==MEDIA_2DD_720KB))
				{
					// 2D 640KB,720KB can be re-formatted to 720KB,640KB
					drv.mediaType=newDiskMediaType;
				}
			}
		}
	}
}
bool FM77AVFDC::IndexHole(unsigned long long fm77avTime) const
{
	return (fm77avTime%INDEXHOLE_INTERVAL<INDEXHOLE_DURATION);
}
