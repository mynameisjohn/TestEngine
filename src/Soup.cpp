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
	TiXmlDocument doc(eI.fileName);
   if (!doc.LoadFile()){
      cout << "Unable to load Graph File " << eI.fileName << endl;
      return nullptr;
   }

   TiXmlHandle h(&doc);
	TiXmlElement * skeleton, * collider;
	skeleton = h.FirstChild("Entity").FirstChild("Skeleton").ToElement();
	collider = h.FirstChild("Entity").FirstChild("Collider").ToElement();
	if (skeleton && collider){
		Skeleton s = getSkeleton(skeleton, shader);
		Collider c = getCollider(collider);
		unique_ptr<Entity> ret(new Entity(c,move(s)));//eI.translate, eI.scale, move(s)));
		return ret;
	}

	cout << "Invalid XML File" << endl;
	return nullptr;
}

unique_ptr<Player> initPlayer(EntInfo eI, JShader& shader){
	unique_ptr<Player> playerPtr(new Player(eI.translate, eI.scale));
	unique_ptr<Player> other(new Player(loadEntFromFile(eI, shader).get()));
	cout << eI.scale << endl;
	Drawable dr = initTexQuad("res/coat.png", shader);//("coat.svg",shader);
	Rig r = initRigFromSVG("res/drawing.svg", shader);

	mat4 MV = glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale);
	r.leftMultMV(MV);
   playerPtr.get()->addDrawable((unique_ptr<Rig>(new Rig(r))));

	r = initRigFromSVG("res/drawing.svg", shader);
	MV = glm::translate(vec3(-0.2, -0.923909,-.1f));
	r.leftMultMV(MV);
	playerPtr.get()->addDrawable((unique_ptr<Rig>(new Rig(r))));

	MV = glm::translate(vec3(-.22f,-.1,-.2f))*glm::scale(vec3(0.44f,.84f,1));
	dr.leftMultMV(MV);
	playerPtr.get()->addDrawable((unique_ptr<Drawable>(new Drawable(dr))));
	
	MV = glm::translate(vec3(.1f,0,-.3f))*glm::scale(vec3(0.8,0.8,1));
	r = initRigFromSVG("res/arm.svg", shader);
	r.leftMultMV(MV);
	playerPtr.get()->addDrawable((unique_ptr<Rig>(new Rig(r))));
	
	MV = glm::translate(vec3(-.03,.95f,.02f))*glm::scale(vec3(0.4,0.4,1));
	dr = initTexQuad("res/face3.png", shader);
	dr.leftMultMV(MV);
   playerPtr.get()->addDrawable((unique_ptr<Drawable>(new Drawable(dr))));

	playerPtr.get()->setRig();
	return playerPtr;
}

unique_ptr<Obstacle> initObstacle(EntInfo eI, JShader& shader){ 
	unique_ptr<Obstacle> obs(new Obstacle(eI.translate, eI.scale));
	Drawable dr = initCube(shader);
	mat4 MV = glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale);
	dr.leftMultMV(MV);
   //dr.setColor(eI.color);
	obs.get()->addDrawable(unique_ptr<Drawable>(new Drawable(dr)));
	return obs;
}

unique_ptr<ActiveEnt> initAe(EntInfo eI, JShader& shader){
   unique_ptr<ActiveEnt> aE(new ActiveEnt(eI.translate, eI.scale));
	Drawable dr = initCube(shader);
	mat4 MV = glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale);
	dr.leftMultMV(MV);
   dr.setColor(eI.color);
	aE.get()->addDrawable(unique_ptr<Drawable>(new Drawable(dr)));
	
	return aE;
}

unique_ptr<Wall> initWall(EntInfo eI, char orientation, JShader& shader){
	unique_ptr<Wall> w(new Wall(eI.translate, eI.scale, orientation));
	Drawable dr = initQuad(shader);
	mat4 MV = glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale);
	dr.leftMultMV(MV);
   dr.setColor(eI.color);
	w.get()->addDrawable(unique_ptr<Drawable>(new Drawable(dr)));
	
	return w;
}
