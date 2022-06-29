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
	ptr.milliSecLeft=0;
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
		ptr.milliSecLeft=0;
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
		ptr.milliSecLeft=data[ptr.dataPtr+1];
		ptr.eot=false;
	}
	
}
void FM77AVTape::MoveTapePointer(TapePointer &ptr,uint64_t fm77avTime) const
{
	if(ptr.fm77avTime<fm77avTime)
	{
		auto nanoSecPassed=fm77avTime-ptr.fm77avTime;
		nanoSecPassed/=1000000;
		unsigned int milliSecPassed=nanoSecPassed;

		ptr.fm77avTime=fm77avTime;

		if(milliSecPassed<ptr.milliSecLeft)
		{
			ptr.milliSecLeft-=milliSecPassed;
			return;
		}

		milliSecPassed-=ptr.milliSecLeft;
		ptr.dataPtr+=2;

		for(;;)
		{
			if(data.size()<=ptr.dataPtr+1)
			{
				ptr.eot=true;
				return;
			}

			if(milliSecPassed<data[ptr.dataPtr+1])
			{
				ptr.milliSecLeft=data[ptr.dataPtr+1]-milliSecPassed;
				break;
			}
			else
			{
				milliSecPassed-=data[ptr.dataPtr+1];
				ptr.dataPtr+=2;
			}
		}
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
		if(data[ptr.dataPtr]<0x40)
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
}
