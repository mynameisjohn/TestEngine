#ifndef LIGAMENT_H
#define LIGAMENT_H

#include <memory>
#include <vector>
#include <list>

#include "Drawable.h"
#include "QuatVec.h"

#include <map>

const int L_CYCLIC(0), L_ONCE(1), L_ACTIVE(2);

class Ligament{
public:
	Ligament();
	Ligament(Drawable * dPtr, string cTex="");
	int addChild(int childOffset, bool invert=true);
	void draw(glm::mat4 parent);
	void setCurTex(string ct);
	void set_u(vec2 U);
	void set_cycle(string s);
	void set_to(string s);//unsigned int t);
	void setColor(vec4 c){mColor = c;};
	void setState(int state);
	void setActive(bool b=true);
	void shift();
	void update();
	void resetTransform();
	void applyTransform(QuatVec qv);
	void applyTransform(vec3 t);
	void applyTransform(fquat q);
	void leftMultMV(mat4 m);
	bool hasChild(Ligament * l);
	vec3 getOrigin(int idx=0);
	QuatVec getTransform();
	mat4 getTransformAsMat4();
	mat4 getMV();
	Drawable * getDrPtr();
private:
	Drawable * mDrawable;
	vector<int> children;
	mat4 MV;
	list<QuatVec> mTransform;
	vec2 u;
	string cTex, from, to;
	vec4 mColor;
	bool active;
	vector<vec4> origins;
	int state;
};

#endif
