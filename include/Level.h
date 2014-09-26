#ifndef LEVEL_H
#define LEVEL_H

#include "JShader.h"
#include "Population.h"
//#include "Geom.h"
#include "Soup.h"
#include "Wall.h"
/*
typedef struct EntInfo{
	vec3 translate;
	vec4 rotate;
	vec3 scale;
	vec3 color;
} EntInfo;
*/
unique_ptr<Population> initLevel(JShader& shader, int level);
unique_ptr<Population> initLevelOne(JShader& shader);
void initWalls(vec3 min, vec3 max, Population * pop, JShader& shader);
/*
unique_ptr<Player> initPlayer(EntInfo eI, JShader& shader);
unique_ptr<Obstacle> initObstacle(EntInfo eI, JShader& shader);
unique_ptr<ActiveEnt> initAe(EntInfo eI, JShader& shader);
unique_ptr<Wall> initWall(EntInfo eI, char orientation, JShader& shader);
unique_ptr<Population> initLevel(JShader& shader);
//unique_ptr<Population> initLevel(JShader& shader, int level);
//unique_ptr<Population> initLevelOne(JShader& shader);
*/
#endif
