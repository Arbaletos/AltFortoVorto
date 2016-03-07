#pragma once
#include <SDL2/SDL.h>
#include <stdio.h>

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
