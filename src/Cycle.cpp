#include "Util.h"
#include "Cycle.h"

Cycle::Cycle(){
	poses = vector<Pose>();
}

Cycle::Cycle(vector<Pose> poseVec)
: poses(poseVec){
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

Cycle Cycle::operator*(const float s){
	Cycle ret(*this);
	for (int i=0;i<ret.poses.size();i++)
		ret.poses[i] = this->poses[i]*s;
	return ret;
}

Cycle Cycle::operator+(const Cycle& other){
	Cycle ret(*this);
	for (int i=0;i<ret.poses.size();i++)
		ret.poses[i] = this->poses[i]+other.poses[i];
	return ret;
}

Pose Cycle::blendPoses(unsigned int p1, unsigned int p2, float x){
	return poses[p1].blend(poses[p2],x);
}

//this is slow but works for now... just like everything else
Pose Cycle::getCurrentPose(float x){
	int p1, p2;
	p1 = (int)x;
	p2 = (p1+1)%poses.size();
	float s = x-(float)p1;
	p1 %= poses.size();
	return blendPoses(p1,p2,s);
}
