#include <iostream>
#include <stdio.h>
#include "fm77avtape.h"
#include "cpputil.h"

bool FM77AVTape::Load(std::string fName)
{
	std::ifstream ifp(fName,std::ios::binary);
	if(true==ifp.is_open())
	{
		ifp.seekg(0,ifp.end);
		uint64_t size=ifp.tellg();
		ifp.seekg(0,ifp.beg);
		if(size<16)
		{
			return false;
		}

		ifp.read((char *)header,16);
		data.resize(size-16);
		ifp.read((char *)data.data(),size-16);

		this->fName=fName;
		return true;
	}
	return false;
}
bool FM77AVTape::Save(void) const
{
	return SaveAs(fName);
}
bool FM77AVTape::SaveAs(std::string fName) const
{
	return false; // Will do.
}
void FM77AVTape::Rewind(TapePointer &ptr) const
{
	ptr.dataPtr=0;
	ptr.fm77avTime=0;
	ptr.eot=false;
}
void FM77AVTape::Seek(TapePointer &ptr,unsigned int dataPtr) const
{
	if(data.size()<=dataPtr+1)
	{
		ptr.eot=true;
	}
	else if(ptr.dataPtr!=dataPtr)
	{
		ptr.dataPtr=dataPtr;
		ptr.fm77avTime=0;
		ptr.eot=false;
	}
}
void FM77AVTape::MotorOn(TapePointer &ptr,uint64_t fm77avTime) const
{
	ptr.fm77avTime=fm77avTime; // FM77AV time is always in nanosec.
	if(data.size()<=ptr.dataPtr+1)
	{
		ptr.eot=true;
	}
	else
	{
		ptr.eot=false;
	}
	
}
void FM77AVTape::MoveTapePointer(TapePointer &ptr,uint64_t fm77avTime) const
{
	if(ptr.fm77avTime<fm77avTime)
	{
		while(ptr.dataPtr+1<data.size())
		{
			uint64_t nanoSecForSegment=data[ptr.dataPtr+1];
			nanoSecForSegment*=MICROSEC_PER_T77_ONE*1000;

			uint64_t segmentEndTime=ptr.fm77avTime+nanoSecForSegment;

			if(fm77avTime<segmentEndTime)
			{
				return;
			}
			else
			{
				ptr.fm77avTime=segmentEndTime;
				ptr.dataPtr+=2;
			}
		}
		ptr.eot=true;
	}
}
uint8_t FM77AVTape::GetLevel(TapePointer ptr) const
{
	if(true==ptr.eot || data.size()<=ptr.dataPtr)
	{
		return 0;
	}
	else
	{
		if(data[ptr.dataPtr]<0x40 || (data[ptr.dataPtr]==0x7F && data[ptr.dataPtr+1]==0xFF))
		{
			return 0;
		}
		else
		{
			return 255;
		}
	}
}

////////////////////////////////////////////////////////////

void FM77AVDataRecorder::Reset(void)
{
	state.motor=false;
}
bool FM77AVDataRecorder::LoadT77(std::string fName)
{
	if(true==state.t77.Load(fName))
	{
		state.t77.Rewind(state.ptr);
		return true;
	}
	return false;
}
void FM77AVDataRecorder::MotorOn(uint64_t fm77avTime)
{
	if(true!=state.motor)
	{
		state.t77.MotorOn(state.ptr,fm77avTime);
		state.motor=true;
	}
}
void FM77AVDataRecorder::MotorOff(void)
{
	state.motor=false;
}
void FM77AVDataRecorder::Move(uint64_t fm77avTime)
{
	if(true==state.motor && true!=state.ptr.eot)
	{
		state.t77.MoveTapePointer(state.ptr,fm77avTime);
	}
}
bool FM77AVDataRecorder::Read(void) const
{
	if(true==state.motor && true!=state.ptr.eot)
	{
		return 0x80<state.t77.GetLevel(state.ptr);
	}
	return false;
}
std::vector <std::string> FM77AVDataRecorder::GetStatusText(uint64_t fm77avTime) const
{
	std::vector <std::string> text;

	std::string str;
	str="Motor ";
	if(true==state.motor)
	{
		str+="ON";
	}
	else
	{
		str+="OFF";
	}
	text.push_back(str);

	char cstr[256];
	sprintf(cstr,"Tape Pointer: %llu",state.ptr.dataPtr);
	text.back()+=cstr;
	if(state.ptr.dataPtr+1<state.t77.data.size())
	{
		uint32_t segTime=state.t77.data[state.ptr.dataPtr+1];
		sprintf(cstr,"SegmentTime:%u(%02x)",segTime*FM77AVTape::MICROSEC_PER_T77_ONE,segTime);
		text.push_back(cstr);
	}

	sprintf(cstr,"Time:%llu  SegStartTime:%llu  TimeIntoSeg:%llu",fm77avTime,state.ptr.fm77avTime,fm77avTime-state.ptr.fm77avTime);
	text.push_back(cstr);

	for(int i=0; i<66; i+=2)
	{
		uint64_t I=state.ptr.dataPtr+i;
		if(state.t77.data.size()<=I+1)
		{
			break;
		}

		if(0==i%22)
		{
			text.push_back("");
		}
		text.back()+=cpputil::Ubtox(state.t77.data[I]);
		text.back()+=" ";
		text.back()+=cpputil::Ubtox(state.t77.data[I+1]);
		text.back()+="  ";
	}

	return text;
}
