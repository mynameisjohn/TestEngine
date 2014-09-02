#include "glmBox.h"
std::ostream& operator<<(std::ostream& os, const glm::vec3& vec){
  std::cout << "<" << vec.x << ", " << vec.y << ", " << vec.z << ">";
  return os;
}

glmBox::glmBox(){
	mPos=vec3();
	mDim=vec3(1, 1, 1);
}

glmBox::glmBox(vec3 dim){
	mPos=vec3();
	mDim=glm::abs(dim);
}

glmBox::glmBox(vec3 pos, vec3 dim){
	mPos=pos;
	mDim=glm::abs(dim);
}

float glmBox::left(){
	return mPos.x;
}

float glmBox::right(){
	return mPos.x+mDim.x;
}

float glmBox::bottom(){
	return mPos.y;
}

float glmBox::top(){
	return mPos.y+mDim.y;
}

float glmBox::near(){
	return mPos.z;
}

//Minus because z goes from 0->-inf
float glmBox::far(){
	return mPos.z-mDim.z;
}

vec3 glmBox::getPos(){
	return mPos;
}

vec3 glmBox::getDim(){
	return mDim;
}

vec3 glmBox::center(){
	return mPos+0.5f*mDim;
}
