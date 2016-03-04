#include <SDL2/SDL.h>
#include <stdio.h>

#define WIDTH 640
#define HEIGHT 480

int main(int argc, char* args[])
{
	SDL_Window* gWindow = NULL;
	SDL_Renderer* gRenderer = NULL;

	if (SDL_Init(SDL_INIT_VIDEO)<0)
	{
		printf("Kein init lolol! %s\n", SDL_GetError());
	}


	gWindow = SDL_CreateWindow( "Forto Vorto", 0,0,WIDTH,HEIGHT,SDL_WINDOW_SHOWN);	
	gRenderer = SDL_CreateRenderer(gWindow, -1,SDL_RENDERER_ACCELERATED);

	SDL_SetRenderDrawColor(gRenderer, 0xBB, 0xBB,0xBB,0xBB);
	
	int quit = 0;
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
	SDL_Quit();
}


