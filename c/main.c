#define WIDTH 800
#define HEIGHT 600
#define SIZE 16
#define MISTOS 10
#define MESSIZE 800*600/16/16
#define STRSIZE 128
#define LONGSTR 256
#define STRLEN 20
#define DIFF 50
#define KRESKO 4
#define FREEPTS 50

#define INITXP 200

#include "struktoj.h"
#include "fortovorto.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

enum battlestates
{
	MAIN_MENU,
	ATTACK_TARGET_MENU,
	WATCH_TARGET_MENU,
	MESSAGE,
	NEW_GAME,
	NEW_GAME_NOMO,
	NEW_GAME_MENU,
	NEW_GAME_CLASS,
	MAIN_MAP,
	LEVEL_UP_MENU,
	BATTLE_MENU,
	CHECK_HERO,
	VICTORY
};
	double sqrts[] = {0,1,1.41421356,1.73205080,2,2.23606798,2.44948974,2.6457513110,2.8284271247,3};

	SDL_Window* gWindow = NULL;
	SDL_Renderer* gRenderer = NULL;
	TTF_Font *gFont = NULL;

	int charX = 50;
	int charY = 37;
	int state = 0;
	int mistqueue = 0;
	int mistint = 0;
	int score = 0;

	FILE* glog;
	
	struct Charo* Mistos[MISTOS];
	
	struct charBuf *logBuf = NULL;
	struct charBuf *fieldBuf = NULL;
	struct charBuf *hintBuf = NULL;
	char subbuf[MESSIZE];
        char curmes[MESSIZE];
	char name[STRLEN];
	char input[STRSIZE];
	char *spiela = "Spiela";
	char sp_class[STRSIZE];
	int xp = 0;
	int sp = 0;

	int level = 1;
	struct nomList *bek = NULL;
	struct nomList *bfin = NULL;

int init();
int load();
int drawChar(char text, SDL_Color tColor, int x, int y);
int drawText(char* text, SDL_Color tColor, int x, int y);


struct Charo* getNextCharo();
void battleRound(int targ);
int startBattle();
void leaveBattle();
int refreshMistos(); //returns count of alive mistos
void newGame();
void spamMistvieh();
void nextTurn();


int parsePos(char* pos);


char* genBestNomo();
void genCharo(Chars* kio, int points);
void getLog(struct charBuf* buff);
void refBuffers();
void drawBattleField(struct charBuf* buff);
void Aut(int symb);
void levelup();
char Spiela_getSymbol();

#include "reg.c"

int main(int argc, char* args[])
{
	if (!init()) exit(0);
	if (!load()) exit(0);
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00,0x00,0x00);
	int quit = 0;
	int i = 0;
	int j = 0;
	SDL_Color tColor= {0,180,0};
	SDL_Event e;
	logBuf = createBuf(logBuf,0,10,50,26,1);
	fieldBuf = createBuf(fieldBuf,5,0,15,10,0);
	hintBuf = createBuf(hintBuf,25,0,20,10,0);
	state = NEW_GAME;
	srand(time(0));
	Aut(0);
	while(state>=0)
	{
		while(SDL_PollEvent(&e)!=0)
		{
			if (e.type == SDL_QUIT) quit = 1;
			if (e.type == SDL_KEYDOWN)
			{
				switch(e.key.keysym.sym)
				{
					case SDLK_RETURN:
							Aut(-1);
							break;
					case SDLK_ESCAPE: Aut(-100);
							break;
					case SDLK_1: 
					case SDLK_KP_1:
						Aut(1);break;
					case SDLK_2:
					case SDLK_KP_2:
						 Aut(2);break;
					case SDLK_3:
					case SDLK_KP_3:
						 Aut(3);break;
					case SDLK_4:
					case SDLK_KP_4:
						 Aut(4);break;
					case SDLK_5:
					case SDLK_KP_5:
						 Aut(5);break;
					case SDLK_6:
					case SDLK_KP_6:
						 Aut(6);break;
					case SDLK_7:
					case SDLK_KP_7:
						 Aut(7);break;
					case SDLK_8:
					case SDLK_KP_8:
						 Aut(8);break;
					case SDLK_9:
					case SDLK_KP_9:
						 Aut(9);break;
					case SDLK_0:
					case SDLK_KP_0:
						 Aut(0);break;

				}
			}
			if (e.type == SDL_TEXTINPUT)
			{
				strcat(input, e.text.text);
				Aut(0);
			}
		}
		SDL_RenderClear(gRenderer);
		printBuf(logBuf);
		printBuf(hintBuf);
		printBuf(fieldBuf);
		SDL_RenderPresent(gRenderer);
	}
	TTF_CloseFont(gFont);
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);

	gFont = NULL;
	gWindow = NULL;
	gRenderer = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int startBattle()
{
	int i;
	int ret;
	Chars nul = {0,0,0,0,0};
	for (i=1;i<MISTOS;i++)
	{
		Mistos[i] = Charo_create(Mistos[i],"nulka",&nul);
	}
	ret = rand()%9+1;
	for (i=0;i<ret;i++)
	{
		spamMistvieh();
	}
	return ret;
}

void newGame()
{
	int i;
	Chars nul = {0,0,0,0,0};
	nul.i = 10;
	Mistos[0] = Charo_create(Mistos[0],name,&nul);
	level = 0;
	sp = INITXP+FREEPTS;
	xp = FREEPTS;
	clearBuf(logBuf);
	getLog(logBuf);
}

void spamMistvieh()
{
	int freeslots[MISTOS-1];
	int free = 0;
	int i;
	for (i=1;i<MISTOS;i++)
	{
		if (!Charo_isAlive(Mistos[i]))
		{
			freeslots[free++]=i;	
		}
	}
	if (!free) mistqueue++;
	else {
		char *nomo = genBestNomo();
		int col = freeslots[rand()%free];
		Chars mist = {0,0,0,0,0};
		genCharo(&mist,DIFF+level);
		Mistos[col] = Charo_create(Mistos[col],nomo,&mist);
		sprintf(curmes,"%s\n%s joined the battle!\n",curmes,nomo);
	}
}

void genCharo(Chars* kio,int points)
{
	kio->i = rand()%20+1;

	int ad = rand()%40+30;//Attack - Defense  30 .. 70
	int eh = rand()%40+30;//Evasion - Health  60 .. 90
	int da = rand()%40+30;//Damage - Attack   60 .. 90
	int a = points*ad/100;
	int d = points - a;
	
	kio->h = d*eh/100;
	kio->e = d - kio->h;
	kio->a = a*da/100;
	kio->d = a - kio->a;
}


char* genBestNomo()
{
	char *ek = nomList_getRandom(bek);
	char *fin = nomList_getRandom(bfin);
	char *ret = malloc((strlen(ek)+strlen(fin)+1)*sizeof(char));
	sprintf(ret,"%s%s",ek,fin);
	ret[0] = ret[0]+'A'-'a';
	return ret;
}

void nextTurn()
{
//	if (!mistint--) {
//		spamMistvieh();
//		mistint = rand()%3;
//	}
	
	input[0] = '\0';
	int ccount = refreshMistos();
	if (!ccount)
        {
		sprintf(curmes,"%s\n Victory!!!\n",curmes);
		state = VICTORY;
		leaveBattle();
	}
	sprintf(curmes,"%s\n%d enemies left!\n",curmes,ccount);
	if (!Charo_isAlive(Mistos[0]))
	{
		sprintf(curmes,"%s\nLooks like u have been defeated Ahahaha!\nYour final score in this dungeon is %d. \nRest in Peace, %s the %s!",curmes,score,Mistos[0]->name, sp_class);
		state = NEW_GAME;
	}
	refBuffers();
}

void refBuffers()
{	
	clearBuf(hintBuf);
	clearBuf(fieldBuf);
	if (fieldBuf->visible) drawBattleField(fieldBuf);
	if (hintBuf->visible)
	{
		writeH(Charo_list(Mistos[0],subbuf),hintBuf,0,1);
		sprintf(subbuf,"Experience: %d\n",xp);
		writeH(subbuf,hintBuf,0,7);
	}
}

void printMistos()
{
	int i;
        for (i=0;i<MISTOS;i++)
        {
		if (Charo_isAlive(Mistos[i]))
		{
			sprintf(curmes,"%s%d) %s\n",curmes,i,Charo_getName(Mistos[i]));
		}
	}
}

char Spiela_getSymbol()
{
	if (Charo_isAlive(Mistos[0])) return '$';
	else return 'x'; 
}

void drawBattleField(struct charBuf *buff)
{
	int i;
	writeH("###############",buff,0,0);
	writeH("###############",buff,0,8);
	for (i = 1; i<8; i++)
	{
		putChar('#',buff,0,i);
		putChar('#',buff,14,i);
	}
	putChar(Charo_getSymbol(Mistos[1]),buff,2,6);
	putChar(Charo_getSymbol(Mistos[2]),buff,4,6);
	putChar(Charo_getSymbol(Mistos[3]),buff,6,6);
	putChar(Charo_getSymbol(Mistos[4]),buff,2,4);
	putChar(Charo_getSymbol(Mistos[5]),buff,4,4);
	putChar(Charo_getSymbol(Mistos[6]),buff,6,4);
	putChar(Charo_getSymbol(Mistos[7]),buff,2,2);
	putChar(Charo_getSymbol(Mistos[8]),buff,4,2);
	putChar(Charo_getSymbol(Mistos[9]),buff,6,2);
	putChar(Spiela_getSymbol(),buff,12,4);
	
}

void Aut(int symb)  //Automato
{
	char mains[LONGSTR];
 	static char mapmes[LONGSTR];
	static char akto[STRLEN];
	static char *nexts;
	static int next;
	sprintf(mains,"Ok,%s. What u wanna do?\n1)%s\n2)Look at Mistvieh\n3)Wait a bit\n4)Run away\n5)Die\n",name,akto);

	if (symb==-100)
	{
		state = -1;
	}
	switch (state)
	{
		case MAIN_MENU:
			if (symb==1) {
				state = ATTACK_TARGET_MENU;
			        input[0] = '\0';
				sprintf(curmes,"Enter Your %s:\n%s",akto,input);
				SDL_StartTextInput();				
				putChar('A',fieldBuf,2,0);
				putChar('O',fieldBuf,4,0);
				putChar('U',fieldBuf,6,0);
				putChar('R',fieldBuf,0,6);
				putChar('S',fieldBuf,0,4);
				putChar('T',fieldBuf,0,2);
	
//				printMistos();
			}
			if (symb==2) {
				state = WATCH_TARGET_MENU;
				sprintf(curmes,"Choose Mistvieh to Watch!\n");
				printMistos();
			}
			if (symb==3){
				 state = MESSAGE;
	//			 int heal = rand() % 10;
				 sprintf(curmes,"U waited mal!\n");
//				 Charo_kuraci(Mistos[0],heal);
				 battleRound(0);
				 nextTurn();
				 next = MAIN_MENU;
				 nexts = mains;
			}
			if (symb==4){
				sprintf(curmes,"U run away like a dog!\n");
				state = MESSAGE;
				next = MAIN_MAP;
				nexts = mapmes;
				battleRound(0);
				levelup();
				nextTurn();
				leaveBattle();
			}
//			if (symb==4){
//				state = LEVEL_UP_MENU;
//				sprintf(curmes,"\nChoose Your characterisk to increase!\n1)Health \n2)Evasion \n3)Attack \n4)Damage\n5)Return Back\n");
//			}
			if (symb==5){
				state = NEW_GAME;
				refBuffers();
				sprintf(curmes,"Ahaha, u defeated yourself by yourself!\nYour final score in this dungeon is %d. \nRest in Peace, %s the %s!",score,Mistos[0]->name, sp_class);
			}
			break;
		case ATTACK_TARGET_MENU:
			if (symb==-1) {
				SDL_StopTextInput();
				state = MESSAGE;
				next = MAIN_MENU;
				nexts = mains;
				sprintf(curmes,"%s\n",curmes);
				battleRound(parsePos(input));
				nextTurn();
				
			}
			else
			{
				sprintf(curmes,"Enter Your %s:\n%s",akto,input);
				refBuffers();
				putChar('A',fieldBuf,2,0);
				putChar('O',fieldBuf,4,0);
				putChar('U',fieldBuf,6,0);
				putChar('R',fieldBuf,0,6);
				putChar('S',fieldBuf,0,4);
				putChar('T',fieldBuf,0,2);
			}
			break;
		case WATCH_TARGET_MENU:
			if (symb==-1) {
				state = MAIN_MENU;
				sprintf(curmes,"%s",mains);
			}
			if (symb>=0 && symb <MISTOS)
			{
				state = MESSAGE;
				next = MAIN_MENU;
				nexts = mains;
				sprintf(curmes,"%s\n",Charo_list(Mistos[symb],subbuf));
			}
			break;
		case MESSAGE:
			if (symb==-1) {
				state = next;
				sprintf(curmes,"%s",nexts);
			}
			break; 
		case LEVEL_UP_MENU:
			if (symb==5){
				hintBuf -> visible = 0;
				refBuffers();
				state = MAIN_MAP;
				sprintf(curmes,mapmes,sp_class);
			}
			if (symb>=1 && symb <=4)
			{
				state = LEVEL_UP_MENU;
				if (xp>=1)
				{
					if (symb==1){
						Mistos[0]->maxhealth++;
						Mistos[0]->health++;
					}
					if (symb==2) Mistos[0]->eva++;
					if (symb==3) Mistos[0]->atk++;
					if (symb==4) Mistos[0]->damage++;
					xp--;
					sprintf(curmes,"\n");
					refBuffers();
				}
				else sprintf(curmes,"You have not Skill Points!\n");
				sprintf(curmes,"%sChoose Your characterisk to increase!\n1)Health \n2)Evasion \n3)Attack \n4)Damage\n5)Return Back\n",curmes);
			}
			break;
		case NEW_GAME:
			leaveBattle();
			state = NEW_GAME_NOMO;
			sprintf(curmes,"Enter deine name, Glorious Hero!\nNomo:%s",input);
			SDL_StartTextInput();
			break;
		case NEW_GAME_NOMO:
			if(symb==-1)
			{
				state = NEW_GAME_CLASS;
				SDL_StopTextInput();
				strcpy(name,input);
				newGame();
				sprintf(curmes,"Greetings, %s.\n You know, you are some king of WordSpeaker now - one of men, who can use words to transform the into Furious Charge. U wanna learn new words and, whatswhy, now you standing near Great Tomb of Leidenbringung, that was closed for ages.\nBefore you start your journey u must select your class, or your way of using words in battle.\n1)Barbaro - Great Warrior, who shouts and attacks his foes in Righteous Fury\n2)Sciencist - cunning intelegist, who uses his mind to reate deadly missiles with help og formulas\n3)Elementist - mage, that can see what is world made of, and who can use elements of this on world or his advantage in battle\n4)Random!",name);
			}
			else 
			{
				SDL_StopTextInput();
				sprintf(curmes,"Enter deine name, Glorious Hero!\nNomo:%s",input);
				SDL_StartTextInput();
				refBuffers();
			}
			break;
		case NEW_GAME_CLASS:
			input[0] = '\0';
			if (symb>=1 && symb<=4)
			{	
				Chars mist = {0,0,0,0,0};
				genCharo(&mist,INITXP);
				Mistos[0]->health=mist.h;
				Mistos[0]->maxhealth = mist.h;
				Mistos[0]->eva = mist.e;
				Mistos[0]->atk = mist.a;
				Mistos[0]->damage = mist.d;
				if (symb==4) symb = rand()%3+1;
				if (symb==1) 
				{
					sprintf(sp_class,"Barbaro");
					sprintf(akto,"Battlecry");
				}	
				if (symb==2) 
				{
					sprintf(sp_class,"Sciencist");
					sprintf(akto,"Formula");
				}
				if (symb==3) 
				{
					sprintf(sp_class,"Elementist");
					sprintf(akto,"Spell");
				}
				state = MAIN_MAP;
				sprintf(mapmes,"Ok,%s the %s! You now in %s. What you going to do?\n\n1)Geradeaus\n2)Camp here for some time\n3)Check Hero\n",name,sp_class,"Halls of Leiden");
				sprintf(curmes,"%s",mapmes);
			}
//			if (symb==1)
//			{
//				int health = INITXP *3 / 10;
//				Mistos[0]->health = health;
//				Mistos[0]->maxhealth = health;
//				Mistos[0]->eva = INITXP / 10;
//				Mistos[0]->atk = INITXP * 5 / 10;
//				Mistos[0]->damage = INITXP / 10;
//				strcpy(sp_class,"Barbaro");
//			}
//			if (symb==2)
//			{
//				int health = INITXP *5 / 10;
//				Mistos[0]->health = health;
//				Mistos[0]->maxhealth = health;
//				Mistos[0]->eva = INITXP / 10;
//				Mistos[0]->atk = INITXP * 3/ 10;
//				Mistos[0]->damage = INITXP /10;
//				strcpy(sp_class,"Sciencist");
//			}
//			if (symb==3)
//			{	
//				int health = INITXP *4 / 10;
//				Mistos[0]->health = health;
//				Mistos[0]->maxhealth = health;
//				Mistos[0]->eva = INITXP / 10;
//				Mistos[0]->atk = INITXP * 4 / 10;
//				Mistos[0]->damage = INITXP / 10;
//				strcpy(sp_class,"Elementist");
//			}
//			if (symb==4)
//			{
//				Chars mist = {0,0,0,0,0};
//				genCharo(&mist,INITXP);
//				Mistos[0]->health=mist.h;
//				Mistos[0]->maxhealth = mist.h;
//				Mistos[0]->eva = mist.e;
//				Mistos[0]->atk = mist.a;
//				Mistos[0]->damage = mist.d;
//				strcpy(sp_class,"Fortunator");
//			}
//			sprintf(Mistos[0]->name,"%s the %s",Mistos[0]->name,sp_class);
			break;
		case VICTORY:
			state = MAIN_MAP;	
			refBuffers();
			sprintf(curmes,"%s",mapmes);
			score++;
		case MAIN_MAP:
			if (symb==1)
			{
				state = BATTLE_MENU;
				int numa = startBattle();
				levelup();
				sprintf(curmes,"U see aproaching group of droglemistojkoj in count of %d! What do u wanna present them?\n1)Face them in fair battle!\n2)Flee like a mitvieh!\n",numa);
			}
			if (symb==2)
			{
				 next = MAIN_MAP;
				 nexts = mapmes;
			         state = MESSAGE;
				 int heal = (rand() % Mistos[0]->maxhealth)/2;
				 sprintf(curmes,"U waited mal and Healed on %dterrible damage,amiko!\n",heal);
				 Charo_kuraci(Mistos[0],heal);
				 levelup();
			}
			if (symb==3)
			{
				state = CHECK_HERO;
				hintBuf->visible = 1;
				refBuffers();
				sprintf(curmes,"1)Spend XP\n2)Back\n");
			}
			break;
		case CHECK_HERO:
			if (symb==1){
				state = LEVEL_UP_MENU;
				sprintf(curmes,"\nChoose Your characterisk to increase!\n1)Health \n2)Evasion \n3)Attack \n4)Damage\n5)Return Back\n");
			}

			if (symb==2)
			{
				hintBuf -> visible = 0;
				refBuffers();
				state = MAIN_MAP;
				sprintf(curmes,mapmes,sp_class);
			}
			break;
		case BATTLE_MENU:
			if (symb==1)
			{	
   				fieldBuf->visible = 1;
				hintBuf->visible = 1;
				refBuffers();
				state = MAIN_MENU;
				sprintf(curmes,mains,sp_class);
			}
			if (symb==2)
			{
				state = MAIN_MAP;
				sprintf(curmes,mapmes,sp_class);
			}
			break;

	}
	getLog(logBuf);
}

void levelup()
{
	level+=KRESKO;
}

void leaveBattle()
{
	
	fieldBuf->visible = 0;
	hintBuf->visible = 0;
	refBuffers();
}

struct Charo* getNextCharo()
{
	int i;
	int max = 0;
	int maxid = 0;
	for (i=0;i<MISTOS;i++)
	{
		if ((Charo_isAlive(Mistos[i])) && (Charo_getInitiative(Mistos[i]) >= max) && (!Charo_isTired(Mistos[i])))
		{
			maxid = i;
			max = Charo_getInitiative(Mistos[maxid]);
		}
	}
	if (!max) return NULL;
	Charo_setTired(Mistos[maxid],1);
	return Mistos[maxid];
}

int refreshMistos()
{
	int i;
	int count = 0;
	for (i=0;i<MISTOS;i++)
	{
		if (Charo_isAlive(Mistos[i]) && i>0) count++;
		Charo_setTired(Mistos[i],0);
	}
	return count;
}

void battleRound(int targ)
{
	struct Charo* curChar = getNextCharo();
	static struct battleRes *loga = NULL;
	if(!loga) loga = malloc(sizeof(struct battleRes));
        if (Charo_isAlive(curChar))
	{
		if (curChar==Mistos[0])
		{
			sprintf(curmes,"%sTargets:%d\n",curmes,targ);
			int count = 0;
			int i = 0;
			int cur = 1;
			for (i=1;i<=9;i++)
			{
				if (targ & cur) {
					count++;
//					sprintf(curmes,"%s Mistvieh:%d",curmes,i);
				}
				cur= cur<<1;
			}
			cur = 1;
			
			if (count){
			sprintf(curmes,"%s%s attacks %d enemies:",curmes,Mistos[0]->name,count);
				int ncount = 0;
				for (i=1;i<=9;i++)
				{
					if (targ & cur)
					{
//						sprintf(curmes,"%s Mistvieh:%d",curmes,i);
						if (Charo_isAlive(Mistos[i]))
						{
							loga = Charo_attack(loga,Mistos[0]->atk/sqrts[count],Mistos[0]->damage/sqrts[count],Mistos[i]->eva);
							sprintf(curmes,"%sdeals %d damage by %d hits to %s",curmes, loga->dmg, loga->strikes, Mistos[i]->name);
							Mistos[i]->health-=loga->dmg;
							if (!Charo_isAlive(Mistos[i]))
							{
								xp++;
								sprintf(curmes,"%s and kills it",curmes);	
							}
							ncount++;
							if(count==ncount) sprintf(curmes,"%s!\n",curmes);
							else sprintf(curmes,"%s, ",curmes);
						}
					}
					cur = cur << 1;
				}
			}
			else sprintf(curmes,"%s%s couldn't attack anybody!\n",curmes,Mistos[0]->name);
		}
		else
		{
			loga = Charo_attack(loga,curChar->atk,curChar->damage,Mistos[0]->eva);
			sprintf(curmes,"%s%s deals %d damage by %d hits to %s",curmes,curChar->name,loga->dmg,loga->strikes,Mistos[0]->name);
			Mistos[0]->health-=loga->dmg;
			if (Charo_isAlive(Mistos[0])) sprintf(curmes,"%s!\n",curmes);
					else 
					{
						sprintf(curmes,"%s and killed him!\n",curmes);
					}

		}
		battleRound(targ);
	}
}

void getLog(struct charBuf* buff)
{
	clearBuf(buff);
	writeH(curmes,buff,0,0);
}


int drawChar(char text, SDL_Color tColor, int x, int y)
{
	char next[] = {text, '\0'};
	return drawText(next,tColor,x,y);
}

int drawText(char* text, SDL_Color tColor, int x,int y)
{
	int ret = 1;
	SDL_Surface* tSurface = TTF_RenderText_Solid(gFont, text, tColor);
	SDL_Texture* tTexture = NULL;
	if (!tSurface)
	{
		printf("kein loadung des text \"%s\", eraro %s!\n",text,SDL_GetError());
		ret = 0;
	}
	
	tTexture = SDL_CreateTextureFromSurface(gRenderer,tSurface);
	if (!tTexture)
	{
		printf("Kein loading texture! %s\n",SDL_GetError());
		ret = 0;
	}

	SDL_Rect tRect = {x,y,tSurface->w,tSurface->h};
	SDL_RenderCopy(gRenderer,tTexture, NULL,&tRect);

	SDL_FreeSurface(tSurface);
	SDL_DestroyTexture(tTexture);
}

void printBuf(struct charBuf *buff)
{
	int i;
	int j;
	SDL_Color tColor = {0,200,0};
	for (i=0;i<buff->wid;i++)
	{
		for (j=0;j<buff->hei;j++)
		{
			drawChar(buff->buf[i+j*buff->wid],tColor,(i+buff->left)*SIZE,(j+buff->top)*SIZE);
		}
	}
}

struct charBuf* createBuf(struct charBuf* kio, int x, int y, int wid, int hei, int vis)
{
	if (kio==NULL) kio = malloc(sizeof(struct charBuf));
	kio->left = x;
	kio->top = y;
	kio->wid = wid;
	kio->hei = hei;
	kio->visible = vis;
	clearBuf(kio);
	
	return kio;
} 

void clearBuf(struct charBuf *buff)
{
	int i;
	for (i=0;i<buff->wid*buff->hei;i++)
	{
		buff->buf[i] = ' ';
	}
}

void putChar(char ch,struct charBuf* buff, int x, int y)
{
	buff->buf[y*buff->wid+x] = ch;
}

void writeH(char* text, struct charBuf *buff, int x, int y)
{
	int i=0;
	int nx = x;
	int ny = y;
	for (i=0;i<strlen(text);i++)
	{
		if (nx>=buff->wid)
		{
			nx -=buff->wid;
			ny++;
		}
		if (text[i]=='\n') 
		{
			nx = -1;
			ny++;
		}
		else 
		{
			buff->buf[ny*buff->wid+nx] = text[i];
		}
		nx++;
	}
	
}

int init()
{
	int ret = 1;
	if (SDL_Init(SDL_INIT_VIDEO)<0)
	{
		printf("Kein init lolol! %s\n", SDL_GetError());
		ret = 0;
	}
	
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) &imgFlags))
	{
		printf("Image init error! %s\n", IMG_GetError());
		ret = 0;
	}

	if (TTF_Init() == -1)
	{
		printf("Kein Fonts ahahaha %s\n", TTF_GetError());
		ret = 0;
	}


	gWindow = SDL_CreateWindow( "Forto Vorto", 0,0,WIDTH,HEIGHT,SDL_WINDOW_SHOWN);	
	if (!gWindow) ret = 0;

	gRenderer = SDL_CreateRenderer(gWindow, -1,SDL_RENDERER_ACCELERATED);
	if (!gRenderer) ret = 0;

	int i =0;
	for (i=0;i<MISTOS;i++)
	{
		Mistos[i] = NULL;
	}
	return ret;
}

int load()
{
	int ret = 1;
	gFont = TTF_OpenFont("res/COURIER.TTF",SIZE+5);
	if (!gFont)
	{
		printf("Some Shit with your Courier Font! %s\n",TTF_GetError());
		ret = 0;
	}
	bek =  nomList_create(bek,"arc/best_ek");
	bfin = nomList_create(bfin,"arc/best_fin");
	return ret;
}



