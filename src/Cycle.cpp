#include "Cycle.h"
#include "Util.h"
#include <glm/gtx/transform.hpp>

Cycle::Cycle(){
	poses.resize(3);
}

void Cycle::setPoses(vector<Pose> poseVec){
	if (poseVec.size()==3)
		poses=poseVec;
}

void Cycle::addPose(Pose p){
	if (poses.size() < 3)
		poses.push_back(p);
}

void Cycle::addPose(vector<Pose> poseVec){
	for (int i=0;i<poseVec.size();i++)
		if (poses.size() < 3)
			poses.push_back(poseVec[i]);
}

Cycle Cycle::operator*(const float& s){
	Cycle ret = *this;
	for (int i=0;i<ret.poses.size();i++)
		ret.poses[i] = ret.poses[i]*s;
	return ret;
}

Cycle Cycle::operator+(const Cycle& other){
	Cycle ret;
	for (int i=0;i<ret.poses.size();i++)
		ret.poses[i] = this->poses[i]+other.poses[i];
	return ret;
}

Pose Cycle::getCurrentPose(float v){
	vec3 L = getLagrangeInterpolants(v);
	return poses[0]*L.x+poses[1]*L.y+poses[2]*L.z;
}
