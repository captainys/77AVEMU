#include "srec.h"

int main(int argc,char *argv[])
{
	SREC srec;
	if(2<=argc)
	{
		if(true!=srec.Open(argv[1]))
		{
			printf("Could not open %s\n",argv[1]);
			return 1;
		}
	}
	else
	{
		printf("Usage: srecdump srecfile.srec\n");
		return 1;
	}

	if(true==srec.VerifyCheckSum())
	{
		printf("Checksum OK.\n");
	}
	else
	{
		return 1;
	}

	auto data=srec.GetData();
	printf("%d bytes of data\n",data.size());

	printf("Start from %x\n",srec.GetFirstAddress());

	return 0;
}
