#ifndef RIG_H
#define RIG_H

#include "Drawable.h"
#include "Cycle.h"

class Rig : public Drawable{
public:
	Rig(JShader * shader = nullptr);
	~Rig();
	void setCycles(vector<Cycle> cycleVec);
	void addCycle(Cycle c);
	void addCycle(vector<Cycle> cycleVec);
	void inc_u(float c);
	void set_u(vec2 u);
	mat4 draw(mat4 parentMV, unsigned int curTex, vec2& u, unsigned int from, unsigned int to);
	mat4 draw(mat4 parentMV, unsigned int curTex=0);
//	void setTex(GLuint tex);
	void set_to(unsigned int t);
	void set_pose(float t);
	void shift();
	float getShift();
	Cycle getCurrentCycle();
	Pose getCurrentPose();
private:
	vector<Cycle> cycles;
	vec2 u;
//	float u, v;
	unsigned int numPoses;
	unsigned int from, to;
};

#endif
