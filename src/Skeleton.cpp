#include "Skeleton.h"

Skeleton::Skeleton() 
: root(nullptr){}

Skeleton::Skeleton(SceneGraph SG, Drawable * r)
: sg(std::move(SG)), root(r){}

Skeleton::Skeleton(Drawable * r) 
: root(r) {}

void Skeleton::setRoot(Drawable * r){
	root = r;
}

Drawable * Skeleton::getRoot(){
	return root;
}

Drawable * Skeleton::operator[](string idx){
	return sg[idx].get();
}
