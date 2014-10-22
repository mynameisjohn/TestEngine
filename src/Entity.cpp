#include "Entity.h"
#include <glm/gtx/transform.hpp>

Entity::Entity()
: flip(false), sigmaSq(1.f){}


//READ THIS
//this is my most popular constructor, but there's a bug that I'm debating letting keep.
//A new skeleton is created, but all of its contents (which are themselves unique pointers)
//are copied from the old one, meaning all entities created from the Entity * f parameter
//share the same Drawable object. This isn't necessarily a problem, so long as I make
//sure not to touch any of the data inside drawable (apart from the initial scales and rotations.)
//However, this more or less invalidates the usefulness of the Drawable's visible member. 
Entity::Entity(Entity * f)
: mCollider(f->mCollider), mSkeleton((f->mSkeleton)), flip(false), sigmaSq(1.f){}

Entity::Entity(vec3 translate, vec3 scale)
: mCollider(BoundBox(scale)), flip(false)
{
	this->translate(translate);
}

Entity::Entity(Collider c, Skeleton s)
: mCollider(c), mSkeleton(move(s)), flip(false){
}

void Entity::reflect(){
	mSkeleton.applyTransform(glm::translate(vec3(mCollider.getDim().x,0,0))*glm::scale(vec3(-1,1,1)));
   flip = !flip;
}

void Entity::takeHit(HitEffect hEff){

}

void Entity::draw(){
	//mDrawables[0].get()->draw(glm::translate(getPos()));
	mSkeleton.draw(glm::translate(getPos()));//getRoot()->draw(glm::translate(getPos()));
}
/*
void Entity::addDrawable(unique_ptr<Drawable> dr){
	mDrawables.push_back(std::move(dr));
}
*/
void Entity::setCol(Collider c){
	mCollider = c;
}

void Entity::setBB(BoundBox bb){
	mCollider.setBB(bb);
}

void Entity::setSkeleton(Skeleton s){
	mSkeleton = move(s);
}
/*
bool Entity::isVisible(){
	return mSkeleton.getRoot()->isVisible();
}

void Entity::setVisible(bool b){
	mSkeleton.getRoot()->setVisibility(b);
}
*/
char Entity::collidesWith(Entity * e){
	char last = mCollider.collidesWith(e->mCollider);
	return last;
}

bool Entity::overlapsWith(Entity * e){
	return mCollider.overlapsWith(e->mCollider);
}

void Entity::moveTo(vec3 pos){
	mCollider.moveTo(pos);
}

void Entity::translate(vec3 trans){
	mCollider.translate(trans); 
} 
vec3 Entity::getPos(){ 
	return mCollider.getPos(); 
}

vec3 Entity::getPotential(vec3 pos){
	vec3 diff, a;
	float distSq;
	
	diff = pos - getPos();
	distSq = glm::dot(diff, diff);
	a = (float)(2.f*A*exp(-distSq/sigmaSq)/sigmaSq) * diff;

	return a;
}

float Entity::toLeft(Entity * e){ 
	return mCollider.toLeft(e->mCollider); 
} 
float Entity::toRight(Entity * e){ 
	return mCollider.toRight(e->mCollider); 
} 
float Entity::toBottom(Entity * e){ 
	return mCollider.toBottom(e->mCollider); 
} 
float Entity::toTop(Entity * e){ 
	return mCollider.toTop(e->mCollider); 
} 
float Entity::toNear(Entity * e){ 
	return mCollider.toNear(e->mCollider); 
} 
float Entity::toFar(Entity * e){ 
	return mCollider.toFar(e->mCollider); 
} 
vec3 Entity::center(){ 
	return mCollider.center(); 
}
