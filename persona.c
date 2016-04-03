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

	int tired; //Have this Charo acted in this turn?
};

struct Charo* Charo_create(char* name,int health, int def, int atk, int damage, int speed){
	static int curid = 1;
	struct Charo *ret = malloc(sizeof(struct Charo));
	ret->health = health;
	ret->maxhealth = health;
	ret->atk = atk;
	ret->damage = damage;
	ret->speed = speed;
	ret->cr = 0;
	ret->cd = 100;
	ret->eva = def;
	ret->name = name;
	ret->id = curid++;
	ret->tired = 0;
	return ret;
}


char* Charo_list(struct Charo* kio, char* buf)
{
	sprintf(buf,"%s:\nHP:%d/%d\nPower:%d\nAttack Rate:%d\nDefense Rate:%d\nInitiative:%d\n",kio->name,kio->health,kio->maxhealth,kio->damage,kio->atk,kio->eva,kio->speed);
	return buf;
}

char* Charo_getName(struct Charo* kiu)
{
	return kiu->name;
}

int Charo_getInitiative(struct Charo* kiu)
{
	return kiu->speed;
}

int Charo_isTired(struct Charo* kiu)
{
	return kiu->tired;
}

void Charo_setTired(struct Charo* kiu, int kio)
{
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
