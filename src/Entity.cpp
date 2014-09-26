#include "Entity.h"
#include <glm/gtx/transform.hpp>

Entity::Entity(){
	sigmaSq=1.f;
}

Entity::Entity(Entity * f)
: mCollider(f->mCollider), mSkeleton(std::move(f->mSkeleton)){
	sigmaSq=1.f;
}

Entity::Entity(vec3 translate, vec3 scale){
	mCollider = Collider(BoundBox((scale)));
	this->translate(translate);
}

Entity::Entity(vec3 translate, vec3 scale, SceneGraph SG)
	: mCollider(BoundBox(scale)), mSkeleton(std::move(SG))
{
	this->translate(translate);
}

Entity::Entity(Collider c){
	mCollider = c;
}

void Entity::draw(){
	mDrawables[0].get()->draw(glm::translate(getPos()));
//	mSkeleton.begin()->second.get()->draw(glm::translate(getPos()));
}

void Entity::addDrawable(unique_ptr<Drawable> dr){
	mDrawables.push_back(std::move(dr));
}

void Entity::setCol(Collider c){
	mCollider = c;
}

void Entity::setBB(BoundBox bb){
	mCollider.setBB(bb);
}

char Entity::collidesWith(Entity * e){
	char last = mCollider.collidesWith(e->mCollider);
	return last;
}

bool Entity::overlapsWith(Entity * e){
	return mCollider.overlapsWith(e->mCollider);
}

void Entity::translate(vec3 trans){
	mTrans += trans;
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
