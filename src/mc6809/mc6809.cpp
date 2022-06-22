#include "mc6809.h"
#include "mc6809util.h"



MC6809::MC6809(VMBase *vmBase) : Device(vmBase)
{
	for(auto &t : instOperaType)
	{
		t=OPER_INHERENT; // No operand
	}

	instOperaType[INST_ADCA_IMM]=OPER_IMM;
	instOperaType[INST_ADCA_DP]=OPER_DP;
	instOperaType[INST_ADCA_IDX]=OPER_IDX;
	instOperaType[INST_ADCA_EXT]=OPER_EXT;

	instOperaType[INST_ADCB_IMM]=OPER_IMM;
	instOperaType[INST_ADCB_DP]=OPER_DP;
	instOperaType[INST_ADCB_IDX]=OPER_IDX;
	instOperaType[INST_ADCB_EXT]=OPER_EXT;

	instOperaType[INST_ADDA_IMM]=OPER_IMM;
	instOperaType[INST_ADDA_DP]=OPER_DP;
	instOperaType[INST_ADDA_IDX]=OPER_IDX;
	instOperaType[INST_ADDA_EXT]=OPER_EXT;

	instOperaType[INST_ADDB_IMM]=OPER_IMM;
	instOperaType[INST_ADDB_DP]=OPER_DP;
	instOperaType[INST_ADDB_IDX]=OPER_IDX;
	instOperaType[INST_ADDB_EXT]=OPER_EXT;

	instOperaType[INST_ADDD_IMM]=OPER_IMM;
	instOperaType[INST_ADDD_DP]=OPER_DP;
	instOperaType[INST_ADDD_IDX]=OPER_IDX;
	instOperaType[INST_ADDD_EXT]=OPER_EXT;

	instOperaType[INST_ANDA_IMM]=OPER_IMM;
	instOperaType[INST_ANDA_DP]=OPER_DP;
	instOperaType[INST_ANDA_IDX]=OPER_IDX;
	instOperaType[INST_ANDA_EXT]=OPER_EXT;

	instOperaType[INST_ANDB_IMM]=OPER_IMM;
	instOperaType[INST_ANDB_DP]=OPER_DP;
	instOperaType[INST_ANDB_IDX]=OPER_IDX;
	instOperaType[INST_ANDB_EXT]=OPER_EXT;

	instOperaType[INST_ANDCC_IMM]=OPER_IMM;

	instOperaType[INST_ASL_DP]=OPER_DP;
	instOperaType[INST_ASL_IDX]=OPER_IDX;
	instOperaType[INST_ASL_EXT]=OPER_EXT;

	instOperaType[INST_ASR_DP]=OPER_DP;
	instOperaType[INST_ASR_IDX]=OPER_IDX;
	instOperaType[INST_ASR_EXT]=OPER_EXT;

	instOperaType[INST_BITA_IMM]=OPER_IMM;
	instOperaType[INST_BITA_DP]=OPER_DP;
	instOperaType[INST_BITA_IDX]=OPER_IDX;
	instOperaType[INST_BITA_EXT]=OPER_EXT;

	instOperaType[INST_BITB_IMM]=OPER_IMM;
	instOperaType[INST_BITB_DP]=OPER_DP;
	instOperaType[INST_BITB_IDX]=OPER_IDX;
	instOperaType[INST_BITB_EXT]=OPER_EXT;

	instOperaType[INST_CLR_DP]=OPER_DP;
	instOperaType[INST_CLR_IDX]=OPER_IDX;
	instOperaType[INST_CLR_EXT]=OPER_EXT;

	instOperaType[INST_CMPA_IMM]=OPER_IMM;
	instOperaType[INST_CMPA_DP]=OPER_DP;
	instOperaType[INST_CMPA_IDX]=OPER_IDX;
	instOperaType[INST_CMPA_EXT]=OPER_EXT;

	instOperaType[INST_CMPB_IMM]=OPER_IMM;
	instOperaType[INST_CMPB_DP]=OPER_DP;
	instOperaType[INST_CMPB_IDX]=OPER_IDX;
	instOperaType[INST_CMPB_EXT]=OPER_EXT;

	instOperaType[INST_CMPD_IMM]=OPER_IMM;
	instOperaType[INST_CMPD_DP]=OPER_DP;
	instOperaType[INST_CMPD_IDX]=OPER_IDX;
	instOperaType[INST_CMPD_EXT]=OPER_EXT;

	instOperaType[INST_CMPS_IMM]=OPER_IMM;
	instOperaType[INST_CMPS_DP]=OPER_DP;
	instOperaType[INST_CMPS_IDX]=OPER_IDX;
	instOperaType[INST_CMPS_EXT]=OPER_EXT;

	instOperaType[INST_CMPU_IMM]=OPER_IMM;
	instOperaType[INST_CMPU_DP]=OPER_DP;
	instOperaType[INST_CMPU_IDX]=OPER_IDX;
	instOperaType[INST_CMPU_EXT]=OPER_EXT;

	instOperaType[INST_CMPX_IMM]=OPER_IMM;
	instOperaType[INST_CMPX_DP]=OPER_DP;
	instOperaType[INST_CMPX_IDX]=OPER_IDX;
	instOperaType[INST_CMPX_EXT]=OPER_EXT;

	instOperaType[INST_CMPY_IMM]=OPER_IMM;
	instOperaType[INST_CMPY_DP]=OPER_DP;
	instOperaType[INST_CMPY_IDX]=OPER_IDX;
	instOperaType[INST_CMPY_EXT]=OPER_EXT;

	instOperaType[INST_COM_DP]=OPER_DP;
	instOperaType[INST_COM_IDX]=OPER_IDX;
	instOperaType[INST_COM_EXT]=OPER_EXT;

	instOperaType[INST_CWAI_IMM]=OPER_IMM;

	instOperaType[INST_DEC_DP]=OPER_DP;
	instOperaType[INST_DEC_IDX]=OPER_IDX;
	instOperaType[INST_DEC_EXT]=OPER_EXT;

	instOperaType[INST_EORA_IMM]=OPER_IMM;
	instOperaType[INST_EORA_DP]=OPER_DP;
	instOperaType[INST_EORA_IDX]=OPER_IDX;
	instOperaType[INST_EORA_EXT]=OPER_EXT;

	instOperaType[INST_EORB_IMM]=OPER_IMM;
	instOperaType[INST_EORB_DP]=OPER_DP;
	instOperaType[INST_EORB_IDX]=OPER_IDX;
	instOperaType[INST_EORB_EXT]=OPER_EXT;

	instOperaType[INST_INC_DP]=OPER_DP;
	instOperaType[INST_INC_IDX]=OPER_IDX;
	instOperaType[INST_INC_EXT]=OPER_EXT;

	instOperaType[INST_JMP_DP]=OPER_DP;
	instOperaType[INST_JMP_IDX]=OPER_IDX;
	instOperaType[INST_JMP_EXT]=OPER_EXT;

	instOperaType[INST_JSR_DP]=OPER_DP;
	instOperaType[INST_JSR_IDX]=OPER_IDX;
	instOperaType[INST_JSR_EXT]=OPER_EXT;

	instOperaType[INST_LDA_IMM]=OPER_IMM;
	instOperaType[INST_LDA_DP]=OPER_DP;
	instOperaType[INST_LDA_IDX]=OPER_IDX;
	instOperaType[INST_LDA_EXT]=OPER_EXT;

	instOperaType[INST_LDB_IMM]=OPER_IMM;
	instOperaType[INST_LDB_DP]=OPER_DP;
	instOperaType[INST_LDB_IDX]=OPER_IDX;
	instOperaType[INST_LDB_EXT]=OPER_EXT;

	instOperaType[INST_LDD_IMM]=OPER_IMM;
	instOperaType[INST_LDD_DP]=OPER_DP;
	instOperaType[INST_LDD_IDX]=OPER_IDX;
	instOperaType[INST_LDD_EXT]=OPER_EXT;

	instOperaType[INST_LDS_IMM]=OPER_IMM;
	instOperaType[INST_LDS_DP]=OPER_DP;
	instOperaType[INST_LDS_IDX]=OPER_IDX;
	instOperaType[INST_LDS_EXT]=OPER_EXT;

	instOperaType[INST_LDU_IMM]=OPER_IMM;
	instOperaType[INST_LDU_DP]=OPER_DP;
	instOperaType[INST_LDU_IDX]=OPER_IDX;
	instOperaType[INST_LDU_EXT]=OPER_EXT;

	instOperaType[INST_LDX_IMM]=OPER_IMM;
	instOperaType[INST_LDX_DP]=OPER_DP;
	instOperaType[INST_LDX_IDX]=OPER_IDX;
	instOperaType[INST_LDX_EXT]=OPER_EXT;

	instOperaType[INST_LDY_IMM]=OPER_IMM;
	instOperaType[INST_LDY_DP]=OPER_DP;
	instOperaType[INST_LDY_IDX]=OPER_IDX;
	instOperaType[INST_LDY_EXT]=OPER_EXT;

	instOperaType[INST_LEAS_IDX]=OPER_IDX;
	instOperaType[INST_LEAU_IDX]=OPER_IDX;
	instOperaType[INST_LEAX_IDX]=OPER_IDX;
	instOperaType[INST_LEAY_IDX]=OPER_IDX;

	instOperaType[INST_LSL_DP]=OPER_DP;
	instOperaType[INST_LSL_IDX]=OPER_IDX;
	instOperaType[INST_LSL_EXT]=OPER_EXT;

	instOperaType[INST_LSR_DP]=OPER_DP;
	instOperaType[INST_LSR_IDX]=OPER_IDX;
	instOperaType[INST_LSR_EXT]=OPER_EXT;

	instOperaType[INST_NEG_DP]=OPER_DP;
	instOperaType[INST_NEG_IDX]=OPER_IDX;
	instOperaType[INST_NEG_EXT]=OPER_EXT;

	instOperaType[INST_ORA_IMM]=OPER_IMM;
	instOperaType[INST_ORA_DP]=OPER_DP;
	instOperaType[INST_ORA_IDX]=OPER_IDX;
	instOperaType[INST_ORA_EXT]=OPER_EXT;

	instOperaType[INST_ORB_IMM]=OPER_IMM;
	instOperaType[INST_ORB_DP]=OPER_DP;
	instOperaType[INST_ORB_IDX]=OPER_IDX;
	instOperaType[INST_ORB_EXT]=OPER_EXT;

	instOperaType[INST_ORCC_IMM]=OPER_IMM;

	instOperaType[INST_ROL_DP]=OPER_DP;
	instOperaType[INST_ROL_IDX]=OPER_IDX;
	instOperaType[INST_ROL_EXT]=OPER_EXT;

	instOperaType[INST_ROR_DP]=OPER_DP;
	instOperaType[INST_ROR_IDX]=OPER_IDX;
	instOperaType[INST_ROR_EXT]=OPER_EXT;

	instOperaType[INST_SBCA_IMM]=OPER_IMM;
	instOperaType[INST_SBCA_DP]=OPER_DP;
	instOperaType[INST_SBCA_IDX]=OPER_IDX;
	instOperaType[INST_SBCA_EXT]=OPER_EXT;

	instOperaType[INST_SBCB_IMM]=OPER_IMM;
	instOperaType[INST_SBCB_DP]=OPER_DP;
	instOperaType[INST_SBCB_IDX]=OPER_IDX;
	instOperaType[INST_SBCB_EXT]=OPER_EXT;

	instOperaType[INST_SUBA_IMM]=OPER_IMM;
	instOperaType[INST_SUBA_DP]=OPER_DP;
	instOperaType[INST_SUBA_IDX]=OPER_IDX;
	instOperaType[INST_SUBA_EXT]=OPER_EXT;

	instOperaType[INST_SUBB_IMM]=OPER_IMM;
	instOperaType[INST_SUBB_DP]=OPER_DP;
	instOperaType[INST_SUBB_IDX]=OPER_IDX;
	instOperaType[INST_SUBB_EXT]=OPER_EXT;

	instOperaType[INST_SUBD_IMM]=OPER_IMM;
	instOperaType[INST_SUBD_DP]=OPER_DP;
	instOperaType[INST_SUBD_IDX]=OPER_IDX;
	instOperaType[INST_SUBD_EXT]=OPER_EXT;

	instOperaType[INST_TST_DP]=OPER_DP;
	instOperaType[INST_TST_IDX]=OPER_IDX;
	instOperaType[INST_TST_EXT]=OPER_EXT;

	instOperaType[INST_BCC_IMM]=OPER_IMM;
	instOperaType[INST_LBCC_IMM16]=OPER_IMM16;

	instOperaType[INST_BCS_IMM]=OPER_IMM;
	instOperaType[INST_LBCS_IMM16]=OPER_IMM16;

	instOperaType[INST_BEQ_IMM]=OPER_IMM;
	instOperaType[INST_LBEQ_IMM16]=OPER_IMM16;

	instOperaType[INST_BGE_IMM]=OPER_IMM;
	instOperaType[INST_LBGE_IMM16]=OPER_IMM16;

	instOperaType[INST_BGT_IMM]=OPER_IMM;
	instOperaType[INST_LBGT_IMM16]=OPER_IMM16;

	instOperaType[INST_BHI_IMM]=OPER_IMM;
	instOperaType[INST_LBHI_IMM16]=OPER_IMM16;

	instOperaType[INST_BLE_IMM]=OPER_IMM;
	instOperaType[INST_LBLE_IMM16]=OPER_IMM16;

	instOperaType[INST_BLS_IMM]=OPER_IMM;
	instOperaType[INST_LBLS_IMM16]=OPER_IMM16;

	instOperaType[INST_BLT_IMM]=OPER_IMM;
	instOperaType[INST_LBLT_IMM16]=OPER_IMM16;

	instOperaType[INST_BMI_IMM]=OPER_IMM;
	instOperaType[INST_LBMI_IMM16]=OPER_IMM16;

	instOperaType[INST_BNE_IMM]=OPER_IMM;
	instOperaType[INST_LBNE_IMM16]=OPER_IMM16;

	instOperaType[INST_BPL_IMM]=OPER_IMM;
	instOperaType[INST_LBPL_IMM16]=OPER_IMM16;

	instOperaType[INST_BRA_IMM]=OPER_IMM;
	instOperaType[INST_LBRA_IMM16]=OPER_IMM16;

	instOperaType[INST_BRN_IMM]=OPER_IMM;
	instOperaType[INST_LBRN_IMM16]=OPER_IMM16;

	instOperaType[INST_BSR_IMM]=OPER_IMM;
	instOperaType[INST_LBSR_IMM16]=OPER_IMM16;

	instOperaType[INST_BVC_IMM]=OPER_IMM;
	instOperaType[INST_LBVC_IMM16]=OPER_IMM16;

	instOperaType[INST_BVS_IMM]=OPER_IMM;
	instOperaType[INST_LBVS_IMM16]=OPER_IMM16;
}

MC6809::Instruction MC6809::FetchInstruction(class MemoryAccess *mem)
{
	Instruction inst;

	inst.opCode=mem->FetchByte(state.PC++);
	inst.clocks=0;
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

	switch(instOperaType[inst.opCode])
	{
	case OPER_IMM:
	case OPER_DP:
		// 1-byte operand
		++inst.length;
		inst.operand[0]=mem->FetchByte(state.PC++);
		break;
	case OPER_EXT:
	case OPER_IMM16:
		// 2-bytes operand
		inst.length+=2;
		inst.operand[0]=mem->FetchByte(state.PC++);
		inst.operand[1]=mem->FetchByte(state.PC++);
		state.PC+=2;
		break;
	case OPER_INHERENT:
		// No operand.
		break;
	case OPER_IDX:
		++inst.length;
		inst.operand[0]=mem->FetchByte(state.PC++);
		inst.indexReg=REG_X+((inst.operand[0]>>5)&3);
		if(0==inst.operand[0]&0x80) // 5-bit offset from REG
		{
			inst.indexType=INDEX_DIR_CONST_OFFSET_FROM_REG;
			inst.offset=(inst.operand[0]&0x1F);
			inst.offset-=(inst.offset&0x10);
			inst.clocks+=1;
		}
		else
		{
			uint8_t idByte=(inst.operand[0]&0b10011111);
			switch(idByte)
			{
			case 0b10000100: // No Offset Direct
				inst.indexType=INDEX_DIR_CONST_OFFSET_FROM_REG;
				inst.offset=0;
				break;
			case 0b10010100: // No Offset Indirect
				inst.clocks+=3;
				inst.indexType=INDEX_INDIR_CONST_OFFSET_FROM_REG;
				inst.offset=0;
				break;
			case 0b10001000: // 8-bit offset from REG Direct
				inst.clocks+=1;
				++inst.length;
				inst.indexType=INDEX_DIR_CONST_OFFSET_FROM_REG;
				inst.operand[1]=mem->FetchByte(state.PC++);
				inst.offset=inst.operand[1];
				inst.offset-=(inst.offset&0x80);
				break;
			case 0b10011000: // 8-bit offset from REG Indirect
				inst.clocks+=4;
				++inst.length;
				inst.indexType=INDEX_INDIR_CONST_OFFSET_FROM_REG;
				inst.operand[1]=mem->FetchByte(state.PC++);
				inst.offset=inst.operand[1];
				inst.offset-=(inst.offset&0x80);
				break;
			case 0b10001001: // 16-bit offset from REG Direct
				inst.clocks+=4;
				inst.length+=2;
				inst.indexType=INDEX_DIR_CONST_OFFSET_FROM_REG;
				inst.operand[1]=mem->FetchByte(state.PC++);
				inst.operand[2]=mem->FetchByte(state.PC++);
				inst.offset=mc6809util::FetchWord(inst.operand[1],inst.operand[2]);
				inst.offset-=(inst.offset&0x8000);
				break;
			case 0b10011001: // 16-bit offset from REG Indirect
				inst.clocks+=7;
				inst.length+=2;
				inst.indexType=INDEX_INDIR_CONST_OFFSET_FROM_REG;
				inst.operand[1]=mem->FetchByte(state.PC++);
				inst.operand[2]=mem->FetchByte(state.PC++);
				inst.offset=mc6809util::FetchWord(inst.operand[1],inst.operand[2]);
				inst.offset-=(inst.offset&0x8000);
				break;
			case 0b10000110: // A reg as offset Direct
				inst.clocks+=1;
				inst.indexType=INDEX_DIR_ACCUM_OFFSET_FROM_REG;
				inst.offset=REG_A;
				break;
			case 0b10010110: // A reg as offset Indirect
				inst.clocks+=4;
				inst.indexType=INDEX_INDIR_ACCUM_OFFSET_FROM_REG;
				inst.offset=REG_A;
				break;
			case 0b10000101: // B reg as offset Direct
				inst.clocks+=1;
				inst.indexType=INDEX_DIR_ACCUM_OFFSET_FROM_REG;
				inst.offset=REG_B;
				break;
			case 0b10010101: // B reg as offset Indirect
				inst.clocks+=4;
				inst.indexType=INDEX_INDIR_ACCUM_OFFSET_FROM_REG;
				inst.offset=REG_B;
				break;
			case 0b10001011: // D reg as offset Direct
				inst.clocks+=4;
				inst.indexType=INDEX_DIR_ACCUM_OFFSET_FROM_REG;
				inst.offset=REG_D;
				break;
			case 0b10011011: // D reg as offset Indirect
				inst.clocks+=7;
				inst.indexType=INDEX_DIR_ACCUM_OFFSET_FROM_REG;
				inst.offset=REG_D;
				break;
			case 0b10000000: // Post-Inc by 1 Direct
				inst.clocks+=2;
				inst.indexType=INDEX_DIR_POST_INC_1;
				break;
			case 0b10000001: // Post-Inc by 2 Direct
				inst.clocks+=3;
				inst.indexType=INDEX_DIR_POST_INC_2;
				break;
			case 0b10010001: // Post-Inc by 2 Indirect
				inst.clocks+=6;
				inst.indexType=INDEX_INDIR_POST_INC_2;
				break;
			case 0b10000010: // Pre-Dec by 1 Direct
				inst.clocks+=2;
				inst.indexType=INDEX_DIR_PRE_DEC_1;
				break;
			case 0b10000011: // Pre-Dec by 2 Direct
				inst.indexType=INDEX_DIR_PRE_DEC_2;
				inst.clocks+=3;
				break;
			case 0b10010011: // Pre-Dec by 2 Indirect
				inst.clocks+=6;
				inst.indexType=INDEX_INDIR_PRE_DEC_2;
				break;
			case 0b10001100: // 8-bit offset from PC Direct
				inst.clocks+=1;
				++inst.length;
				inst.indexType=INDEX_DIR_CONST_OFFSET_FROM_REG;
				inst.indexReg=REG_PC;
				inst.operand[1]=mem->FetchByte(state.PC++);
				inst.offset=inst.operand[1];
				inst.offset-=(inst.offset&0x80);
				break;
			case 0b10011100: // 8-bit offset from PC Indirect
				inst.clocks+=4;
				++inst.length;
				inst.indexType=INDEX_INDIR_CONST_OFFSET_FROM_REG;
				inst.indexReg=REG_PC;
				inst.operand[1]=mem->FetchByte(state.PC++);
				inst.offset=inst.operand[1];
				inst.offset-=(inst.offset&0x80);
				break;
			case 0b10001101: // 16-bit offset from PC Direct
				inst.clocks+=5;
				inst.length+=2;
				inst.indexType=INDEX_DIR_CONST_OFFSET_FROM_REG;
				inst.indexReg=REG_PC;
				inst.operand[1]=mem->FetchByte(state.PC++);
				inst.operand[2]=mem->FetchByte(state.PC++);
				inst.offset=mc6809util::FetchWord(inst.operand[1],inst.operand[2]);
				inst.offset-=(inst.offset&0x8000);
				break;
			case 0b10011101: // 16-bit offset from PC Indirect
				inst.clocks+=8;
				inst.length+=2;
				inst.indexType=INDEX_INDIR_CONST_OFFSET_FROM_REG;
				inst.indexReg=REG_PC;
				inst.operand[1]=mem->FetchByte(state.PC++);
				inst.operand[2]=mem->FetchByte(state.PC++);
				inst.offset=mc6809util::FetchWord(inst.operand[1],inst.operand[2]);
				inst.offset-=(inst.offset&0x8000);
				break;
			case 0b10011111: // Extended Indirect
				inst.clocks+=5;
				inst.indexType=INDEX_EXTENDED_INDIR;
				inst.operand[1]=mem->FetchByte(state.PC++);
				inst.operand[2]=mem->FetchByte(state.PC++);
				inst.offset=mc6809util::FetchWord(inst.operand[1],inst.operand[2]);
				break;
			}
		}
		break;
	}

	return inst;
}
