#include "Camera.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(){
	buf=200.f; 
	mBB = BoundBox(vec3(-1000,0,0),vec3(2000,0,0));
	projMat=glm::perspective((float)M_PI/4.f, 4.f/3, 100.f, 10000.f) *
	glm::rotate((float)M_PI/15.f, vec3(1, 0, 0)) * 
	glm::translate(vec3(0, -800,0));
}

Camera::Camera(float b, mat4 p){
	buf = b;
	projMat=p;
}

void Camera::translate(vec3 trans){
	mBB.translate(-trans);
	projMat = projMat * glm::translate(trans);
}

//what a strange method...
vec3 Camera::push(vec4 player){
	float dX;

	dX = (player.x-mBB.left());
	//printf("%lf\t",dX);	
	if (dX < buf){//don't sweat the small stuff...
		translate(vec3(player.w, 0, 0));
	}
	else{
		dX = (mBB.right() - player.x);
		//printf("%lf\n",dX);	
		if (dX < buf){
			translate(vec3(-player.w, 0, 0));
		}
	}

	vec4 playerPos = projMat*vec4(player.x,player.y,player.z,1);
	//cout << playerPos/playerPos.w << endl;

	return vec3(playerPos)/playerPos.w;
/*
	playerPos /= playerPos.w;

	//returns true of the mouse is to the player's right
	return (playerPos.x - sp.x < 0);
*/
}

void Camera::setProjMat(mat4 p){
	projMat = p;
}

void Camera::leftMult(mat4 m){
	projMat = m * projMat;
}

void Camera::rightMult(mat4 m){
	projMat = projMat * m;
}

mat4 Camera::getProjMat(){
	return projMat;
}

void Camera::updateProj(GLint projHandle){
	glUniformMatrix4fv(projHandle, 1, GL_FALSE, glm::value_ptr(projMat));
}
