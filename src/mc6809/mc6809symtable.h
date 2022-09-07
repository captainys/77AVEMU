#ifndef MC6809SYMTABLE_IS_INCLUDED
#define MC6809SYMTABLE_IS_INCLUDED
/* { */

#include <vector>
#include <string>
#include <map>
#include <stdint.h>

class MC6809Symbol
{
public:
	enum
	{
		SYM_ANY,
		SYM_PROCEDURE,
		SYM_JUMP_DESTINATION,
		SYM_DATA_LABEL,
		SYM_RAW_DATA, // If disassembler hits this address, it just reads rawDataCount bytes and don't disassemble.
		SYM_TABLE_WORD,
		SYM_DP=255
	};

	bool temporary=false;  // If true, it will not be saved to file.
	bool immIsIOAddr=false; // If true, disassembler should take Imm operand as an IO-port address.
	bool immIsSymbol=false; // Take imm as a symbol.
	bool immIsASCII=false; // Take imm as ASCII code.
	bool offsetIsSymbol=false; // Take offset as a symbol.
	unsigned int symType;
	std::string return_type;
	std::string label;
	std::string inLineComment;
	std::string param;
	std::string imported; // Imported from .LST file.
	std::vector <std::string> info;
	unsigned int rawDataBytes;

	MC6809Symbol();
	void CleanUp(void);
	std::string Format(bool returnType=false,bool label=true,bool param=true) const;
};

class MC6809SymbolTable
{
private:
	std::map <unsigned int,MC6809Symbol> symTable;
	std::map <unsigned int,std::string> DPLabel;
	std::map <unsigned int,std::string> IOLabel;
public:
	std::map <unsigned int,std::string> OS9Functions,OS9FunctionExplanation;

	MC6809SymbolTable();
	void MakeOS9FunctionTable(void);

	bool Load(const std::vector <std::string> &txt);
	std::vector <std::string> Save(void) const;

	const MC6809Symbol *Find(unsigned int ptr) const;
	MC6809Symbol *Update(unsigned int ptr,const std::string &label);
	MC6809Symbol *SetComment(unsigned int ptr,const std::string &inLineComment);
	MC6809Symbol *SetImmIsIOPort(unsigned int ptr);
	MC6809Symbol *SetImportedLabel(unsigned int ptr,const std::string &label);
	MC6809Symbol *SetImmIsSymbol(unsigned int ptr);
	MC6809Symbol *SetOffsetIsSymbol(unsigned int ptr);

	void SetDPLabel(unsigned int DP,std::string label);
	std::string GetDPLabel(unsigned int DP) const;


	/*! Disassembler should take IMM operand at the given address as an IO-port address.
	*/
	MC6809Symbol *SetImmIsIO(unsigned int addr);

	bool Delete(unsigned int addr);
	bool DeleteComment(unsigned int addr);
	const std::map <unsigned int,MC6809Symbol> &GetTable(void) const;

	/*! Returns the number of bytes if the address is marked as byte data, otherwize returns zero.
	*/
	unsigned int GetRawDataBytes(unsigned int addr) const;

	/*! Print if a symbol is defined for the SEG:OFFSET
	*/
	void PrintIfAny(unsigned int addr,bool returnType=false,bool label=true,bool param=true) const;

	/*! This function does exhaustive search.
	*/
	std::pair <unsigned int,MC6809Symbol> FindSymbolFromLabel(const std::string &label) const;

	std::vector <std::string> GetList(bool returnType=false,bool label=true,bool param=true) const;

	/*!
	*/
	std::string FormatImmLabel(unsigned int PC,unsigned int imm) const;

	/*!
	*/
	std::string FormatDataLabel(unsigned int PC,unsigned int imm) const;
};


/* } */
#endif
