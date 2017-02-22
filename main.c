int LANG = 1;
#include "tekstaro.h"
#include "fv.h"

enum gamestates
{
  MAIN_MENU,
  CC_MENU,
  WORLD_MENU,
  DUNGEON_MENU,
  BATTLE_MENU
};

enum symbols
{
  NONE,
  INIT,
  ENTER,
  ESCAPE,
  UP,
  DOWN
};

  double sqrts[] = {0,1,1.41421356,1.73205080,2,2.23606798,2.44948974,2.6457513110,2.8284271247,3};

  SDL_Window* gWindow = NULL;
  SDL_Renderer* gRenderer = NULL;
  TTF_Font *gFont = NULL;

  FILE* glog;

  int state;
  int quit = 0;

  charBuf *mainBuf;

  Nomaro* bek;
  Nomaro* bfin;

  int init();
  int load();
  int drawText(char* text, SDL_Color tColor, int x, int y);

  void inputString(char *buff, charBuf* Buf);
  
  char* input;   //Buffer for input strings
  int input_text = 0;   //Fact of inputing curmomente
  charBuf *inputBuf;   //Buffer for write down inputed smt.
  int input_len = 0;

  void Aut(int symb);
  
  char * nomo;



int main(int argc, char* args[])
{
//}---------INITIALIZATION/LOADING-------->
  if (!init()) exit(0);
  if (!load()) exit(0);
  static int enter = 1;
  SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00,0x00,0x00);
  int i = 0;
  int j = 0;
  SDL_Color tColor= {0,180,0};
  SDL_Event e;
//  mainBuf = new charBuf(0,0,WIDTH/SIZE,HEIGHT/SIZE);
  state = MAIN_MENU;
  mainBuf = new charBuf(0,0,WIDTH/SIZE,HEIGHT/SIZE);
//  mainBuf->append("Tell me... Do you bleed?");
//  mainBuf->append("Patro nia kiu estas en la cxielo sankta estu via nomo estu regxeco via kiel en la cxielo tiel ankaux sur la tero panon nian cxiutagan doni al ni hodiaux!");
  Aut(INIT);
  srand(time(0));
//}--------UPDATING----------------------->
  while(!quit)
  {
    while(SDL_PollEvent(&e)!=0)
    {
      if (e.type == SDL_QUIT) quit = 1;
      if (e.type == SDL_TEXTINPUT)
      {
        if (input_text){
          input_len++;
          strcat(input, e.text.text);
          if (inputBuf) inputBuf->append(1, e.text.text);
        }
      }
      if (e.type == SDL_KEYUP)
      {
        if (e.key.keysym.sym == SDLK_RETURN) enter = 0;
      }
      if (e.type == SDL_KEYDOWN)
      {
        if (input_text)
        {
          switch(e.key.keysym.sym)
          {
            case SDLK_BACKSPACE:
              if (input_len>0)
              {
                input_len--;
                input[strlen(input)-2] = '\0';
                if (inputBuf) inputBuf->backspace();
              }
              break;

            case SDLK_RETURN:
              if (!enter){
                if (input_len){
                  SDL_StopTextInput();
                  input_text = 0;
                  Aut(ENTER); 
                  enter = 1;
                }
              }
              break;
          }
        }
        else 
        {
  //        printf("keycode: %d\n",e.key.keysym.sym);
          switch(e.key.keysym.sym)
          {
            case SDLK_UP:
              Aut(UP);
              break;
            case SDLK_DOWN:
              Aut(DOWN);
              break;
            case SDLK_RETURN:
              if (!enter) Aut(ENTER);
              enter = 1;
              break;
            case SDLK_ESCAPE:
              Aut(ESCAPE);
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
      }
    }
    SDL_RenderClear(gRenderer);
    mainBuf->printBuf();
//    nomo = new char(1000);
//    inputString(nomo,mainBuf);
//    if (!input_text) printf("%s",nomo);
    
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

// Avto
void Aut(int symb)  
{
  enum {
    BEGIN,
    ENTER_NAME,
    SELECT_CLASS,
    SELECT_WORLD
  };
  static int select = 0;
  static int prog = BEGIN;
  static char name[100];
  static char buf[BUFFSIZE];
  static int vert_menu = 1;
  static int options = 5;
  static SDL_Color* colors[10];
  static SDL_Color yellow = {200,200,0};
  static SDL_Color red = {200,0,0};
  static SDL_Color* def = NULL;
  

  if (vert_menu)   //Universal up-down menu adapter
  {
    int i;
    if (symb==UP) select--;
    if (symb==DOWN) select++;
    if (select<0) select = options - 1;
    if (select>=options) select = 0;
    for (i=0;i<options;i++)
    {
      colors[i] = def; 
    }
    colors[select] = &yellow;
  }
//mainBuf->append(sprintf(buf, "state: %d, symbol: %d\n",prog,symb),buf);
  switch (state)
  {
    case MAIN_MENU:
      switch (prog)
      {
        case SELECT_WORLD:
          mainBuf->append(sprintf(buf, "I am really sorry, but you can only \
choose the Chaos World for now. Why? Because i havn't implemented other types \
of worlds. The Chaos World is a World, in which every new througrun is \
different, every castname, and inhabtans are different. It's a very good place \
for warriors of Sacred Random, but not so good for Fighters of Stability. Hit \
[ENTER] To Proceed to your destiny, or [ESCAPE] to escape the dangerous messes \
of leiden and adventures!\n"),buf,1);
          if (symb==ENTER)
          {
            state = CC_MENU;
            prog = ENTER_NAME; 
            Aut(NONE);
          }
          if (symb==ESCAPE)
          {
            prog = BEGIN;
            Aut(NONE);
          }
        break;
        case BEGIN:
          options = 5;
          mainBuf->append(sprintf(buf, "Start the game\n"),buf, 1, colors[0]);
          mainBuf->append(sprintf(buf, " Create World\n"),buf, 0, colors[1]);
          mainBuf->append(sprintf(buf, "  Settings\n"),buf, 0, colors[2]);
          mainBuf->append(sprintf(buf, "   Credit\n"),buf, 0, colors[3]);
          mainBuf->append(sprintf(buf, "    Exit\n"),buf, 0, colors[4]);
          if (symb==ENTER)
          {
            if (select==0)
            {
              vert_menu = 0;
              prog = SELECT_WORLD;
              Aut(NONE);
            }
            if (select==4) quit = 1;
          }
        break;
/*
      case BEGIN:
//      sprintf(buf,"Say me your name.\n");
        mainBuf->append(sprintf(buf, "Say me your name, %s\n> ","Beach!"),buf);
        inputString(name, mainBuf);
        prog = ENTER_NAME;
        break;
      case ENTER_NAME:
        if (symb == ENTER)
        {
        SDL_Color blood = {180,0,0};
        mainBuf->append(sprintf(buf, "\nTell me, %s, do you bleed?\n",name), buf);
        mainBuf->append(sprintf(buf, "\nYOU WILL!\n"), buf, 0, &blood);
//          mainBuf->append(sprintf(buf, "This is second phrase, %s\n","Beach!"), buf);
        }
        break;
*/
      }   
    break;
    case CC_MENU:
      switch (prog)
      {
        case ENTER_NAME:
          mainBuf->append(sprintf(buf, "Welcome to FortoVorto game!\n You are the \
one of Tlatoani - the Mighty People, who use words to achieve magnificient \
power! But to get your mind and tongue sharp, and to speak more excellent \
speeches, to ceome one day HUEI TLATOANI, the Great Wordspeaker, you need to \
always find sources of new words!\nSo, in eternal adventures, just like other \
Tlatoani, you seek for lore... So. What is your name, Tlatoani?\n"),buf,1);   
          inputString(name, mainBuf);
          if (symb==ENTER) 
          {
            prog = SELECT_CLASS;
            vert_menu = 1;
            options = 6;
            Aut(NONE);
          }
          select = 0;
        break;
        case SELECT_CLASS:
          options = 6;
          vert_menu = 1;
          colors[select] = &yellow;
          mainBuf->append(sprintf(buf, "There are different classes of Tlatoani. Someone use wordpower to \
fortify their own strength, some enchant their items, some speaks to gods, spirits, souls and e.t.c. \n\
Choose your Tloatoani Archtype wisely!\n\n"),buf, 1);
          mainBuf->append(sprintf(buf, "Barbaro\n"),buf, 0, colors[0]);
          mainBuf->append(sprintf(buf, "Skiencist\n"),buf, 0, colors[1]);
          mainBuf->append(sprintf(buf, "Elementalist\n"),buf, 0, colors[2]);
          mainBuf->append(sprintf(buf, "Necromancer\n"),buf, 0, colors[3]);
          mainBuf->append(sprintf(buf, "Musician\n"),buf, 0, colors[4]);
          mainBuf->append(sprintf(buf, "Back\n\n\n"),buf, 0, colors[5]);
          
          mainBuf->append(sprintf(buf, "CLASS DESCRIPTION:\n"),buf, 0, &red);
          if (select <= options-1)
            mainBuf->append(sprintf(buf, class_desc_mes[select]),buf, 0);
          
   //       if (select==0) 
          if (symb==ENTER)
          {
            if (select==0)
            {
              Aut(NONE);
            }
//            if (select==5) quit();
          }
         break;

       }
    break;
  }
}

void inputString(char *buff, charBuf* Buf=NULL)
{
  SDL_StartTextInput();
  
  inputBuf = Buf;
  input = buff;
  input[0] = '\0';
  input_text = 1;
  input_len = 0;
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
  TTF_ByteSwappedUNICODE(1); 


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
  loadText(LANG);
  if (!gFont)
  {
    printf("Some Shit with your Courier Font! %s\n",TTF_GetError());
    ret = 0;
  }
  bek =  new Nomaro("arc/best_ek");
  bfin = new Nomaro("arc/best_fin");
  return ret;
}



