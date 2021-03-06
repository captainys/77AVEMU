/* LICENSE>>
Copyright 2020 Soji Yamakawa (CaptainYS, http://www.ysflight.com)

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

<< LICENSE */
#ifndef SERIALPORT_IS_INCLUDED
#define SERIALPORT_IS_INCLUDED
/* { */

#include <vector>
#include <string>
#include "device.h"
#include "fm77avdef.h"
#include "i8251.h"

class FM77AVSerialPort : public Device
{
public:
	virtual const char *DeviceName(void) const{return "SERIALPORT";}

	enum
	{
		CODE_SOH=0x01,
		CODE_STX=0x02,
		CODE_EOT=0x04,
		CODE_EOF=0x05, // Is it right?
		CODE_ACK=0x06,
		CODE_NAK=0x15,
		CODE_CAN=0x18,
	};

	enum
	{
		INTENABLE_TXRDY=0x01,
		INTENABLE_RXRDY=0x02,
		INTENABLE_SYNDET=0x04,
		INTENABLE_CS=0x08,
		INTENABLE_CI=0x10,
	};

	class DefaultClient : public i8251::Client
	{
	public:
		bool printRecvText=false;

		long long int toVMPtr=0;
		std::vector <unsigned char> fromVM,toVM;

		enum
		{
			FILETFR_NONE,
			FILETFR_XMODEM_TO_VM,
			FILETFR_XMODEM1024_TO_VM,
			FILETFR_XMODEM_FROM_VM,
		};
		unsigned int fileTfrMode=FILETFR_NONE;
		unsigned int fileTfrPtr=0;
		std::vector <unsigned char> fileTfrData;
		std::string recvLine;
		bool XMODEM_EOT_SENT=false;
		bool XMODEM_USE_CRC=false;
		std::string hostRecvFName;

		void ClearXMODEM(void);
		void SetUpXMODEMtoVM(const std::vector <unsigned char> &data,uint32_t packetLength);
		void SetUpXMODEMfromVM(std::string hostRecvFName);
		void SetUpXMODEMCRCfromVM(std::string hostRecvFName);

		void XMODEM_TO_VM_TransferNextBlock(uint32_t packetLength);
		unsigned int XMODEM_CRC(unsigned char ptr[],unsigned int len);

		virtual bool TxRDY(void);
		virtual void Tx(unsigned char data);
		virtual void SetStopBits(unsigned char stopBits);
		virtual void SetParity(bool enabled,bool evenParity);
		virtual void SetDataLength(unsigned char bitsPerData);
		virtual void SetBaudRate(unsigned int baudRate);
		virtual void Command(bool RTS,bool DTR,bool BREAK);
		virtual bool RxRDY(void);
		virtual unsigned char Rx(void);
		virtual unsigned char PeekRx(void) const;
		virtual bool DSR(void);
	};


	class State
	{
	public:
		bool enabled[FM7_MAX_NUM_COMPORTS];
		i8251 COM[FM7_MAX_NUM_COMPORTS];
	};
	State state;
	DefaultClient cli0;

	FM77AVSerialPort(class FM77AV *fm77avPtr);

	virtual void PowerOn(void);
	virtual void Reset(void);

	virtual uint32_t SerializeVersion(void) const;
	virtual void SpecificSerialize(std::vector <unsigned char> &data,std::string stateFName) const;
	virtual bool SpecificDeserialize(const unsigned char *&data,std::string stateFName,uint32_t version);
};

/* } */
#endif
