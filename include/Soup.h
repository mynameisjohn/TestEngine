#ifndef SOUP_H
#define SOUP_H

#include "Player.h"
#include "Wall.h"

typedef struct EntInfo{
	string name;
   vec3 translate;
   vec4 rotate;
   vec3 scale;
   vec3 color;
} EntInfo;

unique_ptr<Player> initPlayer(EntInfo eI, JShader& shader);
unique_ptr<Obstacle> initObstacle(EntInfo eI, JShader& shader);
unique_ptr<ActiveEnt> initAe(EntInfo eI, JShader& shader);
unique_ptr<Wall> initWall(EntInfo eI, char orientation, JShader& shader);


#endif
