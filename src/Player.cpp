#include <SDL2/SDL.h>
#include "Player.h"
#include <glm/gtx/transform.hpp>

Player::Player() : ActiveEnt(){
	mSpeed = vec3(400.f, 30.f, 10.f);
	mDash = 1.5f;
	A = 10000.f;	
	sigmaSq = 5000.f;
	sigmaSq *= sigmaSq;
}

Player::Player(Collider c) : ActiveEnt(c){
	//NYI
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

using glm::translate;
using glm::rotate;
typedef vec3 v3;

void Player::setRig(){

	mDrawables[0].get()->addChild(mDrawables[1].get());
	mDrawables[0].get()->addChild(mDrawables[2].get());
	mDrawables[2].get()->addChild(mDrawables[3].get());

	Rig * r = (Rig *)mDrawables[0].get();
	r->set_u({1.f,-1.f});
	r = (Rig *)mDrawables[1].get();
	r->set_u({1.f,1.f});
/*
	Rig * r = (Rig *)mDrawables[0].get();

	vector<mat4> joints(3);
   vector<Pose> poses(3);
	vector<Cycle> cycles(3);
	const vec3 z(0,0,1);
	vector<fdualquat> mJoints;

	//Stand cycle is identity(cycles[0] by default)

	//Walk Cycle
	mJoints.push_back(createDQ_t(vec3(.3f,0,0))*createDQ_r(vec4(z,1.57f/6)));
	mJoints.push_back(mJoints.back()*createDQ_r(vec4(z,1.57f/12)));
	mJoints.push_back(mJoints.back()*fdualquat());
	poses[0]=Pose(mJoints);
	mJoints.clear();

	mJoints.push_back(fdualquat());
	mJoints.push_back(mJoints.back()*createDQ_t(vec3(-.2f,.1f,0))*createDQ_r(vec4(-z,1.57f/5)));
	mJoints.push_back(mJoints.back()*createDQ_t(vec3(0,.1f,0))*createDQ_r(vec4(-z,1.57f/3)));
	poses[2]=Pose(mJoints);
	mJoints.clear();
	
	cycles[1].setPoses(poses);
	

	r->setCycles(cycles);	
	r = (Rig *)mDrawables[1].get();

	Pose p=poses[0];
	poses[0]=poses[1];
	poses[1]=poses[2];
	poses[2]=p;
	cycles[1].setPoses(poses);
	r->setCycles(cycles);

	r = (Rig *)mDrawables[3].get();
	mJoints.push_back(createDQ_r(vec4(z,1.57f/12)));
	mJoints.push_back(mJoints.back()*createDQ_r(vec4(z,1.57f/12)));
	mJoints.push_back(mJoints.back()*fdualquat());
	poses[0]=Pose(mJoints);
	mJoints.clear();

	mJoints.push_back(createDQ_t(vec3())*createDQ_r(vec4(z,1.57f/12)));
   mJoints.push_back(mJoints.back()*createDQ_r(vec4(z,1.57f/12)));
   mJoints.push_back(mJoints.back()*fdualquat());
   poses[2]=Pose(mJoints);
   mJoints.clear();

	cycles[0].setPoses(poses);
	cycles[1].setPoses(poses);
	r->setCycles(cycles);
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

	Rig * r = (Rig *)mDrawables[0].get();
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
	r = (Rig *)mDrawables[1].get();
	r->inc_u(c);
	r = (Rig *)mDrawables[3].get();
	r->inc_u(c);

	static float y=0.f;
	mDrawables[2].get()->leftMultMV(glm::translate(vec3(0.f,0.0001f*(sin(2.f*y)+sin(y)),0.f)));
	y+=0.4f;

}
