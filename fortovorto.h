#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>


struct Charo* Charo_create(struct Charo* kiu, char* name, int health, int def, int atk, int damage,int speed);
char* Charo_list(struct Charo* kiu, char* buf);
char* Charo_getName(struct Charo* kiu);
int Charo_getInitiative(struct Charo* kiu);
char* Charo_attack(struct Charo* kiu, struct Charo* kiun, char *log);
int Charo_isTired(struct Charo* kiu);
void Charo_setTired(struct Charo* kiu, int kio);
int Charo_isAlive(struct Charo* kiu);
void Charo_kuraci(struct Charo* kiun,int kiom);
int Charo_isExist(struct Charo* kiu);
void Charo_free(struct Charo* kiu);
char Charo_getSymbol(struct Charo* kiu);

struct nomList *nomList_create(struct nomList *kio, char* filename);
char *nomList_print(struct nomList* kio);
char *nomList_getRandom(struct nomList* kio);

void writeH(char* text,struct charBuf* buff, int x, int y); 
void putChar(char ch,struct charBuf* buff, int x, int y);
void printBuf(struct charBuf* buff);
void clearBuf(struct charBuf* buff);
struct charBuf* createBuf(struct charBuf* kio, int x, int y, int wid, int hei); 
