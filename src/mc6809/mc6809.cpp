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

	inst.clocks=instClock[inst.opCode];

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
