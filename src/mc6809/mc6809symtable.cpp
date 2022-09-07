/* LICENSE>>
Copyright 2020 Soji Yamakawa (CaptainYS, http://www.ysflight.com)

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

<< LICENSE */
#include <sstream>
#include <iostream>

#include "mc6809symtable.h"
#include "cpputil.h"



MC6809Symbol::MC6809Symbol()
{
	CleanUp();
}
void MC6809Symbol::CleanUp(void)
{
	temporary=false;
	immIsIOAddr=false;
	immIsSymbol=false;
	immIsASCII=false;
	offsetIsSymbol=false;
	symType=SYM_ANY;
	label="";
	inLineComment="";
	param="";
	info.clear();
	rawDataBytes=0;
}
std::string MC6809Symbol::Format(bool returnType,bool label,bool param) const
{
	std::string str;

	if(0<imported.size())
	{
		str=imported;
		str.push_back(' ');
	}

	if(true==label)
	{
		str+=this->label;
		if(SYM_JUMP_DESTINATION==symType)
		{
			str+=':';
		}
	}
	if((SYM_PROCEDURE==symType || SYM_ANY==symType) && true==param)
	{
		str+="(";
		str+=this->param;
		str+=")";
	}
	return str;
}

////////////////////////////////////////////////////////////

MC6809SymbolTable::MC6809SymbolTable()
{
	MakeOS9FunctionTable();
}

void MC6809SymbolTable::MakeOS9FunctionTable(void)
{
	for(int i=0; i<256; ++i)
	{
		OS9Functions[i]="?";
		OS9FunctionExplanation[i]="?";
	}

	// Based on OS-9 Operating System System Programmer's Manual
	OS9Functions[0x00]="F$Link";                OS9FunctionExplanation[0x00]="Link to memory module.";
	OS9Functions[0x01]="F$Load";                OS9FunctionExplanation[0x01]="Load module(s) from a file.";
	OS9Functions[0x02]="F$UnLink";              OS9FunctionExplanation[0x02]="Unlink a module.";
	OS9Functions[0x03]="F$Fork";                OS9FunctionExplanation[0x03]="Create a new process.";
	OS9Functions[0x04]="F$Wait";                OS9FunctionExplanation[0x04]="Wait for child process to die.";
	OS9Functions[0x05]="F$Chain";               OS9FunctionExplanation[0x05]="Load and execute a new primary module.";
	OS9Functions[0x06]="F$Exit";                OS9FunctionExplanation[0x06]="Terminate the calling process.";
	OS9Functions[0x07]="F$Mem";                 OS9FunctionExplanation[0x07]="Resize data memory area.";
	OS9Functions[0x08]="F$Send";                OS9FunctionExplanation[0x08]="Send a signal to another process.";
	OS9Functions[0x09]="F$ICPT";                OS9FunctionExplanation[0x09]="Set up a signal intercept trap.";
	OS9Functions[0x0A]="F$Sleep";               OS9FunctionExplanation[0x0A]="Put calling process to sleep.";
	OS9Functions[0x0B]="?";                     OS9FunctionExplanation[0x0B]="?";
	OS9Functions[0x0C]="F$ID";                  OS9FunctionExplanation[0x0C]="Get process ID / user ID.";
	OS9Functions[0x0D]="F$SPrior";              OS9FunctionExplanation[0x0D]="Set process priority.";
	OS9Functions[0x0E]="F$SSWI";                OS9FunctionExplanation[0x0E]="Set SWI vector.";
	OS9Functions[0x0F]="F$PErr";                OS9FunctionExplanation[0x0F]="Print error message.";

	OS9Functions[0x10]="F$PrsNam";              OS9FunctionExplanation[0x10]="Parse a path name.";
	OS9Functions[0x11]="F$CmpNam";              OS9FunctionExplanation[0x11]="Compare two names.";
	OS9Functions[0x12]="F$SchBit";              OS9FunctionExplanation[0x12]="Search bit map for a free area.";
	OS9Functions[0x13]="F$AllBit";              OS9FunctionExplanation[0x13]="Set bits in an allocation bit map.";
	OS9Functions[0x14]="F$DelBit";              OS9FunctionExplanation[0x14]="Deallocate in a bit map.";
	OS9Functions[0x15]="F$Time";                OS9FunctionExplanation[0x15]="Get system date and time.";
	OS9Functions[0x16]="F$STime";               OS9FunctionExplanation[0x16]="Set system date and time.";
	OS9Functions[0x17]="F$CRC";                 OS9FunctionExplanation[0x17]="Compute CRC";
	OS9Functions[0x18]="F$GPrDsc";              OS9FunctionExplanation[0x18]="Get Process Descriptor copy.";
	OS9Functions[0x19]="F$GBlkMp";              OS9FunctionExplanation[0x19]="Get system Block Map copy.";
	OS9Functions[0x1A]="F$GModDr";              OS9FunctionExplanation[0x1A]="Get Module Directory copy.";
	OS9Functions[0x1B]="F$CpyMem";              OS9FunctionExplanation[0x1B]="Copy external Memory.";
	OS9Functions[0x1C]="F$User";                OS9FunctionExplanation[0x1C]="Set User ID number.";
	OS9Functions[0x1D]="F$UnLoad";              OS9FunctionExplanation[0x1D]="Unlink module by name.";
	OS9Functions[0x1E]="?";                     OS9FunctionExplanation[0x1E]="?";
	OS9Functions[0x1F]="?";                     OS9FunctionExplanation[0x1F]="?";

	OS9Functions[0x20]="?";                     OS9FunctionExplanation[0x20]="?";
	OS9Functions[0x21]="?";                     OS9FunctionExplanation[0x21]="?";
	OS9Functions[0x22]="?";                     OS9FunctionExplanation[0x22]="?";
	OS9Functions[0x23]="?";                     OS9FunctionExplanation[0x23]="?";
	OS9Functions[0x24]="?";                     OS9FunctionExplanation[0x24]="?";
	OS9Functions[0x25]="?";                     OS9FunctionExplanation[0x25]="?";
	OS9Functions[0x26]="?";                     OS9FunctionExplanation[0x26]="?";
	OS9Functions[0x27]="?";                     OS9FunctionExplanation[0x27]="?";
	OS9Functions[0x28]="F$RqMem";               OS9FunctionExplanation[0x28]="System memory request.";
	OS9Functions[0x29]="F$RtMem";               OS9FunctionExplanation[0x29]="System memory return.";
	OS9Functions[0x2A]="F$IRQ";                 OS9FunctionExplanation[0x2A]="Add or remove device from IRQ table.";
	OS9Functions[0x2B]="F$IOQU";                OS9FunctionExplanation[0x2B]="Enter I/O queue.";
	OS9Functions[0x2C]="F$AProc";               OS9FunctionExplanation[0x2C]="Insert process in active process queue.";
	OS9Functions[0x2D]="F$NProc";               OS9FunctionExplanation[0x2D]="Start next process.";
	OS9Functions[0x2E]="F$VModule";             OS9FunctionExplanation[0x2E]="Validate module.";
	OS9Functions[0x2F]="F$Find64";              OS9FunctionExplanation[0x2F]="Find a 64 byte memory block.";

	OS9Functions[0x30]="F$All64";               OS9FunctionExplanation[0x30]="Allocate a 64 byte memory block.";
	OS9Functions[0x31]="F$Ret64";               OS9FunctionExplanation[0x31]="Deallocate a 64 byte memory block.";
	OS9Functions[0x32]="F$SSVC";                OS9FunctionExplanation[0x32]="Install function request.";
	OS9Functions[0x33]="F$IODel";               OS9FunctionExplanation[0x33]="Delete I/O device from system.";
	OS9Functions[0x34]="F$SLink";               OS9FunctionExplanation[0x34]="System Link.";
	OS9Functions[0x35]="F$Boot";                OS9FunctionExplanation[0x35]="Bootstrap system.";
	OS9Functions[0x36]="F$BtMem";               OS9FunctionExplanation[0x36]="Bootstrap Memory request.";
	OS9Functions[0x37]="F$GProcP";              OS9FunctionExplanation[0x37]="Get Process Pointer";
	OS9Functions[0x38]="F$Move";                OS9FunctionExplanation[0x38]="Move data (low bound first).";
	OS9Functions[0x39]="F$AllRAM";              OS9FunctionExplanation[0x39]="Allocate RAM blocks.";
	OS9Functions[0x3A]="F$AllImg";              OS9FunctionExplanation[0x3A]="Allocate Image RAM blocks.";
	OS9Functions[0x3B]="F$DelImg";              OS9FunctionExplanation[0x3B]="Deallocate Image RAM blocks.";
	OS9Functions[0x3C]="F$SetImg";              OS9FunctionExplanation[0x3C]="Set process DAT Image.";
	OS9Functions[0x3D]="F$FreeLB";              OS9FunctionExplanation[0x3D]="get Free Low block.";
	OS9Functions[0x3E]="F$FreeHB";              OS9FunctionExplanation[0x3E]="get Free High block.";
	OS9Functions[0x3F]="F$AllTsk";              OS9FunctionExplanation[0x3F]="Allocate process Task number.";

	OS9Functions[0x40]="F$DelTsk";              OS9FunctionExplanation[0x40]="Deallocate process Task number.";
	OS9Functions[0x41]="F$SetTsk";              OS9FunctionExplanation[0x41]="Set process Task DAT registers.";
	OS9Functions[0x42]="F$ResTsk";              OS9FunctionExplanation[0x42]="Reserve Task number";
	OS9Functions[0x43]="F$RelTsk";              OS9FunctionExplanation[0x43]="Release Task number.";
	OS9Functions[0x44]="F$DATLog";              OS9FunctionExplanation[0x44]="Convert DAT block/offset to Logical Addr";
	OS9Functions[0x45]="F$DATTmp";              OS9FunctionExplanation[0x45]="Make Temporary DAT image.";
	OS9Functions[0x46]="F$LDAXY";               OS9FunctionExplanation[0x46]="Load A [X,[Y]]";
	OS9Functions[0x47]="F$LDAXYP";              OS9FunctionExplanation[0x47]="Load A [X+,[Y]]";
	OS9Functions[0x48]="F$LDDDXY";              OS9FunctionExplanation[0x48]="Load D [D+X,[Y]]";
	OS9Functions[0x49]="F$LDABX";               OS9FunctionExplanation[0x49]="Load A from 0,X in task B.";
	OS9Functions[0x4A]="F$STABX";               OS9FunctionExplanation[0x4A]="Store A at 0,X in task B.";
	OS9Functions[0x4B]="F$AllPrc";              OS9FunctionExplanation[0x4B]="Allocate Process descriptor.";
	OS9Functions[0x4C]="F$DelPrc";              OS9FunctionExplanation[0x4C]="Deallocate Process descriptor.";
	OS9Functions[0x4D]="F$ELink";               OS9FunctionExplanation[0x4D]="Link using module directory Entry.";
	OS9Functions[0x4E]="F$FModul";              OS9FunctionExplanation[0x4E]="Find Module directory entry.";
	OS9Functions[0x4F]="F$MapBlk";              OS9FunctionExplanation[0x4F]="Map specific Block.";

	OS9Functions[0x50]="F$ClrBlk";              OS9FunctionExplanation[0x50]="Clear specific Block.";
	OS9Functions[0x51]="F$DelRam";              OS9FunctionExplanation[0x51]="Deallocate RAM blocks.";
	OS9Functions[0x52]="?";                     OS9FunctionExplanation[0x52]="?";
	OS9Functions[0x53]="?";                     OS9FunctionExplanation[0x53]="?";
	OS9Functions[0x54]="?";                     OS9FunctionExplanation[0x54]="?";
	OS9Functions[0x55]="?";                     OS9FunctionExplanation[0x55]="?";
	OS9Functions[0x56]="?";                     OS9FunctionExplanation[0x56]="?";
	OS9Functions[0x57]="?";                     OS9FunctionExplanation[0x57]="?";
	OS9Functions[0x58]="?";                     OS9FunctionExplanation[0x58]="?";
	OS9Functions[0x59]="?";                     OS9FunctionExplanation[0x59]="?";
	OS9Functions[0x5A]="?";                     OS9FunctionExplanation[0x5A]="?";
	OS9Functions[0x5B]="?";                     OS9FunctionExplanation[0x5B]="?";
	OS9Functions[0x5C]="?";                     OS9FunctionExplanation[0x5C]="?";
	OS9Functions[0x5D]="?";                     OS9FunctionExplanation[0x5D]="?";
	OS9Functions[0x5E]="?";                     OS9FunctionExplanation[0x5E]="?";
	OS9Functions[0x5F]="?";                     OS9FunctionExplanation[0x5F]="?";

	OS9Functions[0x80]="I$Attach";              OS9FunctionExplanation[0x80]="Attach a new device to the system.";
	OS9Functions[0x81]="I$Detach";              OS9FunctionExplanation[0x81]="Remove a device from the system.";
	OS9Functions[0x82]="I$Dup";                 OS9FunctionExplanation[0x82]="Duplicate a path.";
	OS9Functions[0x83]="I$Create";              OS9FunctionExplanation[0x83]="Create a path to a new file.";
	OS9Functions[0x84]="I$Open";                OS9FunctionExplanation[0x84]="Open a path to a file or device.";
	OS9Functions[0x85]="I$MakDir";              OS9FunctionExplanation[0x85]="Make a new directory.";
	OS9Functions[0x86]="I$ChgDir";              OS9FunctionExplanation[0x86]="Change working directory.";
	OS9Functions[0x87]="I$Delete";              OS9FunctionExplanation[0x87]="Delete a file.";
	OS9Functions[0x88]="I$Seek";                OS9FunctionExplanation[0x88]="Reposition the logical file pointer.";
	OS9Functions[0x89]="I$Read";                OS9FunctionExplanation[0x89]="Read data from a file or device.";
	OS9Functions[0x8A]="I$Write";               OS9FunctionExplanation[0x8A]="Write Data to File or Device.";
	OS9Functions[0x8B]="I$ReadLn";              OS9FunctionExplanation[0x8B]="Read a text line with editing.";
	OS9Functions[0x8C]="I$WriteLn";             OS9FunctionExplanation[0x8C]="Write Line of Text with Editing.";
	OS9Functions[0x8D]="I$GetStt";              OS9FunctionExplanation[0x8D]="Get file/device status.";
	OS9Functions[0x8E]="I$SetStt";              OS9FunctionExplanation[0x8E]="Set file/device status.";
	OS9Functions[0x8F]="I$Close";               OS9FunctionExplanation[0x8F]="Close a path to a file/device.";
	OS9Functions[0x90]="I$DeletX";              OS9FunctionExplanation[0x90]="Delete a file.";
}

bool MC6809SymbolTable::Load(const std::vector <std::string> &txt)
{
	const int STATE_OUTSIDE=0,STATE_INSIDE=1;
	int state=STATE_OUTSIDE;
	unsigned int curPtr;
	MC6809Symbol curSymbol;

	// Reserved Keywords
	// /main
	// /sub

	// /begin0
	// T 0/1/2/3  Type (0:Any 1:Procedure 2:Jump Destination 3:Data 255:DP)
	// * 2388  Address
	// # Inline Comment
	// R void  Return-Type
	// L main  Label
	// P int argc,char *argv[]
	// I Supplimental Info
	// % Raw Data Byte Count
	// M 0/1 immIsIOAddr flag
	// B 0/1 immIsSymbol flag
	// A 0/1 immIsASCII flag
	// O 0/1 offsetIsSymbol flag
	// X label Imported from .EXP symbol table.
	// /end

	for(auto str : txt)
	{
		if(STATE_OUTSIDE==state)
		{
			cpputil::Capitalize(str);
			if(str=="/BEGIN0")
			{
				curPtr=0;
				curSymbol.CleanUp();
				state=STATE_INSIDE;
			}
		}
		else if(STATE_INSIDE==state)
		{
			if('/'==str[0])
			{
				cpputil::Capitalize(str);
				if("/END"==str)
				{
					if(MC6809Symbol::SYM_DP!=curSymbol.symType)
					{
						symTable[curPtr]=curSymbol;
					}
					else
					{
						DPLabel[curPtr]=curSymbol.label;
					}
				}
			}
			else if(2<=str.size())
			{
				switch(str[0])
				{
				case 't':
				case 'T':
					curSymbol.symType=cpputil::Atoi(str.c_str()+2);
					break;
				case '*':
					curPtr=cpputil::Xtoi(str.c_str()+2);
					break;
				case '#':
					curSymbol.inLineComment=(str.c_str()+2);
					break;
				case 'r':
				case 'R':
					curSymbol.return_type=(str.c_str()+2);
					break;
				case 'l':
				case 'L':
					curSymbol.label=(str.c_str()+2);
					break;
				case 'p':
				case 'P':
					curSymbol.param=(str.c_str()+2);
					break;
				case 'i':
				case 'I':
					curSymbol.info.push_back(str.c_str()+2);
					break;
				case '%':
					curSymbol.rawDataBytes=cpputil::Atoi(str.c_str()+2);
					break;
				case 'm':
				case 'M':
					curSymbol.immIsIOAddr=(0!=cpputil::Atoi(str.c_str()+2));
					break;
				case 'b':
				case 'B':
					curSymbol.immIsSymbol=(0!=cpputil::Atoi(str.c_str()+2));
					break;
				case 'o':
				case 'O':
					curSymbol.offsetIsSymbol=(0!=cpputil::Atoi(str.c_str()+2));
					break;
				case 'x':
				case 'X':
					curSymbol.imported=(str.c_str()+2);
					break;
				case 'a':
				case 'A':
					curSymbol.immIsASCII=(0!=cpputil::Atoi(str.c_str()+2));
					break;
				}
			}
		}
	}
	return true;
}

std::vector <std::string> MC6809SymbolTable::Save(void) const
{
	std::vector <std::string> txt;
	for(auto &ptrAndSym : symTable)
	{
		auto &ptr=ptrAndSym.first;
		auto &sym=ptrAndSym.second;
		if(true!=sym.temporary)
		{
			std::stringstream ss;

			txt.push_back("/begin0");

			ss.str("");
			ss << "T " << (int)(sym.symType);
			txt.push_back(ss.str());

			ss.str("");
			ss << "* " << cpputil::Uitox(ptr);
			txt.push_back(ss.str());

			ss.str("");
			ss << "# " << sym.inLineComment;
			txt.push_back(ss.str());

			ss.str("");
			ss << "R " << sym.return_type;
			txt.push_back(ss.str());

			ss.str("");
			ss << "L " << sym.label ;
			txt.push_back(ss.str());

			ss.str("");
			ss << "P " << sym.param <<  std::endl;
			txt.push_back(ss.str());

			ss.str("");
			ss << "% " << sym.rawDataBytes <<  std::endl;
			txt.push_back(ss.str());

			ss.str("");
			ss << "M " << (sym.immIsIOAddr ? "1" : "0");
			txt.push_back(ss.str());

			ss.str("");
			ss << "B " << (sym.immIsSymbol ? "1" : "0");
			txt.push_back(ss.str());

			ss.str("");
			ss << "A " << (sym.immIsASCII ? "1" : "0");
			txt.push_back(ss.str());

			ss.str("");
			ss << "O " << (sym.offsetIsSymbol ? "1" : "0");
			txt.push_back(ss.str());

			ss.str("");
			ss << "X " << sym.imported ;
			txt.push_back(ss.str());

			for(auto &i : sym.info)
			{
				ss.str("");
				ss << "I " << i;
				txt.push_back(ss.str());
			}

			txt.push_back("/end");
		}
	}
	for(auto iter=DPLabel.begin(); DPLabel.end()!=iter; ++iter)
	{
		if(""!=iter->second)
		{
			std::stringstream ss;

			txt.push_back("/begin0");

			ss.str("");
			ss << "* " << cpputil::Uitox(iter->first);
			txt.push_back(ss.str());

			ss.str("");
			ss << "L " << iter->second ;
			txt.push_back(ss.str());

			txt.push_back("/end");
		}
	}
	return txt;
}

const MC6809Symbol *MC6809SymbolTable::Find(unsigned int addr) const
{
	auto iter=symTable.find(addr);
	if(symTable.end()!=iter)
	{
		return &iter->second;
	}
	return nullptr;
}
std::pair <unsigned int,MC6809Symbol> MC6809SymbolTable::FindSymbolFromLabel(const std::string &label) const
{
	for(auto &addrAndSym : symTable)
	{
		if(addrAndSym.second.label==label || addrAndSym.second.imported==label)
		{
			return addrAndSym;
		}
	}
	std::pair <unsigned int,MC6809Symbol> empty;
	return empty;
}
MC6809Symbol *MC6809SymbolTable::Update(unsigned int ptr,const std::string &label)
{
	auto &symbol=symTable[ptr];
	symbol.label=label;
	return &symbol;
}
MC6809Symbol *MC6809SymbolTable::SetComment(unsigned int ptr,const std::string &inLineComment)
{
	auto &symbol=symTable[ptr];
	symbol.inLineComment=inLineComment;
	return &symbol;
}
MC6809Symbol *MC6809SymbolTable::SetImmIsIOPort(unsigned int ptr)
{
	auto &symbol=symTable[ptr];
	symbol.immIsIOAddr=true;
	return &symbol;
}
MC6809Symbol *MC6809SymbolTable::SetImportedLabel(unsigned int ptr,const std::string &label)
{
	bool makeItProcedure=false; // It can be a data, it can be a procedure, but temporarily make it a procedure, if not in the database yet.
	auto found=symTable.find(ptr);
	if(found==symTable.end())
	{
		makeItProcedure=true;
	}
	auto &symbol=symTable[ptr];
	symbol.imported=label;
	if(true==makeItProcedure)
	{
		symbol.symType=MC6809Symbol::SYM_PROCEDURE;
	}
	return &symbol;
}
MC6809Symbol *MC6809SymbolTable::SetImmIsSymbol(unsigned int ptr)
{
	auto &symbol=symTable[ptr];
	symbol.immIsSymbol=true;
	return &symbol;
}
MC6809Symbol *MC6809SymbolTable::SetOffsetIsSymbol(unsigned int ptr)
{
	auto &symbol=symTable[ptr];
	symbol.offsetIsSymbol=true;
	return &symbol;
}
void MC6809SymbolTable::SetDPLabel(unsigned int DP,std::string label)
{
	DPLabel[DP]=label;
}
std::string MC6809SymbolTable::GetDPLabel(unsigned int DP) const
{
	auto found=DPLabel.find(DP);
	if(DPLabel.end()!=found)
	{
		return found->second;
	}
	return "";
}
bool MC6809SymbolTable::Delete(unsigned int ptr)
{
	auto iter=symTable.find(ptr);
	if(symTable.end()!=iter)
	{
		symTable.erase(iter);
		return true;
	}
	return false;
}
bool MC6809SymbolTable::DeleteComment(unsigned int ptr)
{
	auto iter=symTable.find(ptr);
	if(symTable.end()!=iter)
	{
		iter->second.inLineComment="";
		return true;
	}
	return false;
}
const std::map <unsigned int,MC6809Symbol> &MC6809SymbolTable::GetTable(void) const
{
	return symTable;
}

unsigned int MC6809SymbolTable::GetRawDataBytes(unsigned int ptr) const
{
	auto *sym=Find(ptr);
	if(nullptr!=sym &&
	   (MC6809Symbol::SYM_RAW_DATA==sym->symType ||
	    MC6809Symbol::SYM_TABLE_WORD==sym->symType))
	{
		return sym->rawDataBytes;
	}
	return 0;
}

void MC6809SymbolTable::PrintIfAny(unsigned int addr,bool returnType,bool label,bool param) const
{
	auto *sym=Find(addr);
	if(nullptr!=sym && (0<sym->label.size() || 0<sym->imported.size()))
	{
		std::cout << sym->Format(returnType,label,param) << std::endl;
	}
}

std::vector <std::string> MC6809SymbolTable::GetList(bool returnType,bool label,bool param) const
{
	std::vector <std::string> text;
	for(const auto &addrAndSym : symTable)
	{
		auto &addr=addrAndSym.first;
		auto &sym=addrAndSym.second;

		text.push_back("");
		text.back()+=cpputil::Ustox(addr);

		text.back()+=" ";
		text.back()+=sym.Format(returnType,label,param);
	
		if(0<sym.inLineComment.size())
		{
			text.back()+=" ; ";
			text.back()+=sym.inLineComment;
		}
	}
	return text;
}
std::string MC6809SymbolTable::FormatImmLabel(unsigned int addr,uint32_t imm) const
{
	std::string str;
	auto found=Find(addr);
	if(nullptr!=found && true==found->immIsSymbol)
	{
		auto foundLabel=Find(imm);
		if(nullptr!=foundLabel)
		{
			if(""!=foundLabel->label)
			{
				str="("+foundLabel->label+")";
			}
			else if(""!=foundLabel->imported)
			{
				str="("+foundLabel->imported+")";
			}
		}
	}
	return str;
}

std::string MC6809SymbolTable::FormatDataLabel(unsigned int PC,unsigned int dataPtr) const
{
	std::string str;
	auto found=Find(PC);
	if(nullptr!=found && true==found->offsetIsSymbol)
	{
		auto foundLabel=Find(dataPtr);
		if(nullptr!=foundLabel)
		{
			if(""!=foundLabel->label)
			{
				str="("+foundLabel->label+")";
			}
			else if(""!=foundLabel->imported)
			{
				str="("+foundLabel->imported+")";
			}
		}
	}
	return str;
}
