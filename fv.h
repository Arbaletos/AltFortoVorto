
#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <string>
#include <vector>

using namespace std;


#include "def.h"

#include "nomlist.c"
#include "battle.h"
//#include "charo.c"
#include "charbuf.c"
#include "reg.c"

//struct Charo* Charo_create(struct Charo* kiu, char* name, Chars* kio);
//char* Charo_list(struct Charo* kiu, char* buf);
//char* Charo_getName(struct Charo* kiu);
//int Charo_getInitiative(struct Charo* kiu);
//int Charo_isTired(struct Charo* kiu);
//void Charo_setTired(struct Charo* kiu, int kio);
//int Charo_isAlive(struct Charo* kiu);
//void Charo_kuraci(struct Charo* kiun,int kiom);
//int Charo_isExist(struct Charo* kiu);
//void Charo_free(struct Charo* kiu);
//char Charo_getSymbol(struct Charo* kiu);
//struct battleRes* Charo_attack(struct battleRes* ret, int kiuatk,int kiudmg, int kiuneva);

//void writeH(const char* text,charBuf* buff, int x, int y); 
//void putChar(char ch,charBuf* buff, int x, int y);
void printBuf(charBuf* buff);
//void clearBuf(charBuf* buff);
//charBuf* createBuf(charBuf* kio, int x, int y, int wid, int hei, int vis); 
