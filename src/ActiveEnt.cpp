#include "ActiveEnt.h"

ActiveEnt::ActiveEnt() : Entity(){
	mVel = vec3();
	mSpeed = vec3(40.f, 40.f, 20.12241f);
	speedOsc=0.f;
	grounded=false;
}

ActiveEnt::ActiveEnt(vec3 translate, vec3 scale) : Entity(translate, scale){
   mVel = vec3();
   mSpeed = vec3(40.f, 40.f, 20.12241f);
   speedOsc=0.f;
   grounded=false;
	mCollider.addSub(BoundRect(vec2(scale)));
}

ActiveEnt::ActiveEnt(Collider c) : Entity(c){
	//NYI
}

void ActiveEnt::update(vec3 target){
	float s = 5000.f, A = 10000.f;
	float y = mVel.y;
	vec3 dt;
	s = pow(s,2);
	target = getPos() - target;
	target = A * -2.f * (target/s) * glm::exp((-glm::pow(target, vec3(2,2,2)))/s);
	dt = vec3(mVel.x * target.x < 0.f ? 5.f : 0.5f, 0.f, mVel.z * target.z < 0.f ? 5.f : 0.5f);
	mVel += dt * target;
	mVel = glm::clamp(mVel, -mSpeed, mSpeed);

	std::cout << mVel << std::endl;
	
	if (grounded)
		mVel.y=0;
	else
		mVel.y -= 2.f;
}

void ActiveEnt::moveWRT_walls(){
	grounded=false;
	translate(mVel);/*
	grounded=false;
	mTrans = mCollider.move(mVel);
*/
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
