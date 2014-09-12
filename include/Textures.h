#ifndef TEXTURES_H
#define TEXTURES_H
#include <string>

int * outline(int DIM);
int * invertedOutline(int DIM);
unsigned int fromImage(std::string fileName);
unsigned int outlineTexture();
unsigned int invertedOutlineTexture();

#endif
