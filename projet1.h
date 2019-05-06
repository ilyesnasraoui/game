#ifndef PROJET_H_INCLUDED
#define PROJET_H_INCLUDED
#include <SDL/SDL_rotozoom.h>
typedef struct
{char type[10];
int valeur;
SDL_Surface *object_Surface;
SDL_Rect object_Rect;
float zoom;
}object;

typedef struct
{
char name[10];
int etat;
SDL_Rect pos;
SDL_Surface *surface;
int frame;
float zoom;
int direction;
int posx;
int posy;
}Sentity;
#endif
