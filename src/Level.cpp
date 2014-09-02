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
	
/*
	glm::mat4 MV;
	vec3 color;
	
	//Note the negative Z scale...never got around that	
	MV = glm::translate(vec3(0, 999, -2500)) *
		  glm::scale(vec3(400, 400, -400));
	color = vec3(0.2, 0.4f, 0.8f);
	initPlayer(MV, color, shader, pop->getPlayer());
	
	MV = glm::translate(vec3(1500, -600, -2500)) *
		  glm::scale(vec3(400, 400, -400));
	color = vec3(0.4f, 0.8f, 0.2f);
	pop->addObs(initObstacle(MV, color, shader));
 
	MV = glm::translate(vec3(500, -600, -2500)) *
		  glm::scale(vec3(400, 400, -400));
	pop->addObs(initObstacle(MV, color, shader));
	
	MV = glm::translate(vec3(1000, -600, -3500)) *
		  glm::scale(vec3(400, 400, -400));
	color = vec3(0.8f, 0.4f, 0.2f);
	pop->addActiveEnt(initAe(MV, color, shader));
	
	color=vec3(1.f, 1.f, 1.f);
	
	//floor...
	MV = glm::translate(glm::vec3(-1000, -600, -2000)) * 
		  glm::rotate((float)(M_PI/2.f), glm::vec3(1, 0, 0)) * 
		  glm::scale(glm::vec3(8000, -2000, -2));
	pop->addObs(initWall(MV, color, shader));
	
	//back wall
	MV = glm::translate(glm::vec3(-1000, -600, -4000)) * 
		  glm::scale(glm::vec3(8000, 2500, -2));
	pop->addObs(initWall(MV, color, shader));

	MV = glm::translate(glm::vec3(-1000, -600, -2000)) * 
		  glm::rotate((float)(M_PI/2.f), glm::vec3(0, 1, 0)) * 
		  glm::scale(glm::vec3(2000, 2500, -200));
	pop->addObs(initWall(MV, color, shader));

	MV = glm::translate(glm::vec3(7000, -600, -2000)) * 
		  glm::rotate((float)(M_PI/2.f), glm::vec3(0, 1, 0)) * 
		  glm::scale(glm::vec3(2000, 2500, -2));
	pop->addObs(initWall(MV, color, shader));
*/
	return pop;
}

std::unique_ptr<Player> initPlayer(EntInfo eI, JShader& shader){//glm::mat4 MV, vec3 color, JShader& shader, Player * playerPtr){
	std::unique_ptr<Player> playerPtr(new Player(eI.translate, eI.scale));
/*
	Drawable dr;
	BoundBox bb;
	Collider c;
	BoundRect rect;

	vec3 translate = vec3(MV * glm::vec4(0, 0, 0, 1));
	vec3 scale = (vec3(MV * glm::vec4(1, 1, 1, 1)) - translate);

   rect = BoundRect(vec2(scale));
	bb = BoundBox(scale);
   c=Collider(wallMin, wallMax, bb);
   c.addSub(rect);
   playerPtr->setCol(c);
	playerPtr->translate(translate);
*/	
	Drawable dr = initCube(shader);
	mat4 MV = glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale);
	dr.setMV(MV);
   dr.setColor(eI.color);
	playerPtr.get()->addDrawable(dr);

	return playerPtr;
}

std::unique_ptr<Obstacle> initObstacle(EntInfo eI, JShader& shader){ //(glm::mat4 MV, vec3 color, JShader& shader){
	Obstacle obs(eI.translate, eI.scale);
/*
	Drawable dr;
	BoundBox bb;
	Collider c;
	BoundRect rect;

	vec3 translate = vec3(MV * glm::vec4(0, 0, 0, 1));
	vec3 scale = (vec3(MV * glm::vec4(1, 1, 1, 1)) - translate);

   rect = BoundRect(vec2(scale));//0, 0, 40, 20);
	bb = BoundBox(scale);
   c=Collider(wallMin, wallMax, bb);
   c.addSub(rect);
	obs.setCol(c);
	obs.translate(translate);
*/
	Drawable dr = initCube(shader);
	mat4 MV = glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale);
	dr.setMV(MV);
   dr.setColor(eI.color);
	obs.addDrawable(dr);
	
	return std::unique_ptr<Obstacle>(new Obstacle(obs));
}

std::unique_ptr<ActiveEnt> initAe(EntInfo eI, JShader& shader){//glm::mat4 MV, vec3 color, JShader& shader){
   ActiveEnt aE(eI.translate, eI.scale);
/*	Drawable dr;
	BoundBox bb;
	Collider c;
	BoundRect rect;

	vec3 translate = vec3(MV * glm::vec4(0, 0, 0, 1));
	vec3 scale = (vec3(MV * glm::vec4(1, 1, 1, 1)) - translate);

   rect = BoundRect(vec2(scale));//0, 0, 40, 20);
	bb = BoundBox(scale);
   c=Collider(wallMin, wallMax, bb);
   c.addSub(rect);
	aE.setCol(c);
	aE.translate(translate);
*/	
	Drawable dr = initCube(shader);
	mat4 MV = glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale);
	dr.setMV(MV);
   dr.setColor(eI.color);
	aE.addDrawable(dr);
/*	
	dr = initCube(MV, shader);
   dr.setColor(color);
	dr.setMV(glm::scale(scale));
	playerPtr->addDrawable(dr);
	dr = initCube(glm::scale(scale), shader);
   dr.setColor(color);
	aE.addDrawable(dr);
*/	
	return std::unique_ptr<ActiveEnt>(new ActiveEnt(aE));
}

std::unique_ptr<Wall> initWall(EntInfo eI, char orientation, JShader& shader){
//	Obstacle obs(eI.translate, eI.scale);
	Wall w(eI.translate, eI.scale, orientation);
/*
	Drawable dr;
	BoundBox bb;
	Collider c;
	BoundRect rect;

	vec3 translate = vec3(MV * glm::vec4(0, 0, 0, 1));
	vec3 scale = (vec3(MV * glm::vec4(1, 1, 1, 1)) - translate);

   rect = BoundRect(vec2(0,0), vec2(40, 20));
	bb = BoundBox(scale);
   c=Collider(wallMin, wallMax, bb);
   c.addSub(rect);
   rect = BoundRect(vec2(0, 20), vec2(40, 20));
   c.addSub(rect);
	obs.setCol(c);
	obs.translate(translate);
*/	
	Drawable dr = initQuad(shader);
	mat4 MV = glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale);
	dr.setMV(MV);
   dr.setColor(eI.color);
	w.addDrawable(dr);
	
	return std::unique_ptr<Wall>(new Wall(w));
}

void initWalls(vec3 min, vec3 max, Population * pop, JShader& shader){
	float thickness = 2.f;
	//floor...
	pop->addObs(initWall(
				  {min,//vec3(-1000, -600, -2000),   //translate
					vec4(1, 0, 0, M_PI/2.f),    //rotate
					vec3(max.x-min.x, max.z-min.z, -thickness),  //scale
					vec3(1,1,1) //color
				  }, 'y', shader));
	
	pop->addObs(initWall(
				  {vec3(min.x, min.y, max.z),//, -600, -4000),   //translate
					vec4(1, 0, 0, 0.f),    //rotate
					vec3(max.x-min.x, max.y-min.y, -thickness),  //scale
					vec3(1,1,1) //color
				  }, 'z', shader));
	
	pop->addObs(initWall(
				  {min,//vec3(-1000, -600, -2000),   //translate
					vec4(0, 1, 0, M_PI/2.f),    //rotate
					vec3(min.z-max.z, max.y-min.y, -thickness),  //scale
					vec3(1,1,1) //color
				  }, 'x', shader));
	
	pop->addObs(initWall(
				  {vec3(max.x, min.y, min.z),//7000, -600, -2000),   //translate
					vec4(0, 1, 0, M_PI/2.f),    //rotate
					vec3(min.z-max.z, max.y-min.y, -thickness),  //scale
					vec3(1,1,1) //color
				  }, 'x', shader));
}
