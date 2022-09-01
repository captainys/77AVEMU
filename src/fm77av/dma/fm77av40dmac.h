#ifndef FM77AV40DMA_IS_INCLUDED
#define FM77AV40DMA_IS_INCLUDED
/* { */



#include "device.h"

class FM77AV40DMAC : public Device
{
public:
	virtual const char *DeviceName(void) const{return "DMAC";}
	FM77AV40DMAC(VMBase *vmBase);

	enum
	{
		NUM_REGS=32,
	};

	class State
	{
	public:
		uint8_t addrLatch=0;
		uint8_t regs[NUM_REGS];
	};
	State state;

	void Reset(void);

	/*! If true, DMA enabled.
	*/
	bool TxRQ(void) const;

	uint16_t Address(void) const;
	uint16_t NumBytes(void) const;
	bool AddressGoUp(void) const;
	bool FDCtoMEM(void) const;
	bool MEMtoFDC(void) const;
	void SetDMAEnd(bool ended);
	void SetBusy(bool busy);

	bool IRQEnabled(void) const;
	void SetIRQ(bool IRQ);

	void WriteFD98(uint8_t data);
	void WriteFD99(uint8_t data);
	void ReadFD99(void);
	uint8_t NonDestructiveReadFD99(void) const;

	virtual uint32_t SerializeVersion(void) const;
	virtual void SpecificSerialize(std::vector <unsigned char> &data,std::string stateFName) const;
	virtual bool SpecificDeserialize(const unsigned char *&data,std::string stateFName,uint32_t version);
};

/* } */
#endif
