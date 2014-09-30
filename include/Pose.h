#ifndef POSE_H
#define POSE_H

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/dual_quaternion.hpp>

#include "QuatVec.h"

using glm::fdualquat;
using glm::fquat;

class Pose{
public:
	Pose();
	Pose(std::vector<fdualquat> inQuats);
	Pose(std::vector<QuatVec> in);
	Pose(std::vector<fdualquat> in, float t, float dt);
	Pose operator*(const float& s);
	Pose operator+(const Pose& other);
	Pose blend(const Pose& other, float x);
	float * getPtr();
	float getTime();
	std::vector<glm::mat4> getMats();
private:
	std::vector<fdualquat> mJoints;
	std::vector<QuatVec> joints;
	float T, dt;
};

//	fdualquat createTransQuat(glm::vec3 trans);
//	fdualquat createRotQuat(glm::vec4 trans);

#endif
