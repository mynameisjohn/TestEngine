#include "Util.h"

#include <glm/gtx/transform.hpp>

float min(float v1, float v2){
   return (v1 < v2 ? v1 : v2);
}
float max(float v1, float v2){
   return (v1 > v2 ? v1 : v2);
}
float clamp(float v, float m, float M){
   return min(max(v,m),M);
}

float degToRad(float d){
	return (float)d*M_PI/180.f;
}

float radToDeg(float r){
	return (float)r*180./M_PI;
}

fdualquat createDQ_t(vec3 trans){
	return fdualquat({1,0,0,0},{0,vec3(0.5f*trans)});
}

fdualquat createDQ_r(fquat rot){
	cout << rot << endl;
	vec3 axis = glm::normalize(vec3(rot.x, rot.y, rot.z));
	float angle = 0.5f*degToRad(rot.w);
	return fdualquat({cos(angle),sin(angle)*axis},{0,vec3()});
}

fdualquat createDQ_r(vec4 rot){
	vec3 axis = glm::normalize(vec3(rot.y, rot.z, rot.w));
	float angle = 0.5f*degToRad(rot.x);
	//rot.w *= 0.5f;
	return fdualquat({cos(angle),sin(angle)*axis},{0,vec3()});
	//return fdualquat({cos(rot.w),sin(rot.w)*axis},{0,vec3()});
}

mat4 getDQMat(const fdualquat& Q){
	return glm::translate(glm::axis(2.f*Q.dual*glm::conjugate(Q.real)))*glm::mat4_cast(Q.real);
}

fdualquat dslerp(const fdualquat& a, const fdualquat& b, float x){//const fdualquat& a, const fdualquat& b, float x){
	return glm::normalize(fdualquat(glm::mix(a.real,b.real, x),glm::lerp(a.dual,b.dual, x)));
}

vec3 getLagrangeInterpolants(float x){
   x = clamp(x,-1.f,1.f);
   return vec3(-0.5f*x*(1.f-x), 1.f-x*x, 0.5f*x*(1.f+x));
}

ostream& operator<<(ostream& os, const vec2& vec){
	os << "{" << vec.x << ", " << vec.y << "}";
   return os;
}

ostream& operator<<(ostream& os, const vec3& vec){
  os << "{" << vec.x << ", " << vec.y << ", " << vec.z << "}";
  return os;
}

ostream& operator<<(ostream& os, const vec4& vec){
  os << "{" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << "}";
  return os;
}

ostream& operator<<(ostream& os, const mat4& mat){
  os << "{\n" << mat[0] << ",\n" << mat[1] << ",\n" << mat[2] << ",\n" << mat[3] << ",\n}";
  return os;
}

ostream& operator<<(ostream& os, const fquat& quat){
  os << "{" << quat.w << ", " << quat.x << ", " << quat.y << ", " << quat.z << "}";
  return os;
}

ostream& operator<<(ostream& os, const fdualquat& dualquat){
  os << "{" << dualquat.real << ", " << dualquat.dual << "}";//mat[0] << ",\n" << mat[1] << ",\n" << mat[2] << ",\n" << mat[3] << ",\n}";
  return os;
}
