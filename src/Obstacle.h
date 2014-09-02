#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "Entity.h"

class Obstacle : public Entity{
public:
	Obstacle();
	Obstacle(vec3 translate, vec3 scale);
	void update();
private:
	float colorOsc;
	//I don't even know what this needs to be honest
};

#endif
