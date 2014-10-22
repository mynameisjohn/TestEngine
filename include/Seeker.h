#ifndef SEEKER_H
#define SEEKER_H

#include "ActiveEnt.h"

class Seeker : public ActiveEnt{
public:
	Seeker();
	Seeker(Entity *);
	Seeker(vec3, vec3);
	void update(vec3);
	void takeHit(HitEffect eff);
};

#endif
