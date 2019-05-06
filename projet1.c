#include "projet1.h"

void Init_Object(object *O,char tmpType[10],int a,int b)
{
strcpy(O->type,tmpType);
char Dir[100];
sprintf(Dir,"src/Image/objects/level1/%s00.png",O->type);
printf("%s",Dir);
O->object_Surface=IMG_Load(Dir);

O->zoom=1;
SDL_GetClipRect(O->object_Surface,&O->object_Rect);

O->object_Rect.x=a;
O->object_Rect.y=b;
if(strcmp(O->type,"Health")==0)
O->valeur=10;
else if(strcmp(O->type,"Energy")==0)
O->valeur=5;
else if(strcmp(O->type,"Repair")==0)
O->valeur=1;
}

void Afficher_object(object o,SDL_Surface *Screen)
{
if(o.object_Surface!=NULL)
SDL_BlitSurface(o.object_Surface,NULL,Screen,&o.object_Rect);
}

void Animation_Sentity(Sentity *c,int Raz)
{
char Dir[100];
if (Raz  == 0 || c->frame == 21)
c->frame = 0;
if (c->etat==1)
{
sprintf(Dir,"./src/Image/Sentity/%s/Idle/View%d/image%04d.png",c->name,c->direction,c->frame);
}
else if(c->etat==2 )
{
sprintf(Dir,"./src/Image/Sentity/%s/Run/View%d/image%04d.png",c->name,c->direction,c->frame);
}
else if(c->etat==3)
sprintf(Dir,"./src/Image/Sentity/%s/Run/View%d/image%04d.png",c->name,c->direction,c->frame);
SDL_Surface* tmp=c->surface;
c->surface = IMG_Load(Dir);
SDL_FreeSurface(tmp);
c->pos.h=c->surface->h;
c->pos.w=c->surface->w;
c->frame++;
}
//----------------------------



//--------------------
void Init_Sentity(Sentity *c,char tmpname[10],int a,int b)
{
char Dir[100];
strcpy(c->name,tmpname);
sprintf(Dir,"src/Image/Sentity/%s/Idle/View1/image0000.png",c->name);
printf("%s",Dir);
c->surface=IMG_Load(Dir);
SDL_GetClipRect(c->surface,&c->pos);
c->pos.x=a;
c->pos.y=b;
c->posx=a;
c->posy=b;
c->direction=1;
c->etat=1;
c->frame=0;
c->zoom=1;
}
void Afficher_Sentity(Sentity C,SDL_Surface* Screen)
{
if(C.surface!=NULL)
SDL_BlitSurface(C.surface,NULL,Screen,&C.pos);
}

void Move_Alea(Sentity *D,int *Count,int x,int xx)
{
  static int alea;
if ((*Count)==0)
{
  alea=rand()%4;
}
  switch(alea)
  {
  case 0:
if((D->posx+D->pos.w)<=x)
{
  D->posx+=3;
  D->etat=2;
}
else
D->etat=1;
  D->direction=1;
  break;
  case 1:
if(D->posx>=xx)
{
  D->posx-=3;
  D->etat=2;
}
else
D->etat=1;
  D->direction=2;
break;
default:
D->etat=1;
  }

(*Count)++;
if((*Count)==21)
(*Count)=0;

}


//---------------------------------------------------------------


void Update_Rect(SDL_Rect Crop_Rect,SDL_Rect *Rect,int x,int y)
{
if(x-Crop_Rect.x>-250 && y-Crop_Rect.y>-250)
{
Rect->x=x-Crop_Rect.x;
Rect->y=y-Crop_Rect.y;
}
else
{
Rect->x=5000;
Rect->y=5000;
}
}




int State (SDL_Rect* C,Sentity* D)
{
  int dx=100,dy=100;
  if (abs(C->x-(D->pos.x)) >=300 || abs(C->y-(D->pos.y))>=300)
    {printf("Waiting\n");
    return 1;}
   else if ((abs(C->x-(D->pos.x)) <=300 && abs(C->y-(D->pos.y))<=300) && (abs(C->x-(D->pos.x)) >=110))
     {printf("Following\n");
     return 2;}
    else
    return 3;}


void moveSentity(Sentity *D,SDL_Rect C)
{
  switch(D->direction)
  {
  	case 2:
	if((D->pos.x/10)*10 >((C.x/10)*10)+100)
  	D->posx-=2;
  	break;
  	case 1:
	if((D->pos.x/10)*10 <((C.x/10)*10)-100)
  	D->posx+=2;
  	break;
  }


}

void follow(SDL_Rect C,Sentity *D)
{
  if((D->pos.x) <((C.x)))
     D->direction=1;
   else if((D->pos.x) >((C.x)))
     D->direction=2;
   else {D->etat=3;}
   if(D->etat==2)
   	moveSentity(D,C);
}
//----------------------------------------------------------
void Zoom(Sentity *C,int Input)
{
  
{
switch(Input)
{
case 5:
C->zoom*=(1/1.05);
break;

case 1:
C->zoom*=(1.05);
break;
}
}
}

void Update_Zoom(Sentity* C,int Input)
{
int x=C->pos.x,y=C->pos.y;
Zoom(C,Input);
C->surface=zoomSurface(C->surface,C->zoom,C->zoom,0);
SDL_GetClipRect(C->surface,&C->pos);
C->pos.x=x;
C->pos.y=y;

}


