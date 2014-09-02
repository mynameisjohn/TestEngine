#include "iRect.h"

iRect::iRect(){
	x=0; y=0;
	w=40; h=40;
}

iRect::iRect(int w, int h){
	x=0; y=0;
	this->w=w; this->h=h;
}

iRect::iRect(int x, int y, int w, int h){
	this->x=x; this->y=y;
	this->w=w; this->h=h;
}

void iRect::translate(int x, int y){
	this->x+=x;
	this->y+=y;
}
