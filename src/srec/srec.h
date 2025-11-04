#ifndef SREC_LIB_IS_INCLUDED
#define SREC_LIB_IS_INCLUDED

#include <vector>
#include <string>
#include <stdint.h>

// Can be made by:
//  asm6809 source.asm -o output.srec --srec

class SREC
{
public:
	class Block
	{
	public:
		char type;
		size_t byteCount;
		size_t address;
		std::vector <uint8_t> data;
		uint8_t checkSum;
	};

	std::vector <Block> blocks;

	bool Open(std::string fileName);
	static uint8_t HexDigitToI(char c);
	static uint32_t Xtoi(const char str[]);

	bool VerifyCheckSum(void) const;

	std::vector <uint8_t> GetData(void) const;
	uint32_t GetFirstAddres(void) const;
};

#endif
