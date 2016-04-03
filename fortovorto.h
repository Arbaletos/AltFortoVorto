#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>


struct Charo* Charo_create(char* name, int health, int def, int atk, int damage,int speed);
char* Charo_list(struct Charo* kiu, char* buf);
char* Charo_getName(struct Charo* kiu);
int Charo_getInitiative(struct Charo* kiu);
char* Charo_attack(struct Charo* kiu, struct Charo* kiun, char *log);
int Charo_isTired(struct Charo* kiu);
void Charo_setTired(struct Charo* kiu, int kio);

