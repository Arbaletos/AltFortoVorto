#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#define WIDTH 800
#define HEIGHT 600
#define SIZE 20

	SDL_Window* gWindow = NULL;
	SDL_Renderer* gRenderer = NULL;
	TTF_Font *gFont = NULL;

	int charX = WIDTH/SIZE;
	int charY = HEIGHT/SIZE;
	
	char buf[WIDTH*HEIGHT/SIZE/SIZE];

int init();
int load();
int drawChar(char text, SDL_Color tColor, int x, int y);
int drawText(char* text, SDL_Color tColor, int x, int y);
void writeH(char* text,char* buff, int x, int y); 
void printBuf(char* buff);
void clearBuf(char* buff);


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
	clearBuf(buf);
	while(!quit)
	{
		while(SDL_PollEvent(&e)!=0)
		{
			if (e.type == SDL_QUIT) quit = 1;
			if (e.type == SDL_KEYDOWN)
			{
				switch(e.key.keysym.sym)
				{
					case SDLK_ESCAPE: quit = 1;
							break;
				}
			}
		}
			
		SDL_RenderClear(gRenderer);
		clearBuf(buf);
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
	int i=0;
	for (i=0;i<strlen(text);i++)
	{
		buf[y*charX+x+i] = text[i];
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



