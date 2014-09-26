#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "JShader.h"
#include "Util.h"

#include <vector>

class Drawable{
	public:
		Drawable(JShader * shader = nullptr, int mode=0x1);
		virtual ~Drawable();
		void identity();
		void translate(GLfloat x, GLfloat y, GLfloat z);
		void setMV(mat4 newMatrix);
		void setVAO(GLuint VAO);
		virtual void setTex(GLuint tex);
		void setMode(int mode);
		void setColor(float r, float g, float b);
		void setColor(vec3 color);
		void leftMultMV(mat4 left);
		void setNElements(int n);
		//void uploadData(GLint MVHandle, GLint ColorHandle);
		void addChild(Drawable * child);
		virtual void draw(mat4 parentMV);//GLint MVHandle, GLint ColorHandle, mat4 parentMV);
		bool isVisible();
		int getNumElems();
		GLuint getVAO();
		GLuint getTex();
		GLfloat * getMVPtr();
		GLfloat * getColorPtr();
		mat4 getMVMat();
	protected:
		mat4 MV;
		vec4 mColor; //probably phase this out
		GLuint mVAO, mTex;
		bool visible;
		int mElementCount;
		int mMode;
		vector<Drawable *> children;
		JShader * mShader;
};

#endif
