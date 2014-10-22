#include "Projectile.h"
#include <glm/gtx/transform.hpp>

//these have to be formally deactivated (by a function)
//also add a counter

const int DEAD(0), POISED(1), ACTIVE(2), LIFETIME(500);

Projectile::Projectile()
: ActiveEnt(), state(0), ticker(0){
}

Projectile::Projectile(Entity * e)
: ActiveEnt(e), state(0), ticker(0){
//	mSkeleton.setRoot(mSkeleton["root"]);
	cout << (&mSkeleton) << "\t" << mSkeleton.getRoot() << "\t" <<mSkeleton.getRoot()->mDrawable << endl;
}

Projectile::Projectile(vec3 translate, vec3 scale)
: ActiveEnt(translate, scale), state(0), ticker(0){

}

void Projectile::draw(){
//	cout << &mSkeleton << "\t" << mSkeleton["root"] << endl;

	if (state==ACTIVE)
		mSkeleton.draw(glm::translate(getPos()) * getAlignMat(mVel, 'x'));
	else if (state == POISED)
		mSkeleton.draw(glm::translate(getPos()));
//	if (state)
//		cout << (&mSkeleton) << "\t" << mSkeleton.getRoot() << "\t" <<mSkeleton.getRoot()->mDrawable << endl;
	
	mSkeleton.resetLigaments();
}

void Projectile::update(){
	if (grounded)
      mVel.y=0.f;
   else
      mVel.y -= 2.f;

	if (++ticker > LIFETIME)
		kill();
}

char Projectile::moveWRT_ent(Entity * e){
	char last = ActiveEnt::moveWRT_ent(e);
//
	switch (last){
		case 'x':
		case 'y':
		case 'z':	
//			state = false;
//			mVel = vec3();
			e->takeHit({10.f,{0,0,0}});
			//cout << last << endl;
		default:
			break;
	}

	return last;
}


void Projectile::kill(){
	state=DEAD;
	mVel=vec3();
//	moveTo({0,0,0});
//	cout << "projectile killed" << endl;
}

bool Projectile::overlapsWith(Entity * e){
	bool ret = Entity::collidesWith(e);

	if (ret)
		e->takeHit({10.f,{0,0,0}});


	return ret;
}


void Projectile::launch(vec3 dir, float speed){
	mVel = speed * glm::normalize(dir);//vec3(10,0,0);
	mSkeleton.resetLigaments();
}

void Projectile::launch(vec2 dir, float speed){
	mVel = speed * vec3(glm::normalize(dir),0);//vec3(10,0,0);
	//cout << mVel << endl;
}

void Projectile::setState(int s){
	state = s;
	if (state != DEAD)
		grounded = false;
}

bool Projectile::isAlive(){
	return state != DEAD;
}
bool Projectile::isPoised(){

	return state == POISED;
}

bool Projectile::isActive(){
	return state == ACTIVE;
}
