#version 130

uniform mat4 u_Proj;
uniform mat4 u_MV;

attribute vec4 a_Position;
attribute vec2 a_TexCoord;
varying vec2 v_TexCoord;

void main(){
	v_TexCoord = a_TexCoord;
	gl_Position = u_Proj * u_MV * a_Position;
}
