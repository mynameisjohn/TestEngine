#ifndef QUATVEC_H
#define QUATVEC_H

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class QuatVec{
public:
	glm::vec3 trans;
	glm::fquat rot;
	QuatVec();
	QuatVec(glm::vec3, glm::fquat);
	QuatVec operator * (const float s);
	QuatVec operator * (const QuatVec& other);
	QuatVec operator + (const QuatVec& other);
	void operator *= (const float s);
	QuatVec blend(const QuatVec& other, float a);
	glm::mat4 toMat4();
};

#endif
