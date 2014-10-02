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

//I've read that float -> int is rather slow
Pose Cycle::collapsePoses(float& t){
	int top, bottom, center;
	bool red=true;
	if (poses.size() <= C+1)
		C = poses.size()-1;

	if (t>(float)poses.size())
		t=0.f;

	bottom = -(C/2);
   top = C/2+(C%2!=0);
	center = (int)t;
	vector<Pose> tmp(top-bottom); 
	for (int i=bottom,k=0;i<top;i++,k++){
      int i1=center+i, i2=i1+1;
		float x = lagrangeTime((float)i1, (float)i2, t);
//		cout << x << endl;
      i1 = wrap(poses.size(),i1);
      i2 = wrap(poses.size(),i2);
		tmp[k]=blendPoses(i1,i2,x);//lagrangeTime((float)i1, (float)i2, t));
	}

	for (int j=1;j<C;j++){
      for (int i=bottom+j/2, k=0;i<top-(j+1)/2;i++, k++){
         int t1 = center+i, t2 = t1+red+1;
			float x = lagrangeTime((float)t1, (float)(t2), t);
         if (t>0) 
				cout << center << "\t" << t << "\t" << t1 << "\t" << t2 << endl;
         t1 = wrap(poses.size(),t1);
         t2 = wrap(poses.size(),t2);
         tmp[k]=tmp[k].blend(tmp[k+1],x);//lagrangeTime((float)t1, (float)t2, t));
      }
      red = !red;
   }
	return tmp[0];
}

//this is slow but works for now... just like everything else
Pose Cycle::getCurrentPose(float& x){
	C=3;
	int p1, p2;
	p1 = (int)x;
	p2 = (p1+1)%poses.size();
	float s = x-(float)p1;
	p1 %= poses.size();
	return collapsePoses(x);
	return blendPoses(p1,p2,s);
}
