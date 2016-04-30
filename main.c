
#define WIDTH 800
#define HEIGHT 600
#define SIZE 16
#define MISTOS 10
#define MESSIZE 800*600/16/16
#define STRSIZE 128
#define DIFF 100

#include "struktoj.h"
#include "fortovorto.h"

enum battlestates
{
	MAIN_MENU,
	ATTACK_TARGET_MENU,
	WATCH_TARGET_MENU,
	MESSAGE,
	NEW_GAME,
	NEW_GAME_MENU,
	LEVEL_UP_MENU
};

	SDL_Window* gWindow = NULL;
	SDL_Renderer* gRenderer = NULL;
	TTF_Font *gFont = NULL;

	int charX = 50;
	int charY = 37;
	int state = 0;
	int mistqueue = 0;
	int mistint = 0;

	FILE* glog;
	
	struct Charo* Mistos[MISTOS];
	
	struct charBuf *logBuf = NULL;
	struct charBuf *fieldBuf = NULL;
	struct charBuf *hintBuf = NULL;
	char subbuf[MESSIZE];
        char curmes[MESSIZE];
	char *spiela = "Spiela";
	int xp = 0;
	int sp = 0;
	struct nomList *bek = NULL;
	struct nomList *bfin = NULL;

int init();
int load();
int drawChar(char text, SDL_Color tColor, int x, int y);
int drawText(char* text, SDL_Color tColor, int x, int y);


struct Charo* getNextCharo();
void battleRound(int curmist);
void refreshMistos();
void newGame();
void spamMistvieh();
void nextTurn();

char* genBestNomo();
void genCharo(Chars* kio, int points);
void getLog(struct charBuf* buff);
void refBuffers();
void drawBattleField(struct charBuf* buff);
void Aut(int symb);

char Spiela_getSymbol();

int main(int argc, char* args[])
{
	if (!init()) close();
	if (!load()) close();
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00,0x00,0x00);
	int quit = 0;
	int i = 0;
	int j = 0;
	SDL_Color tColor= {0,180,0};
	SDL_Event e;
	logBuf = createBuf(logBuf,0,10,50,26);
	fieldBuf = createBuf(fieldBuf,5,0,15,10);
	hintBuf = createBuf(hintBuf,25,0,20,10);
	state = NEW_GAME;
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
					case SDLK_ESCAPE: Aut(-1);
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

void newGame()
{
	int i;
	Chars nul = {0,0,0,0,0};
	for (i=1;i<MISTOS;i++)
	{
		Mistos[i] = Charo_create(Mistos[i],"nulka",&nul);
	}
	nul.h = 20;
	nul.e = 20;
	nul.a = 20;
	nul.d = 20;
	nul.i = 10;
	Mistos[0] = Charo_create(Mistos[0],spiela,&nul);
	sp = 100;
	xp = 0;
	spamMistvieh();
	clearBuf(logBuf);
	getLog(logBuf);
	mistint = rand()%3;	
	refBuffers();
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
		genCharo(&mist,DIFF+xp);
		Mistos[col] = Charo_create(Mistos[col],nomo,&mist);
		sprintf(curmes,"%s\n%s joined the battle!\n",curmes,nomo);
	}
}

void genCharo(Chars* kio,int points)
{
	kio->i = rand()%20+1;
	int ad = rand()%80+10;
	int eh = rand()%91+5;
	int da = rand()%91+5;
	int a = points*ad/100;
	int d = points - a;
	kio->d = a*da/100;
	kio->a = a - kio->d;
	kio->e = d*eh/100;
	kio->h = d - kio->e;
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
	if (mistqueue) 
	{
		mistqueue--;
		spamMistvieh();
	}
	if (!mistint--) {
		spamMistvieh();
		mistint = rand()%3;
	}
	refreshMistos();
	if (!Charo_isAlive(Mistos[0]))
	{
		sprintf(curmes,"%s\nLooks like u have been defeated Ahahaha!\n",curmes);
		state = NEW_GAME;
	}
	refBuffers();
}

void refBuffers()
{	
	clearBuf(hintBuf);
	clearBuf(fieldBuf);
	drawBattleField(fieldBuf);
	writeH(Charo_list(Mistos[0],subbuf),hintBuf,0,1);
	sprintf(subbuf,"Skill Points: %d\nExperience: %d\n",sp,xp);
	writeH(subbuf,hintBuf,0,7);
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

void Aut(int symb)
{
	char *mains = "Ok, warior. What u wanna do?\n1)Attack Mistvieh\n2)Look at Mistvieh\n3)Wait a bit\n4)Spread skill points\n5)Surrender and Die";
	switch (state)
	{
		case MAIN_MENU:
			if (symb==-1) state = -1;
			if (symb==1) {
				state = ATTACK_TARGET_MENU;
				sprintf(curmes,"Choose Mistvieh to Attack!\n");
				printMistos();
			}
			if (symb==2) {
				state = WATCH_TARGET_MENU;
				sprintf(curmes,"Choose Mistvieh to Watch!\n");
				printMistos();
			}
			if (symb==3){
				 state = MESSAGE;
				 int heal = rand() % 10;
				 sprintf(curmes,"U waited mal kaj healed %d damage points\n",heal);
				 Charo_kuraci(Mistos[0],heal);
				 battleRound(-1);
				 nextTurn();
			}
			if (symb==4){
				state = LEVEL_UP_MENU;
				sprintf(curmes,"\nChoose Your characterisk to increase!\n1)Health \n2)Evasion \n3)Attack \n4)Damage\n5)Return Back\n");
			}
			if (symb==5){
				state = NEW_GAME;
				sprintf(curmes,"Ahaha, u defeated yourself by yourself!\n");
			}
			break;
		case ATTACK_TARGET_MENU:
			if (symb==-1) {
				state = MAIN_MENU;
				sprintf(curmes,"%s",mains);
			}
			if (symb>=0 && symb <MISTOS)
			{
				state = MESSAGE;
				sprintf(curmes,"\n");
				battleRound(symb);
 				nextTurn();
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
				sprintf(curmes,"%s\n",Charo_list(Mistos[symb],subbuf));
			}
			break;
		case MESSAGE:
			if (symb==-1) {
				state = MAIN_MENU;
				sprintf(curmes,"%s",mains);
			}
			break; 
		case LEVEL_UP_MENU:
			if (symb==5){
				state = MAIN_MENU;
				sprintf(curmes,"%s",mains);
			}
			if (symb>=1 && symb <=4)
			{
				state = LEVEL_UP_MENU;
				if (sp>=1)
				{
					if (symb==1){
						Mistos[0]->maxhealth++;
						Mistos[0]->health++;
					}
					if (symb==2) Mistos[0]->eva++;
					if (symb==3) Mistos[0]->atk++;
					if (symb==4) Mistos[0]->damage++;
					sp--;
					sprintf(curmes,"\n");
					refBuffers();
				}
				else sprintf(curmes,"You have not Skill Points!\n");
				sprintf(curmes,"%sChoose Your characterisk to increase!\n1)Health \n2)Evasion \n3)Attack \n4)Damage\n5)Return Back\n",curmes);
			}
			break;
		case NEW_GAME:
			state = MAIN_MENU;
			sprintf(curmes,"%s",mains);
			newGame();
			break;

	}
	getLog(logBuf);
}

struct Charo* getNextCharo()
{
	int i;
	int max = 0;
	int maxid = 0;
	for (i=0;i<MISTOS;i++)
	{
		if ((Charo_getInitiative(Mistos[i]) >= max) && (!Charo_isTired(Mistos[i])))
		{
			maxid = i;
			max = Charo_getInitiative(Mistos[maxid]);
		}
	}
	if (!max) return NULL;
	Charo_setTired(Mistos[maxid],1);
	return Mistos[maxid];
}

void refreshMistos()
{
	int i;
	for (i=0;i<MISTOS;i++)
	{
		Charo_setTired(Mistos[i],0);
	}
}

void battleRound(int curmist)
{
	struct Charo* curChar = getNextCharo();
        if (Charo_isAlive(curChar))
	{
		if (Charo_getName(curChar)==spiela)
		{
			if (curmist>=0){
				if (!Charo_isExist(Mistos[curmist])) sprintf(curmes,"%s U attacked Free space. Lol.\n",curmes);
				else {
					sprintf(curmes,"%s%s",curmes,Charo_attack(curChar,Mistos[curmist],subbuf));
					if (Charo_isAlive(Mistos[curmist])) sprintf(curmes,"%s!\n",curmes);
					else 
					{
						sprintf(curmes,"%s and killed him!\n",curmes);
						sp++;
						xp++;
					}
				}
			}
		}
		else
		{
			sprintf(curmes,"%s%s",curmes,Charo_attack(curChar,Mistos[0],subbuf));
			if (Charo_isAlive(Mistos[0])) sprintf(curmes,"%s!\n",curmes);
					else 
					{
						sprintf(curmes,"%s and killed him!\n",curmes);
					}

		}
		battleRound(curmist);
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

struct charBuf* createBuf(struct charBuf* kio, int x, int y, int wid, int hei)
{
	if (kio==NULL) kio = malloc(sizeof(struct charBuf));
	kio->left = x;
	kio->top = y;
	kio->wid = wid;
	kio->hei = hei;
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
		if (text[i]=='\n') 
		{
			nx=0-i-1;
			ny++;
		}
		else 
		{
			buff->buf[ny*buff->wid+nx+i] = text[i];
		}
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



