#include <iostream>
#include <algorithm>
#include <stdio.h>
#include "fm77avtape.h"
#include "cpputil.h"
#include "outside_world.h"

bool FM77AVTape::Load(std::string fName)
{
	std::ifstream ifp(fName,std::ios::binary);
	if(true==ifp.is_open())
	{
		ifp.seekg(0,ifp.end);
		uint64_t size=ifp.tellg();
		ifp.seekg(0,ifp.beg);
		if(size<18)
		{
			return false;
		}

		ifp.read((char *)header,18);
		data.resize(size-18);
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
	std::ofstream ofp(fName,std::ios::binary);
	if(true==ofp.is_open())
	{
		char header[18]={"XM7 TAPE IMAGE 0"};
		header[16]=0x00; // Wasn't these two bytes intended for storing information like write-protect?
		header[17]=0x00;

		ofp.write(header,18);

		ofp.write((char *)data.data(),data.size());

		modified=false;

		return true;
	}
	return false; // Will do.
}
void FM77AVTape::Eject(void)
{
	fName="";
	data.clear();
	writeProtect=false;
	modified=false;
	lastModifiedTime=0;
}
void FM77AVTape::Rewind(TapePointer &ptr) const
{
	ptr.dataPtr=0;
	ptr.fm77avTime=0;
	ptr.eot=false;
}
void FM77AVTape::Dniwer(TapePointer &ptr) const
{
	ptr.dataPtr=data.size();
	ptr.fm77avTime=0;
	ptr.eot=true;
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

std::vector <FM77AVTape::TapeFile> FM77AVTape::Files(void) const
{
	std::vector <uint8_t> rawT77;
	for(int i=0; i<16; ++i)
	{
		rawT77.push_back(0);
	}
	rawT77.insert(rawT77.end(),data.begin(),data.end());

	T77Decoder t77Dec;
	t77Dec.DumpT77((std::vector <unsigned char> &&)rawT77);
	t77Dec.Decode();

	std::vector <TapeFile> files;
	for(int i=0; i<t77Dec.filePtr.size() && i<t77Dec.fileDump.size(); ++i)
	{
		TapeFile tf;
		tf.fName=t77Dec.GetDumpFileName(t77Dec.fileDump[i]);
		auto fmDat=t77Dec.DumpToFMFormat(t77Dec.fileDump[i]);
		tf.fType=FM7File::DecodeFMHeaderFileType(fmDat[10],fmDat[11]);
		tf.ptr=t77Dec.filePtr[i]-16;
		files.push_back(tf);
	}
	return files;
}

////////////////////////////////////////////////////////////

void FM77AVDataRecorder::TapePointerPair::Rewind(void)
{
	t77.Rewind(ptr);
}
void FM77AVDataRecorder::TapePointerPair::Dniwer(void)
{
	t77.Dniwer(ptr);
}

////////////////////////////////////////////////////////////

void FM77AVDataRecorder::Reset(void)
{
	Device::Reset();
	state.motor=false;
}
bool FM77AVDataRecorder::LoadT77(std::string fName)
{
	if(true==state.primary.t77.Load(fName))
	{
		state.primary.Rewind();
		return true;
	}
	return false;
}
bool FM77AVDataRecorder::LoadAutoSaveT77(std::string fName)
{
	if(true==state.toSave.t77.Load(fName))
	{
		state.toSave.Rewind();
		return true;
	}
	return false;
}
void FM77AVDataRecorder::Eject(void)
{
	SaveModifiedTapeImages();
	state.primary.Rewind();
	state.primary.t77.Eject();
}
void FM77AVDataRecorder::MotorOn(uint64_t fm77avTime)
{
	if(true!=state.motor)
	{
		state.primary.t77.MotorOn(state.primary.ptr,fm77avTime);
		state.motor=true;
		state.motorOnTime=fm77avTime;
		state.lastBitFlipTime=fm77avTime;
	}
}
void FM77AVDataRecorder::MotorOff(uint64_t fm77avTime)
{
	if(true==state.motor)
	{
		if(true==state.recButton && true!=state.primary.t77.writeProtect)
		{
			WriteBit(state.primary,fm77avTime);
		}
		else if(state.motorOnTime!=state.lastBitFlipTime)
		{
			// Probably the user forgot to push REC button, or intended to write to the auto-save tape.
			WriteBit(state.toSave,fm77avTime);
			std::cout << "Written to Auto-Save TAPE image." << std::endl;
			std::cout << "Tape Pointer=" << state.toSave.ptr.dataPtr << std::endl;
			goto RETURN;
		}
	}
	std::cout << "Motor Off Tape Pointer=" << state.primary.ptr.dataPtr << std::endl;

RETURN:
	state.motor=false;
}
void FM77AVDataRecorder::Move(uint64_t fm77avTime)
{
	if(true==state.motor && true!=state.primary.ptr.eot)
	{
		state.primary.t77.MoveTapePointer(state.primary.ptr,fm77avTime);
		outside_world->indicatedTapePosition=state.primary.ptr.dataPtr;
	}
}
bool FM77AVDataRecorder::Read(void) const
{
	if(true==state.motor && true!=state.primary.ptr.eot)
	{
		return 0x80<state.primary.t77.GetLevel(state.primary.ptr);
	}
	return false;
}
void FM77AVDataRecorder::Rewind(void)
{
	state.primary.Rewind();
}
void FM77AVDataRecorder::WriteBit(TapePointerPair &tape,uint64_t fm77avTime)
{
	if(true!=tape.t77.writeProtect)
	{
		uint8_t out[2];
		uint32_t sinceLastFlip=fm77avTime-state.lastBitFlipTime;
		if(state.motorOnTime==state.lastBitFlipTime && HEAD_SILENT_TIME<=sinceLastFlip) // First flip since the last motor on
		{
			out[0]=0x7F;
			out[1]=0xFF;
		}
		else
		{
			if(true==state.writeData)
			{
				out[0]=0x80;
			}
			else
			{
				out[0]=0x00;
			}

			uint32_t t77Count=sinceLastFlip/(FM77AVTape::MICROSEC_PER_T77_ONE*1000);
			out[1]=std::min<uint32_t>(t77Count,0xFF);
		}
		if(tape.ptr.dataPtr+1<tape.t77.data.size())
		{
			tape.t77.data[tape.ptr.dataPtr++]=out[0];
			tape.t77.data[tape.ptr.dataPtr++]=out[1];
		}
		else
		{
			tape.t77.data.push_back(out[0]);
			tape.t77.data.push_back(out[1]);
			tape.ptr.dataPtr=tape.t77.data.size();
		}
		tape.ptr.fm77avTime=fm77avTime;
		tape.t77.modified=true;
		tape.t77.lastModifiedTime=fm77avTime;
		outside_world->indicatedTapePosition=tape.ptr.dataPtr;
	}
}

void FM77AVDataRecorder::WriteFD00(uint64_t fm77avTime,uint8_t data)
{
	if(0!=(data&2)) // MOTOR ON
	{
		MotorOn(fm77avTime);
	}
	else
	{
		MotorOff(fm77avTime);
	}
	bool nextWriteData=(0!=(data&1));
	if(nextWriteData!=state.writeData)
	{
		if(true!=state.primary.t77.writeProtect && true==state.recButton)
		{
			WriteBit(state.primary,fm77avTime);
		}
		else
		{
			WriteBit(state.toSave,fm77avTime);
		}
		state.lastBitFlipTime=fm77avTime;
	}
	state.writeData=nextWriteData;
}

void FM77AVDataRecorder::SaveModifiedTapeImagesAfterOneSecond(uint64_t fm77avTime)
{
	if(true==state.primary.t77.modified && state.primary.t77.lastModifiedTime+1000000000<=fm77avTime)
	{
		if(true!=state.primary.t77.Save())
		{
			std::cout << "Auto Save T77 (primary)failed." << std::endl;
			state.primary.t77.modified=false;
		}
	}
	if(true==state.toSave.t77.modified && state.toSave.t77.lastModifiedTime+1000000000<=fm77avTime)
	{
		if(true!=state.toSave.t77.Save())
		{
			std::cout << "Auto Save T77 (save-only) failed." << std::endl;
			state.toSave.t77.modified=false;
		}
	}
}
void FM77AVDataRecorder::SaveModifiedTapeImages(void)
{
	if(true==state.primary.t77.modified)
	{
		if(true!=state.primary.t77.Save())
		{
			std::cout << "Auto Save T77 (primary)failed." << std::endl;
			state.primary.t77.modified=false;
		}
	}
	if(true==state.toSave.t77.modified)
	{
		if(true!=state.toSave.t77.Save())
		{
			std::cout << "Auto Save T77 (save-only) failed." << std::endl;
			state.toSave.t77.modified=false;
		}
	}
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
	sprintf(cstr,"Tape Pointer: %llu",state.primary.ptr.dataPtr);
	text.back()+=cstr;
	if(state.primary.ptr.dataPtr+1<state.primary.t77.data.size())
	{
		uint32_t segTime=state.primary.t77.data[state.primary.ptr.dataPtr+1];
		sprintf(cstr,"SegmentTime:%u(%02x)",segTime*FM77AVTape::MICROSEC_PER_T77_ONE,segTime);
		text.push_back(cstr);
	}

	sprintf(cstr,"Time:%llu  SegStartTime:%llu  TimeIntoSeg:%llu",fm77avTime,state.primary.ptr.fm77avTime,fm77avTime-state.primary.ptr.fm77avTime);
	text.push_back(cstr);

	for(int i=0; i<66; i+=2)
	{
		uint64_t I=state.primary.ptr.dataPtr+i;
		if(state.primary.t77.data.size()<=I+1)
		{
			break;
		}

		if(0==i%22)
		{
			text.push_back("");
		}
		text.back()+=cpputil::Ubtox(state.primary.t77.data[I]);
		text.back()+=" ";
		text.back()+=cpputil::Ubtox(state.primary.t77.data[I+1]);
		text.back()+="  ";
	}

	return text;
}

/* virtual */ uint32_t FM77AVDataRecorder::SerializeVersion(void) const
{
	return 0;
}
/* virtual */ void FM77AVDataRecorder::SpecificSerialize(std::vector <unsigned char> &data,std::string stateFName) const
{
	for(int i=0; i<2; ++i)
	{
		const TapePointerPair *tpp=(0==i ? &state.primary : &state.toSave);

		PushString(data,tpp->t77.fName);
		PushBool(data,tpp->t77.writeProtect);

		PushUint32(data,tpp->ptr.dataPtr);
		PushUint64(data,tpp->ptr.fm77avTime);
		PushBool(data,tpp->ptr.eot);
	}

	PushBool(data,state.motor);
	PushBool(data,state.recButton);
	PushBool(data,state.writeData);

	PushUint64(data,state.motorOnTime);
	PushUint64(data,state.lastBitFlipTime);
}
/* virtual */ bool FM77AVDataRecorder::SpecificDeserialize(const unsigned char *&data,std::string stateFName,uint32_t version)
{
	for(int i=0; i<2; ++i)
	{
		TapePointerPair *tpp=(0==i ? &state.primary : &state.toSave);

		tpp->t77.fName=ReadString(data);
		tpp->t77.writeProtect=ReadBool(data);

		tpp->ptr.dataPtr=ReadUint32(data);
		tpp->ptr.fm77avTime=ReadUint64(data);
		tpp->ptr.eot=ReadBool(data);

		if(true!=tpp->t77.Load(tpp->t77.fName))
		{
			tpp->t77.Eject();
			tpp->ptr.eot=true;
			tpp->ptr.fm77avTime=0;
		}
	}

	state.motor=ReadBool(data);
	state.recButton=ReadBool(data);
	state.writeData=ReadBool(data);

	state.motorOnTime=ReadUint64(data);
	state.lastBitFlipTime=ReadUint64(data);

	return true;
}
