#ifndef FM77AVRKANA_IS_INCLUDED
#define FM77AVRKANA_IS_INCLUDED
/* { */

struct RKanaTable
{
	const char *keyComb;
	bool leaveFirstChar;
	unsigned char output[4];
};
extern struct RKanaTable rKanaTable[];

/* } */
#endif
