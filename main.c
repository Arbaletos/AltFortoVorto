#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <malloc.h>

#define WIDTH 640
#define HEIGHT 480

	SDL_Window* gWindow = NULL;
	SDL_Renderer* gRenderer = NULL;




int main(int argc, char* args[])
{	if (SDL_Init(SDL_INIT_VIDEO)<0)
	{
		printf("Kein init lolol! %s\n", SDL_GetError());
	}
	
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) &imgFlags))
	{
		printf("Image init error! %s\n", IMG_GetError());
	}

	if (TTF_Init() == -1)
	{
		printf("Kein Fonts ahahaha %s\n", TTF_GetError());
	}

	gWindow = SDL_CreateWindow( "Forto Vorto", 0,0,WIDTH,HEIGHT,SDL_WINDOW_SHOWN);	

	gRenderer = SDL_CreateRenderer(gWindow, -1,SDL_RENDERER_ACCELERATED);

	SDL_SetRenderDrawColor(gRenderer, 0xBB, 0xBB,0xBB,0xBB);
	
	int quit = 0;
	SDL_Color tColor= {142,0,0};
	SDL_Event e;
	while(!quit)
	{
		while(SDL_PollEvent(&e)!=0)
		{
			if (e.type == SDL_QUIT) quit = 1;
		}
			
		SDL_RenderClear(gRenderer);
		SDL_RenderPresent(gRenderer);
	}
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);

	gWindow = NULL;
	gRenderer = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
