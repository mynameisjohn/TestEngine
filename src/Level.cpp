#include "Level.h"
#include "Rig.h"
#include <glm/gtx/transform.hpp> 

unique_ptr<Population> initLevel(JShader& shader){
	const vec3 wallMin(-1000, -600, -1000);
	const vec3 wallMax(7000, 2000, -4000);

	unique_ptr<Population> pop(new Population(
	initPlayer({vec3(0, 999, -2000),   //translate
					vec4(1, 0, 0, 0.f),    //rotate
					vec3(300, 400, -2),  //scale
					vec3(1,1,1) //color
				  }, shader)
	));
	
	pop->addObs(initObstacle(
				  {vec3(1500, -600, -2500),   //translate
					vec4(1, 0, 0, 0.f),    //rotate
					vec3(400, 400, -400),  //scale
					vec3(0.2, 0.4f, 0.8f) //color
				  }, shader));
	
	pop->addObs(initObstacle(
				  {vec3(4500, -600, -2500),   //translate
					vec4(1, 0, 0, 0.f),    //rotate
					vec3(400, 400, -400),  //scale
					vec3(0.2, 0.4f, 0.8f) //color
				  }, shader));
/*	
	pop->addActiveEnt(initAe(
				  {vec3(6000, 999, -3500),   //translate
					vec4(1, 0, 0, 0.f),    //rotate
					vec3(400, 400, -400),  //scale
					vec3(0.8f, 0.2f, 0.4f) //color
				  }, shader));
	
	pop->addActiveEnt(initAe(
				  {vec3(4000, 999, -2500),   //translate
					vec4(1, 0, 0, 0.f),    //rotate
					vec3(400, 400, -400),  //scale
					vec3(0.8f, 0.2f, 0.4f) //color
				  }, shader));
*/	
	//This doesn't make a front wall, so you can go over the edge
	initWalls(wallMin, wallMax, pop.get(), shader);
	
	return pop;
}

unique_ptr<Player> initPlayer(EntInfo eI, JShader& shader){
	unique_ptr<Player> playerPtr(new Player(eI.translate, vec3(0.5f,1.6,1)*eI.scale));
	Drawable dr = initPolyFromSVG("coat.svg",shader);
	Rig r = initRigFromSVG("drawing.svg", shader);

	mat4 MV = glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale);
	r.setMV(MV);
   playerPtr.get()->addDrawable((unique_ptr<Rig>(new Rig(r))));
	MV = glm::translate(vec3(.02f,0,-.1f));
	r.setMV(MV);
	playerPtr.get()->addDrawable((unique_ptr<Rig>(new Rig(r))));

	MV = glm::translate(vec3(-.02f,.65f,-.2f));
	dr.setMV(MV);
	playerPtr.get()->addDrawable((unique_ptr<Drawable>(new Drawable(dr))));
	
	MV = glm::translate(vec3(.1f,0,-.3f))*glm::scale(vec3(0.8,0.8,1));
	r = initRigFromSVG("arm.svg", shader);
	r.setMV(MV);
	playerPtr.get()->addDrawable((unique_ptr<Rig>(new Rig(r))));
/*
	r = initRigFromSVG("drawing.svg", shader);
	r.setMV(MV);
	playerPtr.get()->addDrawable((unique_ptr<Rig>(new Rig(r))));
*/	

	playerPtr.get()->setRig();
	return playerPtr;
}

unique_ptr<Obstacle> initObstacle(EntInfo eI, JShader& shader){ 
	unique_ptr<Obstacle> obs(new Obstacle(eI.translate, eI.scale));
	Drawable dr = initCube(shader);
	mat4 MV = glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale);
	dr.setMV(MV);
   dr.setColor(eI.color);
	obs.get()->addDrawable(unique_ptr<Drawable>(new Drawable(dr)));
	return obs;
}

unique_ptr<ActiveEnt> initAe(EntInfo eI, JShader& shader){
   unique_ptr<ActiveEnt> aE(new ActiveEnt(eI.translate, eI.scale));
	Drawable dr = initCube(shader);
	mat4 MV = glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale);
	dr.setMV(MV);
   dr.setColor(eI.color);
	aE.get()->addDrawable(unique_ptr<Drawable>(new Drawable(dr)));
	
	return aE;
}

unique_ptr<Wall> initWall(EntInfo eI, char orientation, JShader& shader){
	unique_ptr<Wall> w(new Wall(eI.translate, eI.scale, orientation));
	Drawable dr = initQuad(shader);
	mat4 MV = glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale);
	dr.setMV(MV);
   dr.setColor(eI.color);
	w.get()->addDrawable(unique_ptr<Drawable>(new Drawable(dr)));
	
	return w;
}

//Sort of a black box at the moment...
void initWalls(vec3 min, vec3 max, Population * pop, JShader& shader){
	float thickness = 2.f;

	//floor
	pop->addObs(initWall(
				  {min,//vec3(-1000, -600, -2000),   //translate
					vec4(1, 0, 0, M_PI/2.f),    //rotate
					vec3(max.x-min.x, max.z-min.z, -thickness),  //scale
					vec3(1,1,1) //color
				  }, 'y', shader));

	//back wall	
	pop->addObs(initWall(
				  {vec3(min.x, min.y, max.z),//, -600, -4000),   //translate
					vec4(1, 0, 0, 0.f),    //rotate
					vec3(max.x-min.x, max.y-min.y, -thickness),  //scale
					vec3(1,1,1) //color
				  }, 'z', shader));
	
	//Left Wall
	pop->addObs(initWall(
				  {min,//vec3(-1000, -600, -2000),   //translate
					vec4(0, 1, 0, M_PI/2.f),    //rotate
					vec3(min.z-max.z, max.y-min.y, -thickness),  //scale
					vec3(1,1,1) //color
				  }, 'x', shader));
	
	//Right Wall
	pop->addObs(initWall(
				  {vec3(max.x, min.y, min.z),//7000, -600, -2000),   //translate
					vec4(0, 1, 0, M_PI/2.f),    //rotate
					vec3(min.z-max.z, max.y-min.y, -thickness),  //scale
					vec3(1,1,1) //color
				  }, 'x', shader));
}

