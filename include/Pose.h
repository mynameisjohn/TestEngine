#ifndef POSE_H
#define POSE_H

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/dual_quaternion.hpp>

using glm::fdualquat;
using glm::fquat;

class Pose{
public:
	Pose();
	Pose(std::vector<fdualquat> inQuats);
	Pose(std::vector<glm::mat4>);
	void setMats(std::vector<glm::mat4> matVec);
	void addMat(glm::mat4 mat);
	void addMat(std::vector<glm::mat4> matVec);
	Pose operator*(const float& s);
	Pose operator+(const Pose& other);
	Pose blend(const Pose& other, float x);
	float * getPtr();
	vector<mat4> getMats();
private:
	std::vector<glm::mat4> joints;
	std::vector<fdualquat> mJoints;
};

//	fdualquat createTransQuat(glm::vec3 trans);
//	fdualquat createRotQuat(glm::vec4 trans);

#endif
