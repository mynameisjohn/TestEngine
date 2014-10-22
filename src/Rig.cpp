#include "Rig.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Rig::Rig(JShader * shader)
: Drawable(shader, 1), from(0), to(0), numPoses(0), u(0,0)
{}

Rig::~Rig(){
	children.clear();
	mVAO=0;
}

void Rig::setCycles(vector<Cycle> cycleVec){
	uint8_t i=0;
	//ensure a consisten number of poses across cycles
	uint8_t nP = cycleVec[i++].getNumPoses();
	for (i; i<cycleVec.size(); i++){
		if (nP != cycleVec[i].getNumPoses()){
			cout << "Inconsistent number of poses. Segfault iminent." << endl;
			return;
		}
	}
	
	numPoses = (unsigned int)nP;
	cycles=cycleVec;
	mMode = cycles[0].getNumJoints();
}
/*
void Rig::setTex(GLuint tex){
	mTexVec[0] = tex;
}
*/
void Rig::addCycle(Cycle c){
	if (cycles.size() < 3)
		cycles.push_back(c);
}

void Rig::addCycle(vector<Cycle> cycleVec){
	for (int i=0;i<cycleVec.size();i++)
		if (cycles.size() < 3)
			cycles.push_back(cycleVec[i]);
}

//can't do this yet
void Rig::shift(){
	u.y += (float)(cycles[0].getNumPoses())/2.f;
}

float Rig::getShift(){
	return (float)(cycles[0].getNumPoses())/2.f;
}

//These two methods are useless and will be phased out asap
void Rig::set_u(vec2 u){
	this->u=u;//glm::clamp(u,vec2(-1,-1), vec2(1,1));
}

void Rig::set_pose(float t){
	u.y=t;
}

void Rig::set_to(unsigned int t){
	if (t < cycles.size())
		to = t;
}
/*
//How do I get c where it needs to be?
//only deal with DT Here
void Rig::inc_u(float c){
	const float dt = .1f;
	float dx = dt*(c-u.x);
//	set_u({clamp(u.x+dx,-1,1), u.y+.1f}); This line causes a bug where the run cycle gets chosen(only for arm?)
	set_u({c, u.y+.1f});
}
*/

mat4 Rig::draw(mat4 parentMV, unsigned int curTex, vec2& u, unsigned int from, unsigned int to){
	vector<mat4> rigData;
   if (from == to)
      rigData = cycles[from].getCurrentPose(u.y).getMats();
   else
      rigData = cycles[from].blend(cycles[to], u.x).getCurrentPose(u.y).getMats();

	//Find inherited transform
   mat4 transform = parentMV * MV;
	
   glUniformMatrix4fv(mShader->getMVHandle(), 1, GL_FALSE, glm::value_ptr(transform));
	glUniformMatrix4fv(mShader->getRigMatHandle(), numPoses, GL_FALSE, (GLfloat *)rigData.data());
   glUniform4fv(mShader->getColorHandle(), 1, glm::value_ptr(mColor));
   glUniform1i(mShader->getModeHandle(), mMode);
   
   //Bind Texture/VAO and Draw
	if (mTexVec.size())
	   glBindTexture(GL_TEXTURE_2D, mTexVec[curTex]); //Make my texture active
   glBindVertexArray(mVAO); //Bind my VAO
   glDrawElements(GL_TRIANGLE_STRIP, mElementCount, GL_UNSIGNED_INT, NULL);

	return transform * rigData.back();
}

mat4 Rig::draw(mat4 parentMV, unsigned int curTex){
	if (!visible)
		return mat4();

//	cout << u << endl;

	//Find inherited transform
   mat4 transform = parentMV * MV;
   //Upload data to device
//	vector<mat4> rigData = getCurrentPose().getMats();
   glUniformMatrix4fv(mShader->getMVHandle(), 1, GL_FALSE, glm::value_ptr(transform));
//	glUniformMatrix4fv(mShader->getRigMatHandle(), numPoses, GL_FALSE, (GLfloat *)rigData.data());
   glUniform4fv(mShader->getColorHandle(), 1, glm::value_ptr(mColor));
   glUniform1i(mShader->getModeHandle(), 0);//mMode);
   
   //Bind Texture/VAO and Draw
	if (mTexVec.size())
	   glBindTexture(GL_TEXTURE_2D, mTexVec[curTex]); //Make my texture active
   glBindVertexArray(mVAO); //Bind my VAO
   glDrawElements(GL_TRIANGLE_STRIP, mElementCount, GL_UNSIGNED_INT, NULL);
/*
   //Recursively draw children
   vector<Drawable *>::iterator childIt;
   for (childIt=children.begin(); childIt!=children.end(); childIt++)
      (*childIt)->draw(transform, *rigData.back(), curTex);
*/
	return transform;// * rigData.back();
}


Pose Rig::getCurrentPose(){
//	cout << from << "\t" << to << endl;
	const float eps = 0.001f;
	const float dt = 0.1f;	
	if (1.f-u.x < eps){
		from = to;
		u.x=0.f;
	}
	if (from == to)
		return cycles[from].getCurrentPose(u.y);
	else{
		u.x += dt;
		return cycles[from].blend(cycles[to], u.x).getCurrentPose(u.y);
	}
}

/*
//I'm going to phase this out...it was a nice idea but not worth it in the end
//(I suspect it's incompatible with my interpolation
Cycle Rig::getCurrentCycle(){
	vec3 L = getLagrangeInterpolants(u.x);

   return cycles[0]*L.x+cycles[1]*L.y+cycles[2]*L.z;
}

Pose Rig::getCurrentPose(){
//	cout << u << endl;
	return getCurrentCycle().getCurrentPose((u.y));
}
*/
