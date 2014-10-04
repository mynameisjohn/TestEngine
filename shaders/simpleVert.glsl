#version 130

//integers representing the rendering modes of the shader
const int MODE_COLOR = 1;
const int MODE_TEX   = 2;
const int MODE_RIG   = 3;

//Projection and ModelView matrices
uniform mat4 u_Proj;
uniform mat4 u_MV;

//Skeleton matrix and weights - hard coded at three "joints" for now
uniform mat4 u_RigMat[3];
attribute vec3 a_W;

//Rendering mode
uniform int u_Mode;

//Input vertices and texture coordinates
attribute vec4 a_Position;
attribute vec2 a_TexCoord;

//Varying texture coordinate, used in Fragment Shader
varying vec2 v_TexCoord;

varying vec4 weightColor;

void main(){
	if (u_Mode == MODE_RIG){
		gl_Position = u_Proj * u_MV * (a_W.x*u_RigMat[0]+a_W.y*u_RigMat[1]+a_W.z*u_RigMat[2]) * a_Position;
		v_TexCoord = a_TexCoord;
		weightColor = vec4(a_W.x,a_W.y,a_W.z,1);
	}
	else if (u_Mode == MODE_TEX){
		gl_Position = u_Proj * u_MV * a_Position;
		v_TexCoord = a_TexCoord;
	}
	else if (u_Mode == MODE_COLOR){
	//Assume just color at this point
	gl_Position = u_Proj * u_MV * a_Position;
	}
}
