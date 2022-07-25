/* LICENSE>>
Copyright 2020 Soji Yamakawa (CaptainYS, http://www.ysflight.com)

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

<< LICENSE */
#include "device.h"
#include "fm77av.h"
#include "fm77avdef.h"
#include "fm77avserialport.h"
#include "cpputil.h"



void FM77AVSerialPort::DefaultClient::ClearXMODEM(void)
{
	fileTfrMode=FILETFR_NONE;
}
void FM77AVSerialPort::DefaultClient::SetUpXMODEMtoVM(const std::vector <unsigned char> &data,uint32_t packetLength)
{
	fileTfrMode=(128==packetLength ? FILETFR_XMODEM_TO_VM : FILETFR_XMODEM1024_TO_VM);
	fileTfrPtr=0;
	fileTfrData=data;
	while(0!=(fileTfrData.size()%packetLength))
	{
		fileTfrData.push_back(CODE_EOF);
	}
	XMODEM_EOT_SENT=false;
	toVM.clear();
}
void FM77AVSerialPort::DefaultClient::SetUpXMODEMfromVM(std::string hostRecvFName)
{
	fileTfrMode=FILETFR_XMODEM_FROM_VM;
	fileTfrData.clear();
	fromVM.clear();
	toVMPtr=toVM.size();
	XMODEM_USE_CRC=false;
	toVM.push_back(CODE_NAK);
	this->hostRecvFName=hostRecvFName;
}

void FM77AVSerialPort::DefaultClient::SetUpXMODEMCRCfromVM(std::string hostRecvFName)
{
	fileTfrMode=FILETFR_XMODEM_FROM_VM;
	fileTfrData.clear();
	fromVM.clear();
	toVMPtr=toVM.size();
	XMODEM_USE_CRC=true;
	toVM.push_back('C');
	this->hostRecvFName=hostRecvFName;
}

void FM77AVSerialPort::DefaultClient::XMODEM_TO_VM_TransferNextBlock(uint32_t packetLength)
{
	unsigned int blk=((fileTfrPtr/packetLength)+1)&255;
	toVMPtr=toVM.size();
	if(128==packetLength)
	{
		toVM.push_back(CODE_SOH);
	}
	else
	{
		toVM.push_back(CODE_STX);
	}
	toVM.push_back(blk);
	toVM.push_back(~blk);
	unsigned int checkSum=0;
	for(unsigned int i=0; i<packetLength; ++i)
	{
		if(fileTfrPtr+i<fileTfrData.size())
		{
			toVM.push_back(fileTfrData[fileTfrPtr+i]);
		}
		else
		{
			toVM.push_back(CODE_EOF);
		}
		checkSum+=toVM.back();
	}
	if(true==XMODEM_USE_CRC)
	{
		auto CRC=XMODEM_CRC(fileTfrData.data()+fileTfrPtr,packetLength);
		toVM.push_back((CRC>>8)&0xFF);
		toVM.push_back(CRC&0xFF);
	}
	else
	{
		toVM.push_back(checkSum&0xFF);
	}

	fileTfrPtr+=packetLength;
}

/* virtual */ bool FM77AVSerialPort::DefaultClient::TxRDY(void)
{
	// Always ready to accept a data.
	return true;
}
/* virtual */ void FM77AVSerialPort::DefaultClient::Tx(unsigned char data)
{
	uint32_t packetLength=128;

	// It is Tx from the VM point of view.  It is Rx from the host point of view.
	switch(fileTfrMode)
	{
	case FILETFR_NONE:
	default:
		// Just echo
		// std::cout << "Tx from VM:" << cpputil::Ubtox(data) << std::endl;
		fromVM.push_back(data);
		if(0x0D==data)
		{
			if(true==printRecvText)
			{
				std::cout << "TX from VM:" << recvLine << std::endl;
				recvLine.clear();
			}
		}
		else if(0x0A!=data)
		{
			recvLine.push_back(data);
		}
		break;
	case FILETFR_XMODEM_TO_VM:
	case FILETFR_XMODEM1024_TO_VM:
		packetLength=(FILETFR_XMODEM1024_TO_VM==fileTfrMode ? 1024 : 128);
		// std::cout << "Tx from VM:" << cpputil::Ubtox(data) << std::endl;
		if(0==fileTfrPtr && 'C'==data)
		{
			XMODEM_USE_CRC=true;
			XMODEM_TO_VM_TransferNextBlock(packetLength);
		}
		else if(0==fileTfrPtr && CODE_NAK==data)
		{
			XMODEM_USE_CRC=false;
			XMODEM_TO_VM_TransferNextBlock(packetLength);
		}
		else if(CODE_NAK==data || CODE_ACK==data || 'C'==data) // WINK2.EXP uses 'C' for the first two packets apparently.  Confirmed in WINK2 source XMODEM2.C line 463
		{
			if(CODE_CAN==data)
			{
				// Make it available for the next attempt.
				fileTfrPtr=0;
				XMODEM_EOT_SENT=false;
			}
			else if(CODE_NAK==data && packetLength<=fileTfrPtr)
			{
				fileTfrPtr-=packetLength;
				XMODEM_TO_VM_TransferNextBlock(packetLength);
			}
			else if(CODE_ACK==data || (true==XMODEM_USE_CRC && 'C'==data))
			{
				if(fileTfrData.size()<=fileTfrPtr)
				{
					if(true!=XMODEM_EOT_SENT)
					{
						// toVM.clear();
						toVM.push_back(CODE_EOT);
						XMODEM_EOT_SENT=true;
					}
					else
					{
						fileTfrMode=FILETFR_NONE;
					}
				}
				else
				{
					XMODEM_TO_VM_TransferNextBlock(packetLength);
				}
			}
		}
		break;
	case FILETFR_XMODEM_FROM_VM:
		if(0==fromVM.size())
		{
			if(CODE_SOH==data || CODE_STX==data)
			{
				fromVM.push_back(data);
			}
			else if(CODE_EOT==data)
			{
				toVM.push_back(CODE_ACK);
				fileTfrMode=FILETFR_NONE;
				if(true==cpputil::WriteBinaryFile(hostRecvFName,fileTfrData.size(),fileTfrData.data()))
				{
					std::cout << "Saved:" << hostRecvFName << std::endl;
				}
				else
				{
					std::cout << "!File Write Error!:" << hostRecvFName << std::endl;
				}
			}
		}
		else
		{
			const unsigned int packetSize=(CODE_SOH==fromVM[0] ? 128 : 1024);
			const unsigned int blockSize=(true==XMODEM_USE_CRC ? packetSize+5 : packetSize+4);
			fromVM.push_back(data);
			if(blockSize==fromVM.size())
			{
				if(true==XMODEM_USE_CRC)
				{
					auto CRC=XMODEM_CRC(fromVM.data()+3,packetSize);
					if(((CRC>>8)&0xFF)!=fromVM[blockSize-2] ||
					   ( CRC    &0xFF)!=fromVM[blockSize-1])
					{
						toVM.push_back(CODE_NAK);
						fromVM.clear();
						break;
					}
				}
				else
				{
					unsigned int sum=0;
					for(int i=3; i<3+packetSize; ++i)
					{
						sum+=fromVM[i];
					}
					if(fromVM[1]!=(255&(~fromVM[2])) || fromVM[blockSize-1]!=(sum&255))
					{
						toVM.push_back(CODE_NAK);
						fromVM.clear();
						break;
					}
				}

				for(int i=3; i<3+packetSize; ++i)
				{
					fileTfrData.push_back(fromVM[i]);
				}
				toVM.push_back(CODE_ACK);
				fromVM.clear();
			}
		}
		break;
	}
}
/* virtual */ void FM77AVSerialPort::DefaultClient::SetStopBits(unsigned char stopBits)
{
}
/* virtual */ void FM77AVSerialPort::DefaultClient::SetParity(bool enabled,bool evenParity)
{
}
/* virtual */ void FM77AVSerialPort::DefaultClient::SetDataLength(unsigned char bitsPerData)
{
}
/* virtual */ void FM77AVSerialPort::DefaultClient::SetBaudRate(unsigned int baudRate)
{
}
/* virtual */ void FM77AVSerialPort::DefaultClient::Command(bool RTS,bool DTR,bool BREAK)
{
}
/* virtual */ bool FM77AVSerialPort::DefaultClient::RxRDY(void)
{
	return (toVMPtr<toVM.size());
}
/* virtual */ unsigned char FM77AVSerialPort::DefaultClient::Rx(void)
{
	if(toVMPtr<toVM.size())
	{
		auto data=toVM[toVMPtr++];
		if(toVM.size()<=toVMPtr)
		{
			toVMPtr=0;
			toVM.clear();
		}
		return data;
	}
	return 0;
}
/* virtual */ unsigned char FM77AVSerialPort::DefaultClient::PeekRx(void) const
{
	if(toVMPtr<toVM.size())
	{
		return toVM[toVMPtr];
	}
	return 0;
}
/* virtual */ bool FM77AVSerialPort::DefaultClient::DSR(void)
{
	return true; // Always ready
}

/* References:
     http://web.mit.edu/6.115/www/amulet/xmodem.htm
     http://mdfs.net/Info/Comp/Comms/CRC16.htm
*/
unsigned int FM77AVSerialPort::DefaultClient::XMODEM_CRC(unsigned char ptr[],unsigned int len)
{
	const unsigned int magicNumber=0x1021;
	unsigned int crc=0;
	for(unsigned int i=0; i<len; ++i)
	{
		unsigned int dat=ptr[i];
		dat<<=8;
		crc=crc^dat;
		for(unsigned int j=0; j<8; ++j)
		{
			if(crc&0x8000)
			{
				crc<<=1;
				crc^=magicNumber;
			}
			else
			{
				crc<<=1;
			}
		}
	}
	return crc;
}

////////////////////////////////////////////////////////////

FM77AVSerialPort::FM77AVSerialPort(class FM77AV *fm77avPtr) : Device(fm77avPtr)
{
	for(auto &e : state.enabled)
	{
		e=false;
	}
	state.COM[0].clientPtr=&cli0;
}
/* virtual */ void FM77AVSerialPort::PowerOn(void)
{
	Reset();
}
/* virtual */ void FM77AVSerialPort::Reset(void)
{
	for(auto &com : state.COM)
	{
		com.Reset();
	}
}
/* virtual */ uint32_t FM77AVSerialPort::SerializeVersion(void) const
{
	return 0;
}
/* virtual */ void FM77AVSerialPort::SpecificSerialize(std::vector <unsigned char> &data,std::string stateFName) const
{
}
/* virtual */ bool FM77AVSerialPort::SpecificDeserialize(const unsigned char *&data,std::string stateFName,uint32_t version)
{
	return true;
}
