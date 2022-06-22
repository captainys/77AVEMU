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

	instOperaType[INST_SUBD_IMM]=OPER_IMM;
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

	instClock[INST_ASLA]=2;
	instClock[INST_ASLB]=2;

	instClock[INST_ASL_DP]=6;
	instClock[INST_ASL_IDX]=6;
	instClock[INST_ASL_EXT]=7;

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

	instLabel[INST_ASLA]=         "ASLA";
	instLabel[INST_ASLB]=         "ASLB";

	instLabel[INST_ASL_DP]=       "ASL";
	instLabel[INST_ASL_IDX]=      "ASL";
	instLabel[INST_ASL_EXT]=      "ASL";

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

	instLabel[INST_BRN_IMM]=      "BRN";
	instLabel[INST_LBRN_IMM16]=   "LBRN";

	instLabel[INST_BSR_IMM]=      "BSR";
	instLabel[INST_LBSR_IMM16]=   "LBSR";

	instLabel[INST_BVC_IMM]=      "BVC";
	instLabel[INST_LBVC_IMM16]=   "LBVC";

	instLabel[INST_BVS_IMM]=      "BVS";
	instLabel[INST_LBVS_IMM16]=   "LBVS";

	regToReg[ 0]=REG_D;
	regToReg[ 1]=REG_X;
	regToReg[ 2]=REG_Y;
	regToReg[ 3]=REG_U;
	regToReg[ 4]=REG_S;
	regToReg[ 5]=REG_PC;
	regToReg[ 6]=REG_INVALID; // W reg if 6309
	regToReg[ 7]=REG_INVALID; // V reg if 6309
	regToReg[ 8]=REG_A;
	regToReg[ 9]=REG_B;
	regToReg[10]=REG_CC;
	regToReg[11]=REG_DP;
	regToReg[12]=REG_INVALID; // Always value 0 if 6309
	regToReg[13]=REG_INVALID; // Always value 0 if 6309
	regToReg[14]=REG_INVALID; // E reg if 6309
	regToReg[15]=REG_INVALID; // F reg if 6309
}

MC6809::Instruction MC6809::FetchInstruction(class MemoryAccess *mem,uint16_t &PC) const
{
	Instruction inst;

	inst.opCode=mem->FetchByte(PC++);
	inst.length=1;
	if(0x10==inst.opCode)
	{
		inst.length=2;
		inst.opCode=0x100|mem->FetchByte(PC++);
	}
	else if(0x11==inst.opCode)
	{
		inst.length=2;
		inst.opCode=0x200|mem->FetchByte(PC++);
	}

	inst.clocks=instClock[inst.opCode];
	inst.operType=instOperaType[inst.opCode];

	switch(instOperaType[inst.opCode])
	{
	case OPER_IMM:
	case OPER_DP:
	case OPER_REG:
		// 1-byte operand
		++inst.length;
		inst.operand[0]=mem->FetchByte(PC++);
		break;
	case OPER_EXT:
	case OPER_IMM16:
		// 2-bytes operand
		inst.length+=2;
		inst.operand[0]=mem->FetchByte(PC++);
		inst.operand[1]=mem->FetchByte(PC++);
		break;
	case OPER_INHERENT:
		// No operand.
		break;
	case OPER_IDX:
		++inst.length;
		inst.operand[0]=mem->FetchByte(PC++);
		inst.indexReg=REG_X+((inst.operand[0]>>5)&3);
		if(0==inst.operand[0]&0x80) // 5-bit offset from REG
		{
			inst.indexType=INDEX_CONST_OFFSET_FROM_REG;
			inst.indexIndir=false;
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
				inst.indexType=INDEX_CONST_OFFSET_FROM_REG;
				inst.indexIndir=false;
				inst.offset=0;
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
				inst.operand[1]=mem->FetchByte(PC++);
				inst.offset=inst.operand[1];
				inst.offset-=(inst.offset&0x80);
				break;
			case 0b10011000: // 8-bit offset from REG Indirect
				inst.clocks+=4;
				++inst.length;
				inst.indexType=INDEX_CONST_OFFSET_FROM_REG;
				inst.indexIndir=true;
				inst.operand[1]=mem->FetchByte(PC++);
				inst.offset=inst.operand[1];
				inst.offset-=(inst.offset&0x80);
				break;
			case 0b10001001: // 16-bit offset from REG Direct
				inst.clocks+=4;
				inst.length+=2;
				inst.indexType=INDEX_CONST_OFFSET_FROM_REG;
				inst.indexIndir=false;
				inst.operand[1]=mem->FetchByte(PC++);
				inst.operand[2]=mem->FetchByte(PC++);
				inst.offset=mc6809util::FetchWord(inst.operand[1],inst.operand[2]);
				inst.offset-=(inst.offset&0x8000);
				break;
			case 0b10011001: // 16-bit offset from REG Indirect
				inst.clocks+=7;
				inst.length+=2;
				inst.indexType=INDEX_CONST_OFFSET_FROM_REG;
				inst.indexIndir=true;
				inst.operand[1]=mem->FetchByte(PC++);
				inst.operand[2]=mem->FetchByte(PC++);
				inst.offset=mc6809util::FetchWord(inst.operand[1],inst.operand[2]);
				inst.offset-=(inst.offset&0x8000);
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
				inst.indexIndir=false;
				inst.offset=REG_D;
				break;
			case 0b10000000: // Post-Inc by 1 Direct
				inst.clocks+=2;
				inst.indexType=INDEX_POST_INC_1;
				inst.indexIndir=false;
				break;
			case 0b10000001: // Post-Inc by 2 Direct
				inst.clocks+=3;
				inst.indexType=INDEX_POST_INC_2;
				inst.indexIndir=false;
				break;
			case 0b10010001: // Post-Inc by 2 Indirect
				inst.clocks+=6;
				inst.indexType=INDEX_POST_INC_2;
				inst.indexIndir=true;
				break;
			case 0b10000010: // Pre-Dec by 1 Direct
				inst.clocks+=2;
				inst.indexType=INDEX_PRE_DEC_1;
				inst.indexIndir=false;
				break;
			case 0b10000011: // Pre-Dec by 2 Direct
				inst.indexType=INDEX_PRE_DEC_2;
				inst.indexIndir=false;
				inst.clocks+=3;
				break;
			case 0b10010011: // Pre-Dec by 2 Indirect
				inst.clocks+=6;
				inst.indexType=INDEX_PRE_DEC_2;
				inst.indexIndir=true;
				break;
			case 0b10001100: // 8-bit offset from PC Direct
				inst.clocks+=1;
				++inst.length;
				inst.indexType=INDEX_CONST_OFFSET_FROM_REG;
				inst.indexIndir=false;
				inst.indexReg=REG_PC;
				inst.operand[1]=mem->FetchByte(PC++);
				inst.offset=inst.operand[1];
				inst.offset-=(inst.offset&0x80);
				break;
			case 0b10011100: // 8-bit offset from PC Indirect
				inst.clocks+=4;
				++inst.length;
				inst.indexType=INDEX_CONST_OFFSET_FROM_REG;
				inst.indexIndir=true;
				inst.indexReg=REG_PC;
				inst.operand[1]=mem->FetchByte(PC++);
				inst.offset=inst.operand[1];
				inst.offset-=(inst.offset&0x80);
				break;
			case 0b10001101: // 16-bit offset from PC Direct
				inst.clocks+=5;
				inst.length+=2;
				inst.indexType=INDEX_CONST_OFFSET_FROM_REG;
				inst.indexIndir=false;
				inst.indexReg=REG_PC;
				inst.operand[1]=mem->FetchByte(PC++);
				inst.operand[2]=mem->FetchByte(PC++);
				inst.offset=mc6809util::FetchWord(inst.operand[1],inst.operand[2]);
				inst.offset-=(inst.offset&0x8000);
				break;
			case 0b10011101: // 16-bit offset from PC Indirect
				inst.clocks+=8;
				inst.length+=2;
				inst.indexType=INDEX_CONST_OFFSET_FROM_REG;
				inst.indexIndir=true;
				inst.indexReg=REG_PC;
				inst.operand[1]=mem->FetchByte(PC++);
				inst.operand[2]=mem->FetchByte(PC++);
				inst.offset=mc6809util::FetchWord(inst.operand[1],inst.operand[2]);
				inst.offset-=(inst.offset&0x8000);
				break;
			case 0b10011111: // Extended Indirect
				inst.clocks+=5;
				inst.indexType=INDEX_EXTENDED;
				inst.indexIndir=true;
				inst.operand[1]=mem->FetchByte(PC++);
				inst.operand[2]=mem->FetchByte(PC++);
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

void MC6809::DecodeExgTfrReg(uint8_t reg[2],uint8_t postByte) const
{
	reg[0]=regToReg[postByte>>4];
	reg[1]=regToReg[postByte&0x0F];
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
		case INST_LBCC_IMM16: //0x124, // 10 24
		case INST_BCS_IMM: //   0x25,
		case INST_LBCS_IMM16: //0x125, // 10 25
		case INST_BEQ_IMM: //   0x27,
		case INST_LBEQ_IMM16: //0x127, // 10 27
		case INST_BGE_IMM: //   0x2C,
		case INST_LBGE_IMM16: //0x12C, // 10 2C
		case INST_BGT_IMM: //   0x2E,
		case INST_LBGT_IMM16: //0x12E, // 10 2E
		case INST_BHI_IMM: //   0x22,
		case INST_LBHI_IMM16: //0x122, // 10 22
		case INST_BLE_IMM: //   0x2F,
		case INST_LBLE_IMM16: //0x12F, // 10 2F
		case INST_BLS_IMM: //   0x23,
		case INST_LBLS_IMM16: //0x123, // 10 23
		case INST_BLT_IMM: //   0x2D,
		case INST_LBLT_IMM16: //0x12D, // 10 2D
		case INST_BMI_IMM: //   0x2B,
		case INST_LBMI_IMM16: //0x12B, // 10 2B
		case INST_BNE_IMM: //   0x26,
		case INST_LBNE_IMM16: //0x126, // 10 26
		case INST_BPL_IMM: //   0x2A,
		case INST_LBPL_IMM16: //0x12A, // 10 2A
		case INST_BRA_IMM: //   0x20,
		case INST_LBRA_IMM16: //0x120, // 10 20
		case INST_BRN_IMM: //   0x21,
		case INST_LBRN_IMM16: //0x121, // 10 21
		case INST_BSR_IMM: //   0x8D,
		case INST_LBSR_IMM16: //0x18D, // 10 2D
		case INST_BVC_IMM: //   0x28,
		case INST_LBVC_IMM16: //0x128, // 10 28
		case INST_BVS_IMM: //   0x29,
		case INST_LBVS_IMM16: //0x129, // 10 29
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
		case INDEX_CONST_OFFSET_FROM_REG:
		case INDEX_8BIT_OFFSET:
		case INDEX_16BIT_OFFSET:
			{
				if(REG_PC==inst.indexReg)
				{
					int addr=(PC+inst.length+inst.offset)&0xFFFF;
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
