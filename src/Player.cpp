#include <SDL2/SDL.h>
#include "Player.h"
#include <glm/gtx/transform.hpp>
//#include <algorithm>
const unsigned int STAND  = 0;
const unsigned int WALK   = 1;
const unsigned int RUN    = 2;
const unsigned int JUMP   = 3;

const vec3 speed(40.f, 40.f, 20.f);

const vec3 offset(75,200,0);

Player::Player() : ActiveEnt(), jumping(false), activeProj(0){
	mSpeed = speed;//vec3(40.f, 30.f, 10.f);
	mDash = 1.5f;
	A = 10000.f;	
	sigmaSq = 5000.f;
	sigmaSq *= sigmaSq;
}

Player::Player(Entity * f) : ActiveEnt(f), jumping(false), activeProj(0){
	mSpeed = speed;///vec3(40.f, 30.f, 10.f);
	mDash = 1.5f;
	A = 10000.f;	
	sigmaSq = 5000.f;
	sigmaSq *= sigmaSq;
	projList.clear();	
//	mSkeleton.print();
}

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
//	vector<unique_ptr<Projectile> >::iterator projIt;
	list<Projectile>::iterator pIt;
	
	for (pIt=projList.begin(); pIt!=projList.end(); pIt++)
		if (pIt->isActive())
			if (pIt->moveWRT_ent(e)){
			pIt->kill();
			pIt = projList.erase(pIt);
		}
/*

   for (projIt=mProjectiles.begin();projIt!=mProjectiles.end();projIt++){
      if ((*projIt)->isActive() && (*projIt)->moveWRT_ent(e))//projIt->get()->move();
			(*projIt)->kill();
   }
*/
}

void Player::move(){
	ActiveEnt::move();
/*
	for_each(projList.begin(), projList.end(), [](Projectile& p)
	{
		if (p.isPoised())
			p.moveTo(center()+offset);
		else if (p.isActive())
			p.move();
	});
*/	
	list<Projectile>::iterator pIt;
	for (pIt=projList.begin();pIt!=projList.end();pIt++){
		if (pIt->isPoised())
			pIt->moveTo(center()+offset);
		else if (pIt->isActive())
			pIt->move();
	}
/*
	vector<unique_ptr<Projectile> >::iterator projIt;
	for (projIt=mProjectiles.begin();projIt!=mProjectiles.end();projIt++){
		if ((*projIt)->isPoised())
			(*projIt)->moveTo(center()+offset);
		else if ((*projIt)->isActive())
			(*projIt)->move();//projIt->get()->move();
	}
*/
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

	mSkeleton["arm1"]->setCurTex(0);
	mSkeleton["arm2"]->setCurTex(0);
	mSkeleton.resetLigaments();	
	
	updateKeys();
	updateMouse();
	updateSkeleton();

	list<Projectile>::iterator pIt;
	for (pIt=projList.begin();pIt!=projList.end();pIt++){
		if (pIt->isActive())
			pIt->update();
		else if (!pIt->isAlive())
			pIt = projList.erase(pIt);
	}
/*
	vector<unique_ptr<Projectile> >::iterator projIt;
   for (projIt=mProjectiles.begin();projIt!=mProjectiles.end();projIt++)
      if ((*projIt)->isActive())
			(*projIt)->update();
*/
	//cout << mSkeleton["arm1"] << "\t" << mSkeleton["arm2"];
}

void Player::updateMouse(){
//	vec2 worldMouse = eReg.getWorldMouse();

	vec2 dir = eReg.worldMouse - vec2(center());
	bool right = dir.x > 0;

	//this is about as good as it'll get for now
   if ((!right && !flip) || (right && flip))
		reflect();

	//this is gonna need some work
	vec2 launchOffset( (flip ? -2 : 1) * 100,250);

	//make non static
	static float charge(0.f);
	static bool charging(false);


	//If I were to do this with a list....
	/*

	//I'd have the prototype as a member
	Projectile mProjectile;
	
	if (eReg.lmb){
		if (charging)
			charge some more
		otherwise
			start charging
			create a new projectile from the prototype
			push it to the back of the list, make sure it gets drawn somehow
	}
	else if (charging){
		//for now, I guess I can assume that the back of the list is the active one
		//which may not always be the case
		set the back of the list to be active, and launch it
	}

	//and then if I have to kill something, I should already have its iterator. 
	//lists have an erase function that takes an iterator.

	states: active, poised, dead

	*/

//	mSkeleton["arm1"]->setActive(eReg.lmb ? true : false);

	if (eReg.lmb){
		if (charging){
			charge = min(150.f, charge+3.f);
			vec2 r = eReg.worldMouse-(vec2(center()+offset));
			vec3 t(-charge,0,0);
			QuatVec qv = QuatVec(getQuatFromVec2(r), t);
			projList.back().mSkeleton.getRoot()->applyTransform(qv);
		}
		else{
			charging = true;
			projList.emplace_back(mProj);//push_back(std::move(mProj));
			projList.back().setState(1);

			Projectile * p = &(projList.back());
//			cout << &(p->mSkeleton) << "\t" << p->mSkeleton.getRoot() << "\t" << p->mSkeleton.getRoot()->mDrawable << endl;
		}
	}
	else if (charging){
		projList.back().setState(2);
		vec2 launchDir = eReg.worldMouse - (launchOffset + vec2(center()));
		float speed = 50.f+charge;
		projList.back().launch(launchDir, speed);
		charge = 0.f;
		charging = false;
	}
/*
	Projectile * p = mProjectiles[wrap(mProjectiles.size(),activeProj)].get();
	//if the left mouse button's pressed
	if (eReg.lmb){
		vec2 r = eReg.worldMouse-(vec2(center()+offset));
		vec3 t(-charge,0,0);
		QuatVec qv = QuatVec(getQuatFromVec2(r), t);
		p->mSkeleton.getRoot()->applyTransform(qv);//QuatVec(t, getQuatFromVec2(r)));
//setRotate(vec4(radToDeg(acos(r.x)), sin(asin(r.y)/2)*vec3(0,0,1)));
//		mSkeleton["arm1"]->setCurTex(2);

		if (charging)//if we're already charging
			charge = min(150.f, charge+3.f);
		else if (!p->isActive()){
			charging = true;
			//this doesn't do quite what I want yet...
//			p->moveTo(vec3(launchOffset,0) + center());
			p->setState(1);
			//mSkeleton["arm1"]->setCurTex(2);
		}
	}//if we were already charging and lmb is false, release and reset
	else if (charging){
		//cout << charge << "\t";
		p->setState(2);
      p->launch(eReg.worldMouse - (launchOffset + vec2(center())), 50.f + charge);
      charge = 0.f;
      charging = false;	
		activeProj++;
	}
*/
}

void Player::updateSkeleton(){
	Ligament * l;
	if (eReg.lmb){
		vec2 r = eReg.worldMouse-(vec2(center()+offset));
		float s,v;
		//half angle formulas, yo
		s = (float)(sqrt(.5f*(1.f+r.x)));
		v = (float)(sqrt(.5f*(1.f-r.x)));
		QuatVec rot(vec3(), getQuatFromVec2(r));//fquat(s, (r.y>0?1:-1)*v*vec3(0,0,1)));
		
		l = mSkeleton["arm1"];
		l->setActive(true);
		l->applyTransform(rot);
		l->setCurTex(2);

		l = mSkeleton["arm2"];
		l->setActive(true);
		l->applyTransform(rot * QuatVec(vec3(), fquat(.707f, .707f*vec3(0,0,1))));
	}
	else{
		mSkeleton["arm1"]->setActive(false);
		mSkeleton["arm2"]->setActive(false);
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
/*
	vector<unique_ptr<Projectile> >::iterator projIt;
   for (projIt=mProjectiles.begin();projIt!=mProjectiles.end();projIt++){
      if ((*projIt)->isActive() && (*projIt)->overlapsWith(e))// && e->isVisible())
			(*projIt)->kill();
   }
*/
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
	unsigned int to(0);
	bool moving = (eReg.getKeyState(SDLK_w) || 
					   eReg.getKeyState(SDLK_a) || 
					   eReg.getKeyState(SDLK_s) || 
					   eReg.getKeyState(SDLK_d));

//This whole moving to jump thing's all messed up

//	mSkeleton["arm1"]->setCurTex(0);
//	mSkeleton["arm2"]->setCurTex(0);
	if (moving){
		if (dash){
			to = RUN;
			mSkeleton["arm1"]->setCurTex(1);
			mSkeleton["arm2"]->setCurTex(1);
		}
		else
			to = WALK;
	}
	else
		to = STAND;

	mSkeleton["leg1"]->set_to(to);
	mSkeleton["leg2"]->set_to(to);
	mSkeleton["arm1"]->set_to(to);
	mSkeleton["arm2"]->set_to(to);
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

void Player::setProjectile(Projectile * p){
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
