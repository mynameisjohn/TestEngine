#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "Entity.h"

class Obstacle : public Entity{
public:
	Obstacle();
	Obstacle(Entity *);
	Obstacle(vec3 translate, vec3 scale);
	void update();
private:
	float colorOsc;
};

#endif
