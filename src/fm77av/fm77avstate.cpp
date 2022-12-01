#include <fstream>

#include "fm77av.h"

// Disk-image (Disc-image) search rule:
// (1) Hard-disk image is not auto-mounted.
// (2) Try using the filename stored in the state file as is.
// (3) Try state path+relative path
// (4) Try image search path+file name
// (5) Try state path+file name
// (6) If floppy-disk image, use image stored in the state file.


bool FM77AV::SaveState(std::string fName) const
{
	std::ofstream ofp(fName,std::ios::binary);
	if(true==ofp.is_open())
	{
		std::vector <const Device *> allDevices;
		allDevices.push_back(this);
		allDevices.push_back(&physMem);
		allDevices.push_back(&mainMemAcc);
		allDevices.push_back(&mainCPU);
		allDevices.push_back(&subMemAcc);
		allDevices.push_back(&subCPU);
		allDevices.push_back(&crtc);
		allDevices.push_back(&fdc);
		allDevices.push_back(&keyboard);
		allDevices.push_back(&dataRecorder);
		allDevices.push_back(&sound);
		allDevices.push_back(&gameport);
		allDevices.push_back(&serialport);
		allDevices.push_back(&dmac);

		for(auto devPtr : allDevices)
		{
			auto dat=devPtr->Serialize(fName);
			uint32_t len=(uint32_t)dat.size();

			ofp.write((char *)&len,4);
			ofp.write((char *)dat.data(),len);
		}
		return true;
	}
	return false;
}
bool FM77AV::LoadState(std::string fName,class Outside_World &outsideWorld)
{
	std::ifstream ifp(fName,std::ios::binary);
	if(true==ifp.is_open())
	{
		std::vector <Device *> allDevices;
		allDevices.push_back(this);
		allDevices.push_back(&physMem);
		allDevices.push_back(&mainMemAcc);
		allDevices.push_back(&mainCPU);
		allDevices.push_back(&subMemAcc);
		allDevices.push_back(&subCPU);
		allDevices.push_back(&crtc);
		allDevices.push_back(&fdc);
		allDevices.push_back(&keyboard);
		allDevices.push_back(&dataRecorder);
		allDevices.push_back(&sound);
		allDevices.push_back(&gameport);
		allDevices.push_back(&serialport);
		allDevices.push_back(&dmac);

		while(true!=ifp.eof())
		{
			uint32_t len=0;
			ifp.read((char *)&len,4);
			if(0==len)
			{
				break;
			}

			std::vector <unsigned char> data;
			data.resize(len);
			ifp.read((char *)data.data(),len);

			bool successful=false;
			for(auto devPtr : allDevices)
			{
				if(true==devPtr->Deserialize(data,fName))
				{
					successful=true;
					break;
				}
			}

			if(true!=successful)
			{
				std::string readDeviceId=(const char *)data.data();
				std::cout << "Failed " << readDeviceId << std::endl;
				return false;
			}
		}

		// I was first running a loop for unscheduling all devices,
		// and then a loop for re-scheduling devices that has non-null scheduleTime
		// only to realize that UnscheduleDeviceCallBack was nullifying the scheduleTime.

		for(auto devPtr : allDevices)
		{
			if(TIME_NO_SCHEDULE!=devPtr->commonState.scheduleTime)
			{
				ScheduleDeviceCallBack(*devPtr,devPtr->commonState.scheduleTime);
			}
			else
			{
				UnscheduleDeviceCallBack(*devPtr);
			}
		}

		var.justLoadedState=true;

		return true;
	}
	return false;
}

/* virtual */ uint32_t FM77AV::SerializeVersion(void) const
{
	// Version 1
	//   subSysHaltSoon flag.
	return 1;
}

/* virtual */ void FM77AV::SpecificSerialize(std::vector <unsigned char> &data,std::string stateFName) const
{
	PushUint32(data,state.machineType);
	PushUint32(data,state.keyboardIRQHandler);
	PushUint64(data,state.fm77avTime);
	PushUint64(data,state.nextDevicePollingTime);
	PushUint64(data,state.nextFastDevicePollingTime);
	PushUint64(data,state.nextRenderingTime);
	PushUint64(data,state.next20msTimer);
	PushUint64(data,state.next2msTimer);
	PushUint64(data,state.nextSecondInfm77avTime);

	PushUint64(data,0); // Sub-CPU temporary ready time is no longer used.
	PushUint64(data,0);

	PushInt32(data,state.timeBalance);

	PushInt64(data,state.timeDeficit);

	PushBool(data,state.subSysBusy);
	PushBool(data,state.subSysHalt);
	PushBool(data,state.subNMIMask);

	PushBool(data,state.CRTCHaltsSubCPU);

	PushUint16(data,state.main.irqEnableBits);
	PushUint16(data,state.main.irqSource);
	PushUint16(data,state.main.firqSource);
	PushUint16(data,state.sub.irqEnableBits);
	PushUint16(data,state.sub.irqSource);
	PushUint16(data,state.sub.firqSource);

	// Version 1
	PushBool(data,state.subSysHaltSoon);
}
/* virtual */ bool FM77AV::SpecificDeserialize(const unsigned char *&data,std::string stateFName,uint32_t version)
{
	state.machineType=ReadUint32(data);
	state.keyboardIRQHandler=ReadUint32(data);
	state.fm77avTime=ReadUint64(data);
	state.nextDevicePollingTime=ReadUint64(data);
	state.nextFastDevicePollingTime=ReadUint64(data);
	state.nextRenderingTime=ReadUint64(data);
	state.next20msTimer=ReadUint64(data);
	state.next2msTimer=ReadUint64(data);
	state.nextSecondInfm77avTime=ReadUint64(data);

	auto subCPUTemporaryReadyTimeIsNoLongerUsed=ReadUint64(data);
	auto subCPUTemporaryReadyTimeDeltaIsNoLongerUsed=ReadUint64(data);

	state.timeBalance=ReadInt32(data);

	state.timeDeficit=ReadInt64(data);

	state.subSysBusy=ReadBool(data);
	state.subSysHalt=ReadBool(data);
	state.subNMIMask=ReadBool(data);

	state.CRTCHaltsSubCPU=ReadBool(data);

	state.main.irqEnableBits=ReadUint16(data);
	state.main.irqSource=ReadUint16(data);
	state.main.firqSource=ReadUint16(data);
	state.sub.irqEnableBits=ReadUint16(data);
	state.sub.irqSource=ReadUint16(data);
	state.sub.firqSource=ReadUint16(data);

	VMBase::vmAbort=false;

	if(1<=version)
	{
		state.subSysHaltSoon=ReadBool(data);
	}
	else
	{
		state.subSysHaltSoon=false;
	}

	return true;
}
