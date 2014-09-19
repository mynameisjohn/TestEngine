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

float min(float v1, float v2);
float max(float v1, float v2);
float clamp(float v, float m, float M);

fdualquat createDQ_t(glm::vec3 trans);
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
