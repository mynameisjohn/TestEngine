#include "Level.h"
#include <glm/gtx/transform.hpp> 

std::unique_ptr<Population> initLevel(JShader& shader){
	const vec3 wallMin(-1000, -600, -2000);
	const vec3 wallMax(7000, 2000, -4000);

	std::unique_ptr<Population> pop(new Population(
	initPlayer({vec3(0, 999, -3200),   //translate
					vec4(1, 0, 0, 0.f),    //rotate
					vec3(400, 400, -400),  //scale
					vec3(0.2, 0.4f, 0.8f) //color
				  }, shader)
	));
	
	pop->addObs(initObstacle(
				  {vec3(1500, -600, -2500),   //translate
					vec4(1, 0, 0, 0.f),    //rotate
					vec3(400, 400, -400),  //scale
					vec3(0.2, 0.4f, 0.8f) //color
				  }, shader));
	
	pop->addObs(initObstacle(
				  {vec3(500, -600, -2500),   //translate
					vec4(1, 0, 0, 0.f),    //rotate
					vec3(400, 400, -400),  //scale
					vec3(0.2, 0.4f, 0.8f) //color
				  }, shader));
	
	pop->addActiveEnt(initAe(
				  {vec3(1000, -600, -3500),   //translate
					vec4(1, 0, 0, 0.f),    //rotate
					vec3(400, 400, -400),  //scale
					vec3(0.2f, 0.4f, 0.8f) //color
				  }, shader));
	
	//This doesn't make a front wall, so you can go over the edge
	initWalls(wallMin, wallMax, pop.get(), shader);
	
	return pop;
}

std::unique_ptr<Player> initPlayer(EntInfo eI, JShader& shader){
	std::unique_ptr<Player> playerPtr(new Player(eI.translate, eI.scale));
	Drawable dr = initCube(shader);
	mat4 MV = glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale);
	dr.setMV(MV);
   dr.setColor(eI.color);
	playerPtr.get()->addDrawable(dr);

	return playerPtr;
}

std::unique_ptr<Obstacle> initObstacle(EntInfo eI, JShader& shader){ 
	Obstacle obs(eI.translate, eI.scale);
	Drawable dr = initCube(shader);
	mat4 MV = glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale);
	dr.setMV(MV);
   dr.setColor(eI.color);
	obs.addDrawable(dr);
	
	return std::unique_ptr<Obstacle>(new Obstacle(obs));
}

std::unique_ptr<ActiveEnt> initAe(EntInfo eI, JShader& shader){
   ActiveEnt aE(eI.translate, eI.scale);
	Drawable dr = initCube(shader);
	mat4 MV = glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale);
	dr.setMV(MV);
   dr.setColor(eI.color);
	aE.addDrawable(dr);
	
	return std::unique_ptr<ActiveEnt>(new ActiveEnt(aE));
}

std::unique_ptr<Wall> initWall(EntInfo eI, char orientation, JShader& shader){
	Wall w(eI.translate, eI.scale, orientation);
	Drawable dr = initQuad(shader);
	mat4 MV = glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale);
	dr.setMV(MV);
   dr.setColor(eI.color);
	w.addDrawable(dr);
	
	return std::unique_ptr<Wall>(new Wall(w));
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
