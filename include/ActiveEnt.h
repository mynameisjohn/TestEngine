#ifndef ACTIVEENT_H
#define ACTIVEENT_H

#include "Entity.h"
#include <stdio.h>

class ActiveEnt: public Entity{
	public:
		ActiveEnt();
		ActiveEnt(vec3 translate, vec3 scale);
		ActiveEnt(Collider c);
		void update(vec3 a);//NYI
		void moveWRT_ent(Entity * e);
		void moveToEdge(int, Entity *);
		virtual void move();//Make this pure asap
		vec3 getVel();
	protected:
		vec3 mVel; //Active velocity
		vec3 mSpeed; //Individual speed
		bool grounded; //Whether it's on top of something
		float speedOsc;
};

#endif
