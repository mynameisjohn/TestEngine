#include "Util.h"
#include "Cycle.h"

Cycle::Cycle(){
	poses = vector<Pose>();
}

Cycle::Cycle(vector<Pose> poseVec)
: poses(poseVec){
}

Cycle::Cycle(vector<Pose> poseVec, unsigned int C)
: poses(poseVec), C(C){
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

int wrap(int dim, int num){
	return (dim+num)%dim;
}

//ok here goes some shit
Pose Cycle::collapsePoses(unsigned int C, float t){
	int top, bottom, center;
	bool red=true;

	bottom = -(C/2);
   top = C/2+(C%2!=0);
	center = (int)t;
	vector<Pose> tmp(top-bottom); 
	for (int i=bottom,k=0;i<top;i++,k++){
      int i1=center+i, i2=i1+1;
      i1 = ((poses.size()+i1)%poses.size());
      i2 = ((poses.size()+i2)%poses.size());
		tmp[k]=blendPoses(i1,i2,lagrangeTime(poses[i1].getTime(), poses[i2].getTime(), t));
	}

	for (int j=1;j<C;j++){
      for (int i=bottom+j/2, k=0;i<top-(j+1)/2;i++, k++){
         int t1 = center+i, t2 = t1+red+1;
         cout << j << "\t" << i << "\t" << t1 << "\t" << t2 << endl;
         t1 = ((poses.size()+t1)%poses.size());
         t2 = ((poses.size()+t2)%poses.size());
         tmp[k].blend(tmp[k+1],lagrangeTime(poses[t1].getTime(), poses[t2].getTime(), t));
      }
      red = !red;
   }

	return tmp[0];
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
