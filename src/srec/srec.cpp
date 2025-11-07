#include <stdio.h>
#include <fstream>
#include <iostream>
#include "srec.h"

uint8_t SREC::HexDigitToI(char c)
{
	if('0'<=c && c<='9')
	{
		return c-'0';
	}
	c|=0x20;
	if('a'<=c && c<='f')
	{
		return 10+c-'a';
	}
	return 0x7f;
}
uint32_t SREC::Xtoi(const char *str)
{
	uint32_t i=0;
	while(0!=*str)
	{
		auto digit=HexDigitToI(*str);
		if(0xf<digit)
		{
			break;
		}
		i<<=4;
		i|=digit;
		++str;
	}
	return i;
}

void SREC::Clear(void)
{
	blocks.clear();
}

bool SREC::Open(std::string fileName)
{
	std::vector <uint8_t> bin;
	std::ifstream ifp(fileName,std::ios::binary);
	if(true==ifp.is_open())
	{
		blocks.clear();

		ifp.seekg(0,std::ios::end);
		size_t s=ifp.tellg();
		ifp.seekg(0,std::ios::beg);

		bin.resize(s);
		ifp.read((char *)bin.data(),s);


		for(size_t i=0; i<bin.size(); )
		{
			if(0x0D==bin[i] || 0x0A==bin[i])
			{
				++i;
				continue;
			}
			if(bin.size()<=i+7)
			{
				std::cout << "Record too short\n";
				return false;
			}
			if('S'==bin[i++])
			{
				Block blk;
				blk.type=bin[i++];
				char byteCountTxt[3]={bin[i],bin[i+1],0};
				i+=2;

				blk.byteCount=Xtoi(byteCountTxt);

				uint32_t addrSize=0;
				switch(blk.type)
				{
				case '0':
				case '1':
				case '5':
				case '9':
					// 16-bit address field
					addrSize=4; // 4 digits.
					break;
				case '2':
				case '6':
				case '8':
					// 24-bit address field
					addrSize=6; // 6 digits.
					break;
				case '3':
				case '7':
					// 32-bit address field
					addrSize=8; // 8 digits.
					break;
				default:
					std::cout << "Undefined record type\n";
					return false;
				}

				char addrTxt[9]={0,0,0,0,0,0,0,0,0};
				for(int j=0; j<addrSize; ++j)
				{
					addrTxt[j]=bin[i++];
				}

				blk.address=Xtoi(addrTxt);
				auto dataByteCount=blk.byteCount-(addrSize/2)-1;
				for(int j=0; j<dataByteCount; ++j)
				{
					char byteTxt[3]={0,0,0};
					byteTxt[0]=bin[i++];
					byteTxt[1]=bin[i++];
					auto byteData=Xtoi(byteTxt);
					blk.data.push_back(byteData);
				}

				addrTxt[0]=bin[i++];
				addrTxt[1]=bin[i++];
				addrTxt[2]=0;
				blk.checkSum=Xtoi(addrTxt);

				blocks.push_back((Block &&)blk);
			}
			else
			{
				std::cout << "Record must start with 'S'\n";
				return false;
			}
		}

		return true;
	}
	return false;
}

uint8_t SREC::Block::RecalculateCheckSum(void) const
{
	unsigned int checkSum=0;
	for(auto bc=byteCount; 0!=bc; bc>>=8) // SREC spec implies the byte count is 8-bit, but is it true?
	{
		checkSum+=bc;
	}
	auto addr=this->address;
	while(0!=addr)
	{
		checkSum+=(addr&0xff);
		addr>>=8;
	}
	for(auto d : this->data)
	{
		checkSum+=d;
	}

	checkSum&=0xFF;
	checkSum=0xFF-checkSum;

	return checkSum;
}

bool SREC::VerifyCheckSum(void) const
{
	for(auto &b : blocks)
	{
		auto checkSum=b.RecalculateCheckSum();

		printf("%x %x\n",b.checkSum,checkSum);
		if(checkSum!=b.checkSum && checkSum+3!=b.checkSum) // asm6809 calculates checksum wrong.  Need +3.
		{
			std::cout << "Check Sum Error\n";
			return false;
		}
	}
	return true;
}

std::vector <uint8_t> SREC::GetData(void) const
{
	std::vector <uint8_t> data;
	for(auto &b : blocks)
	{
		if('1'==b.type || '2'==b.type || '3'==b.type) // Assume data bytes are tightly packed.
		{
			data.insert(data.end(),b.data.begin(),b.data.end());
		}
	}
	return data;
}

uint32_t SREC::GetFirstAddress(void) const
{
	for(auto &b : blocks)
	{
		if('1'==b.type || '2'==b.type || '3'==b.type) // Assume data bytes are tightly packed.
		{
			return b.address;
		}
	}
	return ~0;
}


void SREC::Make(uint32_t addr,const std::vector <uint8_t> &data)
{
	Clear();
	if(addr<0x10000) // Type '1'
	{
		const size_t blockSize=0x20;
		char type='1';
		if(0x10000<=addr)
		{
			type='2';
		}
		else if(0x1000000<=addr)
		{
			type='3';
		}
		for(size_t i=0; i<data.size(); i+=blockSize)
		{
			Block b;
			b.type=type;
			b.byteCount=std::min(data.size()-i,blockSize);
			b.address=addr;

			for(size_t j=0; j<blockSize; ++j)
			{
				b.data.push_back(data[i+j]);
			}

			addr+=blockSize;

			blocks.push_back(b);
		}
	}
}

void SREC::Make(uint32_t startAddr,const std::vector <uint8_t> &data,uint32_t execAddr)
{
	Make(startAddr,data);

	Block b;
	b.type='9';
	if(0x10000<=startAddr)
	{
		b.type='8';
	}
	else if(0x1000000<=startAddr)
	{
		b.type='7';
	}
	b.address=execAddr;
	blocks.push_back(b);
}
