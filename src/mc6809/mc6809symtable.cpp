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
