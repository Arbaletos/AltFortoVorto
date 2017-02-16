int LANG = 0;
#include "tekstaro.h"
#include "fv.h"

  enum gamestates
{
  MAIN_MENU,
  WORLD_MENU,
  DUNGEON_MENU,
  BATTLE_MENU
};

  double sqrts[] = {0,1,1.41421356,1.73205080,2,2.23606798,2.44948974,2.6457513110,2.8284271247,3};

  SDL_Window* gWindow = NULL;
  SDL_Renderer* gRenderer = NULL;
  TTF_Font *gFont = NULL;

  FILE* glog;

  charBuf *mainBuf;

  Nomaro* bek;
  Nomaro* bfin;

  int init();
  int load();
  int drawChar(char text, SDL_Color tColor, int x, int y);
  int drawText(char* text, SDL_Color tColor, int x, int y);


int main(int argc, char* args[])
{
//}---------INITIALIZATION/LOADING-------->
  if (!init()) exit(0);
  if (!load()) exit(0);
  SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00,0x00,0x00);
  int quit = 0;
  int i = 0;
  int j = 0;
  SDL_Color tColor= {0,180,0};
  SDL_Event e;
//  mainBuf = new charBuf(0,0,WIDTH/SIZE,HEIGHT/SIZE);
  mainBuf = new charBuf(0,0,WIDTH/SIZE,HEIGHT/SIZE);
  mainBuf->append("Tell me... Do you bleed? You will!");
  mainBuf->append("Patro nia kiu estas en la cxielo sankta estu via nomo estu regxeco via kiel en la cxielo tiel ankaux sur la tero panon nian cxiutagan doni al ni hodiaux!");
  srand(time(0));
//}--------UPDATING----------------------->
  while(!quit)
  {
    while(SDL_PollEvent(&e)!=0)
    {
      if (e.type == SDL_QUIT) quit = 1;
      if (e.type == SDL_KEYDOWN)
      {
        switch(e.key.keysym.sym)
        {
          case SDLK_BACKSPACE:
          break;
          case SDLK_RETURN:
          break;
          case SDLK_ESCAPE:
          break;
        case SDLK_HOME:
          {
            SDL_Surface* pScreenShot = SDL_CreateRGBSurface(0,WIDTH,HEIGHT, 32, 0x00ff0000, 0x0000ff00, 0x000000ff,0xff000000);
            if (pScreenShot)
            {
              SDL_RenderReadPixels(gRenderer,NULL,SDL_GetWindowPixelFormat(gWindow),pScreenShot->pixels,pScreenShot->pitch);
              SDL_SaveBMP(pScreenShot,"Screen.bmp");
              SDL_FreeSurface(pScreenShot);
            }
          }
          break;
        case SDLK_1: 
        case SDLK_KP_1:
          break;
        case SDLK_2:
        case SDLK_KP_2:
          break;
        case SDLK_3:
        case SDLK_KP_3:
          break;
        case SDLK_4:
        case SDLK_KP_4:
          break;
        case SDLK_5:
        case SDLK_KP_5:
          break;
        case SDLK_6:
        case SDLK_KP_6:
          break;
        case SDLK_7:
        case SDLK_KP_7:
          break;
        case SDLK_8:
        case SDLK_KP_8:
          break;
        case SDLK_9:
        case SDLK_KP_9:
          break;
        case SDLK_0:
        case SDLK_KP_0:
          break;
        }
      }
      if (e.type == SDL_TEXTINPUT)
      {
      }
    }
    SDL_RenderClear(gRenderer);
    printBuf(mainBuf);
    SDL_RenderPresent(gRenderer);
  }
 
//}---------FINISHING-------->
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

char* genBestNomo()
{
  char *ek = bek->getRandom();
  char *fin = bfin->getRandom();
  char *ret;
  ret = new char(strlen(ek)+strlen(fin)+1);
  sprintf(ret,"%s%s",ek,fin);
  ret[0] = ret[0]+'A'-'a';
  return ret;
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

  SDL_Rect tRect = {x+(SIZE - tSurface->w)/2,y,tSurface->w,tSurface->h};
  SDL_RenderCopy(gRenderer,tTexture, NULL,&tRect);

  SDL_FreeSurface(tSurface);
  SDL_DestroyTexture(tTexture);
}

void printBuf(charBuf *buff)	//PrintBuf
{
  int i;
  int j;
  SDL_Color tColor = {0,200,0};
  for (i=0;i<buff->getHeight();i++)
  {
    for (j=0;j<buff->getWidth();j++)
    {
     // if (buff->getChar(i,j)) drawChar(buff->getChar(j,i),tColor,(j+buff->getLeft())*SIZE,(i+buff->getTop())*SIZE);
      drawChar(buff->getChar(j,i),tColor,(j+buff->getLeft())*SIZE,(i+buff->getTop())*SIZE);
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
  return ret;
}

int load()
{
  int ret = 1;
  gFont = TTF_OpenFont("res/COURIER.TTF",SIZE);
  if (!gFont)
  {
    printf("Some Shit with your Courier Font! %s\n",TTF_GetError());
    ret = 0;
  }
  bek =  new Nomaro("arc/best_ek");
  bfin = new Nomaro("arc/best_fin");
  return ret;
}



