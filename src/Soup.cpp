#include "Soup.h"
#include "Textures.h"
#include "Closet.h"
#include <glm/gtx/transform.hpp> 

unique_ptr<Entity> loadEntFromFile(EntInfo eI, JShader& shader){
	TiXmlDocument doc(eI.fileName);
   if (!doc.LoadFile()){
      cout << "Unable to load Graph File " << eI.fileName << endl;
      return nullptr;
   }

   TiXmlHandle h(&doc);
   TiXmlElement * rootDrawable = h.FirstChild("drawable").ToElement();
	SceneGraph sg = initSceneGraph(rootDrawable, shader);

	unique_ptr<Entity> ret(new Entity(eI.translate, eI.scale, std::move(sg)));

	return ret;
}

unique_ptr<Player> initPlayer(EntInfo eI, JShader& shader){
	unique_ptr<Player> other(new Player(eI.translate, eI.scale));
	unique_ptr<Player> playerPtr(new Player(loadEntFromFile(eI, shader).get()));

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
