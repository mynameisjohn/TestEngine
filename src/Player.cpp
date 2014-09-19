#include <SDL2/SDL.h>
#include "Player.h"
#include <glm/gtx/transform.hpp>

Player::Player() : ActiveEnt(){
	mSpeed = vec3(40.f, 40.f, 20.12241f);
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
	mSpeed = vec3(40.f, 40.f, 20.12241f);
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

	Rig * r = (Rig *)mDrawables[0].get();
/*
	vector<fdualquat> joints;
   vector<Pose> poses;
	vector<Cycle> cycles;
*/

	vector<mat4> joints(3);
//	vector<fdualquat> joints(3);
   vector<Pose> poses(3);
	vector<Cycle> cycles(3);
	const vec3 z(0,0,1);
	fdualquat d = createDQ_t({.25f,0,0})*createDQ_r(vec4(z,1.57f/6));
	
//	joints[0]=getDQMat(createDQ_t(vec3())*createDQ_r(vec4(-z,1.57f/6)));
//glm::translate(v3(-.2f,.1f,0))*rotate(1.57f/6,-z);
	joints[1]=getDQMat(createDQ_t(vec3(-.2f,.1f,0))*createDQ_r(vec4(-z,1.57f/5)));//joints[0];//*getDQMat(createDQ_t(vec3(-.25f,0,0))*createDQ_r(vec4(-z,1.57f/3)));
//joints[0]*glm::scale(v3(1.5,.9,1))*glm::translate(v3(-.2f,.2f,0))*rotate(1.57f/3,-z);
	joints[2]=joints[1]*getDQMat(createDQ_t(vec3(0,.1f,0))*createDQ_r(vec4(-z,1.57f/3)));
//joints[1]*glm::translate(v3(.0f,.2f,0))*rotate(1.57f/3,-z);
	
	cout << getDQMat(fdualquat()) << endl;

	poses[2]=Pose({
		fdualquat(),//identity?
		createDQ_t(vec3(-.2f,.1f,0))*createDQ_r(vec4(-z,1.57f/5)),
		createDQ_t(vec3(-.2f,.1f,0))*createDQ_r(vec4(-z,1.57f/5))*createDQ_t(vec3(0,.1f,0))*createDQ_r(vec4(-z,1.57f/3))
	});
	poses[2].setMats(joints);
	

	poses[0]=Pose({
		createDQ_t(vec3(.3f,0,0))*createDQ_r(vec4(z,1.57f/6)),
		createDQ_t(vec3(.3f,0,0))*createDQ_r(vec4(z,1.57f/6))*createDQ_r(vec4(z,1.57f/12)),
		createDQ_t(vec3(.3f,0,0))*createDQ_r(vec4(z,1.57f/6))*createDQ_r(vec4(z,1.57f/12))
	});
		
	joints[0]=getDQMat(createDQ_t(vec3(.25f,0,0))*createDQ_r(vec4(z,1.57f/6)));
//glm::translate(v3(.25,0,0))*rotate(1.57f/6,z);
	joints[1]=joints[0]*getDQMat(createDQ_r(vec4(z,1.57f/15)));
	joints[2]=joints[1];
	poses[0].setMats(joints);
	cycles[1].setPoses(poses);
	

	r->setCycles(cycles);	
	r = (Rig *)mDrawables[1].get();

	Pose p=poses[0];
	poses[0]=poses[1];
	poses[1]=poses[2];
	poses[2]=p;
	cycles[1].setPoses(poses);
	r->setCycles(cycles);
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
}
