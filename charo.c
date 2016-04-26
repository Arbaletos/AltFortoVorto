#include <malloc.h>
#include "struktoj.h"


struct Charo* Charo_create(struct Charo* kiu, char* name,int health, int def, int atk, int damage, int speed){
	static int curid = 1;
	if(kiu==NULL) kiu = malloc(sizeof(struct Charo));
	kiu->health = health;
	kiu->maxhealth = health;
	kiu->atk = atk;
	kiu->damage = damage;
	kiu->speed = speed;
	kiu->cr = 0;
	kiu->cd = 100;
	kiu->eva = def;
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

char* Charo_attack(struct Charo* kiu, struct Charo* kiun, char *log)
{
	int strikes = kiu->atk / 100*2;
	if(!strikes) strikes = 1;
	int rate = kiu->atk / strikes;
	int eva = kiun->eva / 100*2;
	if(!eva) eva = 1;
	int evarate = kiun->eva/eva;
	int i;
	int alldamage=0;
	for (i=0; i<strikes; i++)
	{
		if (rate>= rand()%100)
			{
				int curdamage = kiu->damage;
				int j;
				int mul = 0;
				if (kiu->cr >= rand()%100) curdamage+=kiu->cd*curdamage/100;
				for (j=0;j<eva;j++)
				{
					if(evarate>=rand()%100) mul++;
				}
				if (mul) curdamage/=mul*2;	
				alldamage+=curdamage;
			}
	}
	kiun->health-=alldamage;
	sprintf(log,"%s deals %d damage by %d Hits to %s!\n",kiu->name,alldamage, strikes,kiun->name);
	return log;
}
