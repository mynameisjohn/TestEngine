#include "Util.h"
#include "Pose.h"

Pose::Pose(){
	joints.resize(3);
}

void Pose::setMats(vector<mat4> matVec){
	if (matVec.size()==3)
		joints=matVec;
}

void Pose::addMat(mat4 mat){
	if (joints.size() < 3)
		joints.push_back(mat);
}

void Pose::addMat(vector<mat4> matVec){
	for (int i=0;i<matVec.size();i++)
		if (joints.size() < 3)
			joints.push_back(matVec[i]);
}

Pose Pose::operator*(const float& s){
	Pose ret=*this;
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

float * Pose::getPtr(){
	return (float *)joints.data();//glm::value_ptr(joints[0]);
}
