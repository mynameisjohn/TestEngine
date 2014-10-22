#include <GL/glew.h>
#include "Drawable.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

Drawable::Drawable(JShader * shader, int mode)
:	cTex(0), MV(), mColor(1,1,1,1), visible(true), mShader(shader), mMode(mode)
{
	children.clear();
	mTexVec.clear();
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
	//This line negates any parent transforms at the start - 
	//it may seem like an error, but I like the control this gives me

	child->leftMultMV(glm::inverse(MV));
	children.push_back(child);
}

void Drawable::setVisibility(bool b){
	visible = b;
}

void Drawable::setMV(mat4 newMatrix){
	MV = newMatrix;
}

void Drawable::identity(){
	MV = mat4();
}

void Drawable::setColor(float r, float g, float b, float a){
	mColor = {r,g,b,a};
}

void Drawable::setColor(vec3 color){
	mColor = vec4(color, 1.f);
}

void Drawable::setVAO(GLuint VAO){
	mVAO = VAO;
}

int Drawable::addTex(GLuint tex){
	if (mMode < 0)
		mMode = 0;
	mTexVec.push_back(tex);
	return mTexVec.size()-1;
}

int Drawable::addTex(vector<GLuint> texVec){
	for (int i=0;i<texVec.size();i++)
		addTex(texVec[i]);
	return mTexVec.size()-1;
}

/*
void Drawable::setTex(GLuint tex){
	mTexVec[0] = tex;
	mMode = 2;
}
*/
void Drawable::leftMultMV(mat4 left){
	MV = left * MV;
}

void Drawable::setNElements(int n){
	mElementCount = n;
}

mat4 Drawable::draw(mat4 parentMV, unsigned int curTex){//GLint MVHandle, GLint ColorHandle, mat4 parentMV){
	if (!visible)
		return mat4();
	
	//Find inherited transform
	mat4 transform = parentMV * MV;
	
	//Upload data to device
	glUniformMatrix4fv(mShader->getMVHandle(), 1, GL_FALSE, glm::value_ptr(transform));
	glUniform4fv(mShader->getColorHandle(), 1, glm::value_ptr(mColor));
	glUniform1i(mShader->getModeHandle(), mMode);

	//Bind Texture/VAO and Draw
	if (mTexVec.size())
		glBindTexture(GL_TEXTURE_2D, mTexVec[0]); //Make my texture active
	glBindVertexArray(mVAO); //Bind my VAO
	glDrawElements(GL_TRIANGLE_STRIP, mElementCount, GL_UNSIGNED_INT, NULL);
/*
	//Recursively draw children
	vector<Drawable *>::iterator childIt;
	for (childIt=children.begin(); childIt!=children.end(); childIt++){
		(*childIt)->draw(transform, curTex);
	}
*/	
	return transform;
}

bool Drawable::isVisible(){
	return visible;
}

int Drawable::getNumElems(){
	return mElementCount;
}

void Drawable::setCurTex(GLuint ct){
	cTex = wrap(mTexVec.size(), ct);
}

GLuint Drawable::getVAO(){
	return mVAO;
}

GLuint Drawable::getTex(){
	return mTexVec.back();
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

mat4 Drawable::getMVInverse(){
	return glm::inverse(MV);
}
