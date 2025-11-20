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

		uint8_t RecalculateCheckSum(void) const;
	};

	void Clear(void);

	std::vector <Block> blocks;

	bool Open(std::string fileName);
	static uint8_t HexDigitToI(char c);
	static uint32_t Xtoi(const char str[]);
	static std::string ToHexString32(uint32_t i); // Always returns 8 hex digits.
	static void ToHexString8(char data[2],uint32_t i);

	bool VerifyCheckSum(void) const;

	uint8_t RecalculateCheckSum(void) const;

	std::vector<Block> GetBlocks(void) const;

	/*! This function only looks at the address of the first block.
	    All bytes are concatenated even if blocks are not continuous.
	*/
	std::vector <uint8_t> GetData(void) const;

	uint32_t GetFirstAddress(void) const;

	void Make(uint32_t startAddr,const std::vector <uint8_t> &data);
	void Make(uint32_t startAddr,const std::vector <uint8_t> &data,uint32_t execAddr);

	std::vector <char> Encode(void) const;
};

#endif
