#ifndef MC6809UTIL_IS_INCLUDED
#define MC6809UTIL_IS_INCLUDED
/* { */

#include <stdint.h>

namespace mc6809util
{
inline uint16_t FetchWord(uint8_t *ptr)
{
	return (ptr[0]<<8)|ptr[1];
}
inline uint16_t FetchWord(uint8_t firstByte,uint8_t secondByte)
{
	return (firstByte<<8)|secondByte;
}
inline void StoreWord(uint8_t *ptr,uint16_t data)
{
	ptr[0]=(data>>8);
	ptr[1]=(data&0xFF);
}
inline void StoreWordSigned(uint8_t *ptr,int16_t data)
{
	ptr[0]=(data>>8);
	ptr[1]=(data&0xFF);
}
inline void StoreWord(uint8_t &firstBytePtr,uint8_t &secondBytePtr,uint16_t data)
{
	firstBytePtr=(data>>8);
	secondBytePtr=(data&0xFF);
}

};

/* } */
#endif
