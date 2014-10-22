#include "Ligament.h"
#include "Util.h"
#include <typeinfo>

#include "Rig.h"

Ligament::Ligament()
{
	resetTransform();
}

Ligament::Ligament(Drawable * dPtr, QuatVec qv)
: mDrawable(dPtr), mQV(qv), curTex(), from(), to(), active(false){
   mTransform.clear();
   mTransform.push_front(qv);
}
/*
Ligament::Ligament(std::unique_ptr<Drawable> dPtr, QuatVec qv)
: mDrawable(move(dPtr)), mQV(qv), curTex(), from(), to(), active(false){
	mTransform.clear();
	mTransform.push_front(qv);
}
*/
/*
Ligament::Ligament(Drawable d, QuatVec qv)
: mDrawable(new Drawable(d)), mQV(qv), curTex(), from(), to(), active(false){
	mTransform.clear();
	mTransform.push_front(qv);
}
*/
/*
void Ligament::addChild(map<string, Ligament>::iterator it){
	Ligament * child = (&it->second);
	child->mTransform.push_front(getTransform().inverse() * (child->getTransform()));
	child->resetTransform();
	cVec.push_back(it);

	cout << it->first << "\t" << &(it->second) << endl;
}

void Ligament::addChild(Ligament * child){
	vector<Ligament *>::iterator childIt;
	
	for (childIt=child->children.begin(); childIt!=child->children.end(); childIt++){
		if ((*childIt)->hasChild(this)){
			cout << "Ligament: cycle created!" << endl;
         return;
		}
	}

	//this makes setting things up a bit easier...feels gross
//	child->leftMultMV(mDrawable->getMVInverse());
	
	child->mTransform.push_front(getTransform().inverse() * (child->getTransform()));
	child->resetTransform();

//	child->mQV = mQV.inverse() * child->mQV;
	children.push_back(child);
}
*/

void Ligament::addChild(int offset){
	//in future check if already exists
	children.push_back(offset);

	//here's where it gets weird
	this[offset].mTransform.push_front(getTransform().inverse() * this[offset].getTransform());	
}

void Ligament::resetTransform(){
	mTransform.resize(1);
}

QuatVec Ligament::getTransform(){
	QuatVec transform(mTransform.front());
	list<QuatVec>::iterator it = mTransform.begin();
	for (it++; it!=mTransform.end(); it++)
		transform = *it * transform;

	return transform;
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
//	cout << "who is " << mDrawable << endl;
	parent = parent * getTransform().toMat4();//mQV.toMat4();// * parent;

//	if (Rig * r = dynamic_cast<Rig *>(mDrawable.get())){
	if (Rig * r = dynamic_cast<Rig *>(mDrawable)){
		if (active)
				parent = r->draw(parent, curTex);
		else{
			parent = r->draw(parent, curTex, u, from, to);
			update();
		}
	}
	else
		parent = mDrawable->draw(parent, curTex);
		//parent = mDrawable_x->draw(parent, curTex);
	//mDrawable->draw(parent);
/*
	vector<Ligament *>::iterator childIt;
	for (childIt=children.begin(); childIt!=children.end(); childIt++)
		(*childIt)->draw(parent);
*/
/*
	//wow
	vector<map<string, Ligament>::iterator>::iterator it;
	for (it=cVec.begin(); it!=cVec.end(); it++)
		(*it)->second.draw(parent);
*/
	vector<int>::iterator it;
	for (it=children.begin(); it!=children.end(); it++)
		this[*it].draw(parent);
}

//we don't want to be doing this kind of thing
void Ligament::leftMultMV(mat4 m){
	mDrawable->leftMultMV(m);
}

void Ligament::setCurTex(unsigned int ct){
	curTex = ct;
}

void Ligament::set_u(vec2 U){
	u=U;
}

void Ligament::set_to(unsigned int t){
	to = t;
}

void Ligament::setActive(bool b){
	active = b;
}
//phase these out
void Ligament::setColor(vec3 color){
	mDrawable->setColor(color);
}

void Ligament::setColor(float r, float g, float b, float a){
	mDrawable->setColor(r,g,b,a);
}

void Ligament::shift(){
	Rig * r = dynamic_cast<Rig *>(mDrawable);
	if (r)//typeid(*(mDrawable)) == typeid(Rig)){
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

void Ligament::setRotate(vec4 R){
	mQV.rot = getRQ(R);
}

void Ligament::setTranslate(vec3 t){
	mQV.trans = t;
}

void Ligament::setQuatVec(QuatVec qv){
	mQV = qv;
}

bool Ligament::hasChild(Ligament * l){
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
