#ifndef J_SHADER_H
#define J_SHADER_H

#include "ShaderProgram.h"

class JShader : public ShaderProgram{
	public:
		JShader();                         //Default Constructor
		bool loadProgram();                //Load Program
		bool loadVert(std::string vertStr);//Load Vert Shader from string
		bool loadFrag(std::string fragStr);//Load Frag Shader from string
		GLint getPosHandle();              //get vertex position handle
		GLint getTexCoordHandle();
		GLint getTexHandle();
		GLint getMVHandle();
		GLint getProjHandle();
		GLint getColorHandle();
	private:
		GLuint a_TexCoordHandle; //Handle to Texture Coordinates
		GLint u_MVHandle;   //Handle to MVP matrix
		GLint u_ProjHandle; //Handle to Projection Matrix
		GLint u_ColorHandle;//Handle to Frag Color (needed?)
		GLint a_PosHandle;  //Handle to Vertex Position
		GLuint mVS, mFS;   //References to vertex and fragment shaders
};

#endif
