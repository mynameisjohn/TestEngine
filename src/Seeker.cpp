#include "Seeker.h"

Seeker::Seeker()
: ActiveEnt()
{}

Seeker::Seeker(const Entity& e)
: ActiveEnt(e) {}
/*
Seeker::Seeker(Entity * e)
: ActiveEnt(e)
{}
*/
Seeker::Seeker(vec3 translate, vec3 scale)
: ActiveEnt(translate, scale)
{}

void Seeker::update(vec3 goal){
	ActiveEnt::update();

	//a += glm::linearRand(-vec3(.25f,.25f,.25f), vec3(.25f,.25f,.25f));
   vec3 dt(mVel.x*goal.x < 0.f ? 7.5f : 0.75f, 0.f, mVel.z*goal.z < 0.f ? 7.5f : 0.75f);
   mVel = glm::clamp(mVel+dt*goal, -mSpeed, mSpeed);

   //jump
   if (grounded && fabs(goal.y) > 0.25f)
      mVel.y += mSpeed.y;
/*
	if (health > 0)
		mSkel.setColor(health/100.f,health/100.f,health/100.f);
	else
		mSkel.setColor(0,0,0);//health/100.f,health/100.f,health/100.f)

	bool right = mVel.x < 0;
	if ((!right && !flip) || (right && flip))
      reflect();
*/
}

void Seeker::takeHit(HitEffect hEff){
	health -= hEff.damage;
//	if (health < 0)
		//setVisible(false);//;lol
}
