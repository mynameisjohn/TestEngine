#ifndef SKELETON_H
#define SKELETON_H

#include "Drawable.h"
#include <memory>

typedef std::map<std::string, std::unique_ptr<Drawable> > SceneGraph;

class Skeleton{
public:
	Skeleton() : root(nullptr){}
	Skeleton(SceneGraph SG, Drawable * r=nullptr) : sg(std::move(SG)), root(r){}
	Skeleton(Drawable * r) : root(r) {}
	void setRoot(Drawable * r) {root = r;}
private:
	SceneGraph sg;
	Drawable * root;
};

#endif
