#include "Level.h"
#include "Rig.h"
#include <glm/gtx/transform.hpp> 

unique_ptr<Population> initLevel(JShader& shader, int level){
	return initLevelOne(shader);
}


unique_ptr<Population> initLevelOne(JShader& shader){
	const vec3 wallMin(-1000, -600, -1000);
   const vec3 wallMax(7000, 2000, -4000);

	unique_ptr<Population> pop(new Population(
   initPlayer({"girl",
					vec3(0, 999, -2000),   //translate
               vec4(1, 0, 0, 0.f),    //rotate
               vec3(600, 600, -2),  //scale
               vec3(1,1,1) //color
              }, shader)
   ));

   pop->addObs(initObstacle(
              {"",
               vec3(1500, -600, -2500),   //translate
               vec4(1, 0, 0, 0.f),    //rotate
               vec3(400, 400, -400),  //scale
               vec3(0.2, 0.4f, 0.8f) //color
              }, shader));

   pop->addObs(initObstacle(
              {"",
               vec3(4500, -600, -2500),   //translate
               vec4(1, 0, 0, 0.f),    //rotate
               vec3(400, 400, -400),  //scale
               vec3(0.2, 0.4f, 0.8f) //color
              }, shader));
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
					vec3(1,1,1) //color
				  }, 'y', shader));

	//back wall	
	pop->addObs(initWall(
				  {"",
               vec3(min.x, min.y, max.z), //translate
					vec4(1, 0, 0, 0.f),    //rotate
					vec3(max.x-min.x, max.y-min.y, -thickness),  //scale
					vec3(1,0,1) //color
				  }, 'z', shader));
	
	//Left Wall
	pop->addObs(initWall(
				  {"",
               min,  //translate
					vec4(0, 1, 0, M_PI/2.f),    //rotate
					vec3(min.z-max.z, max.y-min.y, -thickness),  //scale
					vec3(1,1,0) //color
				  }, 'x', shader));
	
	//Right Wall
	pop->addObs(initWall(
				  {"",
               vec3(max.x, min.y, min.z), //translate
					vec4(0, 1, 0, M_PI/2.f),    //rotate
					vec3(min.z-max.z, max.y-min.y, -thickness),  //scale
					vec3(0,1,1) //color
				  }, 'x', shader));
}
