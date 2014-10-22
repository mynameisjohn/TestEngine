#ifndef SKELETON_H
#define SKELETON_H

#include <map>
#include "Drawable.h"
#include <memory>
#include "Ligament.h"
#include <unordered_map>

/*
	See the Entity::Entity(Entity * f) constructor in Entity.cpp
	for details regarding a bug involving this class. 
*/

//typedef std::map<std::string, std::unique_ptr<Drawable> > SceneGraph;

class Skeleton{
public:
	Skeleton();
//	Skeleton(const Skeleton& other);
//	Skeleton(SceneGraph SG, Drawable * r);
	Skeleton(map<string, Ligament>, string r);// Ligament * l);
	Skeleton(Drawable * r);
	Skeleton(unordered_map<string, int> nM, vector<Ligament> lV);
	Skeleton(const Drawable& dr);
	//void setRoot(Ligament * l);
	//void add(string, Ligament l);
	void print();
//	void addToRoot(string, Ligament l);
	void draw(mat4 transform);
	void update();
	void applyTransform(mat4 m);
	void resetLigaments();
	Ligament * getRoot();
	Ligament * operator[](string idx);
private:
	//SceneGraph sg;
	map<string, Ligament> lMap;
//	Drawable * root;
	Ligament * root;
	map<string, Ligament>::iterator itRoot;
	mat4 MV;
	string R;

	unordered_map<string, int> nameMap;
	vector<Ligament> lVec;
};

#endif
