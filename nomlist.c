#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include "struktoj.h"

struct nomList *nomList_create(struct nomList *kio, char* filename)
{
	FILE * src = fopen(filename,"r");
	int c,i;
	int len=0;
	struct nomList *ret;
	char **list;
	char curstr[VORTLEN];
	if (kio) free(kio);
	while ((c=fgetc(src)) != EOF)
	{
		if (c=='\n') len++;
	}
	ret = malloc(sizeof(struct nomList));
	list = malloc(len*sizeof(char*));
	src = freopen(filename,"r",src);
	for (i=0;i<len;i++)
	{
		list[i] = malloc((VORTLEN+1) * sizeof(char));
		fgets(list[i],VORTLEN,src);
		list[i][strlen(list[i])-1]='\0';
	}
	ret->list = list;
	ret->len = len;
}

char *nomList_print(struct nomList* kio)
{
	int i;
	for (i=0;i<kio->len;i++)
	{
		printf("%s",kio->list[i]);
	}
}

char *nomList_getRandom(struct nomList* kio)
{
	return kio->list[rand()%kio->len];
}
