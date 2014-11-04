#include "Ligament.h"
#include "Util.h"
#include <typeinfo>
#include <glm/gtx/transform.hpp>
#include "Rig.h"

Ligament::Ligament(){
	resetTransform();
}

//To Do: change constructor so that the QuatVec list starts off empty,
//since any initial transform goes in MV. yuck
Ligament::Ligament(Drawable * dPtr, string ctex)
: mDrawable(dPtr), from(""), to(""), active(false), cTex(ctex), state(L_CYCLIC){
   mTransform.clear();
	//origin = MV*mDrawable->getOrigin();
	origins.clear();
	if (Rig * r = dynamic_cast<Rig *>(mDrawable)){
		origins = r->getOrigins();
		from = to = r->getFirstCycle();
	}
	else
		origins.push_back(mDrawable->getOrigin());
}

void Ligament::setState(int state){
	this->state = state;
}

int Ligament::addChild(int offset, bool invert){
	//in future check if already exists
	children.push_back(offset);

	return children.back();
}

void Ligament::set_cycle(string s){
	to = from = s;
}

void Ligament::resetTransform(){
	mTransform.clear();//resize(1);
}

QuatVec Ligament::getTransform(){
	QuatVec transform(mTransform.front());
	list<QuatVec>::iterator it = mTransform.begin();
	for (it++; it!=mTransform.end(); it++)
		transform = *it * transform;

	return transform;
}

mat4 Ligament::getTransformAsMat4(){
 // i don't like this
	mat4 ret;
	bool s(false);
	QuatVec transform(mTransform.front());
	list<QuatVec>::iterator it = mTransform.begin();

	for (it++; it!=mTransform.end(); it++){
		if (*it & transform)
			transform = *it * transform;
		else{
			s=true;
			ret = transform.toMat4();
			break;
		}
	}

	if (s){
		for (it; it!=mTransform.end(); it++)
			ret = ret * it->toMat4();
	}
	else ret = transform.toMat4();

	return ret;
}

void Ligament::update(){
	const float eps = 0.001f;
   const float dt = 0.1f;

	active = false;

   if (1.f-u.x < eps){
      from = to;
      u.x=0.f;
   }

	if (from != to)
		u.x += dt;
}

void Ligament::draw(glm::mat4 parent){
	//upload color here, for debugging purposes
	mDrawable->uploadColor(mColor);

	if (Rig * r = dynamic_cast<Rig *>(mDrawable)){
		switch(state){
			case L_CYCLIC:{
				//get current pose
				Pose p(r->getCurrentPose(from,to,u));
				//get first joint's QV, plus whatever transform this ligament has
				QuatVec qv(mTransform.empty() ? p.joints[0] : getTransform()*p.joints[0]);
				//Translate to origin, rotate, translate out, then translate this ligament's transform
				mat4 m(glm::translate(qv.trans+vec3(origins[0]))*
						 glm::mat4_cast(qv.rot)*
						 glm::translate(-vec3(origins[0])));

				//this all seems rather expensive
				vector<mat4> R(origins.size());
				R[0] = m;//parent * m * MV;
	
				for (int i=1; i<R.size(); i++){
					R[i] = R[i-1]*
								  glm::translate(vec3(origins[i])+p.joints[i].trans)*
								  glm::mat4_cast(p.joints[i].rot)*
								  glm::translate(-vec3(origins[i]));
				}
				R.back() = parent*R.back();
				for (vector<int>::iterator it = children.begin(); it!=children.end(); it++)
					this[*it].draw(R.back());
				R.back() = R.back()*MV;
	
				for (int i=0; i<R.size()-1; i++)
					R[i]=parent*R[i]*MV;

				r->uploadRigMats(R);
				r->draw(cTex,true);


				update();
				break;
			}
			case L_ONCE:
				break;
			case L_ACTIVE:{
				if (!mTransform.empty()){
					QuatVec qv(getTransform());
					parent = parent * glm::translate(qv.trans+vec3(origins[0]))*
											glm::mat4_cast(qv.rot)*
											glm::translate(-vec3(origins[0]));
				}
				mat4 m = parent*MV;
				r->uploadMV(m);
				r->draw(cTex, false);
				for (vector<int>::iterator it = children.begin(); it!=children.end(); it++)
					this[*it].draw(parent);
			}
			default:
				break;
		}
	}
	else{
		if (!mTransform.empty()){
//			parent = parent*getTransformAsMat4();
			QuatVec qv(getTransform());
			parent = parent * glm::translate(qv.trans+vec3(origins[0]))*
									glm::mat4_cast(qv.rot)*
									glm::translate(-vec3(origins[0]));
		}
		mat4 m = parent * MV;
		mDrawable->uploadMV(m);
		mDrawable->draw(cTex);
		for (vector<int>::iterator it = children.begin(); it!=children.end(); it++)
			this[*it].draw(parent);
	}


/*
	if (!mTransform.empty())
		parent=parent*getTransformAsMat4();
	mat4 m = parent*MV;
	mDrawable->uploadMV(m);
*/
/*
	parent = mTransform.empty() ? parent * MV : parent * getTransformAsMat4() * MV;
	mDrawable->uploadMV(parent * );
*/
/*
	if (Rig * r = dynamic_cast<Rig *>(mDrawable)){
		if (active)
				r->draw(cTex);
		else{
			mat4 back = r->draw(mat4(), u, from, to, cTex);
			back = glm::translate(vec3(origin))*back*glm::translate(-vec3(origin));
			parent = parent*back;//parent*r->draw(mat4(), u, from, to, cTex);
			update();
		}
	}
	else
		mDrawable->draw(cTex);//, color);//curTex);
//		parent = mDrawable->draw(parent, curTex);
*/
/*
	//lambda
	for_each(children.begin(), children.end(), [&](int idx){
		this[idx].draw(parent);
	});
*/
/*
	for (vector<int>::iterator it = children.begin(); it!=children.end(); it++)
		this[*it].draw(parent);
*/
}

void Ligament::setCurTex(string ct){
	cTex = ct;
}
/*
void Ligament::setCurTex(unsigned int ct){
	curTex = ct;
}
*/
void Ligament::set_u(vec2 u){
	this->u = u;
}

void Ligament::set_to(string s){//unsigned int t){
	to = s;
}

void Ligament::setActive(bool b){
	active = b;
}

void Ligament::shift(){
	if (Rig * r = dynamic_cast<Rig *>(mDrawable))
		u.y += r->getShift();
}

void Ligament::applyTransform(QuatVec qv){
	mTransform.push_back(qv);
}

void Ligament::applyTransform(vec3 t){
	mTransform.emplace_back(t,fquat());
}

void Ligament::applyTransform(fquat q){
	mTransform.emplace_back(vec3(),q);
}

void Ligament::leftMultMV(mat4 m){
	MV = m*MV;
	origins.clear();
	if (Rig * r = dynamic_cast<Rig *>(mDrawable))
		origins = r->getOrigins(MV);
	else
		origins.push_back(mDrawable->getOrigin(MV));
}

bool Ligament::hasChild(Ligament * l){
	for (vector<int>::iterator it = l->children.begin(); it!=l->children.end(); it++){
		
	}
/*	if (children.size() == 0)
		return false;
	
	vector<Ligament *>::iterator childIt;
	for (childIt=children.begin(); childIt!=children.end(); childIt++)
		if (l == (*childIt))
			return true;
		
	for (childIt=children.begin(); childIt!=children.end(); childIt++)
		return (*childIt)->hasChild(l);
*/
	return false;
}

vec3 Ligament::getOrigin(int idx){
	return vec3(origins[idx > origins.size()-1 ? origins.size()-1 : idx]);
}

mat4 Ligament::getMV(){
	return MV;
}

Drawable * Ligament::getDrPtr(){
	return mDrawable;
}
