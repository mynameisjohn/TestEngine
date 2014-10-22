#ifndef CAMERA_H
#define CAMERA_H

#include "BoundBox.h"

using glm::mat4;

class Camera{
	public:
		Camera();
		Camera(float b, mat4 p);
		void translate(vec3 trans);
		void setProjMat(mat4 p);
		void leftMult(mat4 m);
		void rightMult(mat4 m);
		void updateProj(int projHandle);
		vec3 push(glm::vec4 player);
		mat4 getProjMat();
	private:
		BoundBox mBB;
		mat4 projMat;
		float buf;
};

#endif
