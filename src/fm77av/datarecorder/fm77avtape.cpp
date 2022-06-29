#include <iostream>
#include "fm77avtape.h"

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
	ptr.dataPtr=16;
	ptr.microSecLeft=0;
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
		ptr.microSecLeft=0;
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
		ptr.microSecLeft=data[ptr.dataPtr+1]*MICROSEC_PER_T77_ONE;
		ptr.eot=false;
	}
	
}
void FM77AVTape::MoveTapePointer(TapePointer &ptr,uint64_t fm77avTime) const
{
	if(ptr.fm77avTime<fm77avTime)
	{
		auto nanoSecPassed=fm77avTime-ptr.fm77avTime;
		nanoSecPassed/=1000;
		unsigned int microSecPassed=nanoSecPassed;

		if(microSecPassed<ptr.microSecLeft)
		{
			ptr.microSecLeft-=microSecPassed;
			return;
		}

		ptr.fm77avTime=fm77avTime;

		microSecPassed-=ptr.microSecLeft;
		ptr.dataPtr+=2;

		while(ptr.dataPtr+1<data.size())
		{
			unsigned int microSecForSegment=data[ptr.dataPtr+1];
			microSecForSegment*=MICROSEC_PER_T77_ONE;
			if(microSecPassed<microSecForSegment)
			{
				ptr.microSecLeft=microSecForSegment-microSecPassed;
				return;
			}
			else
			{
				microSecPassed-=microSecForSegment;
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
		if(data[ptr.dataPtr]<0x40 || (data[ptr.dataPtr]==0x7F && data[ptr.dataPtr]==0xFF))
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
