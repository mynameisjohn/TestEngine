#include "iCube.h"

iCube::iCube(){
   x=0; y=0; z=0;
   w=40; y=40; z=40;
}

iCube::iCube(int W, int H, int D){
	x=0; y=0; z=0;
   w=W; h=H; d=D;
}

iCube::iCube(int X, int Y, int Z, int W, int H, int D){
   x=X; y=Y; z=Z;
   w=W; h=H; d=D;
}
