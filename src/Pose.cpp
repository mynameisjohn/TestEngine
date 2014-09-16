#include "Util.h"

#include "Pose.h"

Pose Pose::operator*(const float& s){
	Pose ret;
	for (int i=0;i<ret.joints.size();i++)
		ret.joints[i] *= s;
	return ret;
}

Pose Pose::operator+(const Pose& other){
	Pose ret;
	for (int i=0;i<ret.joints.size();i++)
		ret.joints[i]=this->joints[i]+other.joints[i];
	return ret;
}

Pose::Pose(){
	joints = vector<mat4>(3);
}

float * Pose::getPtr(){
	return (float *)joints.data();//glm::value_ptr(joints[0]);
}
