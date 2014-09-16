#ifndef POSE_H
#define POSE_H

#include <vector>
#include <glm/glm.hpp>

class Pose{
public:
	Pose();
	void setMats(std::vector<glm::mat4> matVec);
	void addMat(glm::mat4 mat);
	void addMat(std::vector<glm::mat4> matVec);
	Pose operator*(const float& s);
	Pose operator+(const Pose& other);
	float * getPtr();
private:
	std::vector<glm::mat4> joints;
};

#endif
