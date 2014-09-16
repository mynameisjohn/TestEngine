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
	void draw(mat4 parentMV);
	Cycle getCurrentCycle();
	Pose getCurrentPose();
private:
	vector<Cycle> cycles;
	vec2 u;
};

#endif
