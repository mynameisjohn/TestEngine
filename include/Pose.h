#ifndef POSE_H
#define POSE_H

#include <vector>
#include <glm/glm.hpp>

class Pose{
public:
	Pose();
	Pose operator*(const float& s);
	Pose operator+(const Pose& other);
	float * getPtr();
private:
	std::vector<glm::mat4> joints;
};

#endif
