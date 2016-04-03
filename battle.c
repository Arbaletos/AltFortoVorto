#include "fortovorto.h"

#define BSIZE 10


struct Charo* chars[BSIZE];
//   147
// 0 258
//   369  SPLENDID! 9 eneimes - 9 targets! Yahoo! 

struct Charo* getNextCharo()
{
	int i;
	int curma[BSIZE];
	int curco = 0;
	int max =0;
	for (i=0;i<BSIZE;i++)
	{
		if (Charo_GetInitiative(chars[i] == max))
		{
			curma[curco++]=i;
		}
		if (Charo_GetInitiative(chars[i] >max))
		{
			max = chars[i];
			curco = 1;
			curma[0] = i;
		}
	}
	int curt = (random*(curco+1)-0.5);
	chars[curt]->tired = 1;
	return chars[curt];
}

void battleRound()
{
	struct Charo* curChar = getNextCharo();
}
