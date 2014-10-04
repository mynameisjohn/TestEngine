#ifndef SKELETON_H
#define SKELETON_H

#include <map>
#include "Drawable.h"
#include <memory>

typedef std::map<std::string, std::unique_ptr<Drawable> > SceneGraph;

class Skeleton{
public:
	Skeleton();
	Skeleton(SceneGraph SG, Drawable * r);
	Skeleton(Drawable * r);
	void setRoot(Drawable * r);
	void add(std::string, std::unique_ptr<Drawable>);
	Drawable * getRoot();
	Drawable * operator[](string idx);
private:
	SceneGraph sg;
	Drawable * root;
};

#endif
