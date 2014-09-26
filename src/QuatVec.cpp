#include "QuatVec.h"
#include <glm/gtx/transform.hpp>

using glm::fquat;
using glm::vec3;
using glm::mat4;
using glm::translate;
using glm::normalize;
using glm::slerp;
using glm::mix;

QuatVec::QuatVec()
: trans(), rot() {}

QuatVec::QuatVec(vec3 t, fquat r)
: trans(t), rot(normalize(r)) {}

QuatVec QuatVec::operator * (const float s){
	return QuatVec(s*trans, s*rot);
}

QuatVec QuatVec::operator * (const QuatVec& other){
	return QuatVec(trans+other.trans, rot*other.rot);
}

QuatVec QuatVec::operator + (const QuatVec& other){
	return QuatVec(trans+other.trans, rot+other.rot);
}

void QuatVec::operator *= (const float s){
	this->trans *= s;
	this->rot *= s;
}

QuatVec QuatVec::blend(const QuatVec& other, float x){
	return QuatVec(mix(trans, other.trans, x), slerp(rot, other.rot, x));
}

mat4 QuatVec::toMat4(){
	return glm::translate(trans)*glm::mat4_cast(rot);
}
