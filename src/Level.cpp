#include "Level.h"
#include "Rig.h"
#include <glm/gtx/transform.hpp> 
/*
unique_ptr<Population> initLevel(JShader& shader, int level){
	return initLevelOne(shader);
}

//bug: if AE spawns <= WallMin.y, it falls through
unique_ptr<Population> initLevelOne(JShader& shader){
	const vec3 wallMin(-1000, -600, -1000);
   const vec3 wallMax(7000, 2000, -4000);

	unique_ptr<Population> pop(new Population(
   initPlayer({"girl",
					vec3(0, -500, -2000),   //translate
               vec4(1, 0, 0, 0.f),    //rotate
               vec3(600, 600, -2),  //scale
               vec3(1,1,1), //color
					"stand",10
              }, shader)
   ));

  
   pop->addObs(initObstacle(
              {"",//this will change soon
               vec3(1500, -600, -2500),   //translate
               vec4(1, 0, 0, 0.f),    //rotate
               vec3(400, 400, -400),  //scale
               vec3(0.2, 0.4f, 0.8f), //color
					"",-1
              }, shader));

   pop->addObs(initObstacle(
              {"",
               vec3(4500, -600, -2500),   //translate
               vec4(1, 0, 0, 0.f),    //rotate
               vec3(400, 400, -400),  //scale
               vec3(0.2, 0.4f, 0.8f), //color
					"",-1
              }, shader));
/*
	pop->addSeeker(initSeeker(
              {"",
               vec3(2000, 500, -2500),   //translate
               vec4(1, 0, 0, 0.f),    //rotate
               vec3(400, 900, -400),  //scale
               vec3(0.8, 0.4f, 0.2f), //color
					"",-1
              }, shader));
   
	pop->addSeeker(initSeeker(
              {"",
               vec3(6500, -500, -2500),   //translate
               vec4(1, 0, 0, 0.f),    //rotate
               vec3(400, 400, -400),  //scale
               vec3(0.8, 0.4f, 0.2f), //color
					"",-1
              }, shader));
*/
/*
	initWalls(wallMin, wallMax, pop.get(), shader);


	return pop;
}

//Sort of a black box at the moment...
void initWalls(vec3 min, vec3 max, Population * pop, JShader& shader){
	float thickness = 2.f;

	//floor
	pop->addObs(initWall(
				  {"",
               min,   //translate
					vec4(1, 0, 0, M_PI/2.f),    //rotate
					vec3(max.x-min.x, max.z-min.z, -thickness),  //scale
					vec3(1,1,1), //color
					"",-1
				  }, 'y', shader));

	//back wall	
	pop->addObs(initWall(
				  {"",
               vec3(min.x, min.y, max.z), //translate
					vec4(1, 0, 0, 0.f),    //rotate
					vec3(max.x-min.x, max.y-min.y, -thickness),  //scale
					vec3(1,.4,.5), //color
					"",-1
				  }, 'z', shader));
	
	//Left Wall
	pop->addObs(initWall(
				  {"",
               min,  //translate
					vec4(0, 1, 0, M_PI/2.f),    //rotate
					vec3(min.z-max.z, max.y-min.y, -thickness),  //scale
					vec3(1,1,0), //color
					"",-1
				  }, 'x', shader));
	
	//Right Wall
	pop->addObs(initWall(
				  {"",
               vec3(max.x, min.y, min.z), //translate
					vec4(0, 1, 0, M_PI/2.f),    //rotate
					vec3(min.z-max.z, max.y-min.y, -thickness),  //scale
					vec3(0,1,1), //color
					"",-1
				  }, 'x', shader));
}
*/
Level::Level()
: s(&dMap), activePop(0) {
	popVec.resize(1);
}

unique_ptr<Population> Level::initLevelOne(JShader& shader){
	const vec3 wallMin(-1000, -600, -1000);
   const vec3 wallMax(7000, 2000, -4000);

	unique_ptr<Population> pop(new Population(
   s.initPlayer({"girl",
					vec3(0, -500, -2000),   //translate
               vec4(1, 0, 0, 0.f),    //rotate
               vec3(600, 600, -2),  //scale
               vec3(1,1,1), //color
					"stand",10
              }, shader)
   ));
  
   pop->addObs(s.initObstacle(
              {"obs",//this will change soon
               vec3(1500, -600, -2500),   //translate
               vec4(1, 0, 0, 0.f),    //rotate
               vec3(400, 400, -400),  //scale
               vec3(0.2, 0.4f, 0.8f), //color
					"",-1
              }, shader));

   pop->addObs(s.initObstacle(
              {"obs",
               vec3(4500, -600, -2500),   //translate
               vec4(1, 0, 0, 0.f),    //rotate
               vec3(400, 400, -400),  //scale
               vec3(0.2, 0.4f, 0.8f), //color
					"",-1
              }, shader));
/*
	pop->addSeeker(initSeeker(
              {"",
               vec3(2000, 500, -2500),   //translate
               vec4(1, 0, 0, 0.f),    //rotate
               vec3(400, 900, -400),  //scale
               vec3(0.8, 0.4f, 0.2f), //color
					"",-1
              }, shader));
   
	pop->addSeeker(initSeeker(
              {"",
               vec3(6500, -500, -2500),   //translate
               vec4(1, 0, 0, 0.f),    //rotate
               vec3(400, 400, -400),  //scale
               vec3(0.8, 0.4f, 0.2f), //color
					"",-1
              }, shader));
*/

	initWalls(wallMin, wallMax, pop.get(), shader);

	return pop;
}

//Sort of a black box at the moment...
void Level::initWalls(vec3 min, vec3 max, Population * pop, JShader& shader){
	float thickness = 2.f;

	//floor
	pop->addObs(s.initWall(
				  {"floor",
               min,   //translate
					vec4(1, 0, 0, M_PI/2.f),    //rotate
					vec3(max.x-min.x, max.z-min.z, -thickness),  //scale
					vec3(1,1,1), //color
					"",-1
				  }, 'y', shader));

	//back wall	
	pop->addObs(s.initWall(
				  {"back wall",
               vec3(min.x, min.y, max.z), //translate
					vec4(1, 0, 0, 0.f),    //rotate
					vec3(max.x-min.x, max.y-min.y, -thickness),  //scale
					vec3(1,.4,.5), //color
					"",-1
				  }, 'z', shader));
	
	//Left Wall
	pop->addObs(s.initWall(
				  {"left wall",
               min,  //translate
					vec4(0, 1, 0, M_PI/2.f),    //rotate
					vec3(min.z-max.z, max.y-min.y, -thickness),  //scale
					vec3(1,1,0), //color
					"",-1
				  }, 'x', shader));
	
	//Right Wall
	pop->addObs(s.initWall(
				  {"right wall",
               vec3(max.x, min.y, min.z), //translate
					vec4(0, 1, 0, M_PI/2.f),    //rotate
					vec3(min.z-max.z, max.y-min.y, -thickness),  //scale
					vec3(0,1,1), //color
					"",-1
				  }, 'x', shader));
}

//this was segfaulting, and now it isn't. be careful
Level::Level(int l, JShader& shader)
: Level(){
//this is bogus for now
	popVec[0] = std::move(initLevelOne(shader));
//	cout << popVec[0]->getPlayer() << endl;
}

vec4 Level::move(){
	return popVec[activePop]->move();
}

void Level::update(){
	popVec[activePop]->update();
}

void Level::draw(){
	popVec[activePop]->draw();
}

Player * Level::getPlayer(){
	return popVec[activePop]->getPlayer();
}
