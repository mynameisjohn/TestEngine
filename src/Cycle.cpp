#include "Cycle.h"
#include "Util.h"

Cycle::Cycle(){
	poses = vector<Pose>(3);
}

Cycle Cycle::operator*(const float& s){
	Cycle ret;
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
