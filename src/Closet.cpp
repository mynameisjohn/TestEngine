#include "Util.h"
#include "Closet.h"
#include <glm/gtx/transform.hpp>
#include <sstream>

unique_ptr<Drawable> initDrawable(TiXmlElement * el, JShader& shader){
	string fileName = string("res/") + el->Attribute("fileName");
	unique_ptr<Drawable> ret;
	bool rigged = string(el->Attribute("type")) == string("r");
	bool tex = (string(el->Attribute("type")) == string("t")) && !rigged;

	vec3 scale, translate;
	string S = el->Attribute("S"), T=el->Attribute("T"), d=",";
	size_t p1=0, p2=0;
	for (int i=0;i<3;i++){
		p1=S.find(d);
		p2=T.find(d);
		stringstream(S.substr(0,p1)) >> scale[i];
		stringstream(T.substr(0,p2)) >> translate[i];
		S.erase(0,p1+d.length());
		T.erase(0,p2+d.length());
	}
	
	if (rigged)
		ret = unique_ptr<Drawable>(new Rig(initRigFromSVG(fileName, shader)));
	else if (tex)
		ret = unique_ptr<Drawable>(new Drawable(initTexQuad(fileName, shader)));
	else
		ret = unique_ptr<Drawable>(new Drawable(initQuad(shader)));

	ret.get()->leftMultMV(glm::translate(translate)*glm::scale(scale));

	return ret;
}

//Fills scene graph recursively, according to provided file
 Drawable * fillSceneGraph(SceneGraph& sg, TiXmlElement * el, JShader& shader){
//void fillSceneGraph(SceneGraph& sg, TiXmlElement * el, JShader& shader){
	TiXmlElement * i, * p;
	string name;

	//Create the drawable
	name = el->Attribute("name");
	if (sg.find(name) == sg.end())
		sg[name] = initDrawable(el, shader);
	else cout <<"duplicate drawable in hierarchy\n";


/*	
	//If it has a parent, connect them
	p = el->Parent()->ToElement();
	if (p)
		sg[p->Attribute("name")].get()->addChild(sg[name].get());
*/	
	//For all children, do the above
	for (i=el->FirstChildElement("drawable"); i; i=i->NextSiblingElement("drawable"))
		//fillSceneGraph(sg, i, shader);
		sg[name].get()->addChild(fillSceneGraph(sg, i, shader));
	//Once we're done, or if there are no children, 
	return sg[name].get();	
	
}

//Initializes Scene graph, stored as a string map for convenience
Skeleton getSkeleton(TiXmlElement * skeleton, JShader& shader){
	TiXmlElement * root = skeleton->FirstChildElement("drawable");
	if (root){
		SceneGraph sg;
		fillSceneGraph(sg, root, shader);
		return Skeleton(move(sg), sg[root->Attribute("name")].get());
	}
	else
		cout << "Invalid XML file" << endl;
	
	return Skeleton();
}
