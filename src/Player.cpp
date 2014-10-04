#include <SDL2/SDL.h>
#include "Player.h"
#include <glm/gtx/transform.hpp>

Player::Player() : ActiveEnt(){
	mSpeed = vec3(40.f, 30.f, 10.f);
	mDash = 1.5f;
	A = 10000.f;	
	sigmaSq = 5000.f;
	sigmaSq *= sigmaSq;
}

Player::Player(Entity * f) : ActiveEnt(f){
	mSpeed = vec3(40.f, 30.f, 10.f);
	mDash = 1.5f;
	A = 10000.f;	
	sigmaSq = 5000.f;
	sigmaSq *= sigmaSq;
}

Player::Player(vec3 translate, vec3 scale)
: ActiveEnt(translate, scale){
	mSpeed = vec3(20.f, 40.f, 20.12241f);
	mDash=1.5f;
	A = 10000.f;	
	sigmaSq = 5000.f;
	sigmaSq *= sigmaSq;
}

Player::~Player(){
	//NYI
}

void Player::setRig(){
/*
	mDrawables[0].get()->addChild(mDrawables[1].get());
	mDrawables[0].get()->addChild(mDrawables[2].get());
	mDrawables[2].get()->addChild(mDrawables[3].get());
	mDrawables[2].get()->addChild(mDrawables[4].get());

*/
	Rig * r = (Rig *)mSkeleton["leg1"];
//mDrawables[0].get();
	r->set_u({1.f,0});
	r = (Rig *)mSkeleton["leg2"];
	r->set_u({1.f,2.f});
	r = (Rig *)mSkeleton["arm1"];
	r->set_u({1.f,0.f});
	r = (Rig *)mSkeleton["arm2"];
	r->set_u({1.f,2.f});
/*	
	r = (Rig *)mSkeleton["body"];
	r->leftMultMV(glm::translate(vec3(-.1,0,0))*glm::rotate(.2f,vec3(0,0,-1)));
*/
}

int Player::setChildren(){
//	mDrawables[0].addChild(&mDrawables[1]);
	return 1;
}

void Player::update(){
	if (grounded)
		mVel.y=0.f;
	else
		mVel.y -= 2.f;//gravity?
	getHandleInfo();
}

void Player::handleKey(int k){
	mHandler.handleKey(k);
}

void Player::getHandleInfo(){
// uncomment to prohibit mid-air movement
//	if (mCollider.isGrounded){
	float dashSpeed = mSpeed.x*(mHandler.getKeyState(SDLK_LSHIFT) ? mDash : 1.f);
	bool jump = mHandler.getKeyState(SDLK_SPACE);
	bool dir = mVel.x < 0;
	
	mVel.x=0;
	if (mHandler.getKeyState(SDLK_a))
		mVel.x -= dashSpeed;
	if (mHandler.getKeyState(SDLK_d))
		mVel.x += dashSpeed;
	
	mVel.y += jump && grounded ? mSpeed.y : 0;
	
	mVel.z=0;
	if (mHandler.getKeyState(SDLK_w))
		mVel.z -= mSpeed.z;
	if (mHandler.getKeyState(SDLK_s))
		mVel.z += mSpeed.z;
//	}

	Rig * r = (Rig *)mSkeleton["leg1"];
	float c;
	if (mHandler.getKeyState(SDLK_a) || mHandler.getKeyState(SDLK_d) || mHandler.getKeyState(SDLK_s) || mHandler.getKeyState(SDLK_w)){
		if (mHandler.getKeyState(SDLK_LSHIFT))
			c=1.f;//r->set_u({1,mod});
		else
			c=0.f;//r->set_u({0,mod});
	}
	else
		c=-1.f;//r->set_u({-1,mod});
	r->inc_u(c);

	r = (Rig *)mSkeleton["leg2"];
	r->inc_u(c);
	r = (Rig *)mSkeleton["arm1"];
	r->inc_u(c);
	r = (Rig *)mSkeleton["arm2"];
	r->inc_u(c);
}
