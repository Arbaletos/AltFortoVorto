#include <malloc.h>
struct Charo
{
	int maxhealth;
	int health;
	int atk;
	int eva;
	int damage;
	int speed;
	int id;
	char* name;
	int cr;
	int cd;
};

struct Charo* Charo_create(char* name,int health, int atk, int damage, int speed){
	static int curid = 1;
	struct Charo *ret = malloc(sizeof(struct Charo));
	ret->health = health;
	ret->maxhealth = health;
	ret->atk = atk;
	ret->damage = damage;
	ret->speed = speed;
	ret->cr = 0;
	ret->cd = 100;
	ret->eva = 0;
	ret->name = name;
	ret->id = curid++;
	return ret;
}


char* Charo_list(struct Charo* kio, char* buf)
{
	sprintf(buf,"%s:\nHP:%d/%d\nPower:%d\nAttack Rate:%d\nDefense Rate:%d\nInitiative:%d\n",kio->name,kio->health,kio->maxhealth,kio->damage,kio->atk,kio->eva,kio->speed);
	return buf;
}

char* Charo_getName(struct Charo* kio)
{
	return kio->name;
}

char* Charo_attack(struct Charo* kio, struct Charo* kion, char *log)
{
	int strikes = kio->atk / 100*2;
	if(!strikes) strikes = 1;
	int rate = kio->atk / strikes;
	int eva = kion->eva / 100*2;
	if(!eva) eva = 1;
	int evarate = kion->eva/eva;
	int i;
	int alldamage=0;
	for (i=0; i<strikes; i++)
	{
		if (rate>= rand()%100)
			{
				int curdamage = kio->damage;
				int j;
				int mul = 0;
				if (kio->cr >= rand()%100) curdamage+=kio->cd*curdamage/100;
				for (j=0;j<eva;j++)
				{
					if(evarate>=rand()%100) mul++;
				}
				curdamage/=mul*2;	
				alldamage+=curdamage;
			}
	}
	kion->health-=alldamage;
	sprintf(log,"%s deals %d damage by %d Hits to %s!\n",kio->name,alldamage, strikes,kion->name);
	return log;
}
