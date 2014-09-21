#ifndef CYCLE_H
#define CYCLE_H

#include "Pose.h"

class Cycle{
public:
   Cycle();
	Cycle(std::vector<Pose> poseVec);
	void setPoses(std::vector<Pose> poseVec);
	void addPose(Pose p);
	void addPose(std::vector<Pose> poseVec);
   Cycle operator*(const float s);
   Cycle operator+(const Cycle& other);
   float * getPtr();
	Pose blendPoses(unsigned int p1, unsigned int p2, float x);
   Pose getCurrentPose(float v);
private:
	unsigned int cp;
   std::vector<Pose> poses;
};

#endif
