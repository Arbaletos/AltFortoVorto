#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "fortovorto.h"

#define WIDTH 800
#define HEIGHT 600
#define SIZE 16
#define MISTOS 4
#define MESSIZE 800*600/16/16
#define STRSIZE 128

	SDL_Window* gWindow = NULL;
	SDL_Renderer* gRenderer = NULL;
	TTF_Font *gFont = NULL;

	int charX = WIDTH/SIZE;
	int charY = HEIGHT/SIZE;
	int state = 0;
	int curmist;

	FILE* glog;
	
	struct Charo* Mistos[MISTOS];
	
	char buf[MESSIZE];
	char subbuf[MESSIZE];
        char curmes[MESSIZE];
	char *spiela = "Spiela";

int init();
int load();
int drawChar(char text, SDL_Color tColor, int x, int y);
int drawText(char* text, SDL_Color tColor, int x, int y);
void writeH(char* text,char* buff, int x, int y); 
void printBuf(char* buff);
void clearBuf(char* buff);


struct Charo* getNextCharo();
void battleRound(int curmist);
void refreshMistos();

void getLine(char* buff, int next);
void Aut(int symb);



int main(int argc, char* args[])
{
	if (!init()) close();
	if (!load()) close();
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00,0x00,0x00);
	glog = fopen("log.arc","r+");
	int quit = 0;
	int i = 0;
	int j = 0;
	SDL_Color tColor= {0,180,0};
	SDL_Event e;
	clearBuf(buf);
	Mistos[0] = Charo_create(spiela,30,75,60,10,10);
	Mistos[1] = Charo_create("Misto 1",30,40,40,10,5);
	Mistos[2] = Charo_create("Misto 2", 15,30,60,15,12);
	Mistos[3] = Charo_create("Misto 3",20,45, 30, 20,7);
	sprintf(curmes,"Ok, warior. What u wanna do?\n1)Attack Mistvieh\n2)Look at Mistvieh\n3)Look at yourself");
	getLine(buf,1);	
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

					case SDLK_1: Aut(1);
						break;
					case SDLK_2: Aut(2);
						break;
					case SDLK_3: Aut(3);
						break;
					case SDLK_4: Aut(4);
						break;
					case SDLK_5: Aut(5);
						break;
				}
			}
		}
		getLine(buf,0);
		SDL_RenderClear(gRenderer);
		printBuf(buf);
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

void printMistos()
{
	int i;
        for (i=0;i<MISTOS;i++)
        {
		sprintf(curmes,"%s%d) %s\n",curmes,i+1,Charo_getName(Mistos[i]));
	}
}

void Aut(int symb)
{
	char *mains = "Ok, warior. What u wanna do?\n1)Attack Mistvieh\n2)Look at Mistvieh\n3)Look at yourself\n";
	switch (state)
	{
		case 0:
			if (symb==-1) state = -1;
			if (symb==1) {
				state = 1;
				sprintf(curmes,"Choose Mistvieh to Attack!\n");
				printMistos();
			}
			if (symb==2) {
				state = 2;
				sprintf(curmes,"Choose Mistvieh to Watch!\n");
				printMistos();
			}
			if (symb==3){
				 state = 3;
				 sprintf(curmes,"%s\n",Charo_list(Mistos[0],subbuf));
			}
			break;
		case 1:
			if (symb==-1) {
				state = 0;
				sprintf(curmes,"%s",mains);
			}
			if (symb>=1 && symb <=MISTOS)
			{
				state = 4;
				sprintf(curmes,"\n");
				battleRound(symb-1);
 				refreshMistos();
			}
			break;
		case 2:
			if (symb==-1) {
				state = 0;
				sprintf(curmes,"%s",mains);
			}
			if (symb>=1 && symb <=MISTOS)
			{
				state = 4;
				sprintf(curmes,"%s\n",Charo_list(Mistos[symb-1],subbuf));
			}
			break;
		case 3:
		case 4:
			if (symb==-1) {
				state = 0;
				sprintf(curmes,"%s",mains);
			}
			break; 

	}
	getLine(buf,1);
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
        if (curChar)
	{
		if (Charo_getName(curChar)==spiela)
		{
			sprintf(curmes,"%s%s",curmes,Charo_attack(curChar,Mistos[curmist],subbuf));
		}
		else
		{
			sprintf(curmes,"%s%s",curmes,Charo_attack(curChar,Mistos[0],subbuf));

		}
		battleRound(curmist);
	}
}

void getLine(char* buff, int next)
{
	if (next) clearBuf(buff);
	writeH(curmes,buff,0,0);
}


int drawChar(char text, SDL_Color tColor, int x, int y)
{
	char next[] = {text, '\0'};
//	next[0] = text;
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

void printBuf(char *buff)
{
	int i;
	int j;
	SDL_Color tColor = {0,200,0};
	for (i=0;i<charX;i++)
	{
		for (j=0;j<charY;j++)
		{
			drawChar(buff[i+j*charX],tColor,i*SIZE,j*SIZE);
		}
	}
}

void clearBuf(char *buff)
{
	int i;
	int j;
	for (i=0;i<=charX;i++)
	{
		for (j=0;j<=charY;j++)
		{
			buff[i+j*charX] = ' ';
		}
	}
}

void writeH(char* text, char* buff, int x, int y)
{
	int static pos=0;
	int i=0;
	int nx = x;
	int ny = y;
	if (x>=0) pos = 0;
	for (i=0;i<strlen(text);i++)
	{
		if (text[i]=='\n') 
		{
			nx=0-i-1;
			ny++;
		}
		else 
		{
			buf[pos+ny*charX+nx+i] = text[i];
		}
	}
	pos = ny*charX+nx+strlen(text);
	
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
	return ret;
}



