#include<stdio.h>
#include<stdlib.h>
#include<SDL/SDL.h>
#include<SDL/SDL_image.h>
#include<SDL/SDL_ttf.h>
#include<SDL/SDL_mixer.h>
#include<string.h>
#include<math.h>
#include "projet1.h"
#include "projet1.c"
#include <SDL/SDL_rotozoom.h>

//#include "trigo.c"
#include "enigme.c"
#include <time.h>
#ifdef WIN32
#pragma comment(lib,"sdl.lib")
#pragma comment(lib,"sdlmain.lib")
#endif

#define STAT_SOL 0
#define STAT_AIR 1

int arduinoWriteData(int x);
int arduinoReadData(int *x);
int directionArduino=0;
int received;


typedef struct
{
	int status;
	float x,y;
	float vx,vy;
	SDL_Rect dimensions;
} Sprite;



void InitSprite(Sprite* Sp)
{
	Sp->x = 100.0f;
	Sp->y = 220.0f;
	Sp->status = STAT_SOL;
	Sp->vx = Sp->vy = 0.0f;
	
	Sp->dimensions.x=0;
        Sp->dimensions.y=0;
        Sp->dimensions.h=200;
        Sp->dimensions.w=60;
}

SDL_Rect Render(Sprite* Sp,SDL_Surface* screen,SDL_Surface *image,SDL_Rect camera,SDL_Surface * background )
{

	SDL_Rect R;
	R.x = (Sint16)Sp->x;
	R.y = (Sint16)Sp->y+140;

	SDL_BlitSurface(background,&camera,screen,NULL);
	SDL_BlitSurface(image,&Sp->dimensions,screen,&R);
	//SDL_FillRect(screen,&R,SDL_MapRGB(screen->format,0,255,0));
return R;
	
}

void Saute(Sprite* Sp,float impulsion)
{
	Sp->vy = -impulsion;
	Sp->status = STAT_AIR;
}

void ControleSol(Sprite* Sp)
{
	if (Sp->y>250.0f)
	{
		Sp->y = 250.0f;
		if (Sp->vy>0)
			Sp->vy = 0.0f;
		Sp->status = STAT_SOL;
	}
}

void Gravite(Sprite* Sp,float factgravite,float factsautmaintenu,Uint8* keys)
 {
	if (Sp->status == STAT_AIR &&( keys[SDLK_SPACE]||directionArduino==3))
		factgravite/=factsautmaintenu;
	Sp->vy += factgravite;
}

//--------------------------------------------
int collusion_trigonometrique_inscrit(SDL_Surface *image1, SDL_Surface *image2,SDL_Rect position_image1,SDL_Rect position_image2)
{
 float centre1_x=position_image1.x+(position_image1.w/2);
 float centre1_y=position_image1.y+(position_image1.h/2);
 float r1,distance;
 if(position_image1.w<position_image1.h)
  r1=position_image1.w/2;
 else r1=position_image1.h/2;

 float centre2_x=position_image2.x+(position_image2.w/2);
 float centre2_y=position_image2.y+(position_image2.h/2);
 float r2;
 if(position_image2.w<position_image2.h)
  r2=position_image2.w/2;
 else r2=position_image2.h/2;

double a=(centre2_x-centre1_x)*(centre2_x-centre1_x)+(centre2_y-centre1_y)*(centre2_y-centre1_y);

 distance=sqrt(a)-(r1+r2);
 

 if(distance<=0)
  return 1;
 else return 0;
}

//-------------------------------

void Evolue(Sprite* Sp,Uint8* keys,SDL_Rect * camera,int *input)
{
	float lateralspeed = 0.5f;
	float airlateralspeedfacteur = 1.0f;
	float maxhspeed = 3.0f;//+speed nik rouhk}
	float adherance = 1.5f;
	float impulsion = 6.0f;
	float factgravite = 0.5f;
	float factsautmaintenu = 3.0f;
       


          //input from arduino

arduinoReadData(&received);
switch(received){
case 0: directionArduino = 0; //mise à jour de la direction (directionArduino)
break;
case 1:directionArduino = 1;//right
break;
case 2:directionArduino = 2;//left
break;
case 3:directionArduino = 3;//up
break;
}

 //controle lateral(acceleration)
(*input)=0;
if (keys[SDLK_F1])
(*input)=5;
else if (keys[SDLK_F2])
(*input)=1;





	if (Sp->status == STAT_AIR) // (*2)
		lateralspeed*= airlateralspeedfacteur;
	if (keys[SDLK_LEFT]||directionArduino==2) // (*1)
		{Sp->vx-=lateralspeed;
		/*Sp->vx-=20;*/}//+speed nik rouhk}
	if (keys[SDLK_RIGHT]||directionArduino==1)
		{
		if(Sp->dimensions.x!=240)
                    Sp->dimensions.x=Sp->dimensions.x+60;
                 else
                  Sp->dimensions.x=0;
		 Sp->vx+=lateralspeed;
		 
		//Sp->vx+=20;

		}//+speed nik rouhk}
	if (Sp->status == STAT_SOL && !keys[SDLK_LEFT] && directionArduino!=2 && !keys[SDLK_RIGHT]&& directionArduino!=1) // (*3)
		Sp->vx/=adherance;

// limite vitesse
	if (Sp->vx>maxhspeed) // (*4)
		Sp->vx = maxhspeed;
	if (Sp->vx<-maxhspeed)
		Sp->vx = -maxhspeed;
// saut
	if ((keys[SDLK_SPACE] || directionArduino==3) && Sp->status == STAT_SOL)
{
		Saute(Sp,impulsion);
                arduinoWriteData(1);
}
	Gravite(Sp,factgravite,factsautmaintenu,keys);
	ControleSol(Sp);

// application du vecteur à la position.
	if(Sp->x<640)	
	{
	 Sp->x +=Sp->vx;
	 if(Sp->x<0)
	Sp->x -=Sp->vx;
	}	
	
         else 
	camera->x+=Sp->vx;
	if(camera->x<0)
	camera->x-=Sp->vx;
	Sp->y +=Sp->vy;

}

int main(int argc,char** argv)
{
 int count=0;
int input=0;

system("stty -F /dev/ttyACM0 9600 -parenb cs8 -cstopb");


	SDL_Surface* screen;
	SDL_Surface *image;
//--------------------------------------------	
	SDL_Surface *background;
	background =IMG_Load("adam.png");
	image=IMG_Load("ghada.png");
	SDL_Rect camera;
	camera.x=0;
	camera.y=0;
	camera.h=720;
	camera.w=1280;
	int cont=1;
Sentity sec;
object o;


Init_Object(&o,"Health",500,425);

SDL_Rect posJ;
	

//-----------------------------------------
	int numkeys;
	Uint8 * keys;
	Sprite S;
	Uint32 timer,elapsed;
	SDL_Init(SDL_INIT_VIDEO);
	screen=SDL_SetVideoMode(1280,720,32,SDL_SWSURFACE|SDL_DOUBLEBUF);
	InitSprite(&S);
 int distance;
Init_Sentity(&sec,"jack",300,425);
	do 
	{

			
		timer = SDL_GetTicks();
		SDL_FillRect(screen,NULL,0);
		SDL_PumpEvents();
		keys = SDL_GetKeyState(&numkeys);

//------------------------------------------
//AI

Update_Rect(camera,&sec.pos,sec.posx,sec.posy);

   sec.etat=State(&posJ,&sec);
	if (sec.etat==2)
	{follow(posJ,&sec);}
	else if (sec.etat==1)
	Move_Alea(&sec,&count,500,300);
        Animation_Sentity(&sec,1);

//--------------------------------------------




		Evolue(&S,keys,&camera,&input);
		posJ=Render(&S,screen,image,camera,background);
printf("%d",input);
Update_Zoom(&sec,input);
Afficher_Sentity(sec,screen);
//Afficher_object(o,screen);

distance=collusion_trigonometrique_inscrit(image,sec.surface,posJ,sec.pos);
if(distance)
{
arduinoWriteData(7);

//cont=0;
}else
arduinoWriteData(1);




		SDL_Flip(screen);
		elapsed = SDL_GetTicks() - timer;
		if (elapsed<20)
		SDL_Delay(20-elapsed);
	} while (!keys[SDLK_ESCAPE] && cont==1);
	SDL_Quit();
	return 0;
}
int arduinoWriteData(int x)
{
    char chemin[]="/dev/ttyACM0";
    FILE*f;

    f=fopen(chemin,"w");
    if(f == NULL)
        return(-1);

    fprintf(f,"%d",x);
    fclose(f);

    return(0);
}

int arduinoReadData(int *x)
{
    char chemin[]="/dev/ttyACM0";
    FILE*f;
    char c;
    f=fopen(chemin,"r");

    if(f == NULL)
        return(-1);

    fscanf(f,"%d",x);

    fclose(f);

    return(0);
}
