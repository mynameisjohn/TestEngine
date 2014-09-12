#ifndef DRAWABLE_H
#define DRAWABLE_H
#include "JShader.h"
//#include <GL/gl.h>

#ifndef GLM_FORCE_RADIANS 
#define GLM_FORCE_RADIANS 
#endif

#include <glm/glm.hpp>
#include <vector>

using glm::mat4;
using glm::vec3;

class Drawable{
	public:
		Drawable(JShader * shader = nullptr, int mode=0x1);
		~Drawable();
		void identity();
		void translate(GLfloat x, GLfloat y, GLfloat z);
		void setPos(vec3 pos);
		void setMV(glm::mat4 newMatrix);
		void setVAO(GLuint VAO);
		void setTex(GLuint tex);
		void setMode(int mode);
		void setColor(float r, float g, float b);
		void setColor(vec3 color);
		void leftMultMV(glm::mat4 left);
		void setNElements(int n);
		void uploadData(GLint MVHandle, GLint ColorHandle);
		void addChild(Drawable * child);
		void draw(mat4 parentMV);//GLint MVHandle, GLint ColorHandle, mat4 parentMV);
		bool isVisible();
		int getNumElems();
		GLuint getVAO();
		GLuint getTex();
		GLfloat * getMVPtr();
		GLfloat * getColorPtr();
		glm::mat4 getMVMat();
	private:
		glm::mat4 MV;
		glm::vec4 mColor; //probably phase this out
		glm::vec3 mPos;
		GLuint mVAO, mTex;
		bool visible;
		int mElementCount;
		int mMode;
		std::vector<Drawable *> children;
		JShader * mShader;
};

#endif
