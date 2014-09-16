#include <GL/glew.h>
#include "Drawable.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

Drawable::Drawable(JShader * shader, int mode){
	MV = mat4();
	mColor = {1.f, 1.f, 1.f, 1.f};
	visible = true;
	mElementCount=4;
	children.clear();
	mShader = shader;
	mMode=mode;
}

Drawable::~Drawable(){
	children.clear();
	mVAO=0;
}

void Drawable::addChild(Drawable * child){
	vector<Drawable *>::iterator drChildIt;
	for (drChildIt=child->children.begin();drChildIt!=child->children.end();drChildIt++){
		if (this == *drChildIt){
			cout << "Drawable: cycle created!" << endl;
			return;
		}
	}
	children.push_back(child);
}


void Drawable::setMV(mat4 newMatrix){
	MV = newMatrix;
}

void Drawable::identity(){
	MV = mat4();
}

void Drawable::setColor(float r, float g, float b){
	mColor = {r,g,b,1.f};
}

void Drawable::setColor(vec3 color){
	mColor = vec4(color, 1.f);
}

void Drawable::setVAO(GLuint VAO){
	mVAO = VAO;
}

void Drawable::setTex(GLuint tex){
	mTex = tex;
}

void Drawable::leftMultMV(mat4 left){
	MV = left * MV;
}

void Drawable::setNElements(int n){
	mElementCount = n;
}
/*
	if (mMode==3){
		static float osc = 0.f;
		float mod = 0.5f*sin(osc)+0.5f;
		
		vector<mat4> rigMats(3);
		osc += 0.125f;  mod = sin(osc);

		rigMats[0] = (float)(-0.5f*sin(osc)*(1-sin(osc)))*glm::rotate(1.57f/12.f, vec3(0,0,1))*glm::translate(vec3(.0f,0.f,0.f))+
						(float)(1-pow(sin(osc),2))*mat4()+
						(float)(0.5f*sin(osc)*(1+sin(osc)))*glm::translate(vec3(-0.f,0.0f,0.f))*glm::rotate(-1.57f/15.f, vec3(0,0,1));

		rigMats[1] = (float)(-0.5f*sin(osc)*(1-sin(osc)))*glm::rotate(1.57f/4.f, vec3(0,0,1))*glm::translate(vec3(.1f,0.f,0.f))+
						(float)(1-pow(sin(osc),2))*mat4()+
						(float)(0.5f*sin(osc)*(1+sin(osc)))*glm::translate(vec3(-0.2f,0.0f,0.f))*glm::rotate(-1.57f/4.f, vec3(0,0,1));

		rigMats[2] = (float)(-0.5f*sin(osc)*(1-sin(osc)))*glm::rotate(1.57f/10.f, vec3(0,0,1))*glm::translate(vec3(.1f,0.f,0.f))+
						(float)(1-pow(sin(osc),2))*mat4()+
						(float)(0.5f*sin(osc)*(1+sin(osc)))*glm::translate(vec3(-0.2f,0.2f,0.f))*glm::rotate(-1.57f/3.f, vec3(0,0,1));

		glUniformMatrix4fv(mShader->getRigMatHandle(),rigMats.size(),GL_FALSE,(glm::value_ptr(rigMats[0])));
	}
*/
void Drawable::draw(mat4 parentMV){//GLint MVHandle, GLint ColorHandle, mat4 parentMV){
	//Find inherited transform
	mat4 transform = parentMV * MV;
	
	//Upload data to device
	glUniformMatrix4fv(mShader->getMVHandle(), 1, GL_FALSE, glm::value_ptr(transform));
	glUniform4fv(mShader->getColorHandle(), 1, glm::value_ptr(mColor));
	glUniform1i(mShader->getModeHandle(), mMode);
	
	//Bind Texture/VAO and Draw
	glBindTexture(GL_TEXTURE_2D, mTex); //Make my texture active
	glBindVertexArray(mVAO); //Bind my VAO
	glDrawElements(GL_TRIANGLE_STRIP, mElementCount, GL_UNSIGNED_INT, NULL);

	//Recursively draw children
	vector<Drawable *>::iterator childIt;
	for (childIt=children.begin(); childIt!=children.end(); childIt++){
		(*childIt)->draw(transform);
	}
}

bool Drawable::isVisible(){
	return visible;
}

int Drawable::getNumElems(){
	return mElementCount;
}

GLuint Drawable::getVAO(){
	return mVAO;
}

GLuint Drawable::getTex(){
	return mTex;
}

GLfloat * Drawable::getMVPtr(){
	return glm::value_ptr(MV);
}

GLfloat * Drawable::getColorPtr(){
	return glm::value_ptr(mColor);
}

mat4 Drawable::getMVMat(){
	return MV;
}
