#ifndef CAMERA_H
#define CAMERA_H

#include "BoundBox.h"

using glm::mat4;

class Camera{
	public:
		Camera();
		Camera(float b, mat4 p);
		void translate(vec3 trans);
		void push(glm::vec4 player);
		void setProjMat(mat4 p);
		void leftMult(mat4 m);
		void rightMult(mat4 m);
		mat4 getProjMat();
		void updateProj(int projHandle);
	private:
		BoundBox mBB;
		mat4 projMat;
		float buf;
};

#endif
