//Special Program for testing damn battle.
#define LANG 1 		//0 - EO, 1 - RUS, 2 - ENG
#define LANGS 3		//Number of language avaiable.
#define TEMPLATES 5
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "tekstaro.h"
#include "../battle.h"

char* t_names[TEMPLATES];
struct Chars t_pars[TEMPLATES];

char* sp_name = "Spiela";
Charo* spieler = NULL;

char bufalo[1024];

cell scell = {1,0,NULL};
cell mcell = {0,0,NULL};

struct Chars homo = {100,100,100,100,10};
struct Chars slime = {250,50,50,50,10};
struct Chars raben = {50,50,250,50,10};
struct Chars boar = {50,50,50,250,10};
struct Chars fly = {50,250,50,50,10};


battleQueue bq(NULL,NULL);

void dialog()
{
  int ans;
  int i;
  printf("1)Add enemy\n");
  printf("2)Attack enemy\n");
  printf("3)List enemies\n");
  printf("4)Reset battle\n");
  scanf("%d",&ans);
  if (ans == 1)
  {
    int misto = 0;
    printf("New enemy must be:\n"); 
    printf("1)Homo\n");
    printf("2)Slime\n");
    printf("3)Raben\n");
    printf("4)Boar\n");
    printf("5)Fly\n");
    printf("6)Random\n");
    scanf("%d",&misto);
    if (misto==6) misto = rand()%5 +1;
    if (misto>=1 && misto<=5);
    int created = 0;
    for (i=0;i<9;i++)
    {
      if (mcell.mistos[i]->isExist()==0 && created==0)
      {
        created = 1;
        printf("New %s joins the battle!\n",t_names[misto-1]);
        mcell.mistos[i]->recreate(t_pars[misto-1],t_names[misto-1]);
      }
    }
    if (!created) printf ("No place for new bicholga!\n");
  }
  if (ans == 2)
  {
    int misto = 0;
    printf("Select enemy to fight!\n");
    for (i=0;i<9;i++)
    {
      if (mcell.mistos[i]->isExist())
      {
        printf("%d) %s\n",i+1,mcell.mistos[i]->getName());
        bq.addAgo(new Ago(mcell.mistos[i], NULL, new Celaro(spieler), 0, ATAKI_IUN),mcell.mistos[i]->getInitiative());
      }
    }
    scanf("%d",&misto);
    if (mcell.mistos[misto-1]->isExist()) bq.addAgo(new Ago(spieler, NULL, new Celaro(mcell.mistos[misto-1]), 0, ATAKI_IUN),spieler->getInitiative());
    else printf("Where is no enemy where!)\n");
    bq.run();
  }
  if (ans==3)
  {
    printf("0) %s\n",spieler->list(bufalo));
    for (i=0;i<9;i++)
    {
      if (mcell.mistos[i]->isExist()) printf("%d) %s\n\n",i+1,mcell.mistos[i]->list(bufalo));
    }
  }  
}

int main(int argc, char** argv)
{
  int i;
  Charo* mistos[9];
  Charo* mistos2[9];
  t_names[0] = "Homus";
  t_names[1] = "Slime";
  t_names[2] = "Raben";
  t_names[3] = "Fly";
  t_names[4] = "Boar";
  
  t_pars[0] = homo;
  t_pars[1] = slime;
  t_pars[2] = raben;
  t_pars[3] = fly;
  t_pars[4] = boar;

  for (i=0;i<9;i++)
  {
    mistos[i] = new Charo();
    mistos2[i] = new Charo();
  }
  spieler = new Charo(homo,sp_name);
  mistos2[4] = spieler;
  for (i=0;i<9;i++)
  {
    mcell.mistos[i] = mistos[i]; 
    scell.mistos[i] = mistos2[i];
  }
  
//  trigger undead = {ON_TIME,19,new Ago(spieler, NULL,NULL,10000,HEAL_DAMAGE)};
//  spieler->addTrigger(&undead);
  u_val uval = {10};
  spieler->addEfekt(BLEEDING,10,&uval); 
  bq = battleQueue(&mcell,&scell); 
  printf("Welcome! To the Arena!\n");
  srand(time(0));
  while (spieler->isAlive()) dialog();
  printf("Entschuldigung, but you're dead.\n");
  return 0;
}
