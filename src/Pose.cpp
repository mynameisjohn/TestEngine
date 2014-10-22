#include "Util.h"
#include "Pose.h"

Pose::Pose()
: joints(3), T(1.f), dt(0.01f){}

Pose::Pose(vector<QuatVec> in, float delT)
: joints(in), dt(delT){}

Pose Pose::operator*(const float& s){
	Pose ret(this->joints);
	for (int i=0;i<ret.joints.size();i++)
		ret.joints[i] *= s; 
	return ret;
}

//Can I just add these?
Pose Pose::operator+(const Pose& other){
	Pose ret(this->joints);
	for(int i=0;i<joints.size();i++)
		ret.joints[i] = ret.joints[i]+other.joints[i];
	return ret;
}

Pose Pose::blend(const Pose& other, float x){
	vector<QuatVec> newJoints(joints.size());
	for (int i=0;i<newJoints.size();i++)
		newJoints[i]=joints[i].blend(other.joints[i], x);

	return Pose(newJoints, lerp(dt, other.dt, x));
}

void Pose::setDT(float dt){
	this->dt = dt;
}

int Pose::getNumJoints(){
	return joints.size();
}

float Pose::getDT(){
	return dt;
}

float Pose::maxDiff(const Pose& other){
	//s = cos(th/2)
	float s = (glm::conjugate(joints.back().rot)*other.joints.back().rot).w;
	
	return radToDeg(2.f*acos(s));//(glm::conjugate(joints.back().rot)*other.joints.back().rot).w));
}

//Maybe I can upload the quaternion rather than a matrix
vector<mat4> Pose::getMats(){
	vector<mat4> ret;
	vector<QuatVec>::iterator it;
	for (it=joints.begin();it!=joints.end();it++)
		ret.push_back(it->toMat4());
	return ret;
}
