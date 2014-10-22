#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "JShader.h"
#include "Util.h"

#include <vector>

class Drawable{
	public:
		Drawable(JShader * shader = nullptr, int mode=-1);
		virtual ~Drawable();
		void identity();
		void translate(GLfloat x, GLfloat y, GLfloat z);
		void setMV(mat4 newMatrix);
		void setVAO(GLuint VAO);
//		virtual void setTex(GLuint tex);
		void setCurTex(GLuint ct);
		void setMode(int mode);
		void setColor(float r, float g, float b, float a=1.f);
		void setColor(vec3 color);
		void leftMultMV(mat4 left);
		void setNElements(int n);
		//void uploadData(GLint MVHandle, GLint ColorHandle);
		void addChild(Drawable * child);
		virtual mat4 draw(mat4 parentMV, unsigned int curTex);//GLint MVHandle, GLint ColorHandle, mat4 parentMV);
		bool isVisible();
		void setVisibility(bool);
		int getNumElems();
		int addTex(GLuint tex);
		int addTex(vector<GLuint> texVec);
		GLuint getVAO();
		GLuint getTex();
		GLfloat * getMVPtr();
		GLfloat * getColorPtr();
		mat4 getMVMat();
		mat4 getMVInverse();
	protected:
		mat4 MV;
		vec4 mColor; //probably phase this out
		GLuint mVAO;//,mTex;
		vector<GLuint> mTexVec;
		GLuint cTex;
		bool visible;
		int mElementCount;
		int mMode;
		vector<Drawable *> children;
		JShader * mShader;
};

#endif
