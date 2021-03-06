#include "Soup.h"
#include "Textures.h"
#include "Closet.h"
#include <glm/gtx/transform.hpp> 

Collider getCollider(TiXmlElement * collider){
	string d=",";
	BoundBox bb;
	vector<BoundRect> recVec;
	for (TiXmlElement * box=collider->FirstChildElement("box"); box; box=box->NextSiblingElement("box")){
		vec3 dim;
		string in = box->Attribute("S");
		size_t pos=0;
		for (int i=0;i<3;i++){
			pos=in.find(d);
			stringstream(in.substr(0,pos)) >> dim[i];
			in.erase(0,pos+d.length());
		}
		bb = BoundBox(dim);
		for (TiXmlElement  * rect=box->FirstChildElement("rect"); rect; rect=rect->NextSiblingElement("rect")){
			vec2 rDim;
			pos=0;
			in = rect->Attribute("S");
			for (int i=0;i<2;i++){
				pos=in.find(d);
				stringstream(in.substr(0,pos)) >> rDim[i];
				in.erase(0,pos+d.length());
			}
			recVec.emplace_back(rDim);
		}
	}
	Collider c(bb);
	for (vector<BoundRect>::iterator it=recVec.begin(); it!=recVec.end(); it++)
		c.addSub(*it);
//	c.scale(vec3(600,600,2));
	return c;
}

unique_ptr<Entity> loadEntFromFile(EntInfo eI, JShader& shader){
	string fileName = "res/"+eI.name+"/"+eI.name+".xml";
	TiXmlDocument doc(fileName);
   if (!doc.LoadFile()){
      cout << "Unable to load Graph File " << eI.name << endl;
      return nullptr;
   }

   TiXmlHandle h(&doc);
	TiXmlElement * skeleton, * collider;
	skeleton = h.FirstChild("Entity").FirstChild("Skeleton").ToElement();
	collider = h.FirstChild("Entity").FirstChild("Collider").ToElement();
	if (skeleton && collider){
		Skeleton s = getSkeleton(skeleton, shader);
		mat4 MV = glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale);
		s.getRoot()->leftMultMV(MV);
		Collider c = getCollider(collider);
		c.scale(glm::abs(eI.scale));
		c.translate(eI.translate);
		unique_ptr<Entity> ret(new Entity(c,move(s)));//eI.translate, eI.scale, move(s)));
		return ret;
	}

	cout << "Invalid XML File" << endl;
	return nullptr;
}

unique_ptr<Player> initPlayer(EntInfo eI, JShader& shader){
	unique_ptr<Player> other(new Player(eI.translate, eI.scale));
	unique_ptr<Player> playerPtr(new Player(loadEntFromFile(eI, shader).get()));
	playerPtr.get()->setRig();
	return playerPtr;
}

unique_ptr<Obstacle> initObstacle(EntInfo eI, JShader& shader){ 
	const string name = "dr";
	Skeleton s;
	s.add(name,move(unique_ptr<Drawable>(new Drawable(initCube(shader)))));
	s.setRoot(s[name]);
	Collider c(BoundBox(eI.scale));
	mat4 MV = glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale);
	s[name]->leftMultMV(MV);
	unique_ptr<Obstacle> obs(new Obstacle(new Entity(c, move(s))));
	obs.get()->translate(eI.translate);	
/*
	unique_ptr<Obstacle> obs(new Obstacle(eI.translate, eI.scale));
	Drawable dr = initCube(shader);
	mat4 MV = glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale);
	dr.leftMultMV(MV);
   //dr.setColor(eI.color);
	obs.get()->addDrawable(unique_ptr<Drawable>(new Drawable(dr)));
*/
	return obs;
}

unique_ptr<ActiveEnt> initAe(EntInfo eI, JShader& shader){
	Skeleton s;
	s.add("dr",unique_ptr<Drawable>(new Drawable(initCube(shader))));
	s.setRoot(s["dr"]);
	Collider c(BoundBox(eI.scale));
	unique_ptr<ActiveEnt> aE(new ActiveEnt(new Entity(c, move(s))));
	mat4 MV = glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale);
	s["dr"]->leftMultMV(MV);
	aE.get()->translate(eI.translate);	
  /*
	unique_ptr<ActiveEnt> aE(new ActiveEnt(eI.translate, eI.scale));
	Drawable dr = initCube(shader);
	mat4 MV = glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale);
	dr.leftMultMV(MV);
   dr.setColor(eI.color);
	aE.get()->addDrawable(unique_ptr<Drawable>(new Drawable(dr)));
	*/
	return aE;
}

unique_ptr<Wall> initWall(EntInfo eI, char orientation, JShader& shader){
	const string name = "dr";
	Skeleton s;
	mat4 MV = glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale);
	s.add(name,unique_ptr<Drawable>(new Drawable(initQuad(shader))));
	s.setRoot(s[name]);
	s[name]->leftMultMV(MV);
	s[name]->setColor(eI.color);
	unique_ptr<Wall> w(new Wall(eI.translate, eI.scale, orientation));
	w.get()->setSkeleton(move(s));

/*	
	
	Collider c(BoundBox(eI.scale));
	mat4 MV = glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale);
	s[name]->leftMultMV(MV);
	unique_ptr<Wall> w(new Wall(new Entity(c, move(s))));
	w.get()->translate(eI.translate);	

	unique_ptr<Wall> w(new Wall(eI.translate, eI.scale, orientation));
	Drawable dr = initQuad(shader);
	Skeleton s;
	mat4 MV = glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale);
	dr.leftMultMV(MV);
   dr.setColor(eI.color);
	w.get()->addDrawable(unique_ptr<Drawable>(new Drawable(dr)));
*/	
	return w;
}
