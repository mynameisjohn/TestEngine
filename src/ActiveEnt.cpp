#include "ActiveEnt.h"
#include <glm/gtc/random.hpp>

ActiveEnt::ActiveEnt() : Entity(){
	mVel = vec3();
	mSpeed = vec3(50.f, 40.f, 30.f);
	speedOsc=0.f;
	grounded=false;
	A = 50000.f;
   sigmaSq = 100.f;
   sigmaSq *= sigmaSq;
}

ActiveEnt::ActiveEnt(vec3 translate, vec3 scale) : Entity(translate, scale){
   mVel = vec3();
	mSpeed = vec3(50.f, 40.f, 30.f);
   speedOsc=0.f;
   grounded=false;
	mCollider.addSub(BoundRect(vec2(scale)));
	A = 50000.f;
   sigmaSq = 200.f;
   sigmaSq *= sigmaSq;
}

ActiveEnt::ActiveEnt(Entity * frame) : Entity(frame){
   mVel = vec3();
	mSpeed = vec3(50.f, 40.f, 30.f);
   speedOsc=0.f;
   grounded=false;
	A = 50000.f;
   sigmaSq = 200.f;
   sigmaSq *= sigmaSq;
}

void ActiveEnt::update(vec3 a){
	if (grounded)
		mVel.y=0;
	else
		mVel.y -= 2.f;
	
	//a += glm::linearRand(-vec3(.25f,.25f,.25f), vec3(.25f,.25f,.25f));
	vec3 dt(mVel.x*a.x < 0.f ? 7.5f : 0.75f, 0.f, mVel.z*a.z < 0.f ? 7.5f : 0.75f);
	mVel = glm::clamp(mVel+dt*a, -mSpeed, mSpeed);

	//jump
	if (grounded && fabs(a.y) > 0.25f)
		mVel.y += mSpeed.y;
	
}

void ActiveEnt::move(){
	grounded=false;
	translate(mVel);
}

vec3 ActiveEnt::getVel(){
	return mVel;
}

/* The goal here is to find the direction in which the last collision
(bounding box violation) occurred, so that the entity can translate
right up against it.*/
void ActiveEnt::moveWRT_ent(Entity * e){
	char last = collidesWith(e);

	switch (last){
		case 'x':
			if (mVel.x > 0)
            translate(vec3(toLeft(e), 0, 0));
         else if (mVel.x < 0)
            translate(vec3(toRight(e), 0, 0));
			break;
	
		case 'y':
			if (mVel.y < 0){
            translate(vec3(0,toTop(e), 0));
				grounded = true;
			}
         else if (mVel.y > 0)
            translate(vec3(0, toBottom(e), 0));
			break;

		case 'z':
			if (mVel.z > 0)
            translate(vec3(0, 0, toFar(e)));
         else if (mVel.z < 0)
            translate(vec3(0, 0, toNear(e)));
			break;

		default:
			break;
	}	
}
