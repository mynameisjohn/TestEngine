#include "QuatVec.h"
#include <glm/gtx/transform.hpp>
#include "Util.h"
using glm::fquat;
using glm::vec3;
using glm::mat4;
using glm::translate;
using glm::normalize;
using glm::slerp;
using glm::mix;

QuatVec::QuatVec()
: trans(), rot() , RT(false){
}

QuatVec::QuatVec(vec3 t, fquat r)
: trans(t), rot(normalize(r)), RT(false) {
}

QuatVec::QuatVec(fquat r, vec3 t)
: trans(t), rot(normalize(r)), RT(true) {
}

QuatVec QuatVec::operator * (const float s){
	return QuatVec(s*trans, s*rot);
}

//I suspect this will cause MASSIVE problems down the road...
QuatVec QuatVec::operator * (const QuatVec& other){
	bool rt = RT || other.RT; // this line in particular...
	return (rt ? QuatVec(rot*other.rot, trans+other.trans) : QuatVec(trans+other.trans, rot*other.rot));
}

QuatVec QuatVec::operator + (const QuatVec& other){
	return QuatVec(trans+other.trans, rot+other.rot);
}

void QuatVec::operator *= (const float s){
	this->trans *= s;
	this->rot *= s;
}
/*
QuatVec QuatVec::blend(const QuatVec& other, float x){
	return QuatVec(mix(trans, other.trans, x), slerp(rot, other.rot, x));
}
*/
QuatVec QuatVec::blend(const QuatVec& other, const float a){
	const float EPS = 0.001;
	vec3 T = glm::mix(trans, other.trans, a);
   fquat c(other.rot);

//cout << this->rot << "\t" << this->trans << "\n" << other.rot << "\t" << other.trans << endl;
   
	float cosTheta = glm::dot(rot, c);  
 
	if (cosTheta < 0){
      c = - c;
      cosTheta = -cosTheta;
   }
   if (cosTheta > (1 - EPS)){//if the interpolation angle is very small, just lerp
      return QuatVec(T, fquat(
         glm::mix(rot.w, c.w, a),
         glm::mix(rot.x, c.x, a),
         glm::mix(rot.y, c.y, a),
         glm::mix(rot.z, c.z, a)
      ));
   }

   // Essential Mathematics, page 467
   float angle = acos(cosTheta);
   return QuatVec(T,(rot*(float)sin(angle*(1-a))+c*(float)sin(a*angle))/(float)sin(angle));
}

QuatVec QuatVec::inverse(){
	return QuatVec(-trans, glm::inverse(rot));
}

mat4 QuatVec::toMat4(){
	return (RT ? glm::mat4_cast(rot)*glm::translate(trans) : glm::translate(trans)*glm::mat4_cast(rot));
}

