#include <iostream>
#include "mc6809.h"
#include "mc6809util.h"



MC6809::MC6809(VMBase *vmBase) : Device(vmBase)
{
	regBits[REG_CC]=8;
	regBits[REG_A]=8;
	regBits[REG_B]=8;
	regBits[REG_DP]=8;

	regBits[REG_X]=16;
	regBits[REG_Y]=16;
	regBits[REG_U]=16;
	regBits[REG_S]=16;

	regBits[REG_PC]=16;
	regBits[REG_D]=16;

	for(auto &t : instOperaType)
	{
		t=OPER_INHERENT; // No operand
	}

	instOperaType[INST_MULTI_BYTE_10]=OPER_MULTI_BYTE_10;
	instOperaType[INST_MULTI_BYTE_11]=OPER_MULTI_BYTE_11;

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

	instOperaType[INST_ADDD_IMM]=OPER_IMM16;
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

	instOperaType[INST_CMPD_IMM]=OPER_IMM16;
	instOperaType[INST_CMPD_DP]=OPER_DP;
	instOperaType[INST_CMPD_IDX]=OPER_IDX;
	instOperaType[INST_CMPD_EXT]=OPER_EXT;

	instOperaType[INST_CMPS_IMM]=OPER_IMM16;
	instOperaType[INST_CMPS_DP]=OPER_DP;
	instOperaType[INST_CMPS_IDX]=OPER_IDX;
	instOperaType[INST_CMPS_EXT]=OPER_EXT;

	instOperaType[INST_CMPU_IMM]=OPER_IMM16;
	instOperaType[INST_CMPU_DP]=OPER_DP;
	instOperaType[INST_CMPU_IDX]=OPER_IDX;
	instOperaType[INST_CMPU_EXT]=OPER_EXT;

	instOperaType[INST_CMPX_IMM]=OPER_IMM16;
	instOperaType[INST_CMPX_DP]=OPER_DP;
	instOperaType[INST_CMPX_IDX]=OPER_IDX;
	instOperaType[INST_CMPX_EXT]=OPER_EXT;

	instOperaType[INST_CMPY_IMM]=OPER_IMM16;
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

	instOperaType[INST_EXG_REG]=OPER_REG;

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

	instOperaType[INST_LDD_IMM]=OPER_IMM16;
	instOperaType[INST_LDD_DP]=OPER_DP;
	instOperaType[INST_LDD_IDX]=OPER_IDX;
	instOperaType[INST_LDD_EXT]=OPER_EXT;

	instOperaType[INST_LDS_IMM]=OPER_IMM16;
	instOperaType[INST_LDS_DP]=OPER_DP;
	instOperaType[INST_LDS_IDX]=OPER_IDX;
	instOperaType[INST_LDS_EXT]=OPER_EXT;

	instOperaType[INST_LDU_IMM]=OPER_IMM16;
	instOperaType[INST_LDU_DP]=OPER_DP;
	instOperaType[INST_LDU_IDX]=OPER_IDX;
	instOperaType[INST_LDU_EXT]=OPER_EXT;

	instOperaType[INST_LDX_IMM]=OPER_IMM16;
	instOperaType[INST_LDX_DP]=OPER_DP;
	instOperaType[INST_LDX_IDX]=OPER_IDX;
	instOperaType[INST_LDX_EXT]=OPER_EXT;

	instOperaType[INST_LDY_IMM]=OPER_IMM16;
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

	instOperaType[INST_PSHS_REG]=OPER_REG;
	instOperaType[INST_PSHU_REG]=OPER_REG;
	instOperaType[INST_PULS_REG]=OPER_REG;
	instOperaType[INST_PULU_REG]=OPER_REG;

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

	instOperaType[INST_STA_DP]=OPER_DP;
	instOperaType[INST_STA_IDX]=OPER_IDX;
	instOperaType[INST_STA_EXT]=OPER_EXT;

	instOperaType[INST_STB_DP]=OPER_DP;
	instOperaType[INST_STB_IDX]=OPER_IDX;
	instOperaType[INST_STB_EXT]=OPER_EXT;

	instOperaType[INST_STD_DP]=OPER_DP;
	instOperaType[INST_STD_IDX]=OPER_IDX;
	instOperaType[INST_STD_EXT]=OPER_EXT;

	instOperaType[INST_STS_DP]=OPER_DP;
	instOperaType[INST_STS_IDX]=OPER_IDX;
	instOperaType[INST_STS_EXT]=OPER_EXT;

	instOperaType[INST_STU_DP]=OPER_DP;
	instOperaType[INST_STU_IDX]=OPER_IDX;
	instOperaType[INST_STU_EXT]=OPER_EXT;

	instOperaType[INST_STX_DP]=OPER_DP;
	instOperaType[INST_STX_IDX]=OPER_IDX;
	instOperaType[INST_STX_EXT]=OPER_EXT;

	instOperaType[INST_STY_DP]=OPER_DP;
	instOperaType[INST_STY_IDX]=OPER_IDX;
	instOperaType[INST_STY_EXT]=OPER_EXT;

	instOperaType[INST_SUBA_IMM]=OPER_IMM;
	instOperaType[INST_SUBA_DP]=OPER_DP;
	instOperaType[INST_SUBA_IDX]=OPER_IDX;
	instOperaType[INST_SUBA_EXT]=OPER_EXT;

	instOperaType[INST_SUBB_IMM]=OPER_IMM;
	instOperaType[INST_SUBB_DP]=OPER_DP;
	instOperaType[INST_SUBB_IDX]=OPER_IDX;
	instOperaType[INST_SUBB_EXT]=OPER_EXT;

	instOperaType[INST_SUBD_IMM]=OPER_IMM16;
	instOperaType[INST_SUBD_DP]=OPER_DP;
	instOperaType[INST_SUBD_IDX]=OPER_IDX;
	instOperaType[INST_SUBD_EXT]=OPER_EXT;

	instOperaType[INST_TFR_REG]=OPER_REG;

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
	instOperaType[INST_LBRA_IMM16_ALT]=OPER_IMM16;

	instOperaType[INST_BRN_IMM]=OPER_IMM;
	instOperaType[INST_LBRN_IMM16]=OPER_IMM16;

	instOperaType[INST_BSR_IMM]=OPER_IMM;
	instOperaType[INST_LBSR_IMM16]=OPER_IMM16;

	instOperaType[INST_BVC_IMM]=OPER_IMM;
	instOperaType[INST_LBVC_IMM16]=OPER_IMM16;

	instOperaType[INST_BVS_IMM]=OPER_IMM;
	instOperaType[INST_LBVS_IMM16]=OPER_IMM16;



	instClock[INST_ABX]=3;

	instClock[INST_ADCA_IMM]=2;
	instClock[INST_ADCA_DP]=4;
	instClock[INST_ADCA_IDX]=4;
	instClock[INST_ADCA_EXT]=5;

	instClock[INST_ADCB_IMM]=2;
	instClock[INST_ADCB_DP]=4;
	instClock[INST_ADCB_IDX]=4;
	instClock[INST_ADCB_EXT]=5;

	instClock[INST_ADDA_IMM]=2;
	instClock[INST_ADDA_DP]=4;
	instClock[INST_ADDA_IDX]=4;
	instClock[INST_ADDA_EXT]=5;

	instClock[INST_ADDB_IMM]=2;
	instClock[INST_ADDB_DP]=4;
	instClock[INST_ADDB_IDX]=4;
	instClock[INST_ADDB_EXT]=5;

	instClock[INST_ADDD_IMM]=4;
	instClock[INST_ADDD_DP]=6;
	instClock[INST_ADDD_IDX]=6;
	instClock[INST_ADDD_EXT]=7;

	instClock[INST_ANDA_IMM]=2;
	instClock[INST_ANDA_DP]=4;
	instClock[INST_ANDA_IDX]=4;
	instClock[INST_ANDA_EXT]=5;

	instClock[INST_ANDB_IMM]=2;
	instClock[INST_ANDB_DP]=4;
	instClock[INST_ANDB_IDX]=4;
	instClock[INST_ANDB_EXT]=5;

	instClock[INST_ANDCC_IMM]=3;

	instClock[INST_ASRA]=2;
	instClock[INST_ASRB]=2;

	instClock[INST_ASR_DP]=6;
	instClock[INST_ASR_IDX]=6;
	instClock[INST_ASR_EXT]=7;

	instClock[INST_BITA_IMM]=2;
	instClock[INST_BITA_DP]=4;
	instClock[INST_BITA_IDX]=4;
	instClock[INST_BITA_EXT]=5;

	instClock[INST_BITB_IMM]=2;
	instClock[INST_BITB_DP]=4;
	instClock[INST_BITB_IDX]=4;
	instClock[INST_BITB_EXT]=5;

	instClock[INST_CLRA]=2;
	instClock[INST_CLRB]=2;

	instClock[INST_CLR_DP]=6;
	instClock[INST_CLR_IDX]=6;
	instClock[INST_CLR_EXT]=7;

	instClock[INST_CMPA_IMM]=2;
	instClock[INST_CMPA_DP]=4;
	instClock[INST_CMPA_IDX]=4;
	instClock[INST_CMPA_EXT]=5;

	instClock[INST_CMPB_IMM]=2;
	instClock[INST_CMPB_DP]=4;
	instClock[INST_CMPB_IDX]=4;
	instClock[INST_CMPB_EXT]=5;

	instClock[INST_CMPD_IMM]=5;
	instClock[INST_CMPD_DP]=7;
	instClock[INST_CMPD_IDX]=7;
	instClock[INST_CMPD_EXT]=8;

	instClock[INST_CMPS_IMM]=5;
	instClock[INST_CMPS_DP]=7;
	instClock[INST_CMPS_IDX]=7;
	instClock[INST_CMPS_EXT]=8;

	instClock[INST_CMPU_IMM]=5;
	instClock[INST_CMPU_DP]=7;
	instClock[INST_CMPU_IDX]=7;
	instClock[INST_CMPU_EXT]=8;

	instClock[INST_CMPX_IMM]=4;
	instClock[INST_CMPX_DP]=6;
	instClock[INST_CMPX_IDX]=6;
	instClock[INST_CMPX_EXT]=7;

	instClock[INST_CMPY_IMM]=5;
	instClock[INST_CMPY_DP]=7;
	instClock[INST_CMPY_IDX]=7;
	instClock[INST_CMPY_EXT]=8;

	instClock[INST_COMA]=2;
	instClock[INST_COMB]=2;

	instClock[INST_COM_DP]=6;
	instClock[INST_COM_IDX]=6;
	instClock[INST_COM_EXT]=7;

	instClock[INST_CWAI_IMM]=22;

	instClock[INST_DAA]=2;

	instClock[INST_DECA]=2;
	instClock[INST_DECB]=2;

	instClock[INST_DEC_DP]=6;
	instClock[INST_DEC_IDX]=6;
	instClock[INST_DEC_EXT]=7;

	instClock[INST_EORA_IMM]=2;
	instClock[INST_EORA_DP]=4;
	instClock[INST_EORA_IDX]=4;
	instClock[INST_EORA_EXT]=5;

	instClock[INST_EORB_IMM]=2;
	instClock[INST_EORB_DP]=4;
	instClock[INST_EORB_IDX]=4;
	instClock[INST_EORB_EXT]=5;

	instClock[INST_EXG_REG]=8;

	instClock[INST_INCA]=2;
	instClock[INST_INCB]=2;

	instClock[INST_INC_DP]=6;
	instClock[INST_INC_IDX]=6;
	instClock[INST_INC_EXT]=7;

	instClock[INST_JMP_DP]=3;
	instClock[INST_JMP_IDX]=3;
	instClock[INST_JMP_EXT]=4;

	instClock[INST_JSR_DP]=7;
	instClock[INST_JSR_IDX]=7;
	instClock[INST_JSR_EXT]=8;

	instClock[INST_LDA_IMM]=2;
	instClock[INST_LDA_DP]=4;
	instClock[INST_LDA_IDX]=4;
	instClock[INST_LDA_EXT]=5;

	instClock[INST_LDB_IMM]=2;
	instClock[INST_LDB_DP]=4;
	instClock[INST_LDB_IDX]=4;
	instClock[INST_LDB_EXT]=5;

	instClock[INST_LDD_IMM]=3;
	instClock[INST_LDD_DP]=5;
	instClock[INST_LDD_IDX]=5;
	instClock[INST_LDD_EXT]=6;

	instClock[INST_LDS_IMM]=4;
	instClock[INST_LDS_DP]=6;
	instClock[INST_LDS_IDX]=6;
	instClock[INST_LDS_EXT]=7;

	instClock[INST_LDU_IMM]=3;
	instClock[INST_LDU_DP]=5;
	instClock[INST_LDU_IDX]=5;
	instClock[INST_LDU_EXT]=6;

	instClock[INST_LDX_IMM]=3;
	instClock[INST_LDX_DP]=5;
	instClock[INST_LDX_IDX]=5;
	instClock[INST_LDX_EXT]=6;

	instClock[INST_LDY_IMM]=4;
	instClock[INST_LDY_DP]=6;
	instClock[INST_LDY_IDX]=6;
	instClock[INST_LDY_EXT]=7;

	instClock[INST_LEAS_IDX]=4;
	instClock[INST_LEAU_IDX]=4;
	instClock[INST_LEAX_IDX]=4;
	instClock[INST_LEAY_IDX]=4;

	instClock[INST_LSLA]=2;
	instClock[INST_LSLB]=2;

	instClock[INST_LSL_DP]=6;
	instClock[INST_LSL_IDX]=6;
	instClock[INST_LSL_EXT]=7;

	instClock[INST_LSRA]=2;
	instClock[INST_LSRB]=2;

	instClock[INST_LSR_DP]=6;
	instClock[INST_LSR_IDX]=6;
	instClock[INST_LSR_EXT]=7;

	instClock[INST_MUL]=11;

	instClock[INST_NEGA]=2;
	instClock[INST_NEGB]=2;

	instClock[INST_NEG_DP]=6;
	instClock[INST_NEG_IDX]=6;
	instClock[INST_NEG_EXT]=7;

	instClock[INST_NOP]=2;

	instClock[INST_ORA_IMM]=2;
	instClock[INST_ORA_DP]=4;
	instClock[INST_ORA_IDX]=4;
	instClock[INST_ORA_EXT]=5;

	instClock[INST_ORB_IMM]=2;
	instClock[INST_ORB_DP]=4;
	instClock[INST_ORB_IDX]=4;
	instClock[INST_ORB_EXT]=5;

	instClock[INST_ORCC_IMM]=3;

	instClock[INST_PSHS_REG]=5; // Plus number of bytes transferred
	instClock[INST_PSHU_REG]=5;
	instClock[INST_PULS_REG]=5; // Plus number of bytes transferred
	instClock[INST_PULU_REG]=5;

	instClock[INST_ROLA]=2;
	instClock[INST_ROLB]=2;

	instClock[INST_ROL_DP]=6;
	instClock[INST_ROL_IDX]=6;
	instClock[INST_ROL_EXT]=7;

	instClock[INST_RORA]=2;
	instClock[INST_RORB]=2;

	instClock[INST_ROR_DP]=6;
	instClock[INST_ROR_IDX]=6;
	instClock[INST_ROR_EXT]=7;

	instClock[INST_RTI]=15; // If E=0 6 clocks, otherwise 15 clocks.
	instClock[INST_RTS]=5;

	instClock[INST_SBCA_IMM]=2;
	instClock[INST_SBCA_DP]=4;
	instClock[INST_SBCA_IDX]=4;
	instClock[INST_SBCA_EXT]=5;

	instClock[INST_SBCB_IMM]=2;
	instClock[INST_SBCB_DP]=4;
	instClock[INST_SBCB_IDX]=4;
	instClock[INST_SBCB_EXT]=5;

	instClock[INST_SEX]=2;

	instClock[INST_STA_DP]=4;
	instClock[INST_STA_IDX]=4;
	instClock[INST_STA_EXT]=5;

	instClock[INST_STB_DP]=4;
	instClock[INST_STB_IDX]=4;
	instClock[INST_STB_EXT]=5;

	instClock[INST_STD_DP]=5;
	instClock[INST_STD_IDX]=5;
	instClock[INST_STD_EXT]=6;

	instClock[INST_STS_DP]=6;
	instClock[INST_STS_IDX]=6;
	instClock[INST_STS_EXT]=7;

	instClock[INST_STU_DP]=5;
	instClock[INST_STU_IDX]=5;
	instClock[INST_STU_EXT]=6;

	instClock[INST_STX_DP]=5;
	instClock[INST_STX_IDX]=5;
	instClock[INST_STX_EXT]=6;

	instClock[INST_STY_DP]=6;
	instClock[INST_STY_IDX]=6;
	instClock[INST_STY_EXT]=7;

	instClock[INST_SUBA_IMM]=2;
	instClock[INST_SUBA_DP]=4;
	instClock[INST_SUBA_IDX]=4;
	instClock[INST_SUBA_EXT]=5;

	instClock[INST_SUBB_IMM]=2;
	instClock[INST_SUBB_DP]=4;
	instClock[INST_SUBB_IDX]=4;
	instClock[INST_SUBB_EXT]=5;

	instClock[INST_SUBD_IMM]=4;
	instClock[INST_SUBD_DP]=6;
	instClock[INST_SUBD_IDX]=6;
	instClock[INST_SUBD_EXT]=7;

	instClock[INST_SWI]=19;
	instClock[INST_SWI2]=20;
	instClock[INST_SWI3]=20;

	instClock[INST_SYNC]=4;

	instClock[INST_TFR_REG]=6;

	instClock[INST_TSTA]=2;
	instClock[INST_TSTB]=2;

	instClock[INST_TST_DP]=6;
	instClock[INST_TST_IDX]=6;
	instClock[INST_TST_EXT]=7;

	instClock[INST_BCC_IMM]=3;
	instClock[INST_LBCC_IMM16]=5; // +1 clock if it jumps

	instClock[INST_BCS_IMM]=3;
	instClock[INST_LBCS_IMM16]=5; // +1 clock if it jumps

	instClock[INST_BEQ_IMM]=3;
	instClock[INST_LBEQ_IMM16]=5; // +1 clock if it jumps

	instClock[INST_BGE_IMM]=3;
	instClock[INST_LBGE_IMM16]=5; // +1 clock if it jumps

	instClock[INST_BGT_IMM]=3;
	instClock[INST_LBGT_IMM16]=5; // +1 clock if it jumps

	instClock[INST_BHI_IMM]=3;
	instClock[INST_LBHI_IMM16]=5; // +1 clock if it jumps


	instClock[INST_BLE_IMM]=3;
	instClock[INST_LBLE_IMM16]=5; // +1 clock if it jumps

	instClock[INST_BLS_IMM]=3;
	instClock[INST_LBLS_IMM16]=5; // +1 clock if it jumps

	instClock[INST_BLT_IMM]=3;
	instClock[INST_LBLT_IMM16]=5; // +1 clock if it jumps

	instClock[INST_BMI_IMM]=3;
	instClock[INST_LBMI_IMM16]=5; // +1 clock if it jumps

	instClock[INST_BNE_IMM]=3;
	instClock[INST_LBNE_IMM16]=5; // +1 clock if it jumps

	instClock[INST_BPL_IMM]=3;
	instClock[INST_LBPL_IMM16]=5; // +1 clock if it jumps

	instClock[INST_BRA_IMM]=3;
	instClock[INST_LBRA_IMM16]=5;
	instClock[INST_LBRA_IMM16_ALT]=5;

	instClock[INST_BRN_IMM]=3;
	instClock[INST_LBRN_IMM16]=5;

	instClock[INST_BSR_IMM]=7;
	instClock[INST_LBSR_IMM16]=9;

	instClock[INST_BVC_IMM]=3;
	instClock[INST_LBVC_IMM16]=5; // +1 clock if it jumps

	instClock[INST_BVS_IMM]=3;
	instClock[INST_LBVS_IMM16]=5; // +1 clock if it jumps


	instLabel[INST_ABX]=          "ABX";

	instLabel[INST_ADCA_IMM]=     "ADCA";
	instLabel[INST_ADCA_DP]=      "ADCA";
	instLabel[INST_ADCA_IDX]=     "ADCA";
	instLabel[INST_ADCA_EXT]=     "ADCA";

	instLabel[INST_ADCB_IMM]=     "ADCB";
	instLabel[INST_ADCB_DP]=      "ADCB";
	instLabel[INST_ADCB_IDX]=     "ADCB";
	instLabel[INST_ADCB_EXT]=     "ADCB";

	instLabel[INST_ADDA_IMM]=     "ADDA";
	instLabel[INST_ADDA_DP]=      "ADDA";
	instLabel[INST_ADDA_IDX]=     "ADDA";
	instLabel[INST_ADDA_EXT]=     "ADDA";

	instLabel[INST_ADDB_IMM]=     "ADDB";
	instLabel[INST_ADDB_DP]=      "ADDB";
	instLabel[INST_ADDB_IDX]=     "ADDB";
	instLabel[INST_ADDB_EXT]=     "ADDB";

	instLabel[INST_ADDD_IMM]=     "ADDD";
	instLabel[INST_ADDD_DP]=      "ADDD";
	instLabel[INST_ADDD_IDX]=     "ADDD";
	instLabel[INST_ADDD_EXT]=     "ADDD";

	instLabel[INST_ANDA_IMM]=     "ANDA";
	instLabel[INST_ANDA_DP]=      "ANDA";
	instLabel[INST_ANDA_IDX]=     "ANDA";
	instLabel[INST_ANDA_EXT]=     "ANDA";

	instLabel[INST_ANDB_IMM]=     "ANDB";
	instLabel[INST_ANDB_DP]=      "ANDB";
	instLabel[INST_ANDB_IDX]=     "ANDB";
	instLabel[INST_ANDB_EXT]=     "ANDB";

	instLabel[INST_ANDCC_IMM]=    "ANDCC";

	instLabel[INST_ASRA]=         "ASRA";
	instLabel[INST_ASRB]=         "ASRB";

	instLabel[INST_ASR_DP]=       "ASR";
	instLabel[INST_ASR_IDX]=      "ASR";
	instLabel[INST_ASR_EXT]=      "ASR";

	instLabel[INST_BITA_IMM]=     "BITA";
	instLabel[INST_BITA_DP]=      "BITA";
	instLabel[INST_BITA_IDX]=     "BITA";
	instLabel[INST_BITA_EXT]=     "BITA";

	instLabel[INST_BITB_IMM]=     "BITB";
	instLabel[INST_BITB_DP]=      "BITB";
	instLabel[INST_BITB_IDX]=     "BITB";
	instLabel[INST_BITB_EXT]=     "BITB";

	instLabel[INST_CLRA]=         "CLRA";
	instLabel[INST_CLRB]=         "CLRB";

	instLabel[INST_CLR_DP]=       "CLR";
	instLabel[INST_CLR_IDX]=      "CLR";
	instLabel[INST_CLR_EXT]=      "CLR";

	instLabel[INST_CMPA_IMM]=     "CMPA";
	instLabel[INST_CMPA_DP]=      "CMPA";
	instLabel[INST_CMPA_IDX]=     "CMPA";
	instLabel[INST_CMPA_EXT]=     "CMPA";

	instLabel[INST_CMPB_IMM]=     "CMPB";
	instLabel[INST_CMPB_DP]=      "CMPB";
	instLabel[INST_CMPB_IDX]=     "CMPB";
	instLabel[INST_CMPB_EXT]=     "CMPB";

	instLabel[INST_CMPD_IMM]=     "CMPD";
	instLabel[INST_CMPD_DP]=      "CMPD";
	instLabel[INST_CMPD_IDX]=     "CMPD";
	instLabel[INST_CMPD_EXT]=     "CMPD";

	instLabel[INST_CMPS_IMM]=     "CMPS";
	instLabel[INST_CMPS_DP]=      "CMPS";
	instLabel[INST_CMPS_IDX]=     "CMPS";
	instLabel[INST_CMPS_EXT]=     "CMPS";

	instLabel[INST_CMPU_IMM]=     "CMPU";
	instLabel[INST_CMPU_DP]=      "CMPU";
	instLabel[INST_CMPU_IDX]=     "CMPU";
	instLabel[INST_CMPU_EXT]=     "CMPU";

	instLabel[INST_CMPX_IMM]=     "CMPX";
	instLabel[INST_CMPX_DP]=      "CMPX";
	instLabel[INST_CMPX_IDX]=     "CMPX";
	instLabel[INST_CMPX_EXT]=     "CMPX";

	instLabel[INST_CMPY_IMM]=     "CMPY";
	instLabel[INST_CMPY_DP]=      "CMPY";
	instLabel[INST_CMPY_IDX]=     "CMPY";
	instLabel[INST_CMPY_EXT]=     "CMPY";

	instLabel[INST_COMA]=         "COMA";
	instLabel[INST_COMB]=         "COMB";

	instLabel[INST_COM_DP]=       "COM";
	instLabel[INST_COM_IDX]=      "COM";
	instLabel[INST_COM_EXT]=      "COM";

	instLabel[INST_CWAI_IMM]=     "CWAI";

	instLabel[INST_DAA]=          "DAA";

	instLabel[INST_DECA]=         "DECA";
	instLabel[INST_DECB]=         "DECB";

	instLabel[INST_DEC_DP]=       "DEC";
	instLabel[INST_DEC_IDX]=      "DEC";
	instLabel[INST_DEC_EXT]=      "DEC";

	instLabel[INST_EORA_IMM]=     "EORA";
	instLabel[INST_EORA_DP]=      "EORA";
	instLabel[INST_EORA_IDX]=     "EORA";
	instLabel[INST_EORA_EXT]=     "EORA";

	instLabel[INST_EORB_IMM]=     "EORB";
	instLabel[INST_EORB_DP]=      "EORB";
	instLabel[INST_EORB_IDX]=     "EORB";
	instLabel[INST_EORB_EXT]=     "EORB";

	instLabel[INST_EXG_REG]=      "EXG";

	instLabel[INST_INCA]=         "INCA";
	instLabel[INST_INCB]=         "INCB";

	instLabel[INST_INC_DP]=       "INC";
	instLabel[INST_INC_IDX]=      "INC";
	instLabel[INST_INC_EXT]=      "INC";

	instLabel[INST_JMP_DP]=       "JMP";
	instLabel[INST_JMP_IDX]=      "JMP";
	instLabel[INST_JMP_EXT]=      "JMP";

	instLabel[INST_JSR_DP]=       "JSR";
	instLabel[INST_JSR_IDX]=      "JSR";
	instLabel[INST_JSR_EXT]=      "JSR";

	instLabel[INST_LDA_IMM]=      "LDA";
	instLabel[INST_LDA_DP]=       "LDA";
	instLabel[INST_LDA_IDX]=      "LDA";
	instLabel[INST_LDA_EXT]=      "LDA";

	instLabel[INST_LDB_IMM]=      "LDB";
	instLabel[INST_LDB_DP]=       "LDB";
	instLabel[INST_LDB_IDX]=      "LDB";
	instLabel[INST_LDB_EXT]=      "LDB";

	instLabel[INST_LDD_IMM]=      "LDD";
	instLabel[INST_LDD_DP]=       "LDD";
	instLabel[INST_LDD_IDX]=      "LDD";
	instLabel[INST_LDD_EXT]=      "LDD";

	instLabel[INST_LDS_IMM]=      "LDS";
	instLabel[INST_LDS_DP]=       "LDS";
	instLabel[INST_LDS_IDX]=      "LDS";
	instLabel[INST_LDS_EXT]=      "LDS";

	instLabel[INST_LDU_IMM]=      "LDU";
	instLabel[INST_LDU_DP]=       "LDU";
	instLabel[INST_LDU_IDX]=      "LDU";
	instLabel[INST_LDU_EXT]=      "LDU";

	instLabel[INST_LDX_IMM]=      "LDX";
	instLabel[INST_LDX_DP]=       "LDX";
	instLabel[INST_LDX_IDX]=      "LDX";
	instLabel[INST_LDX_EXT]=      "LDX";

	instLabel[INST_LDY_IMM]=      "LDY";
	instLabel[INST_LDY_DP]=       "LDY";
	instLabel[INST_LDY_IDX]=      "LDY";
	instLabel[INST_LDY_EXT]=      "LDY";

	instLabel[INST_LEAS_IDX]=     "LEAS";
	instLabel[INST_LEAU_IDX]=     "LEAU";
	instLabel[INST_LEAX_IDX]=     "LEAX";
	instLabel[INST_LEAY_IDX]=     "LEAY";

	instLabel[INST_LSLA]=         "LSLA";
	instLabel[INST_LSLB]=         "LSLB";

	instLabel[INST_LSL_DP]=       "LSL";
	instLabel[INST_LSL_IDX]=      "LSL";
	instLabel[INST_LSL_EXT]=      "LSL";

	instLabel[INST_LSRA]=         "LSRA";
	instLabel[INST_LSRB]=         "LSRB";

	instLabel[INST_LSR_DP]=       "LSR";
	instLabel[INST_LSR_IDX]=      "LSR";
	instLabel[INST_LSR_EXT]=      "LSR";

	instLabel[INST_MUL]=          "MUL";

	instLabel[INST_NEGA]=         "NEGA";
	instLabel[INST_NEGB]=         "NEGB";

	instLabel[INST_NEG_DP]=       "NEG";
	instLabel[INST_NEG_IDX]=      "NEG";
	instLabel[INST_NEG_EXT]=      "NEG";

	instLabel[INST_NOP]=          "NOP";

	instLabel[INST_ORA_IMM]=      "ORA";
	instLabel[INST_ORA_DP]=       "ORA";
	instLabel[INST_ORA_IDX]=      "ORA";
	instLabel[INST_ORA_EXT]=      "ORA";

	instLabel[INST_ORB_IMM]=      "ORB";
	instLabel[INST_ORB_DP]=       "ORB";
	instLabel[INST_ORB_IDX]=      "ORB";
	instLabel[INST_ORB_EXT]=      "ORB";

	instLabel[INST_ORCC_IMM]=     "ORCC";

	instLabel[INST_PSHS_REG]=     "PSHS";
	instLabel[INST_PSHU_REG]=     "PSHU";
	instLabel[INST_PULS_REG]=     "PULS";
	instLabel[INST_PULU_REG]=     "PULU";

	instLabel[INST_ROLA]=         "ROLA";
	instLabel[INST_ROLB]=         "ROLB";

	instLabel[INST_ROL_DP]=       "ROL";
	instLabel[INST_ROL_IDX]=      "ROL";
	instLabel[INST_ROL_EXT]=      "ROL";

	instLabel[INST_RORA]=         "RORA";
	instLabel[INST_RORB]=         "RORB";

	instLabel[INST_ROR_DP]=       "ROR";
	instLabel[INST_ROR_IDX]=      "ROR";
	instLabel[INST_ROR_EXT]=      "ROR";

	instLabel[INST_RTI]=          "RTI";
	instLabel[INST_RTS]=          "RTS";

	instLabel[INST_SBCA_IMM]=     "SBCA";
	instLabel[INST_SBCA_DP]=      "SBCA";
	instLabel[INST_SBCA_IDX]=     "SBCA";
	instLabel[INST_SBCA_EXT]=     "SBCA";

	instLabel[INST_SBCB_IMM]=     "SBCB";
	instLabel[INST_SBCB_DP]=      "SBCB";
	instLabel[INST_SBCB_IDX]=     "SBCB";
	instLabel[INST_SBCB_EXT]=     "SBCB";

	instLabel[INST_SEX]=          "SEX";

	instLabel[INST_STA_DP]=       "STA";
	instLabel[INST_STA_IDX]=      "STA";
	instLabel[INST_STA_EXT]=      "STA";

	instLabel[INST_STB_DP]=       "STB";
	instLabel[INST_STB_IDX]=      "STB";
	instLabel[INST_STB_EXT]=      "STB";

	instLabel[INST_STD_DP]=       "STD";
	instLabel[INST_STD_IDX]=      "STD";
	instLabel[INST_STD_EXT]=      "STD";

	instLabel[INST_STS_DP]=       "STS";
	instLabel[INST_STS_IDX]=      "STS";
	instLabel[INST_STS_EXT]=      "STS";

	instLabel[INST_STU_DP]=       "STU";
	instLabel[INST_STU_IDX]=      "STU";
	instLabel[INST_STU_EXT]=      "STU";

	instLabel[INST_STX_DP]=       "STX";
	instLabel[INST_STX_IDX]=      "STX";
	instLabel[INST_STX_EXT]=      "STX";

	instLabel[INST_STY_DP]=       "STY";
	instLabel[INST_STY_IDX]=      "STY";
	instLabel[INST_STY_EXT]=      "STY";

	instLabel[INST_SUBA_IMM]=     "SUBA";
	instLabel[INST_SUBA_DP]=      "SUBA";
	instLabel[INST_SUBA_IDX]=     "SUBA";
	instLabel[INST_SUBA_EXT]=     "SUBA";

	instLabel[INST_SUBB_IMM]=     "SUBB";
	instLabel[INST_SUBB_DP]=      "SUBB";
	instLabel[INST_SUBB_IDX]=     "SUBB";
	instLabel[INST_SUBB_EXT]=     "SUBB";

	instLabel[INST_SUBD_IMM]=     "SUBD";
	instLabel[INST_SUBD_DP]=      "SUBD";
	instLabel[INST_SUBD_IDX]=     "SUBD";
	instLabel[INST_SUBD_EXT]=     "SUBD";

	instLabel[INST_SWI]=          "SWI";
	instLabel[INST_SWI2]=         "SWI2";
	instLabel[INST_SWI3]=         "SWI3";

	instLabel[INST_SYNC]=         "SYNC";

	instLabel[INST_TFR_REG]=      "TFR";

	instLabel[INST_TSTA]=         "TSTA";
	instLabel[INST_TSTB]=         "TSTB";

	instLabel[INST_TST_DP]=       "TST";
	instLabel[INST_TST_IDX]=      "TST";
	instLabel[INST_TST_EXT]=      "TST";

	instLabel[INST_BCC_IMM]=      "BCC";
	instLabel[INST_LBCC_IMM16]=   "LBCC";

	instLabel[INST_BCS_IMM]=      "BCS";
	instLabel[INST_LBCS_IMM16]=   "LBCS";

	instLabel[INST_BEQ_IMM]=      "BEQ";
	instLabel[INST_LBEQ_IMM16]=   "LBEQ";

	instLabel[INST_BGE_IMM]=      "BGE";
	instLabel[INST_LBGE_IMM16]=   "LBGE";

	instLabel[INST_BGT_IMM]=      "BGT";
	instLabel[INST_LBGT_IMM16]=   "LBGT";

	instLabel[INST_BHI_IMM]=      "BHI";
	instLabel[INST_LBHI_IMM16]=   "LBHI";

	instLabel[INST_BLE_IMM]=      "BLE";
	instLabel[INST_LBLE_IMM16]=   "LBLE";

	instLabel[INST_BLS_IMM]=      "BLS";
	instLabel[INST_LBLS_IMM16]=   "LBLS";

	instLabel[INST_BLT_IMM]=      "BLT";
	instLabel[INST_LBLT_IMM16]=   "LBLT";

	instLabel[INST_BMI_IMM]=      "BMI";
	instLabel[INST_LBMI_IMM16]=   "LBMI";

	instLabel[INST_BNE_IMM]=      "BNE";
	instLabel[INST_LBNE_IMM16]=   "LBNE";

	instLabel[INST_BPL_IMM]=      "BPL";
	instLabel[INST_LBPL_IMM16]=   "LBPL";

	instLabel[INST_BRA_IMM]=      "BRA";
	instLabel[INST_LBRA_IMM16]=   "LBRA";
	instLabel[INST_LBRA_IMM16_ALT]="LBRA";

	instLabel[INST_BRN_IMM]=      "BRN";
	instLabel[INST_LBRN_IMM16]=   "LBRN";

	instLabel[INST_BSR_IMM]=      "BSR";
	instLabel[INST_LBSR_IMM16]=   "LBSR";

	instLabel[INST_BVC_IMM]=      "BVC";
	instLabel[INST_LBVC_IMM16]=   "LBVC";

	instLabel[INST_BVS_IMM]=      "BVS";
	instLabel[INST_LBVS_IMM16]=   "LBVS";

	instLabel[INST_UNDEF_COMNEGA]="UNDEF_COMNEGA";

	regToReg[ 0]=REG_D;
	regToReg[ 1]=REG_X;
	regToReg[ 2]=REG_Y;
	regToReg[ 3]=REG_U;
	regToReg[ 4]=REG_S;
	regToReg[ 5]=REG_PC;
	regToReg[ 6]=REG_NULL; // W reg if 6309
	regToReg[ 7]=REG_NULL; // V reg if 6309
	regToReg[ 8]=REG_A;
	regToReg[ 9]=REG_B;
	regToReg[10]=REG_CC;
	regToReg[11]=REG_DP;
	regToReg[12]=REG_NULL; // Always value 0 if 6309
	regToReg[13]=REG_NULL; // Always value 0 if 6309
	regToReg[14]=REG_NULL; // E reg if 6309
	regToReg[15]=REG_NULL; // F reg if 6309

	PowerOn();
}

void MC6809::PowerOn(void)
{
	Reset();
}
void MC6809::Reset(void)
{
	Device::Reset();
	state.halt=false;
	state.nmiEnabled=false;     
}
void MC6809::NMI(class MemoryAccess &mem)
{
	if(true==state.nmiEnabled)
	{
		if(true==debugger.enableCallStack)
		{
			debugger.PushCallStack(Debugger::CALLTYPE_NMI,state.S,state.PC,0,mem.NonDestructiveFetchWord(NMI_VECTOR_ADDR));
		}
		state.CC|=EF;
		PushS16(mem,state.PC);
		PushS16(mem,state.U);
		PushS16(mem,state.Y);
		PushS16(mem,state.X);
		PushS8(mem,state.DP);
		PushS8(mem,state.B());
		PushS8(mem,state.A());
		PushS8(mem,state.CC);
		state.CC|=(IRQMASK|FIRQMASK);
		state.PC=mem.FetchWord(NMI_VECTOR_ADDR);
		debugger.OnNMI(*this,mem);
	}
}
void MC6809::IRQ(class MemoryAccess &mem)
{
	if(0==(state.CC&IRQMASK) && true==state.nmiEnabled) // Aren't IRQ and FIRQ as well as NMI blocked until S is set?
	{
		if(true==debugger.enableCallStack)
		{
			debugger.PushCallStack(Debugger::CALLTYPE_IRQ,state.S,state.PC,0,mem.NonDestructiveFetchWord(IRQ_VECTOR_ADDR));
		}
		state.CC|=EF;
		PushS16(mem,state.PC);
		PushS16(mem,state.U);
		PushS16(mem,state.Y);
		PushS16(mem,state.X);
		PushS8(mem,state.DP);
		PushS8(mem,state.B());
		PushS8(mem,state.A());
		PushS8(mem,state.CC);
		state.CC|=(IRQMASK|FIRQMASK);
		state.PC=mem.FetchWord(IRQ_VECTOR_ADDR);
		debugger.OnIRQ(*this,mem);
	}
}
void MC6809::FIRQ(class MemoryAccess &mem)
{
	if(0==(state.CC&FIRQMASK) && true==state.nmiEnabled) // Aren't IRQ and FIRQ as well as NMI blocked until S is set?
	{
		if(true==debugger.enableCallStack)
		{
			debugger.PushCallStack(Debugger::CALLTYPE_FIRQ,state.S,state.PC,0,mem.NonDestructiveFetchWord(FIRQ_VECTOR_ADDR));
		}
		state.CC&=~EF;
		PushS16(mem,state.PC);
		PushS8(mem,state.CC);
		state.CC|=(IRQMASK|FIRQMASK);
		state.PC=mem.FetchWord(FIRQ_VECTOR_ADDR);
		debugger.OnFIRQ(*this,mem);
	}
}
uint32_t MC6809::RunOneInstruction(class MemoryAccess &mem)
{
	debugger.BeforeRunOneInstruction(*this,mem);

	auto inst=FetchInstruction(mem,state.PC);

	switch(inst.opCode)
	{
	case INST_ABX: //       0x3A,
		{
			uint16_t B=state.B();
			state.X+=B;
		}
		break;

	case INST_ADCA_IMM: //  0x89,
		{
			uint16_t imm=inst.operand[0];
			if(0!=(state.CC&CF))
			{
				++imm;
			}
			auto reg=state.A();
			AddByte(reg,imm);
			state.SetA(reg);
		}
		break;
	case INST_ADCA_DP: //   0x99,
		{
			uint16_t add=FetchByte(mem,DecodeDirectPageAddress(inst));
			if(0!=(state.CC&CF))
			{
				++add;
			}
			auto reg=state.A();
			AddByte(reg,add);
			state.SetA(reg);
		}
		break;
	case INST_ADCA_IDX: //  0xA9,
		{
			uint16_t add=FetchByte(mem,DecodeIndexedAddress(inst,mem));
			if(0!=(state.CC&CF))
			{
				++add;
			}
			auto reg=state.A();
			AddByte(reg,add);
			state.SetA(reg);
		}
		break;
	case INST_ADCA_EXT: //  0xB9,
		{
			uint16_t add=FetchByte(mem,inst.ExtendedAddress());
			if(0!=(state.CC&CF))
			{
				++add;
			}
			auto reg=state.A();
			AddByte(reg,add);
			state.SetA(reg);
		}
		break;

	case INST_ADCB_IMM: //  0xC9,
		{
			uint16_t imm=inst.operand[0];
			if(0!=(state.CC&CF))
			{
				++imm;
			}
			auto reg=state.B();
			AddByte(reg,imm);
			state.SetB(reg);
		}
		break;
	case INST_ADCB_DP: //   0xD9,
		{
			uint16_t add=FetchByte(mem,DecodeDirectPageAddress(inst));
			if(0!=(state.CC&CF))
			{
				++add;
			}
			auto reg=state.B();
			AddByte(reg,add);
			state.SetB(reg);
		}
		break;
	case INST_ADCB_IDX: //  0xE9,
		{
			uint16_t add=FetchByte(mem,DecodeIndexedAddress(inst,mem));
			if(0!=(state.CC&CF))
			{
				++add;
			}
			auto reg=state.B();
			AddByte(reg,add);
			state.SetB(reg);
		}
		break;
	case INST_ADCB_EXT: //  0xF9,
		{
			uint16_t add=FetchByte(mem,inst.ExtendedAddress());
			if(0!=(state.CC&CF))
			{
				++add;
			}
			auto reg=state.B();
			AddByte(reg,add);
			state.SetB(reg);
		}
		break;

	case INST_ADDA_IMM: //  0x8B,
		{
			auto reg=state.A();
			AddByte(reg,inst.operand[0]);
			state.SetA(reg);
		}
		break;
	case INST_ADDA_DP: //   0x9B,
		{
			auto reg=state.A();
			AddByte(reg,FetchByte(mem,DecodeDirectPageAddress(inst)));
			state.SetA(reg);
		}
		break;
	case INST_ADDA_IDX: //  0xAB,
		{
			auto reg=state.A();
			AddByte(reg,FetchByte(mem,DecodeIndexedAddress(inst,mem)));
			state.SetA(reg);
		}
		break;
	case INST_ADDA_EXT: //  0xBB,
		{
			auto reg=state.A();
			AddByte(reg,FetchByte(mem,inst.ExtendedAddress()));
			state.SetA(reg);
		}
		break;

	case INST_ADDB_IMM: //  0xCB,
		{
			auto reg=state.B();
			AddByte(reg,inst.operand[0]);
			state.SetB(reg);
		}
		break;
	case INST_ADDB_DP: //   0xDB,
		{
			auto reg=state.B();
			AddByte(reg,FetchByte(mem,DecodeDirectPageAddress(inst)));
			state.SetB(reg);
		}
		break;
	case INST_ADDB_IDX: //  0xEB,
		{
			auto reg=state.B();
			AddByte(reg,FetchByte(mem,DecodeIndexedAddress(inst,mem)));
			state.SetB(reg);
		}
		break;
	case INST_ADDB_EXT: //  0xFB,
		{
			auto reg=state.B();
			AddByte(reg,FetchByte(mem,inst.ExtendedAddress()));
			state.SetB(reg);
		}
		break;

	case INST_ADDD_IMM: //  0xC3,
		AddWord(state.D,mc6809util::FetchWord(inst.operand));
		break;
	case INST_ADDD_DP: //   0xD3,
		AddWord(state.D,FetchWord(mem,DecodeDirectPageAddress(inst)));
		break;
	case INST_ADDD_IDX: //  0xE3,
		AddWord(state.D,FetchWord(mem,DecodeIndexedAddress(inst,mem)));
		break;
	case INST_ADDD_EXT: //  0xF3,
		AddWord(state.D,FetchWord(mem,inst.ExtendedAddress()));
		break;

	case INST_ANDA_IMM: //  0x84,
		state.SetA(AND(state.A(),inst.operand[0]));
		break;
	case INST_ANDA_DP: //   0x94,
		state.SetA(AND(state.A(),FetchByte(mem,DecodeDirectPageAddress(inst))));
		break;
	case INST_ANDA_IDX: //  0xA4,
		state.SetA(AND(state.A(),FetchByte(mem,DecodeIndexedAddress(inst,mem))));
		break;
	case INST_ANDA_EXT: //  0xB4,
		state.SetA(AND(state.A(),FetchByte(mem,inst.ExtendedAddress())));
		break;

	case INST_ANDB_IMM: //  0xC4,
		state.SetB(AND(state.B(),inst.operand[0]));
		break;
	case INST_ANDB_DP: //   0xD4,
		state.SetB(AND(state.B(),FetchByte(mem,DecodeDirectPageAddress(inst))));
		break;
	case INST_ANDB_IDX: //  0xE4,
		state.SetB(AND(state.B(),FetchByte(mem,DecodeIndexedAddress(inst,mem))));
		break;
	case INST_ANDB_EXT: //  0xF4,
		state.SetB(AND(state.B(),FetchByte(mem,inst.ExtendedAddress())));
		break;

	case INST_ANDCC_IMM: // 0x1C,
		state.CC&=inst.operand[0];
		break;

	case INST_ASRA: //      0x47,
		state.SetA(ASR(state.A()));
		break;
	case INST_ASRB: //      0x57,
		state.SetB(ASR(state.B()));
		break;

	case INST_ASR_DP: //    0x07,
		{
			auto addr=DecodeDirectPageAddress(inst);
			StoreByte(mem,addr,ASR(FetchByte(mem,addr)));
		}
		break;
	case INST_ASR_IDX: //   0x67,
		{
			auto addr=DecodeIndexedAddress(inst,mem);
			StoreByte(mem,addr,ASR(FetchByte(mem,addr)));
		}
		break;
	case INST_ASR_EXT: //   0x77,
		{
			auto addr=inst.ExtendedAddress();
			StoreByte(mem,addr,ASR(FetchByte(mem,addr)));
		}
		break;

	case INST_BITA_IMM: //  0x85,
		Test8(state.A()&inst.operand[0]);
		break;
	case INST_BITA_DP: //   0x95,
		Test8(state.A()&FetchByte(mem,DecodeDirectPageAddress(inst)));
		break;
	case INST_BITA_IDX: //  0xA5,
		Test8(state.A()&FetchByte(mem,DecodeIndexedAddress(inst,mem)));
		break;
	case INST_BITA_EXT: //  0xB5,
		Test8(state.A()&FetchByte(mem,inst.ExtendedAddress()));
		break;

	case INST_BITB_IMM: //  0xC5,
		Test8(state.B()&inst.operand[0]);
		break;
	case INST_BITB_DP: //   0xD5,
		Test8(state.B()&FetchByte(mem,DecodeDirectPageAddress(inst)));
		break;
	case INST_BITB_IDX: //  0xE5,
		Test8(state.B()&FetchByte(mem,DecodeIndexedAddress(inst,mem)));
		break;
	case INST_BITB_EXT: //  0xF5,
		Test8(state.B()&FetchByte(mem,inst.ExtendedAddress()));
		break;

	case INST_CLRA: //      0x4F,
		state.SetA(0);
		state.CC&=~(SF|VF|CF);
		state.CC|=ZF;
		break;
	case INST_CLRB: //      0x5F,
		state.SetB(0);
		state.CC&=~(SF|VF|CF);
		state.CC|=ZF;
		break;

	case INST_CLR_DP: //    0x0F,
		{
			auto addr=DecodeDirectPageAddress(inst);
			FetchByte(mem,addr); // Dummy Read before clearing
			StoreByte(mem,addr,0);
			state.CC&=~(SF|VF|CF);
			state.CC|=ZF;
			mem.CLR(addr);
		}
		break;
	case INST_CLR_IDX: //   0x6F,
		{
			auto addr=DecodeIndexedAddress(inst,mem);
			FetchByte(mem,addr); // Dummy Read before clearing
			StoreByte(mem,addr,0);
			state.CC&=~(SF|VF|CF);
			state.CC|=ZF;
			mem.CLR(addr);
		}
		break;
	case INST_CLR_EXT: //   0x7F,
		{
			auto addr=inst.ExtendedAddress();
			FetchByte(mem,addr); // Dummy Read before clearing
			StoreByte(mem,addr,0);
			state.CC&=~(SF|VF|CF);
			state.CC|=ZF;
			mem.CLR(addr);
		}
		break;

	case INST_CMPA_IMM: //  0x81,
		{
			auto reg=state.A();
			SubByte(reg,inst.operand[0]);
		}
		break;
	case INST_CMPA_DP: //   0x91,
		{
			auto reg=state.A();
			SubByte(reg,FetchByte(mem,DecodeDirectPageAddress(inst)));
		}
		break;
	case INST_CMPA_IDX: //  0xA1,
		{
			auto reg=state.A();
			SubByte(reg,FetchByte(mem,DecodeIndexedAddress(inst,mem)));
		}
		break;
	case INST_CMPA_EXT: //  0xB1,
		{
			auto reg=state.A();
			SubByte(reg,FetchByte(mem,inst.ExtendedAddress()));
		}
		break;

	case INST_CMPB_IMM: //  0xC1,
		{
			auto reg=state.B();
			SubByte(reg,inst.operand[0]);
		}
		break;
	case INST_CMPB_DP: //   0xD1,
		{
			auto reg=state.B();
			SubByte(reg,FetchByte(mem,DecodeDirectPageAddress(inst)));
		}
		break;
	case INST_CMPB_IDX: //  0xE1,
		{
			auto reg=state.B();
			SubByte(reg,FetchByte(mem,DecodeIndexedAddress(inst,mem)));
		}
		break;
	case INST_CMPB_EXT: //  0xF1,
		{
			auto reg=state.B();
			SubByte(reg,FetchByte(mem,inst.ExtendedAddress()));
		}
		break;

	case INST_CMPD_IMM: //  0x183, // 10 83
		{
			auto reg=state.D;
			SubWord(reg,mc6809util::FetchWord(inst.operand));
		}
		break;
	case INST_CMPD_DP: //   0x193, // 10 93
		{
			auto reg=state.D;
			SubWord(reg,FetchWord(mem,DecodeDirectPageAddress(inst)));
		}
		break;
	case INST_CMPD_IDX: //  0x1A3, // 10 A3
		{
			auto reg=state.D;
			SubWord(reg,FetchWord(mem,DecodeIndexedAddress(inst,mem)));
		}
		break;
	case INST_CMPD_EXT: //  0x1B3, // 10 B3
		{
			auto reg=state.D;
			SubWord(reg,FetchWord(mem,inst.ExtendedAddress()));
		}
		break;

	case INST_CMPS_IMM: //  0x28C, // 11 8C
		{
			auto reg=state.S;
			SubWord(reg,mc6809util::FetchWord(inst.operand));
		}
		break;
	case INST_CMPS_DP: //   0x29C, // 11 9C
		{
			auto reg=state.S;
			SubWord(reg,FetchWord(mem,DecodeDirectPageAddress(inst)));
		}
		break;
	case INST_CMPS_IDX: //  0x2AC, // 11 AC
		{
			auto value=FetchWord(mem,DecodeIndexedAddress(inst,mem)); // Let it pre-dec or post-inc before comparison.
			auto reg=state.S;
			SubWord(reg,value);
		}
		break;
	case INST_CMPS_EXT: //  0x2BC, // 11 BC
		{
			auto reg=state.S;
			SubWord(reg,FetchWord(mem,inst.ExtendedAddress()));
		}
		break;

	case INST_CMPU_IMM: //  0x283, // 11 83
		{
			auto reg=state.U;
			SubWord(reg,mc6809util::FetchWord(inst.operand));
		}
		break;
	case INST_CMPU_DP: //   0x293, // 11 93
		{
			auto reg=state.U;
			SubWord(reg,FetchWord(mem,DecodeDirectPageAddress(inst)));
		}
		break;
	case INST_CMPU_IDX: //  0x2A3, // 11 A3
		{
			auto value=FetchWord(mem,DecodeIndexedAddress(inst,mem)); // Let it pre-dec or post-inc before comparison.
			auto reg=state.U;
			SubWord(reg,value);
		}
		break;
	case INST_CMPU_EXT: //  0x2B3, // 11 B3
		{
			auto reg=state.U;
			SubWord(reg,FetchWord(mem,inst.ExtendedAddress()));
		}
		break;

	case INST_CMPX_IMM: //  0x8C,
		{
			auto reg=state.X;
			SubWord(reg,mc6809util::FetchWord(inst.operand));
		}
		break;
	case INST_CMPX_DP: //   0x9C,
		{
			auto reg=state.X;
			SubWord(reg,FetchWord(mem,DecodeDirectPageAddress(inst)));
		}
		break;
	case INST_CMPX_IDX: //  0xAC,
		// The following code has been tested on actual FM77AV.
		// If pre-decrementation is given, the value of X is compared
		// after pre-decremted.
		//				ORG		$2000
		//				FDB		#$2000
		//				LDX		#$2002
		//				CMPX	,--X	; Is $2002 compared against $2000?  Or $2000 compared against $2000?
		//				BEQ		EQ
		//				LDA		#0
		//				STA		$1FFF
		//				STX		$1FFC
		//				RTS
		//
		//EQ				LDA		#$FF
		//				STA		$1FFF
		//				STX		$1FFC
		//				RTS

		//; 0 is written to $1FFF
		//				ORG		$2000
		//				FDB		$2000
		//				CLRA
		//				LDX		#$2000
		//				CMPX	,X++
		//				BEQ		EQ
		//				STA		$1FFF	; A=0 Comes to this line.  Comparison is done after post-incrementation.
		//				RTS
		//EQ				COMA
		//				STA		$1FFF	; A=FF
		//				RTS
		{
			auto value=FetchWord(mem,DecodeIndexedAddress(inst,mem)); // Let it pre-dec or post-inc before comparison.
			auto reg=state.X;
			SubWord(reg,value);
		}
		break;
	case INST_CMPX_EXT: //  0xBC,
		{
			auto reg=state.X;
			SubWord(reg,FetchWord(mem,inst.ExtendedAddress()));
		}
		break;

	case INST_CMPY_IMM: //  0x18C, // 10 8C
		{
			auto reg=state.Y;
			SubWord(reg,mc6809util::FetchWord(inst.operand));
		}
		break;
	case INST_CMPY_DP: //   0x19C, // 10 9C
		{
			auto reg=state.Y;
			SubWord(reg,FetchWord(mem,DecodeDirectPageAddress(inst)));
		}
		break;
	case INST_CMPY_IDX: //  0x1AC, // 10 AC
		{
			auto value=FetchWord(mem,DecodeIndexedAddress(inst,mem)); // Let it pre-dec or post-inc before comparison.
			auto reg=state.Y;
			SubWord(reg,value);
		}
		break;
	case INST_CMPY_EXT: //  0x1BC, // 10 BC
		{
			auto reg=state.Y;
			SubWord(reg,FetchWord(mem,inst.ExtendedAddress()));
		}
		break;

	case INST_COMA: //      0x43,
		{
			auto reg=~state.A();
			Test8(reg);
			state.CC|=CF;
			state.SetA(reg);
		}
		break;
	case INST_COMB: //      0x53,
		{
			auto reg=~state.B();
			Test8(reg);
			state.CC|=CF;
			state.SetB(reg);
		}
		break;

	case INST_COM_DP: //    0x03,
		{
			auto addr=DecodeDirectPageAddress(inst);
			auto reg=~FetchByte(mem,addr);
			Test8(reg);
			state.CC|=CF;
			StoreByte(mem,addr,reg);
		}
		break;
	case INST_COM_IDX: //   0x63,
		{
			auto addr=DecodeIndexedAddress(inst,mem);
			auto reg=~FetchByte(mem,addr);
			Test8(reg);
			state.CC|=CF;
			StoreByte(mem,addr,reg);
		}
		break;
	case INST_COM_EXT: //   0x73,
		{
			auto addr=inst.ExtendedAddress();
			auto reg=~FetchByte(mem,addr);
			Test8(reg);
			state.CC|=CF;
			StoreByte(mem,addr,reg);
		}
		break;

	case INST_CWAI_IMM: //  0x3C,
		Abort("Instruction not supported yet.");
		inst.length=0;
		break;

	case INST_DAA: //       0x19,
		{
			uint16_t A=state.A();
			if(0!=(state.CC&HF) || 9<(A&0x0F))
			{
				A+=6;
			}
			if(0x9F<A || 0!=(state.CC&CF))
			{
				A+=0x60;
				state.CC|=CF;
			}
			state.SetA(A);
			state.CC&=~(ZF|SF);
			RaiseZF(0==state.A());
			RaiseSF(0!=(state.A()&0x80));
		}
		break;

	case INST_DECA: //      0x4A,
		state.SetA(DEC(state.A()));
		break;
	case INST_DECB: //      0x5A,
		state.SetB(DEC(state.B()));
		break;

	case INST_DEC_DP: //    0x0A,
		{
			auto addr=DecodeDirectPageAddress(inst);
			StoreByte(mem,addr,DEC(FetchByte(mem,addr)));
		}
		break;
	case INST_DEC_IDX: //   0x6A,
		{
			auto addr=DecodeIndexedAddress(inst,mem);
			StoreByte(mem,addr,DEC(FetchByte(mem,addr)));
		}
		break;
	case INST_DEC_EXT: //   0x7A,
		{
			auto addr=inst.ExtendedAddress();
			StoreByte(mem,addr,DEC(FetchByte(mem,addr)));
		}
		break;

	case INST_EORA_IMM: //  0x88,
		{
			auto reg=state.A()^inst.operand[0];
			Test8(reg);
			state.SetA(reg);
		}
		break;
	case INST_EORA_DP: //   0x98,
		{
			auto reg=state.A()^FetchByte(mem,DecodeDirectPageAddress(inst));
			Test8(reg);
			state.SetA(reg);
		}
		break;
	case INST_EORA_IDX: //  0xA8,
		{
			auto reg=state.A()^FetchByte(mem,DecodeIndexedAddress(inst,mem));
			Test8(reg);
			state.SetA(reg);
		}
		break;
	case INST_EORA_EXT: //  0xB8,
		{
			auto reg=state.A()^FetchByte(mem,inst.ExtendedAddress());
			Test8(reg);
			state.SetA(reg);
		}
		break;

	case INST_EORB_IMM: //  0xC8,
		{
			auto reg=state.B()^inst.operand[0];
			Test8(reg);
			state.SetB(reg);
		}
		break;
	case INST_EORB_DP: //   0xD8,
		{
			auto reg=state.B()^FetchByte(mem,DecodeDirectPageAddress(inst));
			Test8(reg);
			state.SetB(reg);
		}
		break;
	case INST_EORB_IDX: //  0xE8,
		{
			auto reg=state.B()^FetchByte(mem,DecodeIndexedAddress(inst,mem));
			Test8(reg);
			state.SetB(reg);
		}
		break;
	case INST_EORB_EXT: //  0xF8,
		{
			auto reg=state.B()^FetchByte(mem,inst.ExtendedAddress());
			Test8(reg);
			state.SetB(reg);
		}
		break;

	case INST_EXG_REG: //   0x1E,
		{
			uint8_t reg[2];
			DecodeExgTfrReg(reg,inst.operand[0]);
			if(regBits[reg[0]]!=regBits[reg[1]])
			{
				Abort("EXG to different size register not supported yet.");
				inst.length=0;
			}

			uint16_t value[2]=
			{
				GetRegisterValue(reg[0]),
				GetRegisterValue(reg[1]),
			};

			if(REG_PC==reg[0])
			{
				value[0]+=inst.length;
			}
			if(REG_PC==reg[1])
			{
				value[1]+=inst.length;
			}

			SetRegisterValue(reg[1],value[0]);
			SetRegisterValue(reg[0],value[1]);

			if(REG_S==reg[0] || REG_S==reg[1])
			{
				state.nmiEnabled=true;
			}
			else if(REG_PC==reg[0] || REG_PC==reg[1])
			{
				inst.length=0;
			}
		}
		break;

	case INST_INCA: //      0x4C,
		{
			auto reg=state.A();
			reg=INC(reg);
			state.SetA(reg);
		}
		break;
	case INST_INCB: //      0x5C,
		{
			auto reg=state.B();
			reg=INC(reg);
			state.SetB(reg);
		}
		break;

	case INST_INC_DP: //    0x0C,
		{
			auto addr=DecodeDirectPageAddress(inst);
			StoreByte(mem,addr,INC(FetchByte(mem,addr)));
		}
		break;
	case INST_INC_IDX: //   0x6C,
		{
			auto addr=DecodeIndexedAddress(inst,mem);
			StoreByte(mem,addr,INC(FetchByte(mem,addr)));
		}
		break;
	case INST_INC_EXT: //   0x7C,
		{
			auto addr=inst.ExtendedAddress();
			StoreByte(mem,addr,INC(FetchByte(mem,addr)));
		}
		break;

	case INST_JMP_DP: //    0x0E,
		state.PC=DecodeDirectPageAddress(inst);
		inst.length=0;
		break;
	case INST_JMP_IDX: //   0x6E,
		state.PC=DecodeIndexedAddress(inst,mem);
		inst.length=0;
		break;
	case INST_JMP_EXT: //   0x7E,
		state.PC=inst.ExtendedAddress();
		inst.length=0;
		break;

	case INST_JSR_DP: //    0x9D,
		if(true==debugger.enableCallStack)
		{
			debugger.PushCallStack(Debugger::CALLTYPE_BSR_JSR,state.S,state.PC,inst.length,DecodeDirectPageAddress(inst));
		}
		PushS16(mem,state.PC+inst.length);
		state.PC=DecodeDirectPageAddress(inst);
		inst.length=0;
		break;
	case INST_JSR_IDX: //   0xAD,
		{
			auto nextPC=DecodeIndexedAddress(inst,mem);
			if(true==debugger.enableCallStack)
			{
				debugger.PushCallStack(Debugger::CALLTYPE_BSR_JSR,state.S,state.PC,inst.length,nextPC);
			}
			// JSR [,S++] found in Polar Star III.
			// The correct behavior is probably:
			//   (1) Calculate Index Address, and S+=2
			//   (2) Push Return Address
			//   (3) Jump.
			auto retAddr=state.PC+inst.length;
			state.PC=nextPC;
			PushS16(mem,retAddr);
		}
		inst.length=0;
		break;
	case INST_JSR_EXT: //   0xBD,
		if(true==debugger.enableCallStack)
		{
			debugger.PushCallStack(Debugger::CALLTYPE_BSR_JSR,state.S,state.PC,inst.length,inst.ExtendedAddress());
		}
		PushS16(mem,state.PC+inst.length);
		state.PC=inst.ExtendedAddress();
		inst.length=0;
		break;

	case INST_LDA_IMM: //   0x86,
		state.SetA(inst.operand[0]);
		Test8(inst.operand[0]);
		break;
	case INST_LDA_DP: //    0x96,
		state.SetA(FetchByte(mem,DecodeDirectPageAddress(inst)));
		Test8(state.A());
		break;
	case INST_LDA_IDX: //   0xA6,
		state.SetA(FetchByte(mem,DecodeIndexedAddress(inst,mem)));
		Test8(state.A());
		break;
	case INST_LDA_EXT: //   0xB6,
		state.SetA(FetchByte(mem,inst.ExtendedAddress()));
		Test8(state.A());
		break;

	case INST_LDB_IMM: //   0xC6,
		state.SetB(inst.operand[0]);
		Test8(inst.operand[0]);
		break;
	case INST_LDB_DP: //    0xD6,
		state.SetB(FetchByte(mem,DecodeDirectPageAddress(inst)));
		Test8(state.B());
		break;
	case INST_LDB_IDX: //   0xE6,
		state.SetB(FetchByte(mem,DecodeIndexedAddress(inst,mem)));
		Test8(state.B());
		break;
	case INST_LDB_EXT: //   0xF6,
		state.SetB(FetchByte(mem,inst.ExtendedAddress()));
		Test8(state.B());
		break;

	case INST_LDD_IMM: //   0xCC,
		state.SetA(inst.operand[0]);
		state.SetB(inst.operand[1]);
		Test16(state.D);
		break;
	case INST_LDD_DP: //    0xDC,
		state.D=FetchWord(mem,DecodeDirectPageAddress(inst));
		Test16(state.D);
		break;
	case INST_LDD_IDX: //   0xEC,
		state.D=FetchWord(mem,DecodeIndexedAddress(inst,mem));
		Test16(state.D);
		break;
	case INST_LDD_EXT: //   0xFC,
		state.D=FetchWord(mem,inst.ExtendedAddress());
		Test16(state.D);
		break;

	case INST_LDS_IMM: //   0x1CE, // 10 CE
		state.S=mc6809util::FetchWord(inst.operand[0],inst.operand[1]);
		state.nmiEnabled=true;
		Test16(state.S);
		break;
	case INST_LDS_DP: //    0x1DE, // 10 DE
		state.S=FetchWord(mem,DecodeDirectPageAddress(inst));
		state.nmiEnabled=true;
		Test16(state.S);
		break;
	case INST_LDS_IDX: //   0x1EE, // 10 EE
		state.S=FetchWord(mem,DecodeIndexedAddress(inst,mem));
		state.nmiEnabled=true;
		Test16(state.S);
		break;
	case INST_LDS_EXT: //   0x1FE, // 10 FE
		state.S=FetchWord(mem,inst.ExtendedAddress());
		state.nmiEnabled=true;
		Test16(state.S);
		break;

	case INST_LDU_IMM: //   0xCE,
		state.U=mc6809util::FetchWord(inst.operand[0],inst.operand[1]);
		Test16(state.U);
		break;
	case INST_LDU_DP: //    0xDE,
		state.U=FetchWord(mem,DecodeDirectPageAddress(inst));
		Test16(state.U);
		break;
	case INST_LDU_IDX: //   0xEE,
		state.U=FetchWord(mem,DecodeIndexedAddress(inst,mem));
		Test16(state.U);
		break;
	case INST_LDU_EXT: //   0xFE,
		state.U=FetchWord(mem,inst.ExtendedAddress());
		Test16(state.U);
		break;

	case INST_LDX_IMM: //   0x8E,
		state.X=mc6809util::FetchWord(inst.operand[0],inst.operand[1]);
		Test16(state.X);
		break;
	case INST_LDX_DP: //    0x9E,
		state.X=FetchWord(mem,DecodeDirectPageAddress(inst));
		Test16(state.X);
		break;
	case INST_LDX_IDX: //   0xAE,
		// The following code has been tested on actual FM77AV.
		// The result was:
		//   1FFC  FFFE
		//   1FFF  FF
		// In this case, post-incrementation is disregarded.
		//				ORG		$2000
		//				FDB		$FFFE
		//				LDX		#$2000
		//				LDX		,X++	; Value loaded is $FFFE, after incrementation it is 0
		//				BEQ		EQ
		//				STX		$1FFC
		//				LDA		#$FF
		//				STA		$1FFF
		//				RTS
		//EQ
		//				STX		$1FFC
		//				LDA		#$00
		//				STA		$1FFF
		//				RTS
		//; The following code wrote 100 to $1ffC and $1FFE.
		//; Even with post-incrementation, the value is what read from the memory.
		//					ORG		$2000
		//					FDB		100
		//					LDU		#$2000
		//					LDU		,U++
		//					STU		$1FFC
		//					LDU		#$2002
		//					LDU		,--U
		//					STU		$1FFE
		//					RTS
		state.X=FetchWord(mem,DecodeIndexedAddress(inst,mem));
		Test16(state.X);
		break;
	case INST_LDX_EXT: //   0xBE,
		state.X=FetchWord(mem,inst.ExtendedAddress());
		Test16(state.X);
		break;

	case INST_LDY_IMM: //   0x18E, // 10 8E
		state.Y=mc6809util::FetchWord(inst.operand[0],inst.operand[1]);
		Test16(state.Y);
		break;
	case INST_LDY_DP: //    0x19E, // 10 9E
		state.Y=FetchWord(mem,DecodeDirectPageAddress(inst));
		Test16(state.Y);
		break;
	case INST_LDY_IDX: //   0x1AE, // 10 AE
		state.Y=FetchWord(mem,DecodeIndexedAddress(inst,mem));
		Test16(state.Y);
		break;
	case INST_LDY_EXT: //   0x1BE, // 10 BE
		state.Y=FetchWord(mem,inst.ExtendedAddress());
		Test16(state.Y);
		break;

	case INST_LEAS_IDX: //  0x32,
		state.S=DecodeIndexedAddress(inst,mem);
		state.nmiEnabled=true;
		break;
	case INST_LEAU_IDX: //  0x33,
		state.U=DecodeIndexedAddress(inst,mem);
		break;
	case INST_LEAX_IDX: //  0x30,
		state.X=DecodeIndexedAddress(inst,mem);
		if(0==state.X)
		{
			state.CC|=ZF;
		}
		else
		{
			state.CC&=(~ZF);
		}
		break;
	case INST_LEAY_IDX: //  0x31,
		state.Y=DecodeIndexedAddress(inst,mem);
		if(0==state.Y)
		{
			state.CC|=ZF;
		}
		else
		{
			state.CC&=(~ZF);
		}
		break;

	case INST_LSLA: //      0x48,
		state.SetA(LSL(state.A()));
		break;
	case INST_LSLB: //      0x58,
		state.SetB(LSL(state.B()));
		break;

	case INST_LSL_DP: //    0x08,
		{
			uint16_t addr=DecodeDirectPageAddress(inst);
			StoreByte(mem,addr,LSL(FetchByte(mem,addr)));
		}
		break;
	case INST_LSL_IDX: //   0x68,
		{
			uint16_t addr=DecodeIndexedAddress(inst,mem);
			StoreByte(mem,addr,LSL(FetchByte(mem,addr)));
		}
		break;
	case INST_LSL_EXT: //   0x78,
		{
			uint16_t addr=inst.ExtendedAddress();
			StoreByte(mem,addr,LSL(FetchByte(mem,addr)));
		}
		break;

	case INST_LSRA: //      0x44,
		{
			auto reg=state.A();
			state.SetA(LSR(reg));
		}
		break;
	case INST_LSRB: //      0x54,
		{
			auto reg=state.B();
			state.SetB(LSR(reg));
		}
		break;

	case INST_LSR_DP: //    0x04,
		{
			auto addr=DecodeDirectPageAddress(inst);
			StoreByte(mem,addr,LSR(FetchByte(mem,addr)));
		}
		break;
	case INST_LSR_IDX: //   0x64,
		{
			uint16_t addr=DecodeIndexedAddress(inst,mem);
			StoreByte(mem,addr,LSR(FetchByte(mem,addr)));
		}
		break;
	case INST_LSR_EXT: //   0x74,
		{
			uint16_t addr=inst.ExtendedAddress();
			StoreByte(mem,addr,LSR(FetchByte(mem,addr)));
		}
		break;

	case INST_MUL: //       0x3D,
		{
			uint16_t A=state.A();
			uint16_t B=state.B();
			state.D=A*B;
			state.CC&=~(ZF|CF);
			RaiseZF(0==state.D);
			RaiseCF(0!=(state.D&0x80));
		}
		break;

	case INST_NEGA: //      0x40,
		state.SetA(NEG(state.A()));
		break;
	case INST_NEGB: //      0x50,
		state.SetB(NEG(state.B()));
		break;

	case INST_NEG_DP: //    0x00,
		{
			auto addr=DecodeDirectPageAddress(inst);
			auto reg=FetchByte(mem,addr);
			reg=NEG(reg);
			StoreByte(mem,addr,reg);
		}
		break;
	case INST_NEG_IDX: //   0x60,
		{
			auto addr=DecodeIndexedAddress(inst,mem);
			auto reg=FetchByte(mem,addr);
			reg=NEG(reg);
			StoreByte(mem,addr,reg);
		}
		break;
	case INST_NEG_EXT: //   0x70,
		{
			auto addr=inst.ExtendedAddress();
			auto reg=FetchByte(mem,addr);
			reg=NEG(reg);
			StoreByte(mem,addr,reg);
		}
		break;

	case INST_NOP: //       0x12,
		break;

	case INST_ORA_IMM: //   0x8A,
		{
			auto reg=OR(state.A(),inst.operand[0]);
			state.SetA(reg);
		}
		break;
	case INST_ORA_DP: //    0x9A,
		state.SetA(OR(state.A(),FetchByte(mem,DecodeDirectPageAddress(inst))));
		break;
	case INST_ORA_IDX: //   0xAA,
		state.SetA(OR(state.A(),FetchByte(mem,DecodeIndexedAddress(inst,mem))));
		break;
	case INST_ORA_EXT: //   0xBA,
		state.SetA(OR(state.A(),FetchByte(mem,inst.ExtendedAddress())));
		break;

	case INST_ORB_IMM: //   0xCA,
		{
			auto reg=OR(state.B(),inst.operand[0]);
			state.SetB(reg);
		}
		break;
	case INST_ORB_DP: //    0xDA,
		state.SetB(OR(state.B(),FetchByte(mem,DecodeDirectPageAddress(inst))));
		break;
	case INST_ORB_IDX: //   0xEA,
		state.SetB(OR(state.B(),FetchByte(mem,DecodeIndexedAddress(inst,mem))));
		break;
	case INST_ORB_EXT: //   0xFA,
		state.SetB(OR(state.B(),FetchByte(mem,inst.ExtendedAddress())));
		break;

	case INST_ORCC_IMM: //  0x1A,
		state.CC|=inst.operand[0];
		break;

	case INST_PSHS_REG: //	0x34,
		FetchByte(mem,state.S);
		if(0!=(inst.operand[0]&PSH_PC))
		{
			PushS16(mem,state.PC);
			inst.clocks+=2;
		}
		if(0!=(inst.operand[0]&PSH_UorS))
		{
			PushS16(mem,state.U);
			inst.clocks+=2;
		}
		if(0!=(inst.operand[0]&PSH_Y))
		{
			PushS16(mem,state.Y);
			inst.clocks+=2;
		}
		if(0!=(inst.operand[0]&PSH_X))
		{
			PushS16(mem,state.X);
			inst.clocks+=2;
		}
		if(0!=(inst.operand[0]&PSH_DP))
		{
			PushS8(mem,state.DP);
			++inst.clocks;
		}
		if(0!=(inst.operand[0]&PSH_B))
		{
			PushS8(mem,state.B());
			++inst.clocks;
		}
		if(0!=(inst.operand[0]&PSH_A))
		{
			PushS8(mem,state.A());
			++inst.clocks;
		}
		if(0!=(inst.operand[0]&PSH_CC))
		{
			PushS8(mem,state.CC);
			++inst.clocks;
		}
		break;
	case INST_PSHU_REG: //	0x36,
		FetchByte(mem,state.U);
		if(0!=(inst.operand[0]&PSH_PC))
		{
			PushU16(mem,state.PC);
			inst.clocks+=2;
		}
		if(0!=(inst.operand[0]&PSH_UorS))
		{
			PushU16(mem,state.S);
			inst.clocks+=2;
		}
		if(0!=(inst.operand[0]&PSH_Y))
		{
			PushU16(mem,state.Y);
			inst.clocks+=2;
		}
		if(0!=(inst.operand[0]&PSH_X))
		{
			PushU16(mem,state.X);
			inst.clocks+=2;
		}
		if(0!=(inst.operand[0]&PSH_DP))
		{
			PushU8(mem,state.DP);
			++inst.clocks;
		}
		if(0!=(inst.operand[0]&PSH_B))
		{
			PushU8(mem,state.B());
			++inst.clocks;
		}
		if(0!=(inst.operand[0]&PSH_A))
		{
			PushU8(mem,state.A());
			++inst.clocks;
		}
		if(0!=(inst.operand[0]&PSH_CC))
		{
			PushU8(mem,state.CC);
			++inst.clocks;
		}
		break;
	case INST_PULS_REG: //	0x35,
		if(0!=(inst.operand[0]&PSH_CC))
		{
			state.CC=PullS8(mem);
			++inst.clocks;
		}
		if(0!=(inst.operand[0]&PSH_A))
		{
			state.SetA(PullS8(mem));
			++inst.clocks;
		}
		if(0!=(inst.operand[0]&PSH_B))
		{
			state.SetB(PullS8(mem));
			++inst.clocks;
		}
		if(0!=(inst.operand[0]&PSH_DP))
		{
			state.DP=PullS8(mem);
			++inst.clocks;
		}
		if(0!=(inst.operand[0]&PSH_X))
		{
			state.X=PullS16(mem);
			inst.clocks+=2;
		}
		if(0!=(inst.operand[0]&PSH_Y))
		{
			state.Y=PullS16(mem);
			inst.clocks+=2;
		}
		if(0!=(inst.operand[0]&PSH_UorS))
		{
			state.U=PullS16(mem);
			inst.clocks+=2;
		}
		if(0!=(inst.operand[0]&PSH_PC))
		{
			state.PC=PullS16(mem);
			inst.length=0; // Don't increment.
			inst.clocks+=2;
			if(true==debugger.enableCallStack)
			{
				debugger.PopCallStack(state.S,state.PC);
			}
		}
		// Confirmed on actual FM77AV:
		// PULS instruction dummy-reads one extra byte.
		// The byte read from the RAM won't affect any registor, however, the hardware-drawing chip
		// detects and reacts to the memory-read.  Unless it is taken into account, Dragon Buster FM77AV
		// leaves uncleared VRAM bytes.

		// Turned out, FM77AV's hardware drawing even reacts to memory-write operation.
		// And, for PSHU instruction, it writes to the address before U is decremented.
		// Obviously, 6809 is not writing to that address.  My guess is 6809 just addresses
		// or connets S or U register to the address, and then do push or pull.
		// FM77AV's hardware drawing may just be looking at the addressing.
		FetchByte(mem,state.S);
		break;
	case INST_PULU_REG: //	0x37,
		if(0!=(inst.operand[0]&PSH_CC))
		{
			state.CC=PullU8(mem);
			++inst.clocks;
		}
		if(0!=(inst.operand[0]&PSH_A))
		{
			state.SetA(PullU8(mem));
			++inst.clocks;
		}
		if(0!=(inst.operand[0]&PSH_B))
		{
			state.SetB(PullU8(mem));
			++inst.clocks;
		}
		if(0!=(inst.operand[0]&PSH_DP))
		{
			state.DP=PullU8(mem);
			++inst.clocks;
		}
		if(0!=(inst.operand[0]&PSH_X))
		{
			state.X=PullU16(mem);
			inst.clocks+=2;
		}
		if(0!=(inst.operand[0]&PSH_Y))
		{
			state.Y=PullU16(mem);
			inst.clocks+=2;
		}
		if(0!=(inst.operand[0]&PSH_UorS))
		{
			state.S=PullU16(mem);
			state.nmiEnabled=true;
			inst.clocks+=2;
		}
		if(0!=(inst.operand[0]&PSH_PC))
		{
			state.PC=PullU16(mem);
			inst.length=0; // Don't increment.
			inst.clocks+=2;
		}
		// Confirmed the same behavior as PULS instruction on actual FM77AV.
		// It reads and discards an extra byte.
		FetchByte(mem,state.U);
		break;

	case INST_ROLA: //      0x49,
		state.SetA(ROL(state.A()));
		break;
	case INST_ROLB: //      0x59,
		state.SetB(ROL(state.B()));
		break;

	case INST_ROL_DP: //    0x09,
		{
			uint16_t addr=DecodeDirectPageAddress(inst);
			StoreByte(mem,addr,ROL(FetchByte(mem,addr)));
		}
		break;
	case INST_ROL_IDX: //   0x69,
		{
			uint16_t addr=DecodeIndexedAddress(inst,mem);
			StoreByte(mem,addr,ROL(FetchByte(mem,addr)));
		}
		break;
	case INST_ROL_EXT: //   0x79,
		{
			uint16_t addr=inst.ExtendedAddress();
			StoreByte(mem,addr,ROL(FetchByte(mem,addr)));
		}
		break;

	case INST_RORA: //      0x46,
		state.SetA(ROR(state.A()));
		break;
	case INST_RORB: //      0x56,
		state.SetB(ROR(state.B()));
		break;

	case INST_ROR_DP: //    0x06,
		{
			uint16_t addr=DecodeDirectPageAddress(inst);
			StoreByte(mem,addr,ROR(FetchByte(mem,addr)));
		}
		break;
	case INST_ROR_IDX: //   0x66,
		{
			uint16_t addr=DecodeIndexedAddress(inst,mem);
			StoreByte(mem,addr,ROR(FetchByte(mem,addr)));
		}
		break;
	case INST_ROR_EXT: //   0x76,
		{
			uint16_t addr=inst.ExtendedAddress();
			StoreByte(mem,addr,ROR(FetchByte(mem,addr)));
		}
		break;

	case INST_RTI: //       0x3B,
		state.CC=PullS8(mem);
		if(0!=(state.CC&EF))
		{
			state.SetA(PullS8(mem));
			state.SetB(PullS8(mem));
			state.DP=PullS8(mem);
			state.X=PullS16(mem);
			state.Y=PullS16(mem);
			state.U=PullS16(mem);
			inst.clocks=15;
		}
		state.PC=PullS16(mem);
		inst.length=0;
		if(true==debugger.enableCallStack)
		{
			debugger.PopCallStack(state.S,state.PC);
		}
		break;
	case INST_RTS: //       0x39,
		state.PC=PullS16(mem);
		inst.length=0;
		if(true==debugger.enableCallStack)
		{
			debugger.PopCallStack(state.S,state.PC);
		}
		break;

	case INST_SBCA_IMM: //  0x82,
		{
			uint16_t value=inst.operand[0];
			if(0!=(state.CC&CF))
			{
				++value;
			}
			auto reg=state.A();
			SubByte(reg,value);
			state.SetA(reg);
		}
		break;
	case INST_SBCA_DP: //   0x92,
		{
			uint16_t value=FetchByte(mem,DecodeDirectPageAddress(inst));
			if(0!=(state.CC&CF))
			{
				++value;
			}
			auto reg=state.A();
			SubByte(reg,value);
			state.SetA(reg);
		}
		break;
	case INST_SBCA_IDX: //  0xA2,
		{
			uint16_t value=FetchByte(mem,DecodeIndexedAddress(inst,mem));
			if(0!=(state.CC&CF))
			{
				++value;
			}
			auto reg=state.A();
			SubByte(reg,value);
			state.SetA(reg);
		}
		break;
	case INST_SBCA_EXT: //  0xB2,
		{
			uint16_t value=FetchByte(mem,inst.ExtendedAddress());
			if(0!=(state.CC&CF))
			{
				++value;
			}
			auto reg=state.A();
			SubByte(reg,value);
			state.SetA(reg);
		}
		break;

	case INST_SBCB_IMM: //  0xC2,
		{
			uint16_t value=inst.operand[0];
			if(0!=(state.CC&CF))
			{
				++value;
			}
			auto reg=state.B();
			SubByte(reg,value);
			state.SetB(reg);
		}
		break;
	case INST_SBCB_DP: //   0xD2,
		{
			uint16_t value=FetchByte(mem,DecodeDirectPageAddress(inst));
			if(0!=(state.CC&CF))
			{
				++value;
			}
			auto reg=state.B();
			SubByte(reg,value);
			state.SetB(reg);
		}
		break;
	case INST_SBCB_IDX: //  0xE2,
		{
			uint16_t value=FetchByte(mem,DecodeIndexedAddress(inst,mem));
			if(0!=(state.CC&CF))
			{
				++value;
			}
			auto reg=state.B();
			SubByte(reg,value);
			state.SetB(reg);
		}
		break;
	case INST_SBCB_EXT: //  0xF2,
		{
			uint16_t value=FetchByte(mem,inst.ExtendedAddress());
			if(0!=(state.CC&CF))
			{
				++value;
			}
			auto reg=state.B();
			SubByte(reg,value);
			state.SetB(reg);
		}
		break;

	case INST_SEX: //       0x1D,
		state.CC&=~(SF|ZF);
		if(0!=(state.B()&0x80))
		{
			state.CC|=SF;
			state.SetA(0xFF);
		}
		else
		{
			state.SetA(0);
			RaiseZF(0==state.B());
		}
		break;

	case INST_STA_DP: //    0x97,
		StoreByte(mem,DecodeDirectPageAddress(inst),state.A());
		Test8(state.A());
		break;
	case INST_STA_IDX: //   0xA7,
		WriteToIndex8(mem,inst,state.A());
		Test8(state.A());
		break;
	case INST_STA_EXT: //   0xB7,
		StoreByte(mem,inst.ExtendedAddress(),state.A());
		Test8(state.A());
		break;

	case INST_STB_DP: //    0xD7,
		StoreByte(mem,DecodeDirectPageAddress(inst),state.B());
		Test8(state.B());
		break;
	case INST_STB_IDX: //   0xE7,
		WriteToIndex8(mem,inst,state.B());
		Test8(state.B());
		break;
	case INST_STB_EXT: //   0xF7,
		StoreByte(mem,inst.ExtendedAddress(),state.B());
		Test8(state.B());
		break;

	case INST_STD_DP: //    0xDD,
		StoreWord(mem,DecodeDirectPageAddress(inst),state.D);
		Test16(state.D);
		break;
	case INST_STD_IDX: //   0xED,
		WriteToIndex16(mem,inst,state.D);
		Test16(state.D);
		break;
	case INST_STD_EXT: //   0xFD,
		StoreWord(mem,inst.ExtendedAddress(),state.D);
		Test16(state.D);
		break;

	case INST_STS_DP: //    0x1DF, // 10 DF
		StoreWord(mem,DecodeDirectPageAddress(inst),state.S);
		Test16(state.S);
		break;
	case INST_STS_IDX: //   0x1EF, // 10 EF
		// Question:
		//   If, STX ,X++ is done, should the flags be set based on the value written?
		//   Or the value of X after the post-incrementation?
		//   Need experiment on actual hardware.
		// Answer:
		//   Most likely the value of X written to the data bus.
		//   Also, even post-inc, the value written is after incrementation.
		// ; This code writes:
		// ;   $20 $20 to $1FFE
		// ;   $1F $FC to $1FFC
		// ; Regardless of pre-decrementation or post-incrementation,
		// ; the value stored is after the pre-dec or post-inc.
		// 
		// 				ORG		$2000
		// 				LDX		#$1FFE
		// 				STX		,X++
		// 				LDX		#$1FFE
		// 				STX		,--X
		// 				RTS
		{
			auto addr=DecodeIndexedAddress(inst,mem); // Let it increment/decrement.
			StoreWord(mem,addr,state.S);
			Test16(state.S);
		}
		break;
	case INST_STS_EXT: //   0x1FF, // 10 FF
		StoreWord(mem,inst.ExtendedAddress(),state.S);
		Test16(state.S);
		break;

	case INST_STU_DP: //    0xDF,
		StoreWord(mem,DecodeDirectPageAddress(inst),state.U);
		Test16(state.U);
		break;
	case INST_STU_IDX: //   0xEF,
		{
			auto addr=DecodeIndexedAddress(inst,mem); // Let it increment/decrement.
			StoreWord(mem,addr,state.U);
			Test16(state.U);
		}
		break;
	case INST_STU_EXT: //   0xFF,
		StoreWord(mem,inst.ExtendedAddress(),state.U);
		Test16(state.U);
		break;

	case INST_STX_DP: //    0x9F,
		StoreWord(mem,DecodeDirectPageAddress(inst),state.X);
		Test16(state.X);
		break;
	case INST_STX_IDX: //   0xAF,
		{
			auto addr=DecodeIndexedAddress(inst,mem); // Let it increment/decrement.
			StoreWord(mem,addr,state.X);
			Test16(state.X);
		}
		break;
	case INST_STX_EXT: //   0xBF,
		StoreWord(mem,inst.ExtendedAddress(),state.X);
		Test16(state.X);
		break;

	case INST_STY_DP: //    0x19F,
		StoreWord(mem,DecodeDirectPageAddress(inst),state.Y);
		Test16(state.Y);
		break;
	case INST_STY_IDX: //   0x1AF,
		{
			auto addr=DecodeIndexedAddress(inst,mem); // Let it increment/decrement.
			StoreWord(mem,addr,state.Y);
			Test16(state.Y);
		}
		break;
	case INST_STY_EXT: //   0x1BF,
		StoreWord(mem,inst.ExtendedAddress(),state.Y);
		Test16(state.Y);
		break;

	case INST_SUBA_IMM: //  0x80,
		{
			auto reg=state.A();
			SubByte(reg,inst.operand[0]);
			state.SetA(reg);
		}
		break;
	case INST_SUBA_DP: //   0x90,
		{
			auto reg=state.A();
			SubByte(reg,FetchByte(mem,DecodeDirectPageAddress(inst)));
			state.SetA(reg);
		}
		break;
	case INST_SUBA_IDX: //  0xA0,
		{
			auto reg=state.A();
			SubByte(reg,FetchByte(mem,DecodeIndexedAddress(inst,mem)));
			state.SetA(reg);
		}
		break;
	case INST_SUBA_EXT: //  0xB0,
		{
			auto reg=state.A();
			SubByte(reg,FetchByte(mem,inst.ExtendedAddress()));
			state.SetA(reg);
		}
		break;

	case INST_SUBB_IMM: //  0xC0,
		{
			auto reg=state.B();
			SubByte(reg,inst.operand[0]);
			state.SetB(reg);
		}
		break;
	case INST_SUBB_DP: //   0xD0,
		{
			auto reg=state.B();
			SubByte(reg,FetchByte(mem,DecodeDirectPageAddress(inst)));
			state.SetB(reg);
		}
		break;
	case INST_SUBB_IDX: //  0xE0,
		{
			auto reg=state.B();
			SubByte(reg,FetchByte(mem,DecodeIndexedAddress(inst,mem)));
			state.SetB(reg);
		}
		break;
	case INST_SUBB_EXT: //  0xF0,
		{
			auto reg=state.B();
			SubByte(reg,FetchByte(mem,inst.ExtendedAddress()));
			state.SetB(reg);
		}
		break;

	case INST_SUBD_IMM: //  0x83,
		SubWord(state.D,mc6809util::FetchWord(inst.operand));
		break;
	case INST_SUBD_DP: //   0x93,
		SubWord(state.D,FetchWord(mem,DecodeDirectPageAddress(inst)));
		break;
	case INST_SUBD_IDX: //  0xA3,
		SubWord(state.D,FetchWord(mem,DecodeIndexedAddress(inst,mem)));
		break;
	case INST_SUBD_EXT: //  0xB3,
		SubWord(state.D,FetchWord(mem,inst.ExtendedAddress()));
		break;

	case INST_SWI: //       0x3F,
		if(true==debugger.enableCallStack)
		{
			debugger.PushCallStack(Debugger::CALLTYPE_SWI,state.S,state.PC,inst.length,mem.NonDestructiveFetchWord(SWI_VECTOR_ADDR));
		}
		state.CC|=EF;
		PushS16(mem,state.PC+inst.length);
		PushS16(mem,state.U);
		PushS16(mem,state.Y);
		PushS16(mem,state.X);
		PushS8(mem,state.DP);
		PushS8(mem,state.B());
		PushS8(mem,state.A());
		PushS8(mem,state.CC);
		state.CC|=(IRQMASK|FIRQMASK);
		state.PC=mem.FetchWord(SWI_VECTOR_ADDR);
		inst.length=0;
		break;
	case INST_SWI2: //      0x13F,  // 10 3F
		if(true==debugger.enableCallStack)
		{
			debugger.PushCallStack(Debugger::CALLTYPE_SWI2,state.S,state.PC,inst.length,mem.NonDestructiveFetchWord(SWI2_VECTOR_ADDR));
		}
		state.CC|=EF;
		PushS16(mem,state.PC+inst.length);
		PushS16(mem,state.U);
		PushS16(mem,state.Y);
		PushS16(mem,state.X);
		PushS8(mem,state.DP);
		PushS8(mem,state.B());
		PushS8(mem,state.A());
		PushS8(mem,state.CC);
		// Motorola 6809 and Hitatchi 6309 Programming Reference pp. 134.  SWI2 and SWI3 does not mask IRQ and FIRQ.
		state.PC=mem.FetchWord(SWI2_VECTOR_ADDR);
		inst.length=0;
		break;
	case INST_SWI3: //      0x23F,  // 11 3F
		if(true==debugger.enableCallStack)
		{
			debugger.PushCallStack(Debugger::CALLTYPE_SWI3,state.S,state.PC,inst.length,mem.NonDestructiveFetchWord(SWI3_VECTOR_ADDR));
		}
		state.CC|=EF;
		PushS16(mem,state.PC+inst.length);
		PushS16(mem,state.U);
		PushS16(mem,state.Y);
		PushS16(mem,state.X);
		PushS8(mem,state.DP);
		PushS8(mem,state.B());
		PushS8(mem,state.A());
		PushS8(mem,state.CC);
		// Motorola 6809 and Hitatchi 6309 Programming Reference pp. 134.  SWI2 and SWI3 does not mask IRQ and FIRQ.
		state.PC=mem.FetchWord(SWI3_VECTOR_ADDR);
		inst.length=0;
		break;

	case INST_SYNC: //      0x13,   // Is it HLT?
		Abort("Instruction not supported yet.");
		inst.length=0;
		break;

	case INST_TFR_REG: //   0x1F,
		{
			uint8_t reg[2];
			DecodeExgTfrReg(reg,inst.operand[0]);
			if(regBits[reg[0]]<regBits[reg[1]])
			{
				Abort("TFR to larger-size register not supported yet.");
				inst.length=0;
			}

			uint16_t value=GetRegisterValue(reg[0]);
			if(REG_PC==reg[0])
			{
				value+=inst.length;
			}

			SetRegisterValue(reg[1],value); // If 16bit to 8bit TFR, high byte will be erased in SetRegisterValue.
			if(REG_S==reg[1])
			{
				state.nmiEnabled=true;
			}
			else if(REG_PC==reg[1])
			{
				inst.length=0;
			}
		}
		break;

	case INST_TSTA: //      0x4D,
		Test8(state.A());
		break;
	case INST_TSTB: //      0x5D,
		Test8(state.B());
		break;

	case INST_TST_DP: //    0x0D,
		Test8(FetchByte(mem,DecodeDirectPageAddress(inst)));
		break;
	case INST_TST_IDX: //   0x6D,
		Test8(FetchByte(mem,DecodeIndexedAddress(inst,mem)));
		break;
	case INST_TST_EXT: //   0x7D,
		Test8(FetchByte(mem,inst.ExtendedAddress()));
		break;

	case INST_BCC_IMM: //   0x24,
		if(0==(state.CC&CF))
		{
			state.PC+=inst.BranchOffset8();
		}
		break;
	case INST_LBCC_IMM16: //0x124, // 10 24
		if(0==(state.CC&CF))
		{
			state.PC+=inst.BranchOffset16();
		}
		break;

	case INST_BCS_IMM: //   0x25,
		if(0!=(state.CC&CF))
		{
			state.PC+=inst.BranchOffset8();
		}
		break;
	case INST_LBCS_IMM16: //0x125, // 10 25
		if(0!=(state.CC&CF))
		{
			state.PC+=inst.BranchOffset16();
		}
		break;

	case INST_BEQ_IMM: //   0x27,
		if(0!=(state.CC&ZF))
		{
			state.PC+=inst.BranchOffset8();
		}
		break;
	case INST_LBEQ_IMM16: //0x127, // 10 27
		if(0!=(state.CC&ZF))
		{
			state.PC+=inst.BranchOffset16();
		}
		break;

	case INST_BGE_IMM: //   0x2C,
		if((0!=(state.CC&SF))==(0!=(state.CC&VF)))
		{
			state.PC+=inst.BranchOffset8();
		}
		break;
	case INST_LBGE_IMM16: //0x12C, // 10 2C
		if((0!=(state.CC&SF))==(0!=(state.CC&VF)))
		{
			state.PC+=inst.BranchOffset16();
		}
		break;

	case INST_BGT_IMM: //   0x2E,
		if((0!=(state.CC&SF))==(0!=(state.CC&VF)) && 0==(state.CC&ZF))
		{
			state.PC+=inst.BranchOffset8();
		}
		break;
	case INST_LBGT_IMM16: //0x12E, // 10 2E
		if((0!=(state.CC&SF))==(0!=(state.CC&VF)) && 0==(state.CC&ZF))
		{
			state.PC+=inst.BranchOffset16();
		}
		break;

	case INST_BHI_IMM: //   0x22,
		if(0==(state.CC&ZF) && 0==(state.CC&CF))
		{
			state.PC+=inst.BranchOffset8();
		}
		break;
	case INST_LBHI_IMM16: //0x122, // 10 22
		if(0==(state.CC&ZF) && 0==(state.CC&CF))
		{
			state.PC+=inst.BranchOffset16();
		}
		break;

	case INST_BLE_IMM: //   0x2F,
		if((0!=(state.CC&SF))!=(0!=(state.CC&VF)) || 0!=(state.CC&ZF))
		{
			state.PC+=inst.BranchOffset8();
		}
		break;
	case INST_LBLE_IMM16: //0x12F, // 10 2F
		if((0!=(state.CC&SF))!=(0!=(state.CC&VF)) || 0!=(state.CC&ZF))
		{
			state.PC+=inst.BranchOffset16();
		}
		break;

	case INST_BLS_IMM: //   0x23,
		if(0!=(state.CC&ZF) || 0!=(state.CC&CF))
		{
			state.PC+=inst.BranchOffset8();
		}
		break;
	case INST_LBLS_IMM16: //0x123, // 10 23
		if(0!=(state.CC&ZF) || 0!=(state.CC&CF))
		{
			state.PC+=inst.BranchOffset16();
		}
		break;

	case INST_BLT_IMM: //   0x2D,
		if((0!=(state.CC&SF))!=(0!=(state.CC&VF)))
		{
			state.PC+=inst.BranchOffset8();
		}
		break;
	case INST_LBLT_IMM16: //0x12D, // 10 2D
		if((0!=(state.CC&SF))!=(0!=(state.CC&VF)))
		{
			state.PC+=inst.BranchOffset16();
		}
		break;

	case INST_BMI_IMM: //   0x2B,
		if(0!=(state.CC&SF))
		{
			state.PC+=inst.BranchOffset8();
		}
		break;
	case INST_LBMI_IMM16: //0x12B, // 10 2B
		if(0!=(state.CC&SF))
		{
			state.PC+=inst.BranchOffset16();
		}
		break;

	case INST_BNE_IMM: //   0x26,
		if(0==(state.CC&ZF))
		{
			state.PC+=inst.BranchOffset8();
		}
		break;
	case INST_LBNE_IMM16: //0x126, // 10 26
		if(0==(state.CC&ZF))
		{
			state.PC+=inst.BranchOffset16();
		}
		break;

	case INST_BPL_IMM: //   0x2A,
		if(0==(state.CC&SF))
		{
			state.PC+=inst.BranchOffset8();
		}
		break;
	case INST_LBPL_IMM16: //0x12A, // 10 2A
		if(0==(state.CC&SF))
		{
			state.PC+=inst.BranchOffset16();
		}
		break;

	case INST_BRA_IMM: //   0x20,
		state.PC+=inst.BranchOffset8();
		break;
	case INST_LBRA_IMM16: //0x16
	case INST_LBRA_IMM16_ALT: //0x120, // 10 20
		state.PC+=inst.BranchOffset16();
		break;

	case INST_BRN_IMM: //   0x21,
		break;
	case INST_LBRN_IMM16: //0x121, // 10 21
		break;

	case INST_BSR_IMM: //   0x8D,
		if(true==debugger.enableCallStack)
		{
			debugger.PushCallStack(Debugger::CALLTYPE_BSR_JSR,state.S,state.PC,inst.length,state.PC+inst.length+inst.BranchOffset8());
		}
		PushS16(mem,state.PC+inst.length);
		state.PC+=inst.BranchOffset8();
		break;
	case INST_LBSR_IMM16: // 0x17
		if(true==debugger.enableCallStack)
		{
			debugger.PushCallStack(Debugger::CALLTYPE_BSR_JSR,state.S,state.PC,inst.length,state.PC+inst.length+inst.BranchOffset16());
		}
		PushS16(mem,state.PC+inst.length);
		state.PC+=inst.BranchOffset16();
		break;

	case INST_BVC_IMM: //   0x28,
		if(0==(state.CC&VF))
		{
			state.PC+=inst.BranchOffset8();
		}
		break;
	case INST_LBVC_IMM16: //0x128, // 10 28
		if(0==(state.CC&VF))
		{
			state.PC+=inst.BranchOffset16();
		}
		break;

	case INST_BVS_IMM: //   0x29,
		if(0!=(state.CC&VF))
		{
			state.PC+=inst.BranchOffset8();
		}
		break;
	case INST_LBVS_IMM16: //0x129, // 10 29
		if(0!=(state.CC&VF))
		{
			state.PC+=inst.BranchOffset16();
		}
		break;
	case INST_UNDEF_COMNEGA: // 0x42
		// Motorola 6809 and Hitachi 6309 Programming Reference pp.153
		// "execute as a NEG instruction when the Carry bit in CC is 0, and as a COM instruction when the Carry bit is 1."
		if(0==(state.CC&CF))
		{
			state.SetA(NEG(state.A()));
		}
		else
		{
			auto reg=~state.A();
			Test8(reg);
			state.CC|=CF;
			state.SetA(reg);
		}
		break;
	default:
		Abort("Undefined instruction.");
		inst.length=0;
		break;
	}

	state.PC+=inst.length;
	debugger.AfterRunOneInstruction(*this,mem);
	return inst.clocks;
}
uint8_t MC6809::AND(uint8_t a,uint8_t b)
{
	state.CC&=~(SF|ZF|VF);
	a&=b;
	RaiseSF(0!=(0x80&a));
	RaiseZF(0==a);
	return a;
}
uint8_t MC6809::ASR(uint8_t data)
{
	state.CC&=~(SF|ZF|CF);
	uint8_t signBit=(data&0x80);
	RaiseCF(0!=(data&1));
	data=signBit|(data>>1);
	RaiseSF(0!=signBit);
	RaiseZF(0==data);
	return data;
}
uint8_t MC6809::INC(uint8_t data)
{
	state.CC&=~(SF|ZF|VF);
	RaiseVF(0x7F==data);
	++data;
	RaiseSF(0!=(0x80&data));
	RaiseZF(0==data);
	return data;
}
uint8_t MC6809::DEC(uint8_t data)
{
	state.CC&=~(SF|ZF|VF);
	RaiseVF(0x80==data);
	--data;
	RaiseSF(0!=(0x80&data));
	RaiseZF(0==data);
	return data;
}
uint8_t MC6809::LSL(uint8_t data)
{
	state.CC&=~(SF|ZF|VF|CF);
	RaiseCF(0!=(data&0x80));
	RaiseVF(0!=(((data>>6)^(data>>7))&1));
	data<<=1;
	RaiseSF(0!=(data&0x80));
	RaiseZF(0==data);
	return data;
}
uint8_t MC6809::LSR(uint8_t data)
{
	state.CC&=~(SF|ZF|CF);
	// SF is always clear.
	RaiseCF(0!=(data&0x01));
	data>>=1;
	RaiseZF(0==data);
	return data;
}
uint8_t MC6809::OR(uint8_t a,uint8_t b)
{
	state.CC&=~(SF|ZF|VF);
	a|=b;
	RaiseSF(0!=(a&0x80));
	RaiseZF(0==a);
	return a;
}
uint8_t MC6809::NEG(uint8_t data)
{
	state.CC&=~(SF|ZF|VF|CF);
	if(0x80==data)
	{
		state.CC|=VF;
	}
	if(0!=data)
	{
		state.CC|=CF;
	}
	data=~data;
	++data;
	if(0==data)
	{
		state.CC|=ZF;
	}
	if(0!=(0x80&data))
	{
		state.CC|=SF;
	}
	return data;
}
uint8_t MC6809::ROL(uint8_t data)
{
	uint8_t carry=(0!=((state.CC&CF) ? 1 : 0));
	state.CC&=~(SF|ZF|VF|CF);
	RaiseCF(0!=(data&0x80));
	RaiseVF(0!=(((data>>6)^(data>>7))&1));
	data<<=1;
	data|=carry;
	RaiseSF(0!=(data&0x80));
	RaiseZF(0==data);
	return data;
}
uint8_t MC6809::ROR(uint8_t data)
{
	uint8_t carry=(0!=(state.CC&CF) ? 0x80 : 0);
	state.CC&=~(SF|ZF|CF);
	// SF is always clear.
	RaiseSF(0!=carry);
	RaiseCF(0!=(data&0x01));
	data>>=1;
	data|=carry;
	RaiseZF(0==data);
	return data;
}

uint16_t MC6809::DecodeIndexedAddress(const Instruction &inst,MemoryAccess &mem)
{
	auto &indexReg=RegisterRef16(inst.indexReg);
	uint16_t addr=0;
	switch(inst.indexType)
	{
	case INDEX_NO_OFFSET:
		addr=indexReg;
		break;
	case INDEX_CONST_OFFSET_FROM_REG:
	case INDEX_8BIT_OFFSET:
	case INDEX_16BIT_OFFSET:
		addr=indexReg+inst.offset;
		break;
	case INDEX_ACCUM_OFFSET_FROM_REG:
		{
			uint16_t accum=GetRegisterValueSigned(inst.offset);
			addr=indexReg+accum;
		}
		break;
	case INDEX_POST_INC_1:
		addr=(indexReg++);
		break;
	case INDEX_POST_INC_2:
		addr=indexReg;
		indexReg+=2;
		break;
	case INDEX_PRE_DEC_1:
		addr=(--indexReg);
		break;
	case INDEX_PRE_DEC_2:
		indexReg-=2;
		addr=indexReg;
		break;
	case INDEX_EXTENDED:
		addr=inst.offset;
		break;
	}
	if(true==inst.indexIndir)
	{
		addr=FetchWord(mem,addr);
	}
	return addr;
}
uint16_t MC6809::DecodeDirectPageAddress(const Instruction &inst)
{
	uint16_t addr;
	addr=state.DP;
	addr<<=8;
	addr|=inst.operand[0];
	return addr;
}
void MC6809::PushS16(MemoryAccess &mem,uint16_t value)
{
	state.S-=2;
	StoreWord(mem,state.S,value);
}
void MC6809::PushS8(MemoryAccess &mem,uint8_t value)
{
	StoreByte(mem,--state.S,value);
}
uint16_t MC6809::PullS16(MemoryAccess &mem)
{
	auto S=state.S;
	state.S+=2;
	return FetchWord(mem,S);
}
uint8_t MC6809::PullS8(MemoryAccess &mem)
{
	return FetchByte(mem,state.S++);
}

void MC6809::PushU16(MemoryAccess &mem,uint16_t value)
{
	state.U-=2;
	StoreWord(mem,state.U,value);
}
void MC6809::PushU8(MemoryAccess &mem,uint8_t value)
{
	StoreByte(mem,--state.U,value);
}
uint16_t MC6809::PullU16(MemoryAccess &mem)
{
	auto U=state.U;
	state.U+=2;
	return FetchWord(mem,U);
}
uint8_t MC6809::PullU8(MemoryAccess &mem)
{
	return FetchByte(mem,state.U++);
}

void MC6809::WriteToIndex16(class MemoryAccess &mem,const Instruction &inst,uint16_t value)
{
	uint16_t addr=DecodeIndexedAddress(inst,mem);
	StoreWord(mem,addr,value);
}
void MC6809::WriteToIndex8(class MemoryAccess &mem,const Instruction &inst,uint8_t value)
{
	uint16_t addr=DecodeIndexedAddress(inst,mem);
	StoreByte(mem,addr,value);
}

const uint16_t &MC6809::RegisterRef16(uint8_t reg) const
{
	switch(reg)
	{
	case REG_X:
		return state.X;
	case REG_Y:
		return state.Y;
	case REG_U:
		return state.U;
	case REG_S:
		return state.S;

	case REG_PC:
		return state.PC;
	}
	std::string msg;
	msg="Cannot do RegisterRef16 for ";
	msg+=RegToStr(reg);
	Abort(msg);
	return state.X;
}
uint16_t &MC6809::RegisterRef16(uint8_t reg)
{
	switch(reg)
	{
	case REG_X:
		return state.X;
	case REG_Y:
		return state.Y;
	case REG_U:
		return state.U;
	case REG_S:
		return state.S;

	case REG_PC:
		return state.PC;
	}
	std::string msg;
	msg="Cannot do RegisterRef16 for ";
	msg+=RegToStr(reg);
	Abort(msg);
	return state.X;
}

uint16_t MC6809::GetRegisterValue(uint8_t reg) const
{
	switch(reg)
	{
	case REG_CC:
		return state.CC;
	case REG_A:
		return state.A();
	case REG_B:
		return state.B();
	case REG_DP:
		return state.DP;

	case REG_X:
		return state.X;
	case REG_Y:
		return state.Y;
	case REG_U:
		return state.U;
	case REG_S:
		return state.S;

	case REG_PC:
		return state.PC;
	case REG_D:
		return state.D;
	}
	return 0;
}
int16_t MC6809::GetRegisterValueSigned(uint8_t reg) const
{
	int32_t ret;
	switch(reg)
	{
	case REG_CC:
		ret=state.CC;
		return (ret&0x7F)-(ret&0x80);
	case REG_A:
		ret=state.A();
		return (ret&0x7F)-(ret&0x80);
	case REG_B:
		ret=state.B();
		return (ret&0x7F)-(ret&0x80);
	case REG_DP:
		ret=state.DP;
		return (ret&0x7F)-(ret&0x80);

	case REG_X:
		ret=state.X;
		return (ret&0x7FFF)-(ret&0x8000);
	case REG_Y:
		ret=state.Y;
		return (ret&0x7FFF)-(ret&0x8000);
	case REG_U:
		ret=state.U;
		return (ret&0x7FFF)-(ret&0x8000);
	case REG_S:
		ret=state.S;
		return (ret&0x7FFF)-(ret&0x8000);

	case REG_PC:
		ret=state.PC;
		return (ret&0x7FFF)-(ret&0x8000);
	case REG_D:
		ret=state.D;
		return (ret&0x7FFF)-(ret&0x8000);
	}
	return 0;
}
void MC6809::SetRegisterValue(uint8_t reg,uint16_t value)
{
	switch(reg)
	{
	case REG_CC:
		state.CC=value;
		break;
	case REG_A:
		state.SetA(value);
		break;
	case REG_B:
		state.SetB(value);
		break;
	case REG_DP:
		state.DP=value;
		break;

	case REG_X:
		state.X=value;
		break;
	case REG_Y:
		state.Y=value;
		break;
	case REG_U:
		state.U=value;
		break;
	case REG_S:
		state.S=value;
		break;

	case REG_PC:
		state.PC=value;
		break;
	case REG_D:
		state.D=value;
		break;
	}
}

void MC6809::AddWord(uint16_t &value1,uint16_t value2)
{
	uint32_t prevValue=value1&0xffff;
	value1=(value1+value2)&0xffff;
	state.CC&=~(SF|ZF|VF|CF);
	RaiseVF((prevValue&0x8000)==(value2&0x8000) && (prevValue&0x8000)!=(value1&0x8000)); // Two sources have same sign, but the result sign is different.
	RaiseSF(0!=(value1&0x8000));
	RaiseZF(0==value1);
	RaiseCF(value1<prevValue);
	// According to the manual, 16-bit Add does not change HF.
}
void MC6809::AddByte(uint8_t &value1,uint16_t value2)
{
	uint16_t prevValue=value1;
	uint16_t nextValue=prevValue+value2;
	value1=nextValue&0xff;
	state.CC&=~(SF|ZF|VF|CF|HF);
	RaiseVF((prevValue&0x80)==(value2&0x80) && (prevValue&0x80)!=(value1&0x80)); // Two sources have same sign, but the result sign is different.
	RaiseSF(0!=(value1&0x80));
	RaiseZF(0==value1);
	RaiseCF(0xFF<nextValue);
	RaiseHF((value1&0x0F)<(prevValue&0x0F)); // Same as AF in Intel chip?
}
void MC6809::SubWord(uint16_t &value1,uint16_t value2)
{
	uint32_t prevValue=value1;
	value1=(value1-value2)&0xffff;
	state.CC&=~(SF|ZF|VF|CF);
	RaiseVF((prevValue&0x8000)!=(value2&0x8000) && (prevValue&0x8000)!=(value1&0x8000)); // Source values have different signs, but the sign flipped.
	RaiseSF(0!=(value1&0x8000));
	RaiseZF(0==value1);
	RaiseCF(value1>prevValue);
}
void MC6809::SubByte(uint8_t &value1,uint16_t value2)
{
	uint16_t prevValue=value1;
	state.CC&=~(SF|ZF|VF|CF);
	RaiseCF(value1<value2);
	value1=(value1-value2)&0xff;
	RaiseVF((prevValue&0x80)!=(value2&0x80) && (prevValue&0x80)!=(value1&0x80)); // Source values have different signs, but the sign flipped.
	RaiseSF(0!=(value1&0x80));
	RaiseZF(0==value1);
}

class DestructiveMemoryFetch
{
public:
	inline static uint8_t FetchByte(MemoryAccess &mem,uint16_t addr)
	{
		return mem.FetchByte(addr);
	}
	inline static uint16_t FetchWord(MemoryAccess &mem,uint16_t addr)
	{
		return mem.FetchWord(addr);
	}
};
class NonDestructiveMemoryFetch
{
public:
	inline static uint8_t FetchByte(const MemoryAccess &mem,uint16_t addr)
	{
		return mem.NonDestructiveFetchByte(addr);
	}
	inline static uint16_t FetchWord(const MemoryAccess &mem,uint16_t addr)
	{
		return mem.NonDestructiveFetchWord(addr);
	}
};
template <class ConstOrNonConstMemoryAccess,class MemoryFetch>
MC6809::Instruction MC6809::FetchInstructionTemplate(ConstOrNonConstMemoryAccess &mem,uint16_t PC) const
{
	Instruction inst;

	inst.opCode=MemoryFetch::FetchByte(mem,PC++);
	inst.length=1;

REFETCH_MULTI_BYTE:
	inst.clocks=instClock[inst.opCode];
	inst.operType=instOperaType[inst.opCode];

	switch(instOperaType[inst.opCode])
	{
	case OPER_MULTI_BYTE_10:
		inst.length=2;
		inst.opCode=0x100|MemoryFetch::FetchByte(mem,PC++);
		goto REFETCH_MULTI_BYTE;
	case OPER_MULTI_BYTE_11:
		inst.length=2;
		inst.opCode=0x200|MemoryFetch::FetchByte(mem,PC++);
		goto REFETCH_MULTI_BYTE;
	case OPER_IMM:
	case OPER_DP:
	case OPER_REG:
		// 1-byte operand
		++inst.length;
		inst.operand[0]=MemoryFetch::FetchByte(mem,PC++);
		break;
	case OPER_EXT:
	case OPER_IMM16:
		// 2-bytes operand
		inst.length+=2;
		inst.operand[0]=MemoryFetch::FetchByte(mem,PC++);
		inst.operand[1]=MemoryFetch::FetchByte(mem,PC++);
		break;
	case OPER_INHERENT:
		// No operand.
		break;
	case OPER_IDX:
		++inst.length;
		inst.operand[0]=MemoryFetch::FetchByte(mem,PC++);
		inst.indexReg=REG_X+((inst.operand[0]>>5)&3);
		if(0==(inst.operand[0]&0x80)) // 5-bit offset from REG
		{
			inst.indexType=INDEX_CONST_OFFSET_FROM_REG;
			inst.indexIndir=false;
			inst.offset=(inst.operand[0]&0x0F)-(inst.operand[0]&0x10);
			inst.clocks+=1;
		}
		else
		{
			uint8_t idByte=(inst.operand[0]&0b10011111);
			switch(idByte)
			{
			case 0b10000100: // No Offset Direct
				inst.indexType=INDEX_NO_OFFSET;
				inst.indexIndir=false;
				break;
			case 0b10010100: // No Offset Indirect
				inst.clocks+=3;
				inst.indexType=INDEX_CONST_OFFSET_FROM_REG;
				inst.indexIndir=true;
				inst.offset=0;
				break;
			case 0b10001000: // 8-bit offset from REG Direct
				inst.clocks+=1;
				++inst.length;
				inst.indexType=INDEX_CONST_OFFSET_FROM_REG;
				inst.indexIndir=false;
				inst.operand[1]=MemoryFetch::FetchByte(mem,PC++);
				inst.offset=inst.operand[1];
				inst.offset=(inst.offset&0x7F)-(inst.offset&0x80);
				break;
			case 0b10011000: // 8-bit offset from REG Indirect
				inst.clocks+=4;
				++inst.length;
				inst.indexType=INDEX_CONST_OFFSET_FROM_REG;
				inst.indexIndir=true;
				inst.operand[1]=MemoryFetch::FetchByte(mem,PC++);
				inst.offset=inst.operand[1];
				inst.offset=(inst.offset&0x7F)-(inst.offset&0x80);
				break;
			case 0b10001001: // 16-bit offset from REG Direct
				inst.clocks+=4;
				inst.length+=2;
				inst.indexType=INDEX_CONST_OFFSET_FROM_REG;
				inst.indexIndir=false;
				inst.operand[1]=MemoryFetch::FetchByte(mem,PC++);
				inst.operand[2]=MemoryFetch::FetchByte(mem,PC++);
				inst.offset=mc6809util::FetchWord(inst.operand[1],inst.operand[2]);
				inst.offset=(inst.offset&0x7FFF)-(inst.offset&0x8000);
				break;
			case 0b10011001: // 16-bit offset from REG Indirect
				inst.clocks+=7;
				inst.length+=2;
				inst.indexType=INDEX_CONST_OFFSET_FROM_REG;
				inst.indexIndir=true;
				inst.operand[1]=MemoryFetch::FetchByte(mem,PC++);
				inst.operand[2]=MemoryFetch::FetchByte(mem,PC++);
				inst.offset=mc6809util::FetchWord(inst.operand[1],inst.operand[2]);
				inst.offset=(inst.offset&0x7FFF)-(inst.offset&0x8000);
				break;
			case 0b10000110: // A reg as offset Direct
				inst.clocks+=1;
				inst.indexType=INDEX_ACCUM_OFFSET_FROM_REG;
				inst.indexIndir=false;
				inst.offset=REG_A;
				break;
			case 0b10010110: // A reg as offset Indirect
				inst.clocks+=4;
				inst.indexType=INDEX_ACCUM_OFFSET_FROM_REG;
				inst.indexIndir=true;
				inst.offset=REG_A;
				break;
			case 0b10000101: // B reg as offset Direct
				inst.clocks+=1;
				inst.indexType=INDEX_ACCUM_OFFSET_FROM_REG;
				inst.indexIndir=false;
				inst.offset=REG_B;
				break;
			case 0b10010101: // B reg as offset Indirect
				inst.clocks+=4;
				inst.indexType=INDEX_ACCUM_OFFSET_FROM_REG;
				inst.indexIndir=true;
				inst.offset=REG_B;
				break;
			case 0b10001011: // D reg as offset Direct
				inst.clocks+=4;
				inst.indexType=INDEX_ACCUM_OFFSET_FROM_REG;
				inst.indexIndir=false;
				inst.offset=REG_D;
				break;
			case 0b10011011: // D reg as offset Indirect
				inst.clocks+=7;
				inst.indexType=INDEX_ACCUM_OFFSET_FROM_REG;
				inst.indexIndir=true;
				inst.offset=REG_D;
				break;
			case 0b10000000: // Post-Inc by 1 Direct
				inst.clocks+=2;
				inst.indexType=INDEX_POST_INC_1;
				inst.offset=0;
				inst.indexIndir=false;
				break;
			case 0b10000001: // Post-Inc by 2 Direct
				inst.clocks+=3;
				inst.indexType=INDEX_POST_INC_2;
				inst.offset=0;
				inst.indexIndir=false;
				break;
			case 0b10010001: // Post-Inc by 2 Indirect
				inst.clocks+=6;
				inst.indexType=INDEX_POST_INC_2;
				inst.offset=0;
				inst.indexIndir=true;
				break;
			case 0b10000010: // Pre-Dec by 1 Direct
				inst.clocks+=2;
				inst.indexType=INDEX_PRE_DEC_1;
				inst.offset=0;
				inst.indexIndir=false;
				break;
			case 0b10000011: // Pre-Dec by 2 Direct
				inst.indexType=INDEX_PRE_DEC_2;
				inst.offset=0;
				inst.indexIndir=false;
				inst.clocks+=3;
				break;
			case 0b10010011: // Pre-Dec by 2 Indirect
				inst.clocks+=6;
				inst.indexType=INDEX_PRE_DEC_2;
				inst.offset=0;
				inst.indexIndir=true;
				break;
			case 0b10001100: // 8-bit offset from PC Direct
				inst.clocks+=1;
				++inst.length;
				inst.indexType=INDEX_CONST_OFFSET_FROM_REG;
				inst.indexIndir=false;
				inst.indexReg=REG_PC;
				inst.operand[1]=MemoryFetch::FetchByte(mem,PC++);
				inst.offset=inst.operand[1];
				inst.offset=(inst.offset&0x7F)-(inst.offset&0x80)+inst.length;
				break;
			case 0b10011100: // 8-bit offset from PC Indirect
				inst.clocks+=4;
				++inst.length;
				inst.indexType=INDEX_CONST_OFFSET_FROM_REG;
				inst.indexIndir=true;
				inst.indexReg=REG_PC;
				inst.operand[1]=MemoryFetch::FetchByte(mem,PC++);
				inst.offset=inst.operand[1];
				inst.offset=(inst.offset&0x7F)-(inst.offset&0x80)+inst.length;
				break;
			case 0b10001101: // 16-bit offset from PC Direct
				inst.clocks+=5;
				inst.length+=2;
				inst.indexType=INDEX_CONST_OFFSET_FROM_REG;
				inst.indexIndir=false;
				inst.indexReg=REG_PC;
				inst.operand[1]=MemoryFetch::FetchByte(mem,PC++);
				inst.operand[2]=MemoryFetch::FetchByte(mem,PC++);
				inst.offset=mc6809util::FetchWord(inst.operand[1],inst.operand[2]);
				inst.offset=(inst.offset&0x7FFF)-(inst.offset&0x8000)+inst.length;
				break;
			case 0b10011101: // 16-bit offset from PC Indirect
				inst.clocks+=8;
				inst.length+=2;
				inst.indexType=INDEX_CONST_OFFSET_FROM_REG;
				inst.indexIndir=true;
				inst.indexReg=REG_PC;
				inst.operand[1]=MemoryFetch::FetchByte(mem,PC++);
				inst.operand[2]=MemoryFetch::FetchByte(mem,PC++);
				inst.offset=mc6809util::FetchWord(inst.operand[1],inst.operand[2]);
				inst.offset=(inst.offset&0x7FFF)-(inst.offset&0x8000)+inst.length;
				break;
			case 0b10011111: // Extended Indirect
				inst.clocks+=5;
				inst.length+=2;
				inst.indexType=INDEX_EXTENDED;
				inst.indexIndir=true;
				inst.operand[1]=MemoryFetch::FetchByte(mem,PC++);
				inst.operand[2]=MemoryFetch::FetchByte(mem,PC++);
				inst.offset=mc6809util::FetchWord(inst.operand[1],inst.operand[2]);
				break;
			default:
				inst.indexType=INDEX_UNSUPPORTED;
				break;
			}
		}
		break;
	}

	return inst;
}
MC6809::Instruction MC6809::FetchInstruction(class MemoryAccess &mem,uint16_t PC) const
{
	return FetchInstructionTemplate<MemoryAccess,DestructiveMemoryFetch>(mem,PC);
}
MC6809::Instruction MC6809::NonDestructiveFetchInstruction(const class MemoryAccess &mem,uint16_t PC) const
{
	return FetchInstructionTemplate<const MemoryAccess,NonDestructiveMemoryFetch>(mem,PC);
}

void MC6809::DecodeExgTfrReg(uint8_t reg[2],uint8_t postByte) const
{
	reg[0]=regToReg[postByte>>4];
	reg[1]=regToReg[postByte&0x0F];
}

std::string MC6809::WholeDisassembly(class MemoryAccess &mem,uint16_t PC) const
{
	std::string disasm=cpputil::Ustox(PC);
	disasm.push_back(' ');

	auto inst=NonDestructiveFetchInstruction(mem,PC);
	disasm+=FormatByteCode(inst);
	while(disasm.size()<16)
	{
		disasm.push_back(' ');
	}

	disasm+=Disassemble(inst,PC);
	return disasm;
}

std::string MC6809::FormatByteCode(Instruction inst) const
{
	std::string str;

	int n=1;
	if(0x200<=inst.opCode)
	{
		str+="11";
		str+=cpputil::Ubtox(inst.opCode&0xFF);
		n=2;
	}
	else if(0x100<=inst.opCode)
	{
		str+="10";
		str+=cpputil::Ubtox(inst.opCode&0xFF);
		n=2;
	}
	else
	{
		str+=cpputil::Ubtox(inst.opCode);
	}

	for(int i=0; i+n<inst.length; ++i)
	{
		str+=cpputil::Ubtox(inst.operand[i]);
	}
	return str;
}

std::string MC6809::Disassemble(Instruction inst,uint16_t PC) const
{
	std::string disasm;

	disasm=instLabel[inst.opCode];
	if(OPER_INHERENT!=inst.operType)
	{
		while(disasm.size()<8)
		{
			disasm.push_back(' ');
		}
		switch(inst.opCode)
		{
		default:
			disasm+=DisassembleOperand(inst,PC);
			break;
		case INST_PSHS_REG: // 0x34,
		case INST_PULS_REG: // 0x35,
			disasm+=DisassemblePSHPULRegisters(inst.operand[0],'U');
			break;
		case INST_PSHU_REG: // 0x36,
		case INST_PULU_REG: // 0x37,
			disasm+=DisassemblePSHPULRegisters(inst.operand[0],'S');
			break;
		case INST_BCC_IMM: //   0x24,
		case INST_BCS_IMM: //   0x25,
		case INST_BEQ_IMM: //   0x27,
		case INST_BGE_IMM: //   0x2C,
		case INST_BGT_IMM: //   0x2E,
		case INST_BHI_IMM: //   0x22,
		case INST_BLE_IMM: //   0x2F,
		case INST_BLS_IMM: //   0x23,
		case INST_BLT_IMM: //   0x2D,
		case INST_BMI_IMM: //   0x2B,
		case INST_BNE_IMM: //   0x26,
		case INST_BPL_IMM: //   0x2A,
		case INST_BRA_IMM: //   0x20,
		case INST_BRN_IMM: //   0x21,
		case INST_BSR_IMM: //   0x8D,
		case INST_BVC_IMM: //   0x28,
		case INST_BVS_IMM: //   0x29,
			{
				int16_t offset=inst.operand[0];
				offset=(offset&0x7f)-(offset&0x80);
				uint16_t addr=PC+inst.length+offset;
				disasm.push_back('$');
				disasm+=cpputil::Ustox(addr);
			}
			break;
		case INST_LBCC_IMM16: //0x124, // 10 24
		case INST_LBCS_IMM16: //0x125, // 10 25
		case INST_LBEQ_IMM16: //0x127, // 10 27
		case INST_LBGE_IMM16: //0x12C, // 10 2C
		case INST_LBGT_IMM16: //0x12E, // 10 2E
		case INST_LBHI_IMM16: //0x122, // 10 22
		case INST_LBLE_IMM16: //0x12F, // 10 2F
		case INST_LBLS_IMM16: //0x123, // 10 23
		case INST_LBLT_IMM16: //0x12D, // 10 2D
		case INST_LBMI_IMM16: //0x12B, // 10 2B
		case INST_LBNE_IMM16: //0x126, // 10 26
		case INST_LBPL_IMM16: //0x12A, // 10 2A
		case INST_LBRA_IMM16: //0x16
		case INST_LBRA_IMM16_ALT: //0x120, // 10 20
		case INST_LBRN_IMM16: //0x121, // 10 21
		case INST_LBSR_IMM16: //0x18D, // 10 2D
		case INST_LBVC_IMM16: //0x128, // 10 28
		case INST_LBVS_IMM16: //0x129, // 10 29
			{
				int32_t offset=mc6809util::FetchWord(inst.operand[0],inst.operand[1]);
				offset=(offset&0x7FFF)-(offset&0x8000);
				uint16_t addr=PC+inst.length+offset;
				disasm.push_back('$');
				disasm+=cpputil::Ustox(addr);
			}
			break;
		}
	}
	return disasm;
}
std::string MC6809::DisassembleOperand(Instruction inst,uint16_t PC) const
{
	std::string disasm;
	switch(inst.operType)
	{
	case OPER_IMM:
		disasm="#$";
		disasm+=cpputil::Ubtox(inst.operand[0]);
		break;
	case OPER_DP:
		disasm="<$";
		disasm+=cpputil::Ubtox(inst.operand[0]);
		break;
	case OPER_IDX:
		if(true==inst.indexIndir)
		{
			disasm="[";
		}
		switch(inst.indexType)
		{
		case INDEX_NO_OFFSET:
			disasm.push_back(',');
			disasm+=RegToStr(inst.indexReg);
			break;
		case INDEX_CONST_OFFSET_FROM_REG:
		case INDEX_8BIT_OFFSET:
		case INDEX_16BIT_OFFSET:
			{
				if(REG_PC==inst.indexReg)
				{
					int addr=(PC+inst.offset)&0xFFFF;
					disasm.push_back('$');
					disasm+=cpputil::Ustox(addr);
					disasm+=",PCR";
				}
				else
				{
					int offset=inst.offset;
					if(offset<0)
					{
						offset=-offset;
						disasm.push_back('-');
					}
					disasm.push_back('$');
					if(-128<=inst.offset && inst.offset<128)
					{
						disasm+=cpputil::Ubtox(offset);
					}
					else
					{
						disasm+=cpputil::Ustox(offset);
					}
					disasm.push_back(',');
					disasm+=RegToStr(inst.indexReg);
				}
			}
			break;
		case INDEX_ACCUM_OFFSET_FROM_REG:
			disasm+=RegToStr(inst.offset);
			disasm.push_back(',');
			disasm+=RegToStr(inst.indexReg);
			break;
		case INDEX_POST_INC_1:
			disasm.push_back(',');
			disasm+=RegToStr(inst.indexReg);
			disasm.push_back('+');
			break;
		case INDEX_POST_INC_2:
			disasm.push_back(',');
			disasm+=RegToStr(inst.indexReg);
			disasm.push_back('+');
			disasm.push_back('+');
			break;
		case INDEX_PRE_DEC_1:
			disasm.push_back(',');
			disasm.push_back('-');
			disasm+=RegToStr(inst.indexReg);
			break;
		case INDEX_PRE_DEC_2:
			disasm.push_back(',');
			disasm.push_back('-');
			disasm.push_back('-');
			disasm+=RegToStr(inst.indexReg);
			break;
		case INDEX_EXTENDED:
			disasm.push_back('$');
			disasm+=cpputil::Ustox(inst.offset);
			break;
		}
		if(true==inst.indexIndir)
		{
			disasm+="]";
		}
		break;
	case OPER_EXT:
		{
			int32_t imm16=mc6809util::FetchWord(inst.operand[0],inst.operand[1]);
			disasm="$";
			disasm+=cpputil::Ustox(imm16);
		}
		break;
	case OPER_INHERENT:
		break;
	case OPER_REG:
		{
			uint8_t reg[2];
			DecodeExgTfrReg(reg,inst.operand[0]);
			disasm+=RegToStr(reg[0]);
			disasm.push_back(',');
			disasm+=RegToStr(reg[1]);
		}
		break;
	case OPER_IMM16:
		{
			int32_t imm16=mc6809util::FetchWord(inst.operand[0],inst.operand[1]);
			disasm="#$";
			disasm+=cpputil::Ustox(imm16);
		}
		break;
	}
	return disasm;
}

std::string MC6809::DisassemblePSHPULRegisters(uint8_t flags,char UorS) const
{
	std::string disasm;
	if(0!=(flags&PSH_CC))
	{
		disasm="CC";
	}
	if(0!=(flags&PSH_A))
	{
		if(0<disasm.size())
		{
			disasm.push_back(',');
		}
		disasm.push_back('A');
	}
	if(0!=(flags&PSH_B))
	{
		if(0<disasm.size())
		{
			disasm.push_back(',');
		}
		disasm.push_back('B');
	}
	if(0!=(flags&PSH_DP))
	{
		if(0<disasm.size())
		{
			disasm.push_back(',');
		}
		disasm+="DP";
	}
	if(0!=(flags&PSH_X))
	{
		if(0<disasm.size())
		{
			disasm.push_back(',');
		}
		disasm.push_back('X');
	}
	if(0!=(flags&PSH_Y))
	{
		if(0<disasm.size())
		{
			disasm.push_back(',');
		}
		disasm.push_back('Y');
	}
	if(0!=(flags&PSH_UorS))
	{
		if(0<disasm.size())
		{
			disasm.push_back(',');
		}
		disasm.push_back(UorS);
	}
	if(0!=(flags&PSH_PC))
	{
		if(0<disasm.size())
		{
			disasm.push_back(',');
		}
		disasm+="PC";
	}
	return disasm;
}


/* static */ std::string MC6809::RegToStr(unsigned int reg)
{
	switch(reg)
	{
	case REG_A:
		return "A";
	case REG_B:
		return "B";
	case REG_D:
		return "D";
	case REG_DP:
		return "DP";
	case REG_CC:
		return "CC";

	case REG_X:
		return "X";
	case REG_Y:
		return "Y";
	case REG_U:
		return "U";
	case REG_S:
		return "S";

	case REG_PC:
		return "PC";
	}
	return "?";
}
/* static */ std::string MC6809::RegToStrPCR(unsigned int reg)
{
	if(REG_PC==reg)
	{
		return "PCR";
	}
	return RegToStr(reg);
}

/* static */ unsigned int MC6809::StrToReg(std::string str)
{
	if("PC"==str || "pc"==str)
	{
		return REG_PC;
	}
	if("A"==str || "a"==str)
	{
		return REG_A;
	}
	if("B"==str || "b"==str)
	{
		return REG_B;
	}
	if("D"==str || "d"==str)
	{
		return REG_D;
	}
	if("DP"==str || "dp"==str)
	{
		return REG_DP;
	}
	if("CC"==str || "cc"==str)
	{
		return REG_CC;
	}
	if("X"==str || "x"==str)
	{
		return REG_X;
	}
	if("Y"==str || "y"==str)
	{
		return REG_Y;
	}
	if("U"==str || "u"==str)
	{
		return REG_U;
	}
	if("S"==str || "s"==str)
	{
		return REG_S;
	}
	return REG_NULL;
}

std::vector <std::string> MC6809::GetStatusText(void) const
{
	std::vector <std::string> text;
	std::string str;

	str="A=";
	str+=cpputil::Ubtox(state.A());
	str+=" B=";
	str+=cpputil::Ubtox(state.B());
	str+=" DP=";
	str+=cpputil::Ubtox(state.DP);
	str+=" X=";
	str+=cpputil::Ustox(state.X);
	str+=" Y=";
	str+=cpputil::Ustox(state.Y);
	str+=" U=";
	str+=cpputil::Ustox(state.U);
	str+=" S=";
	str+=cpputil::Ustox(state.S);
	text.push_back((std::string &&)str);

	str="EF=";
	str.push_back((0!=(state.CC&EF)) ? '1' : '0');

	str+=" FIRQMask=";
	str.push_back((0!=(state.CC&FIRQMASK)) ? '1' : '0');

	str+=" HF=";
	str.push_back((0!=(state.CC&HF)) ? '1' : '0');

	str+=" IRQMask=";
	str.push_back((0!=(state.CC&IRQMASK)) ? '1' : '0');

	str+=" SF=";
	str.push_back((0!=(state.CC&SF)) ? '1' : '0');

	str+=" ZF=";
	str.push_back((0!=(state.CC&ZF)) ? '1' : '0');

	str+=" VF=";
	str.push_back((0!=(state.CC&VF)) ? '1' : '0');

	str+=" CF=";
	str.push_back((0!=(state.CC&CF)) ? '1' : '0');
	text.push_back((std::string &&)str);

	return text;
}
/* virtual */ uint32_t MC6809::SerializeVersion(void) const
{
	return 0;
}
/* virtual */ void MC6809::SpecificSerialize(std::vector <unsigned char> &data,std::string stateFName) const
{
	PushUint16(data,state.D);
	PushUint16(data,state.DP);
	PushUint16(data,state.CC);
	PushUint16(data,state.X);
	PushUint16(data,state.Y);
	PushUint16(data,state.U);
	PushUint16(data,state.S);
	PushUint16(data,state.PC);

	PushBool(data,state.halt);
	PushBool(data,state.nmiEnabled);

	PushUint32(data,state.freq);
}
/* virtual */ bool MC6809::SpecificDeserialize(const unsigned char *&data,std::string stateFName,uint32_t version)
{
	state.D=ReadUint16(data);
	state.DP=ReadUint16(data);
	state.CC=ReadUint16(data);
	state.X=ReadUint16(data);
	state.Y=ReadUint16(data);
	state.U=ReadUint16(data);
	state.S=ReadUint16(data);
	state.PC=ReadUint16(data);

	state.halt=ReadBool(data);
	state.nmiEnabled=ReadBool(data);

	state.freq=ReadUint32(data);

	return true;
}
