#ifndef LIGAMENT_H
#define LIGAMENT_H

#include <memory>
#include <vector>
#include <list>

#include "Drawable.h"
#include "QuatVec.h"

#include <map>

class Ligament{
public:
	Ligament();
	Ligament(Drawable * dPtr, QuatVec qv = QuatVec());
//	Ligament(std::unique_ptr<Drawable> dPtr, QuatVec qv = QuatVec());
//	Ligament(Drawable d, QuatVec qv = QuatVec());
	void addChild(Ligament * child);
	void addChild(int childOffset);
	void addChild(string child);
	void addChild(map<string,Ligament>::iterator it);//Ligament * child);
	void draw(glm::mat4 parent);
	void setCurTex(unsigned int ct);
	void leftMultMV(glm::mat4 m);
	void set_u(vec2 U);
	void set_to(unsigned int t);
	void setActive(bool b=true);
	void setColor(vec3 v);
	void setColor(float r, float g, float b, float a=1.f);
	void setRotate(vec4 R);//{mQV.rot = getRQ(R);}
	void setTranslate(vec3 t);//{mQV.rot = getRQ(R);}
	void setQuatVec(QuatVec qv);//vec4 R){mQV.rot = getRQ(R);}
	void shift();
	void update();
	void resetTransform();
	void applyTransform(QuatVec qv);
	void applyTransform(vec3 t);
	void applyTransform(fquat q);
	bool hasChild(Ligament * l);
	QuatVec getTransform();
//	unique_ptr<Drawable> mDrawable;
	Drawable * mDrawable;
private:
	//Drawable * mDrawable;
//	vector<Ligament *> children;
//	vector<map<string, Ligament>::iterator> cVec;
	vector<int> children;
	QuatVec mQV;
	list<QuatVec> mTransform;
	vec2 u;
	unsigned int curTex, from, to;
	bool active;
};

#endif
