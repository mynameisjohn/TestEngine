#ifndef UTIL_H
#define UTIL_H

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

#include <glm/glm.hpp>
#include <iostream>
#include <glm/gtx/dual_quaternion.hpp>

using glm::fdualquat;
using glm::fquat;

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

using namespace std;

int wrap(int dim, int num);
float min(float v1, float v2);
float max(float v1, float v2);
float clamp(float v, float m, float M);
float degToRad(float d);
float radToDeg(float r);
float lagrangeTime(float t0, float t1, float t);
float lerp(float x, float y, float a);

float min(vec3 v);
float max(vec3 v);

void printError(string name);
void fillVec(vec2& v, string s);
void fillVec(vec3& v, string s);
void fillVec(vec4& v, string s);

/*
template <typename T>
bool vecContains(vector<T>& vec, T v){
	vector<T>::iterator it;
	for (it=vec.begin();it!=vec.end();it++)
		if (*it == v)
			return true;
	return false;
}

template bool vecContains(vector<int>&, int);
*/
mat4 getAlignMat(vec3 x, vec3 y, vec3 z);
mat4 getAlignMat(vec3 u, char o);

fquat getQuatFromVec2(vec2 r);

template <typename T>
T remap(T v, T m1, T m2, T m3, T m4){
	return (((v-m1)/(m2-m1))*(m4-m3)+m3);
}

template float remap<float>(float, float, float, float, float);
template vec2 remap<vec2>(vec2,vec2,vec2,vec2,vec2);
template vec3 remap<vec3>(vec3,vec3,vec3,vec3,vec3);
template vec4 remap<vec4>(vec4,vec4,vec4,vec4,vec4);

fquat getRQ(vec4 rot);

fdualquat createDQ_t(glm::vec3 trans);
fdualquat createDQ_r(glm::fquat rot);
fdualquat createDQ_r(glm::vec4 trans);
mat4 getDQMat(const fdualquat&);
fdualquat dslerp(const fdualquat&, const fdualquat&, float);//const fdualquat& a, const fdualquat& b, float x);

vec3 getLagrangeInterpolants(float x);

ostream& operator<<(ostream& os, const vec2& vec);
ostream& operator<<(ostream& os, const vec3& vec);
ostream& operator<<(ostream& os, const vec4& vec);
ostream& operator<<(ostream& os, const mat4& mat);
ostream& operator<<(ostream& os, const fquat& quat);
ostream& operator<<(ostream& os, const fdualquat& dualquat);

#endif
