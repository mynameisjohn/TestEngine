#include "Util.h"
#include "Pose.h"

Pose::Pose()
: joints(3), T(1.f), dt(0.01f){}
/*
Pose::Pose(vector<QuatVec> in, float t, float dt)
: joints(in), T(t), dt(dt){}
*/
Pose::Pose(vector<QuatVec> in)
: joints(in), dt(0.01f){}

Pose Pose::operator*(const float& s){
	Pose ret(this->joints);
	for (int i=0;i<ret.joints.size();i++)
		ret.joints[i] *= s; 
	return ret;
}

//Can I just add these?
Pose Pose::operator+(const Pose& other){
	Pose ret(this->joints);
	vector<QuatVec>::iterator qvIt;
	for(int i=0;i<joints.size();i++)
		ret.joints[i] = ret.joints[i]+other.joints[i];
	return ret;
}

Pose Pose::blend(const Pose& other, float x){
	return Pose({
		joints[0].blend(other.joints[0], x),
		joints[1].blend(other.joints[1], x),
		joints[2].blend(other.joints[2], x)
	});
}

void Pose::setDT(float dt){
	this->dt = dt;
}

float Pose::maxDiff(const Pose& other){
	return radToDeg(2*acos((glm::conjugate(joints.back().rot)*other.joints.back().rot).w));
}

//Maybe I can upload the quaternion rather than a matrix
vector<mat4> Pose::getMats(){
	vector<mat4> ret;
	vector<QuatVec>::iterator it;
	for (it=joints.begin();it!=joints.end();it++)
		ret.push_back(it->toMat4());
	return ret;
}
