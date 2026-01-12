#include <stdio.h>
#include <vector>
#include <string>
#include "fm77avparam.h"

int main(int ac,char *av[])
{
	FM77AVProfile profile;
	auto data=profile.Serialize();
	if(true!=profile.Deserialize(data))
	{
		fprintf(stderr,"Deserialization failed.\n");
		return 1;
	}
	printf("Pass.\n");
	return 0;
}
