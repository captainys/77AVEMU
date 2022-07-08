/* LICENSE>>
Copyright 2020 Soji Yamakawa (CaptainYS, http://www.ysflight.com)

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

<< LICENSE */
#ifndef MISCUTIL_IS_INCLUDED
#define MISCUTIL_IS_INCLUDED
/* { */

#include <vector>
#include <string>

#include "mc6809.h"
#include "memory.h"

namespace miscutil
{
	/*! data must be at least wid*hei bytes long. */
	std::vector <std::string> MakeDump(const unsigned char data[],unsigned int STARTADDR,int wid,int hei,int skip,bool shiftJIS,bool includeASCII);
	std::vector <std::string> MakeMemDump(const MC6809 &cpu,const MemoryAccess &mem,uint32_t addr,unsigned int length,bool shiftJIS);
	std::vector <std::string> MakeMemDump2(const MC6809 &cpu,const MemoryAccess &mem,uint32_t addr,int wid,int hei,int skip,bool shiftJIS,bool includeASCII);

	unsigned char GetByte(const MC6809 &cpu,const MemoryAccess &mem,uint32_t addr);
};

/* } */
#endif
