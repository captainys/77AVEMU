#include <iostream>
#include "fm77av.h"
#include "cpputil.h"

bool Verify(FM77AV::MemoryEvaluation &eval,unsigned int shouldBe,std::string str)
{
	eval.Decode(str);

	std::cout << "Testing  " << str << std::endl;

	if(true==eval.error)
	{
		std::cout << "Error before evaluation." << std::endl;
		std::cout << eval.errorMessage << std::endl;
		return 1;
	}

	unsigned int value=eval.Evaluate();
	if(true==eval.error)
	{
		std::cout << "Error after evaluation." << std::endl;
		std::cout << eval.errorMessage << std::endl;
		return 1;
	}

	std::cout << "Evaluated:" << cpputil::Itox(value) << std::endl;
	std::cout << "Should be:" << cpputil::Itox(shouldBe) << std::endl;
	if(shouldBe!=value)
	{
		std::cout << "Error!" << std::endl;
		eval.Print();
		return false;
	}

	return true;
}

int main(void)
{
	FM77AV *fm77av=new FM77AV;

	fm77av->physMem.state.data[0x31000]=0x4E;
	fm77av->physMem.state.data[0x31001]=0x5A;

	fm77av->physMem.state.data[0x3600A]=0x60;
	fm77av->physMem.state.data[0x3600B]=0x0A;

	for(int i=0x37000; i<0x37FFF; i+=2)
	{
		fm77av->physMem.state.data[i  ]=(i>>8)&255;
		fm77av->physMem.state.data[i+1]=i&255;
	}

	fm77av->physMem.state.data[0x100]=0x75;
	fm77av->physMem.state.data[0x101]=0x00;

	FM77AV::MemoryEvaluation eval(fm77av);
	if(true!=Verify(eval,30,"(100+200)/10"))
	{
		return 1;
	}

	if(true!=Verify(eval,0x600A,"WORD_PHYS:0x3600A"))
	{
		return 1;
	}

	if(true!=Verify(eval,0x0A,"(WORD_M:0x1000-0x4E00)%0x50"))
	{
		return 1;
	}

	if(true!=Verify(eval,0x7700,"(WORD_M:($6700+$1000))"))
	{
		return 1;
	}

	if(true!=Verify(eval,0x7500,"WORD_M:(WORD_P:$100)"))
	{
		return 1;
	}

	std::cout << "Pass!" << std::endl;

	delete fm77av;
	return 0;
}
