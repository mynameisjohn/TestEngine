#include <SDL2/SDL.h>
#include "Player.h"
#include <glm/gtx/transform.hpp>
//#include <algorithm>
const unsigned int STAND  = 0;
const unsigned int WALK   = 1;
const unsigned int RUN    = 2;
const unsigned int JUMP   = 3;

const vec3 speed(40.f, 40.f, 20.f);

//this is a good number to center things around
const vec3 offset(100,100,0);

Player::Player() : ActiveEnt(), jumping(false), activeProj(0){
	mSpeed = speed;//vec3(40.f, 30.f, 10.f);
	mDash = 1.5f;
	A = 10000.f;	
	sigmaSq = 5000.f;
	sigmaSq *= sigmaSq;
}

Player::Player(const Entity& e)
: ActiveEnt(e), jumping(false), activeProj(0){
   mSpeed = speed;///vec3(40.f, 30.f, 10.f);
   mDash = 1.5f;
   A = 10000.f;
   sigmaSq = 5000.f;
   sigmaSq *= sigmaSq;
   projList.clear();

	//this crap
	mSkeleton["body"]->set_cycle("stand");
	mSkeleton["leg1"]->set_cycle("stand");
	mSkeleton["leg2"]->set_cycle("stand");
	mSkeleton["arm1"]->set_cycle("stand");
	mSkeleton["arm2"]->set_cycle("stand");
	mSkeleton["forearm1"]->set_cycle("stand");
	mSkeleton["forearm2"]->set_cycle("stand");
}
/*
Player::Player(Entity * f) : ActiveEnt(f), jumping(false), activeProj(0){
	mSpeed = speed;///vec3(40.f, 30.f, 10.f);
	mDash = 1.5f;
	A = 10000.f;	
	sigmaSq = 5000.f;
	sigmaSq *= sigmaSq;
	projList.clear();	
//	mSkeleton.print();
}
*/
Player::Player(vec3 translate, vec3 scale)
: ActiveEnt(translate, scale), jumping(false), activeProj(0){
	mSpeed = speed;//vec3(20.f, 40.f, 20.12241f);
	mDash=1.5f;
	A = 10000.f;	
	sigmaSq = 5000.f;
	sigmaSq *= sigmaSq;
}

Player::~Player(){
	//NYI
}

char Player::moveWRT_ent(Entity * e){
	ActiveEnt::moveWRT_ent(e);
	list<Projectile>::iterator pIt;
	
	for (pIt=projList.begin(); pIt!=projList.end(); pIt++)
		if (pIt->isActive())
			if (pIt->moveWRT_ent(e)){
			pIt->kill();
			pIt = projList.erase(pIt);
		}
}

void Player::move(){
	ActiveEnt::move();
/*
	for_each(projList.begin(), projList.end(), [](Projectile& p)
	Transform();//{
		if (p.isPoised())
			p.moveTo(center()+offset);
		else if (p.isActive())
			p.move();
	});
*/
	list<Projectile>::iterator pIt;
	for (pIt=projList.begin();pIt!=projList.end();pIt++){
		if (pIt->isPoised())
			pIt->moveTo(getOrigin("arm1")-pIt->mSkeleton.getOrigin()+vec3(0,0,-0.05));
		else if (pIt->isActive())
			pIt->move();
	}
}

void Player::update(){//EventInfo evInfo){
	if (grounded){
		mVel.y=0.f;
		eReg.worldMouse.x += mVel.x;
	}
	else{
		eReg.worldMouse += vec2(mVel);
		mVel.y -= 2.f;//gravity?
	}

	mSkeleton.resetTransform();//Ligaments();	
	
	updateSkeleton();
	updateKeys();
	updateMouse();

/*
	static float th(0.f);
	mSkeleton["bow"]->applyTransform(getRQ(vec4(th,vec3(0,0,1))));
	th += 1.f;
*/
	list<Projectile>::iterator pIt;
	for (pIt=projList.begin();pIt!=projList.end();pIt++){
		if (pIt->isActive())
			pIt->update();
		else if (!pIt->isAlive())
			pIt = projList.erase(pIt);
	}
}

void Player::updateMouse(){
//	vec2 worldMouse = eReg.getWorldMouse();

	vec2 dir = eReg.worldMouse - vec2(center());
	bool right = dir.x > 0;

	//this is about as good as it'll get for now
   if ((!right && !mSkeleton.flipped()) || (right && mSkeleton.flipped()))
		reflect();

	//this is gonna need some work
	vec2 launchOffset( (mSkeleton.flipped() ? -2 : 1) * 100,250);

	//make non static
	static float charge(0.f);
	static bool charging(false);

	if (eReg.lmb){
		if (charging){
			charge = min(150.f, charge+3.f);
			vec2 r = eReg.worldMouse-(vec2(center()+offset));
			vec3 t(-charge,0,0);
			const fquat r90(.707f, .707f*vec3(0,0,1));
			fquat mouseRot(getQuatFromVec2(r));

			Ligament * l = mSkeleton["arm1"];
			l->setState(L_ACTIVE);
			l->applyTransform(r90*mouseRot);
			mSkeleton["arm2"]->applyTransform({
			vec3(vec2(mSkeleton["arm1"]->getOrigin()-mSkeleton["arm2"]->getOrigin()),0)
			,r90*mouseRot});
		
			float bullshit(charge/130.f);
			l = projList.back().getSkeleton()->getRoot();
			l->applyTransform(mouseRot);

			projList.back().translate((430.f-charge)*vec3(glm::normalize(r),0));
			mSkeleton["forearm1"]->applyTransform(vec3(0,charge,0)/mSkeleton.getScale());
			mSkeleton["forearm1"]->setCurTex("forearm_draw");
				
/*
			QuatVec qv(vec3(-220,30,0)-t,getQuatFromVec2(r),QV_TRT);
			QuatVec trans(t,fquat());
			projList.back().mSkeleton.applyTransform(trans);//QuatVec();
			projList.back().mSkeleton.applyTransform(qv);
		Ligament * l = projList.back().mSkeleton.getRoot();
		QuatVec qv(vec3(l->getOrigin()),getQuatFromVec2(r),QV_TRT);
		l->applyTransform(qv);
*/
		}
		else{
			charging = true;
			projList.emplace_back(mProj);//push_back(std::move(mProj));
			projList.back().setState(1);

		}
	}
	else if (charging){
		//I need to make a moveTo cal to account for the "draw/pull back" 
		//translation/rotation effect. I could either get the matrix and invert it's
		//effect on a point to find the drawn position, or I could solve for it
		//assuming rotation was handled correctly. The translation is just 
		//a function of the charge, and the rotation should still be 
		//the direction from the worldmouse to the center.
		//either way, start drawing the collider so you can see what happens

		projList.back().setState(2);
		vec2 launchDir = eReg.worldMouse - vec2(offset + center());
		float speed = 50.f+charge;
		projList.back().launch(launchDir, speed);
		charge = 0.f;
		charging = false;
		mSkeleton["forearm1"]->setCurTex("forearm");
		mSkeleton["arm1"]->setState(L_CYCLIC);
	}
}

void Player::updateSkeleton(){
	Ligament * l;
	if (eReg.lmb){
		vec2 r = eReg.worldMouse-(vec2(center()+offset));
		if (r.x < 0)
			r.x *= -1;

/*
		l = mSkeleton["body"];
		l->setActive(true);
		QuatVec qv(l->getOrigin(),fquat(.707f, .707f*vec3(0,0,1))*getQuatFromVec2(r),QV_TRT);
		l->applyTransform(qv);
		l = mSkeleton["body"];
		QuatVec rot(l->getOrigin(), fquat(.707f, .707f*vec3(0,0,1))*getQuatFromVec2(r), QV_TRT);//fquat(s, (r.y>0?1:-1)*v*vec3(0,0,1)));

		l = projList.back().mSkeleton.getRoot();
		l->applyTransform(QuatVec(mSkeleton["arm1"]->getOrigin()-l->getOrigin(),getQuatFromVec2(r),QV_TRT));
*/
/*
		l->setActive(true);
		l->applyTransform(rot);
		l->setCurTex("arm_draw");

		mSkeleton["forearm"]->setCurTex("forearm_draw");
		mSkeleton["forearm"]->applyTransform(QuatVec({0,.2,0},fquat()));

		l = mSkeleton["arm2"];
		l->setActive(true);
		l->applyTransform(rot);// * QuatVec(vec3(-.25,0,0), fquat(.707f, .707f*vec3(0,0,1))));

		l = mSkeleton["forearm2"];
		l->applyTransform({0,.2,0});
*/
	}
	else{
		mSkeleton["arm1"]->setState(L_CYCLIC);
/*
		mSkeleton["arm1"]->setActive(false);
		mSkeleton["arm2"]->setActive(false);
		mSkeleton["forearm"]->setCurTex("forearm");
*/
	}
}

void Player::draw(){
	Entity::draw();

	list<Projectile>::iterator pIt;
	for (pIt=projList.begin(); pIt!=projList.end(); pIt++)
		pIt->draw();
	/*
	vector<unique_ptr<Projectile> >::iterator projIt;
   for (projIt=mProjectiles.begin();projIt!=mProjectiles.end();projIt++)
		(*projIt)->draw();
	*/
}

bool Player::overlapsWith(Entity * e){
	list<Projectile>::iterator pIt;
	for (pIt=projList.begin(); pIt!=projList.end(); pIt++){
		if (pIt->isActive()){
			if (pIt->overlapsWith(e)){
				pIt->kill();
				pIt = projList.erase(pIt);
			}
		}
	}
	
	return Entity::overlapsWith(e);
}

//this is a mess
void Player::updateKeys(){
// uncomment to prohibit mid-air movement
//	if (mCollider.isGrounded){
	bool jump = eReg.getKeyState(SDLK_SPACE);
	bool dash = eReg.getKeyState(SDLK_LSHIFT);
	float dashSpeed = mSpeed.x*(dash ? mDash : 1.f);
	bool dir = mVel.x < 0;
	
	mVel.x=0;
	if (eReg.getKeyState(SDLK_a))
		mVel.x -= dashSpeed;
	if (eReg.getKeyState(SDLK_d))
		mVel.x += dashSpeed;
	
	mVel.y += jump && grounded ? mSpeed.y : 0;
	
	mVel.z=0;
	if (eReg.getKeyState(SDLK_w))
		mVel.z -= mSpeed.z;
	if (eReg.getKeyState(SDLK_s))
		mVel.z += mSpeed.z;
//	}

	//Handle the pose changing here
	Rig * r;
	string to;
	bool moving = (eReg.getKeyState(SDLK_w) || 
					   eReg.getKeyState(SDLK_a) || 
					   eReg.getKeyState(SDLK_s) || 
					   eReg.getKeyState(SDLK_d));

//This whole moving to jump thing's all messed up

	if (moving){
		if (dash){
			to = "run";//RUN;
//			mSkeleton["arm1"]->setCurTex("arm_bent");
//			mSkeleton["arm2"]->setCurTex("arm_bent");
		}
		else
			to = "walk";//WALK;
	}
	else
		to = "stand";//STAND;

	mSkeleton["leg1"]->set_to(to);
	mSkeleton["leg2"]->set_to(to);
	mSkeleton["arm1"]->set_to(to);
	mSkeleton["arm2"]->set_to(to);
	mSkeleton["forearm1"]->set_to(to);
	mSkeleton["forearm2"]->set_to(to);
	mSkeleton["body"]->set_to(to);

/*
	if (grounded){
		if (jumping){
			((Rig *)(mSkeleton["leg1"]))->shift();
			jumping = false;
		}
		else if (jump){
	      ((Rig *)(mSkeleton["leg1"]))->set_pose(2);
	      ((Rig *)(mSkeleton["leg2"]))->set_pose(2);
			to = JUMP;
			jumping = true;
		}
	}
*/
/*
	//Automate this for all rigs in skeleton
	r = (Rig *)mSkeleton["leg1"];
	r->set_to(to);

	r = (Rig *)mSkeleton["leg2"];
	r->set_to(to);

	r = (Rig *)mSkeleton["arm1"];
	r->set_to(to);

	if (dash){
		r->setCurTex(1);
		r = (Rig *)mSkeleton["arm2"];
		r->setCurTex(1);
		r = (Rig *)mSkeleton["forearm1"];
		r->setVisibility(true);
		r = (Rig *)mSkeleton["forearm2"];
		r->setVisibility(true);
	}
	else{
		r->setCurTex(0);
		r = (Rig *)mSkeleton["arm2"];
		r->setCurTex(0);
		r = (Rig *)mSkeleton["forearm1"];
		r->setVisibility(false);
		r = (Rig *)mSkeleton["forearm2"];
		r->setVisibility(false);
	}

	r = (Rig *)mSkeleton["arm2"];
	r->set_to(to);
	
	r = (Rig *)mSkeleton["body"];
	r->set_to(to);
*/
/*
	Rig * r = (Rig *)mSkeleton["leg1"];
	float c;
	if (eReg.getKeyState(SDLK_a) || eReg.getKeyState(SDLK_d) || eReg.getKeyState(SDLK_s) || eReg.getKeyState(SDLK_w)){
		if (eReg.getKeyState(SDLK_LSHIFT))
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
*/
}

void Player::setProjectile(Projectile p){
	mProj = Projectile(p);
}

//Why do they have to be unique_ptrs? I may have goofed
void Player::addProjectile(Projectile p){
//	unique_ptr<Projectile> q(new Projectile(p));
//	mProjectiles.push_back(std::move(q));

	projList.emplace_back(p);
	//projList.back().mSkeleton.print();
/*
	cout << "Projectile " << mProjectiles.size() << ": ";
	mProjectiles.back()->mSkeleton.print();
	cout << endl;
*/
//	Projectile P(p);
//	mProjectiles.push_back(std::move(P));
}

/*
void Player::addProjectile(const Projectile p){
	unique_ptr<Projectile> q(new Projectile());

	mProjectiles.emplace_back(q.get());

//	mProjectiles.push_back(std::move(p));//std::move(p));
}
*/

EventRegister * Player::getRegPtr(){
	return &eReg;
}
