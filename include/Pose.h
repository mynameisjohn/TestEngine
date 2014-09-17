#ifndef POSE_H
#define POSE_H

#include <vector>
#include <glm/glm.hpp>

class Pose{
public:
	Pose();
	Pose(std::vector<glm::mat4>);
	void setMats(std::vector<glm::mat4> matVec);
	void addMat(glm::mat4 mat);
	void addMat(std::vector<glm::mat4> matVec);
	Pose operator*(const float& s);
	Pose operator+(const Pose& other);
	Pose blend(const Pose& other, float x);
	float * getPtr();
private:
	std::vector<glm::mat4> joints;
};

#endif
