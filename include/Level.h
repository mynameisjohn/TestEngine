#ifndef LEVEL_H
#define LEVEL_H

#include "JShader.h"
#include "Population.h"
#include "Geom.h"
#include "Wall.h"

typedef struct EntInfo{
	vec3 translate;
	vec4 rotate;
	vec3 scale;
	vec3 color;
} EntInfo;

void initWalls(vec3 min, vec3 max, Population * pop, JShader& shader);
std::unique_ptr<Player> initPlayer(EntInfo eI, JShader& shader);
//glm::mat4 MV, vec3 color, JShader& shader, Player * playerPtr);
std::unique_ptr<Obstacle> initObstacle(EntInfo eI, JShader& shader);
//glm::mat4 MV, vec3 color, JShader& shader);
std::unique_ptr<ActiveEnt> initAe(EntInfo eI, JShader& shader);
//glm::mat4 MV, vec3 color, JShader& shader);
std::unique_ptr<Wall> initWall(EntInfo eI, char orientation, JShader& shader);
//glm::mat4 MV, vec3 color, JShader& shader);
std::unique_ptr<Population> initLevel(JShader& shader);

#endif
