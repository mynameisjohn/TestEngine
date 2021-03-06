#include "Rig.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Rig::Rig(JShader * shader)
: Drawable(shader, 3){
}

Rig::~Rig(){
	children.clear();
	mVAO=0;
}

void Rig::setCycles(vector<Cycle> cycleVec){
	if (cycleVec.size()==3)
		cycles=cycleVec;
}

void Rig::setTex(GLuint tex){
	mTex = tex;
}

void Rig::addCycle(Cycle c){
	if (cycles.size() < 3)
		cycles.push_back(c);
}

void Rig::addCycle(vector<Cycle> cycleVec){
	for (int i=0;i<cycleVec.size();i++)
		if (cycles.size() < 3)
			cycles.push_back(cycleVec[i]);
}

//These two methods are useless and will be phased out asap
void Rig::set_u(vec2 u){
	this->u=u;//glm::clamp(u,vec2(-1,-1), vec2(1,1));
}

//How do I get c where it needs to be?
//only deal with DT Here
void Rig::inc_u(float c){
	const float dt = .35f;
	float dx = dt*(c-u.x);
//	set_u({clamp(u.x+dx,-1,1), u.y+.1f}); This line causes a bug where the run cycle gets chosen(only for arm?)
	set_u({c, u.y+.1f});
}

void Rig::draw(mat4 parentMV){
	//Find inherited transform
   mat4 transform = parentMV * MV;
   //Upload data to device
	vector<mat4> rigData = getCurrentPose().getMats();
   glUniformMatrix4fv(mShader->getMVHandle(), 1, GL_FALSE, glm::value_ptr(transform));
	glUniformMatrix4fv(mShader->getRigMatHandle(),3,GL_FALSE,(GLfloat *)rigData.data());//getCurrentPose().getPtr());
   glUniform4fv(mShader->getColorHandle(), 1, glm::value_ptr(mColor));
   glUniform1i(mShader->getModeHandle(), mMode);
   
   //Bind Texture/VAO and Draw
   glBindTexture(GL_TEXTURE_2D, mTex); //Make my texture active
   glBindVertexArray(mVAO); //Bind my VAO
   glDrawElements(GL_TRIANGLE_STRIP, mElementCount, GL_UNSIGNED_INT, NULL);

   //Recursively draw children
   vector<Drawable *>::iterator childIt;
   for (childIt=children.begin(); childIt!=children.end(); childIt++)
      (*childIt)->draw(transform);
}

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
