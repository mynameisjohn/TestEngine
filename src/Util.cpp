#include "Util.h"

float min(float v1, float v2){
   return (v1 < v2 ? v1 : v2);
}
float max(float v1, float v2){
   return (v1 > v2 ? v1 : v2);
}
float clamp(float v, float m, float M){
   return min(max(v,m),M);
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
