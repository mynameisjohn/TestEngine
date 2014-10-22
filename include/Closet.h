#ifndef CLOSET_H
#define CLOSET_H
/*
#ifndef TIXML_USE_STL
#define TIXML_USE_STL
#endif

#include <tinyxml.h>
*/
#include "Geom.h"
#include "Skeleton.h"
#include <unordered_map>

class Closet{
public:
	Closet(unordered_map<string, unique_ptr<Drawable> > * dMP = nullptr)
	: dMapPtr(dMP) {}
	Skeleton createSkeleton(TiXmlElement *, JShader&);
private:
	unordered_map<string, unique_ptr<Drawable> > * dMapPtr;
	Ligament createLigament(TiXmlElement * el, JShader& shader);
	int fill(vector<Ligament>& lVec, unordered_map<string, int>& nameMap, TiXmlElement * el, JShader& shader);
	//Ligament * fillMap(map<string, Ligament>& lMap, TiXmlElement * el, JShader& shader);
	//map<string, Ligament>::iterator fillMap(map<string, Ligament>& lMap, TiXmlElement * el, JShader& shader);
};

//Skeleton getSkeleton(TiXmlElement *, JShader&);
//SceneGraph initSceneGraph(TiXmlElement * root, JShader& shader);

#endif
