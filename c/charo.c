#include <malloc.h>
#include <stdlib.h>
#include "struktoj.h"


struct Charo* Charo_create(struct Charo* kiu, char* name,Chars* kio, int speed){
	static int curid = 1;
	if(kiu==NULL) kiu = malloc(sizeof(struct Charo));
	kiu->health = kio->h;
	kiu->maxhealth = kio->h;
	kiu->atk = kio->a;
	kiu->damage = kio->d;
	kiu->speed = kio->i;
	kiu->eva = kio->e;
	kiu->name = name;
	kiu->id = curid++;
	kiu->tired = 0;
	return kiu;
}

char Charo_getSymbol(struct Charo* kiu)
{
	if (kiu== NULL) return ' ';
	if (kiu->health<=0) return ' ';
	return kiu->name[0];
	
}

void Charo_free(struct Charo* kiu)
{
	if (kiu==NULL) return;
	free(kiu);
	kiu = NULL;
}

int Charo_isExist(struct Charo* kiu)
{
	if(kiu==NULL) return 0;
	return 1;
}


char* Charo_list(struct Charo* kiu, char* buf)
{
	if (kiu==NULL) return NULL;
	sprintf(buf,"%s:\nHP:%d/%d\nPower:%d\nAttack Rate:%d\nDefense Rate:%d\nInitiative:%d\n",kiu->name,kiu->health,kiu->maxhealth,kiu->damage,kiu->atk,kiu->eva,kiu->speed);
	return buf;
}

char* Charo_getName(struct Charo* kiu)
{
	if (!kiu) return NULL;
	return kiu->name;
}

int Charo_getInitiative(struct Charo* kiu)
{
	if (!kiu) return 0;
	return kiu->speed;
}

int Charo_isTired(struct Charo* kiu)
{	
	if (!kiu) return 0;
	return kiu->tired;
}

int Charo_isAlive(struct Charo* kiu)
{
	if (!kiu) return 0;
	return kiu->health > 0;
}

void Charo_kuraci(struct Charo* kiun, int kiom)
{
	if (!kiun) return;
	kiun->health+=kiom;
	if (kiun->health>kiun->maxhealth) kiun->health = kiun->maxhealth;
}

void Charo_setTired(struct Charo* kiu, int kio)
{	
	if (!kiu) return;
	kiu->tired = kio;
}

struct battleRes* Charo_attack(struct battleRes* ret, int kiuatk,int kiudmg, int kiuneva)
{
	int strikes = kiuatk / 100*2;
	if(!strikes) strikes = 1;
	int rate = kiuatk / strikes;
	int eva = kiuneva / 100*2;
	if(!eva) eva = 1;
	int evarate = kiuneva/eva;
	int i;
	int alldamage=0;
	ret->bstrikes = 0;
	ret->strikes = strikes;
	for (i=0; i<strikes; i++)
	{
		if (rate>= rand()%100)
			{
				int curdamage = kiudmg;
				int j;
				int mul = 0;
				for (j=0;j<eva;j++)
				{
					if(evarate>=rand()%100) mul++;
				}
				if (mul) curdamage/=2*mul;
				alldamage+=curdamage;
				ret->bstrikes++;
			}
	}
	ret->dmg=alldamage;
	return ret;
}
