#include "Util.h"
#include "Cycle.h"

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

Pose Cycle::blendPoses(unsigned int p1, unsigned int p2, float x){
	return poses[p1].blend(poses[p2],x);
}

Pose Cycle::getCurrentPose(float& x){
	const float x1=-1.f, x2=0.f, x3=1.f, x4=2.f;
	unsigned int p1=0, p2=1;
	if (x>=x4)
      x=x1;
   if (x1 <= x && x < x2)
		return blendPoses(0,1,x-x1);
   else if (x2 <= x && x < x3)
		return blendPoses(1,2,x-x2);
   else if (x3 <= x && x < x4)
		return blendPoses(2,0,x-x3);

	return blendPoses(p1,p2,0.f);
      //blend


/*
//	vec3 L = getLagrangeInterpolants(v);
	float v=x;vec3 L;
	//do something about this nonsense pleas
	//do something about this nonsense pleasee
//	if (v>x4) v=x1;
	L.x=((v-x2)*(v-x3)*(v-x4))/((x1-x2)*(x1-x3)*(x1-x4));
	L.y=((v-x1)*(v-x3)*(v-x4))/((x2-x1)*(x2-x3)*(x2-x4));
	L.z=((v-x2)*(v-x1)*(v-x4))/((x3-x1)*(x3-x2)*(x3-x4));
	L.x+=((v-x2)*(v-x3)*(v-x1))/((x4-x1)*(x4-x3)*(x4-x2));

	L = getLagrangeInterpolants(sin(v));
	return poses[0]*L.x+poses[1]*L.y+poses[2]*L.z;
*/
}
