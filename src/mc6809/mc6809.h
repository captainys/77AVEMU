#ifndef MC6809_IS_INCLUDED
#define MC6809_IS_INCLUDED
/* { */

#include <stdint.h>
#include "vmbase.h"
#include "memory.h"
#include "mc6809util.h"

class MC6809 : public Device
{
public:
	enum
	{
		OPER_IMM,
		OPER_DP,
		OPER_IDX,
		OPER_EXT,
		OPER_INHERENT,
		OPER_REG,
		OPER_IMM16
	};

	enum
	{
		INDEX_UNSUPPORTED,
		INDEX_CONST_OFFSET_FROM_REG,
		INDEX_ACCUM_OFFSET_FROM_REG,
		INDEX_POST_INC_1,
		INDEX_POST_INC_2,
		INDEX_PRE_DEC_1,
		INDEX_PRE_DEC_2,
		INDEX_8BIT_OFFSET,
		INDEX_16BIT_OFFSET,
		INDEX_EXTENDED,
	};

	enum
	{
		INST_ABX=       0x3A,

		INST_ADCA_IMM=  0x89,
		INST_ADCA_DP=   0x99,
		INST_ADCA_IDX=  0xA9,
		INST_ADCA_EXT=  0xB9,

		INST_ADCB_IMM=  0xC9,
		INST_ADCB_DP=   0xD9,
		INST_ADCB_IDX=  0xE9,
		INST_ADCB_EXT=  0xF9,

		// INST_ADCD_IMM=  0x189, // 10 89 (63C09)
		// INST_ADCD_DP=   0x199, // 10 99 (63C09)
		// INST_ADCD_IDX=  0x1A9, // 10 A9 (63C09)
		// INST_ADCD_EXT=  0x1B9, // 10 B9 (63C09)

		// INST_ADCR_REG_IMM=0x131, // 10 31 (63C09)

		INST_ADDA_IMM=  0x8B,
		INST_ADDA_DP=   0x9B,
		INST_ADDA_IDX=  0xAB,
		INST_ADDA_EXT=  0xBB,

		INST_ADDB_IMM=  0xCB,
		INST_ADDB_DP=   0xDB,
		INST_ADDB_IDX=  0xEB,
		INST_ADDB_EXT=  0xFB,

		INST_ADDD_IMM=  0xC3,
		INST_ADDD_DP=   0xD3,
		INST_ADDD_IDX=  0xE3,
		INST_ADDD_EXT=  0xF3,

		INST_ANDA_IMM=  0x84,
		INST_ANDA_DP=   0x94,
		INST_ANDA_IDX=  0xA4,
		INST_ANDA_EXT=  0xB4,

		INST_ANDB_IMM=  0xC4,
		INST_ANDB_DP=   0xD4,
		INST_ANDB_IDX=  0xE4,
		INST_ANDB_EXT=  0xF4,

		INST_ANDCC_IMM= 0x1C,

		// ASL is an alias for LSL
		// INST_ASLA=      0x48,
		// INST_ASLB=      0x58,

		// INST_ASL_DP=    0x08,
		// INST_ASL_IDX=   0x68,
		// INST_ASL_EXT=   0x78,

		INST_ASRA=      0x47,
		INST_ASRB=      0x57,

		INST_ASR_DP=    0x07,
		INST_ASR_IDX=   0x67,
		INST_ASR_EXT=   0x77,

		INST_BITA_IMM=  0x85,
		INST_BITA_DP=   0x95,
		INST_BITA_IDX=  0xA5,
		INST_BITA_EXT=  0xB5,

		INST_BITB_IMM=  0xC5,
		INST_BITB_DP=   0xD5,
		INST_BITB_IDX=  0xE5,
		INST_BITB_EXT=  0xF5,

		INST_CLRA=      0x4F,
		INST_CLRB=      0x5F,

		INST_CLR_DP=    0x0F,
		INST_CLR_IDX=   0x6F,
		INST_CLR_EXT=   0x7F,

		INST_CMPA_IMM=  0x81,
		INST_CMPA_DP=   0x91,
		INST_CMPA_IDX=  0xA1,
		INST_CMPA_EXT=  0xB1,

		INST_CMPB_IMM=  0xC1,
		INST_CMPB_DP=   0xD1,
		INST_CMPB_IDX=  0xE1,
		INST_CMPB_EXT=  0xF1,

		INST_CMPD_IMM=  0x183, // 10 83
		INST_CMPD_DP=   0x193, // 10 93
		INST_CMPD_IDX=  0x1A3, // 10 A3
		INST_CMPD_EXT=  0x1B3, // 10 B3

		INST_CMPS_IMM=  0x28C, // 11 8C
		INST_CMPS_DP=   0x29C, // 11 9C
		INST_CMPS_IDX=  0x2AC, // 11 AC
		INST_CMPS_EXT=  0x2BC, // 11 BC

		INST_CMPU_IMM=  0x283, // 11 83
		INST_CMPU_DP=   0x293, // 11 93
		INST_CMPU_IDX=  0x2A3, // 11 A3
		INST_CMPU_EXT=  0x2B3, // 11 B3

		INST_CMPX_IMM=  0x8C,
		INST_CMPX_DP=   0x9C,
		INST_CMPX_IDX=  0xAC,
		INST_CMPX_EXT=  0xBC,

		INST_CMPY_IMM=  0x18C, // 10 8C
		INST_CMPY_DP=   0x19C, // 10 9C
		INST_CMPY_IDX=  0x1AC, // 10 AC
		INST_CMPY_EXT=  0x1BC, // 10 BC

		INST_COMA=      0x43,
		INST_COMB=      0x53,

		INST_COM_DP=    0x03,
		INST_COM_IDX=   0x63,
		INST_COM_EXT=   0x73,

		INST_CWAI_IMM=  0x3C,

		INST_DAA=       0x19,

		INST_DECA=      0x4A,
		INST_DECB=      0x5A,

		INST_DEC_DP=    0x0A,
		INST_DEC_IDX=   0x6A,
		INST_DEC_EXT=   0x7A,

		INST_EORA_IMM=  0x88,
		INST_EORA_DP=   0x98,
		INST_EORA_IDX=  0xA8,
		INST_EORA_EXT=  0xB8,

		INST_EORB_IMM=  0xC8,
		INST_EORB_DP=   0xD8,
		INST_EORB_IDX=  0xE8,
		INST_EORB_EXT=  0xF8,

		INST_EXG_REG=   0x1E,

		INST_INCA=      0x4C,
		INST_INCB=      0x5C,

		INST_INC_DP=    0x0C,
		INST_INC_IDX=   0x6C,
		INST_INC_EXT=   0x7C,

		INST_JMP_DP=    0x0E,
		INST_JMP_IDX=   0x6E,
		INST_JMP_EXT=   0x7E,

		INST_JSR_DP=    0x9D,
		INST_JSR_IDX=   0xAD,
		INST_JSR_EXT=   0xBD,

		INST_LDA_IMM=   0x86,
		INST_LDA_DP=    0x96,
		INST_LDA_IDX=   0xA6,
		INST_LDA_EXT=   0xB6,

		INST_LDB_IMM=   0xC6,
		INST_LDB_DP=    0xD6,
		INST_LDB_IDX=   0xE6,
		INST_LDB_EXT=   0xF6,

		INST_LDD_IMM=   0xCC,
		INST_LDD_DP=    0xDC,
		INST_LDD_IDX=   0xEC,
		INST_LDD_EXT=   0xFC,

		INST_LDS_IMM=   0x1CE, // 10 CE
		INST_LDS_DP=    0x1DE, // 10 DE
		INST_LDS_IDX=   0x1EE, // 10 EE
		INST_LDS_EXT=   0x1FE, // 10 FE

		INST_LDU_IMM=   0xCE,
		INST_LDU_DP=    0xDE,
		INST_LDU_IDX=   0xEE,
		INST_LDU_EXT=   0xFE,

		INST_LDX_IMM=   0x8E,
		INST_LDX_DP=    0x9E,
		INST_LDX_IDX=   0xAE,
		INST_LDX_EXT=   0xBE,

		INST_LDY_IMM=   0x18E, // 10 8E
		INST_LDY_DP=    0x19E, // 10 9E
		INST_LDY_IDX=   0x1AE, // 10 AE
		INST_LDY_EXT=   0x1BE, // 10 BE

		INST_LEAS_IDX=  0x32,
		INST_LEAU_IDX=  0x33,
		INST_LEAX_IDX=  0x30,
		INST_LEAY_IDX=  0x31,

		INST_LSLA=      0x48,
		INST_LSLB=      0x58,

		INST_LSL_DP=    0x08,
		INST_LSL_IDX=   0x68,
		INST_LSL_EXT=   0x78,

		INST_LSRA=      0x44,
		INST_LSRB=      0x54,

		INST_LSR_DP=    0x04,
		INST_LSR_IDX=   0x64,
		INST_LSR_EXT=   0x74,

		INST_MUL=       0x3D,

		INST_NEGA=      0x40,
		INST_NEGB=      0x50,

		INST_NEG_DP=    0x00,
		INST_NEG_IDX=   0x60,
		INST_NEG_EXT=   0x70,

		INST_NOP=       0x12,

		INST_ORA_IMM=   0x8A,
		INST_ORA_DP=    0x9A,
		INST_ORA_IDX=   0xAA,
		INST_ORA_EXT=   0xBA,

		INST_ORB_IMM=   0xCA,
		INST_ORB_DP=    0xDA,
		INST_ORB_IDX=   0xEA,
		INST_ORB_EXT=   0xFA,

		INST_ORCC_IMM=  0x1A,

		INST_PSHS_REG=	0x34,
		INST_PSHU_REG=	0x36,
		INST_PULS_REG=	0x35,
		INST_PULU_REG=	0x37,

		INST_ROLA=      0x49,
		INST_ROLB=      0x59,

		INST_ROL_DP=    0x09,
		INST_ROL_IDX=   0x69,
		INST_ROL_EXT=   0x79,

		INST_RORA=      0x46,
		INST_RORB=      0x56,

		INST_ROR_DP=    0x06,
		INST_ROR_IDX=   0x66,
		INST_ROR_EXT=   0x76,

		INST_RTI=       0x3B,
		INST_RTS=       0x39,

		INST_SBCA_IMM=  0x82,
		INST_SBCA_DP=   0x92,
		INST_SBCA_IDX=  0xA2,
		INST_SBCA_EXT=  0xB2,

		INST_SBCB_IMM=  0xC2,
		INST_SBCB_DP=   0xD2,
		INST_SBCB_IDX=  0xE2,
		INST_SBCB_EXT=  0xF2,

		INST_SEX=       0x1D,

		INST_STA_DP=    0x97,
		INST_STA_IDX=   0xA7,
		INST_STA_EXT=   0xB7,

		INST_STB_DP=    0xD7,
		INST_STB_IDX=   0xE7,
		INST_STB_EXT=   0xF7,

		INST_STD_DP=    0xDD,
		INST_STD_IDX=   0xED,
		INST_STD_EXT=   0xFD,

		INST_STS_DP=    0x1DF, // 10 DF
		INST_STS_IDX=   0x1EF, // 10 EF
		INST_STS_EXT=   0x1FF, // 10 FF

		INST_STU_DP=    0xDF,
		INST_STU_IDX=   0xEF,
		INST_STU_EXT=   0xFF,

		INST_STX_DP=    0x9F,
		INST_STX_IDX=   0xAF,
		INST_STX_EXT=   0xBF,

		INST_STY_DP=    0x19F,
		INST_STY_IDX=   0x1AF,
		INST_STY_EXT=   0x1BF,

		INST_SUBA_IMM=  0x80,
		INST_SUBA_DP=   0x90,
		INST_SUBA_IDX=  0xA0,
		INST_SUBA_EXT=  0xB0,

		INST_SUBB_IMM=  0xC0,
		INST_SUBB_DP=   0xD0,
		INST_SUBB_IDX=  0xE0,
		INST_SUBB_EXT=  0xF0,

		INST_SUBD_IMM=  0x83,
		INST_SUBD_DP=   0x93,
		INST_SUBD_IDX=  0xA3,
		INST_SUBD_EXT=  0xB3,

		INST_SWI=       0x3F,
		INST_SWI2=      0x13F,  // 10 3F
		INST_SWI3=      0x23F,  // 11 3F

		INST_SYNC=      0x13,   // Is it HLT?

		INST_TFR_REG=   0x1F,

		INST_TSTA=      0x4D,
		INST_TSTB=      0x5D,

		INST_TST_DP=    0x0D,
		INST_TST_IDX=   0x6D,
		INST_TST_EXT=   0x7D,

		INST_BCC_IMM=   0x24,
		INST_LBCC_IMM16=0x124, // 10 24

		INST_BCS_IMM=   0x25,
		INST_LBCS_IMM16=0x125, // 10 25

		INST_BEQ_IMM=   0x27,
		INST_LBEQ_IMM16=0x127, // 10 27

		INST_BGE_IMM=   0x2C,
		INST_LBGE_IMM16=0x12C, // 10 2C

		INST_BGT_IMM=   0x2E,
		INST_LBGT_IMM16=0x12E, // 10 2E

		INST_BHI_IMM=   0x22,
		INST_LBHI_IMM16=0x122, // 10 22

		// BHS is alias for BCC
		// INST_BHS_IMM=   0x24,
		// INST_LBHS_IMM16=0x124, // 10 24

		INST_BLE_IMM=   0x2F,
		INST_LBLE_IMM16=0x12F, // 10 2F

		// BLO is alias for BCS
		// INST_BLO_IMM=   0x25,
		// INST_LBLO_IMM16=0x125, // 10 25

		INST_BLS_IMM=   0x23,
		INST_LBLS_IMM16=0x123, // 10 23

		INST_BLT_IMM=   0x2D,
		INST_LBLT_IMM16=0x12D, // 10 2D

		INST_BMI_IMM=   0x2B,
		INST_LBMI_IMM16=0x12B, // 10 2B

		INST_BNE_IMM=   0x26,
		INST_LBNE_IMM16=0x126, // 10 26

		INST_BPL_IMM=   0x2A,
		INST_LBPL_IMM16=0x12A, // 10 2A

		INST_BRA_IMM=   0x20,
		INST_LBRA_IMM16=0x120, // 10 20

		INST_BRN_IMM=   0x21,
		INST_LBRN_IMM16=0x121, // 10 21

		INST_BSR_IMM=   0x8D,
		INST_LBSR_IMM16=0x18D, // 10 2D

		INST_BVC_IMM=   0x28,
		INST_LBVC_IMM16=0x128, // 10 28

		INST_BVS_IMM=   0x29,
		INST_LBVS_IMM16=0x129, // 10 29
	};

	unsigned char instOperaType[0x300];
	unsigned int instClock[0x300];
	std::string instLabel[0x300];
	unsigned int regToReg[16];


	class Instruction
	{
	public:
		unsigned int length;
		unsigned int clocks;
		unsigned int opCode;
		unsigned int operType;
		unsigned int indexType;
		bool indexIndir;
		unsigned int indexReg; // REG_X,REG_Y,REG_U,or REG_S.  Undefined if indexType is not applicable.
		int offset;   // Can be constant offset or REG_XX depending on indexType.
		uint8_t operand[4];

		inline int BranchOffset8(void) const
		{
			int offset=operand[0];
			offset-=(offset&0x80);
			return offset;
		}
		inline int BranchOffset16(void) const
		{
			int offset=mc6809util::FetchWord(operand[0],operand[1]);
			offset-=(offset&0x8000);
			return offset;
		}
	};

	enum
	{
		REG_CC,
		REG_A,
		REG_B,
		REG_DP,

		REG_X, // Don't change the order of REG_X,Y,U,S.  FetchInstruction assumes this order.
		REG_Y, // Don't change the order of REG_X,Y,U,S.  FetchInstruction assumes this order.
		REG_U, // Don't change the order of REG_X,Y,U,S.  FetchInstruction assumes this order.
		REG_S, // Don't change the order of REG_X,Y,U,S.  FetchInstruction assumes this order.

		REG_PC,
		REG_D,

		REG_INVALID=0xFF
	};
	unsigned int regBits[10];

	enum
	{
		PSH_CC=0x01,
		PSH_A=0x02,
		PSH_B=0x04,
		PSH_DP=0x08,
		PSH_X=0x10,
		PSH_Y=0x20,
		PSH_UorS=0x40,
		PSH_PC=0x80,
	};

	class RegisterSet
	{
	public:
		uint16_t D;
		uint8_t DP,CC;
		uint16_t X,Y,U,S,PC;

		inline uint8_t A(void) const
		{
			return (D>>8);
		}
		inline uint8_t B(void) const
		{
			return D&0xFF;
		}
		inline void SetA(uint16_t value)
		{
			D&=0xFF;
			D|=(value<<8);
		}
		inline void SetB(uint16_t value)
		{
			D&=0xFF00;
			D|=(value&0xFF);
		}
	};

	enum
	{
		CF=0x01,
		VF=0x02,
		ZF=0x04,
		SF=0x08, // Is it officially called Negative Flag?  I always called it Sign Flag.
		IRQMASK=0x10,
		HF=0x10,
		FIRQMASK=0x40,
		EF=0x80,
	};

	class State : public RegisterSet
	{
	public:
		bool halt=false;
	};
	State state;


	class Debugger
	{
	public:
		bool enabled=false;
		bool stop=false;

		void ClearStopFlag(void);
	};
	Debugger debugger;


	virtual const char *DeviceName(void) const{return "MC6809";}
	MC6809(VMBase *vmBase);
	uint32_t RunOneInstruction(class MemoryAccess &mem); // Returns the number of clocks passed

	uint16_t &RegisterRef16(uint8_t reg);
	const uint16_t &RegisterRef16(uint8_t reg) const;
	uint16_t GetRegisterValue(uint8_t reg) const;
	int16_t GetRegisterValueSigned(uint8_t reg) const;
	void SetRegisterValue(uint8_t reg,uint16_t value);

	/*! Check value and set or reset SF and ZF.  VF will be zero.
	*/
	void Test8(uint8_t value);
	void Test16(uint16_t value);

	/*! This function will pre-decrement or post-increment index registers.
	*/
	uint16_t DecodeIndexedAddress(const Instruction &inst,MemoryAccess &mem);

	void WriteToIndex16(class MemoryAccess &mem,const Instruction &inst,uint16_t value);

	Instruction FetchInstruction(class MemoryAccess &mem,uint16_t PC) const;
	void DecodeExgTfrReg(uint8_t reg[2],uint8_t postByte) const;

	std::string WholeDisassembly(class MemoryAccess &mem,uint16_t PC) const;

	std::string FormatByteCode(Instruction inst) const;

	std::string Disassemble(Instruction inst,uint16_t PC) const;
	std::string DisassembleOperand(Instruction inst,uint16_t PC) const;
	std::string DisassemblePSHPULRegisters(uint8_t flags,char UorS) const;

	static std::string RegToStr(unsigned int reg);
	static std::string RegToStrPCR(unsigned int reg);

	std::vector <std::string> GetStatusText(void) const;
};

/* } */
#endif
