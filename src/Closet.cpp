#include "Util.h"
#include "Closet.h"
#include <glm/gtx/transform.hpp>
/*
//Rather than doing this, create a map that associates fileNames
//and drawable pertinents, i.e GLuints to the VAO and textures.
//In doing so, we can re-use these resources on the GPU for multiple
//drawables, making instantations of the same thing much cheaper
Ligament createLigament(TiXmlElement * el, JShader& shader){
	//map fileName to unique_ptr<Drawable>
	string fileName = el->Attribute("fileName");
   unique_ptr<Drawable> drPtr;
	bool shift(false);

	vec3 scale, translate;
	vec4 R(0,0,0,1);
	//string S = el->Attribute("S"), T=el->Attribute("T"), d=",";
	string T=el->Attribute("T"), d=",", S;
	size_t p1=0, p2=0;

	for (int i=0;i<3;i++){
//		p1=S.find(d);
		p2=T.find(d);
//		stringstream(S.substr(0,p1)) >> scale[i];
		stringstream(T.substr(0,p2)) >> translate[i];
//		S.erase(0,p1+d.length());
		T.erase(0,p2+d.length());
	}

	if (el->Attribute("R")){
		S = el->Attribute("R");
		p1=0;
		for (int i=0;i<4;i++){
			p1 = S.find(d);//in degrees
			stringstream(S.substr(0,p1)) >> R[i];
			S.erase(0,p1+d.length());
		}
	}

	if (el->Attribute("sh")){
		stringstream(el->Attribute("sh")) >> shift;//shift = (bool)(el->Attribute("shift")[0]);
	}
	
	char type = el->Attribute("type")[0];
	switch (type){
		case 'r':
			drPtr = unique_ptr<Drawable>(new Rig(initRigFromSVG(fileName, shader)));
			break;
		case 't':
			drPtr = unique_ptr<Drawable>(new Drawable(initTexQuad(fileName, shader)));
			break;
		case 'p':
			drPtr = unique_ptr<Drawable>(new Drawable(initPolyFromSVG(fileName, shader)));
			break;
		case 's':
			drPtr = unique_ptr<Drawable>(new Drawable(initSpriteSheet(fileName, shader)));
			break;
		default:
			drPtr = unique_ptr<Drawable>(new Drawable(initQuad(shader)));
			break;
	}
	//this is bad: we don't want to be multiplying this MV based on the incoming skeleton. 
//	drPtr->leftMultMV(glm::translate(translate));
	Ligament L(move(drPtr), QuatVec(translate,getRQ(R)));
	if (shift)
		L.shift();

	return L;
}
*/
/*
unique_ptr<Drawable> initDrawable(TiXmlElement * el, JShader& shader){
	string fileName = el->Attribute("fileName");
	unique_ptr<Drawable> ret;
*/
/*
	So, if the filename didn't have anything in the map, we'd go through all of this.
	then we'd get the VAO and texture GLuints, make some sort of struct from them, and
	insert it into the map. Later, if this filename gets called up, we'd just return the
	VAO and texture GLuints and make a new Drawable from them. All hierarchy things are 
	taken care of elsewhere... the only unique thing about all of these is the modelview
	matrix. So like, we have two arms; we'd load up the first arm, translate it to wherever, 
	and be done with it. For the second one, we just find its GLuints in the map
	and make a new drawable, then translate it to wherever. the MV  mats would be different,
	but the geometry would be the same, which is what we want. 
*/
/*
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
	
	char type = el->Attribute("type")[0];
	switch (type){
		case 'r':
			ret = unique_ptr<Drawable>(new Rig(initRigFromSVG(fileName, shader)));
			break;
		case 't':
			ret = unique_ptr<Drawable>(new Drawable(initTexQuad(fileName, shader)));
			break;
		case 'p':
			ret = unique_ptr<Drawable>(new Drawable(initPolyFromSVG(fileName, shader)));
			break;
		case 's':
			ret = unique_ptr<Drawable>(new Drawable(initSpriteSheet(fileName, shader)));
			break;
		default:
			ret = unique_ptr<Drawable>(new Drawable(initQuad(shader)));
			break;
	}

	//Under the new paradigm, this becomes some sort of local transform.
	ret.get()->leftMultMV(glm::translate(translate)*glm::scale(scale));
	//as such, rather than return a drawable this'll have to return something
	//altogether different. Maybe but S,T inside Rig file

	return ret;
}
*/
/*
Ligament * fillMap(map<string, Ligament>& lMap, TiXmlElement * el, JShader& shader){
	string name = el->Attribute("name");
	if (lMap.find(name) == lMap.end())
		lMap[name] = createLigament(el, shader);
	else{
		cout << "Duplicate drawable " << name << " in hierarchy. Segfault imminent. " << endl;
		return nullptr;
	}

	for (TiXmlElement * i = el->FirstChildElement("drawable"); i; i=i->NextSiblingElement("drawable"))
		lMap[name].addChild(fillMap(lMap, i, shader));

	return (&(lMap[name]));
}
*/
/*
//Fills scene graph recursively, according to provided file
Drawable * fillSceneGraph(SceneGraph& sg, TiXmlElement * el, JShader& shader){
//void fillSceneGraph(SceneGraph& sg, TiXmlElement * el, JShader& shader){
	TiXmlElement * i, * p;
	string name;

	//Create the drawable
	name = el->Attribute("name");
	if (sg.find(name) == sg.end())
		sg[name] = initDrawable(el, shader);
	else 
		cout <<"duplicate drawable in hierarchy\n";

	//For all children, do the above
	for (i=el->FirstChildElement("drawable"); i; i=i->NextSiblingElement("drawable"))
		sg[name].get()->addChild(fillSceneGraph(sg, i, shader));
	//Once we're done, or if there are no children, 
	return sg[name].get();	
	
}
*/
/*
//Initializes Scene graph, stored as a string map for convenience
Skeleton getSkeleton(TiXmlElement * skeleton, JShader& shader){
	TiXmlElement * root = skeleton->FirstChildElement("drawable");
	if (root){
//		SceneGraph sg;
		map<string, Ligament> lMap;
//		fillSceneGraph(sg, root, shader);
		fillMap(lMap, root, shader);
		Skeleton z(move(lMap), (&(lMap[root->Attribute("name")])));
		return z;//Skeleton(move(sg), sg[root->Attribute("name")].get());
	}
	else
		cout << "Invalid XML file" << endl;
	
	return Skeleton();
}
*/
Skeleton Closet::createSkeleton(TiXmlElement * skeleton, JShader& shader){
	TiXmlElement * root = skeleton->FirstChildElement("drawable");
   if (root){
		vector<Ligament> lVec;
		unordered_map<string, int> nameMap;
		fill(lVec, nameMap, root, shader);
		Skeleton s(nameMap, lVec);

//		map<string, Ligament> lMap;
//		fillMap(lMap, root, shader);
		return s;//Skeleton(move((lMap)), root->Attribute("name"));//o&(lMap[root->Attribute("name")])));
	}
	else
		cout << "Invalid XML file" << endl;

   return Skeleton();
}

/*
Ligament * Closet::fillMap(map<string, Ligament>& lMap, TiXmlElement * el, JShader& shader){
	string name = el->Attribute("name");
	if (lMap.find(name) == lMap.end())
		lMap[name] = move(createLigament(el, shader));
	else{
		cout << "Duplicate drawable " << name << " in hierarchy. Segfault imminent. " << endl;
		return nullptr;
	}

	for (TiXmlElement * i = el->FirstChildElement("drawable"); i; i=i->NextSiblingElement("drawable"))
		lMap[name].addChild(fillMap(lMap, i, shader));

	return (&(lMap[name]));
}
*/

int Closet::fill(vector<Ligament>& lVec, unordered_map<string, int>& nameMap, TiXmlElement * el, JShader& shader){
   string name = el->Attribute("name");

	if (nameMap.find(name) != nameMap.end()){
		cout << "Cycle created in Ligament map: " << name << " already exists. \n Segfault Imminent." << endl;
		return 0;
	}

	int curIdx = lVec.size();
	pair<string, int> p(name, curIdx);
	nameMap[name]=curIdx;
	lVec.push_back(createLigament(el, shader));

	for (TiXmlElement * i = el->FirstChildElement("drawable"); i; i=i->NextSiblingElement("drawable"))
		lVec[curIdx].addChild(fill(lVec, nameMap, i, shader) - curIdx);

	return curIdx;
/*
   if (lMap.find(name) == lMap.end())
      lMap[name] = move(createLigament(el, shader));
   else{
      cout << "Duplicate drawable " << name << " in hierarchy. Segfault imminent. " << endl;
      return lMap.begin();
   }

   for (TiXmlElement * i = el->FirstChildElement("drawable"); i; i=i->NextSiblingElement("drawable"))
      lMap[name].addChild(fillMap(lMap, i, shader));

   return lMap.find(name);//(&(lMap[name]));
*/
}
/*
map<string, Ligament>::iterator Closet::fillMap(map<string, Ligament>& lMap, TiXmlElement * el, JShader& shader){
	string name = el->Attribute("name");
	if (lMap.find(name) == lMap.end())
		lMap[name] = move(createLigament(el, shader));
	else{
		cout << "Duplicate drawable " << name << " in hierarchy. Segfault imminent. " << endl;
		return lMap.begin();
	}

	for (TiXmlElement * i = el->FirstChildElement("drawable"); i; i=i->NextSiblingElement("drawable"))
		lMap[name].addChild(fillMap(lMap, i, shader));

	return lMap.find(name);//(&(lMap[name]));
}
*/
void fillVec(vec3& v, string s){
	size_t pos(0);
	const string d(",");
	for (int i=0;i<3;i++){
      pos = s.find(d);
      if (!(stringstream(s.substr(0,pos)) >> v[i]))
         cout << "Invalid translation string " << s << endl;
      s.erase(0,pos+d.length());
   }
}

Ligament Closet::createLigament(TiXmlElement * el, JShader& shader){
	bool shift(false);
	vec3 scale(1), translate;
	vec4 rotate(0,0,0,1);
	
	const string d(",");
	string fileName(el->Attribute("fileName"));
	string T(el->Attribute("T"));
	
	fillVec(translate, T);
	
	if (el->Attribute("sh"))
      if (!stringstream(el->Attribute("sh")) >> shift)
			cout << "Invalid shift string " << el->Attribute("sh") << endl;

	if (dMapPtr->find(fileName) == dMapPtr->end()){
		unique_ptr<Drawable> drPtr;
		char type = el->Attribute("type")[0];
		switch (type){
			case 'r':
				drPtr = unique_ptr<Drawable>(new Rig(initRigFromSVG(fileName, shader)));
				break;
			case 't':
				drPtr = unique_ptr<Drawable>(new Drawable(initTexQuad(fileName, shader)));
				break;
			case 'p':
				drPtr = unique_ptr<Drawable>(new Drawable(initPolyFromSVG(fileName, shader)));
				break;
			case 's':
				drPtr = unique_ptr<Drawable>(new Drawable(initSpriteSheet(fileName, shader)));
				break;
			default:
				drPtr = unique_ptr<Drawable>(new Drawable(initQuad(shader)));
				break;
		}
		(*dMapPtr)[fileName] = move(drPtr);
	}

	Ligament L = Ligament((*dMapPtr)[fileName].get(), QuatVec(translate, fquat()));
	if (shift) L.shift();

//	cout << shift << " " << fileName << (*dMapPtr)[fileName].get() << endl;

	return L;
}
/*
   unique_ptr<Drawable> drPtr;
	bool shift(false);

	vec3 scale, translate;
	vec4 R(0,0,0,1);
	//string S = el->Attribute("S"), T=el->Attribute("T"), d=",";
	string T=el->Attribute("T"), d=",", S;
	size_t p1=0, p2=0;

	for (int i=0;i<3;i++){
//		p1=S.find(d);
		p2=T.find(d);
//		stringstream(S.substr(0,p1)) >> scale[i];
		stringstream(T.substr(0,p2)) >> translate[i];
//		S.erase(0,p1+d.length());
		T.erase(0,p2+d.length());
	}

	if (el->Attribute("R")){
		S = el->Attribute("R");
		p1=0;
		for (int i=0;i<4;i++){
			p1 = S.find(d);//in degrees
			stringstream(S.substr(0,p1)) >> R[i];
			S.erase(0,p1+d.length());
		}
	}

	if (el->Attribute("sh")){
		stringstream(el->Attribute("sh")) >> shift;//shift = (bool)(el->Attribute("shift")[0]);
	}
	
	char type = el->Attribute("type")[0];
	switch (type){
		case 'r':
			drPtr = unique_ptr<Drawable>(new Rig(initRigFromSVG(fileName, shader)));
			break;
		case 't':
			drPtr = unique_ptr<Drawable>(new Drawable(initTexQuad(fileName, shader)));
			break;
		case 'p':
			drPtr = unique_ptr<Drawable>(new Drawable(initPolyFromSVG(fileName, shader)));
			break;
		case 's':
			drPtr = unique_ptr<Drawable>(new Drawable(initSpriteSheet(fileName, shader)));
			break;
		default:
			drPtr = unique_ptr<Drawable>(new Drawable(initQuad(shader)));
			break;
	}
	//this is bad: we don't want to be multiplying this MV based on the incoming skeleton. 
//	drPtr->leftMultMV(glm::translate(translate));
	Ligament L(move(drPtr), QuatVec(translate,getRQ(R)));
	if (shift)
		L.shift();

	return L;
}
*/
