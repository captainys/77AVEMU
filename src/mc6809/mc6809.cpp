#include "mc6809.h"



MC6809::MC6809(VMBase *vmBase) : Device(vmBase)
{
}

MC6809::Instruction MC6809::FetchInstruction(class MemoryAccess *mem)
{
	Instruction inst;

	inst.opCode=mem->FetchByte(state.PC++);
	inst.length=1;
	if(0x10==inst.opCode)
	{
		inst.length=2;
		inst.opCode=0x100|mem->FetchByte(state.PC++);
	}
	else if(0x11==inst.opCode)
	{
		inst.length=2;
		inst.opCode=0x200|mem->FetchByte(state.PC++);
	}

	return inst;
}
