#include "Geom.h"

#include "JShader.h"
#include "Drawable.h"
#include "Textures.h"
#ifndef TIXML_USE_STL
#define TIXML_USE_STL
#endif
#include <tinyxml.h>
#include <iostream>
#include <sstream>
#include <vector>

float min(float v1, float v2){
	return (v1 < v2 ? v1 : v2);
}
float max(float v1, float v2){
	return (v1 > v2 ? v1 : v2);
}
float clamp(float v, float m, float M){
	return min(max(v,m),M);
}

//assumes "x.svg" as input
Drawable initPolyFromSVG(std::string fileName, JShader& shader){
	Drawable dr;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec4> vertices;
	std::vector<triangle> indices;
	
	TiXmlDocument doc("drawing.svg");
	if (!doc.LoadFile()){
		std::cout << "Unable to load SVG file\n";
		return dr;
	}

	TiXmlHandle mHandle(&doc);
	TiXmlElement * mElement = mHandle.FirstChild("svg").FirstChild("g").FirstChild("path").ToElement();
	if (!mElement){
		std::cout << "Couldn't find path element in SVG document\n";
		return dr;
	}
	
	std::string pathStr = mElement->Attribute("d"), d1=" ", d2=",";
	bool relative = (pathStr[0] == 'm');
	pathStr = pathStr.substr(2,pathStr.length());
	size_t pos;
	
	while (pos != std::string::npos){
		switch (pathStr[0]){
			case 'L':
			case 'l':
            pos = pathStr.find(d1);
            pathStr.erase(0,pos+d1.length());
            break;
         case 'z':
         case 'Z':
            pos = std::string::npos;
            break;
         default:
				glm::vec2 p;
				pos = pathStr.find(d2);
				std::string ptStr = pathStr.substr(0,pos);
				if (!(std::stringstream(ptStr) >> p.x)){
					std::cout << "Incorrect path string\n";
					return dr;
				}
				pathStr.erase(0,pos+d2.length());
				pos = pathStr.find(d1);
				ptStr = pathStr.substr(0,pos);
				if (!(std::stringstream(ptStr) >> p.y)){
               std::cout << "Incorrect path string\n";
               return dr;
            }
				pathStr.erase(0,pos+d2.length());
				if (texCoords.size() && relative)
					p += texCoords.back();
				texCoords.push_back(p);
		}
	}
	glm::vec2 m=texCoords.front(), M=texCoords.front();
	std::vector<glm::vec2>::iterator it;
	for (it=texCoords.begin(); it!=texCoords.end(); it++){
		m.x = min(m.x,it->x); m.y = min(m.y,it->y);
		M.x = max(M.x,it->x); M.y = max(M.y,it->y);
	}
	M -= m;
	for (it=texCoords.begin(); it!=texCoords.end(); it++){
		*it -= m;
		glm::vec2 vert = glm::vec2(*it);
		*it /= M;
		vert.y = M.y-vert.y;
		vertices.push_back(glm::vec4(vert/max(M.x,M.y),0,1));
	}

	int n = vertices.size();
	int nElements = 3*(vertices.size()-2);
	for (GLuint i=0;i<n/2-1;i++){
		triangle odd, even;
		odd = {i,n-i-1,n-i-2};
		even = {i,i+1,n-i-2};
		indices.push_back(odd);
		indices.push_back(even);
	}

	GLuint tmpVAO;
   glGenVertexArrays(1, &tmpVAO);
   glBindVertexArray(tmpVAO);

   GLuint buffers[3];
   glGenBuffers(3, buffers);

   //vertices
   glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*vertices.size(), vertices.data(), GL_STATIC_DRAW);
   glEnableVertexAttribArray(shader.getPosHandle());
   glVertexAttribPointer(shader.getPosHandle(), 4, GL_FLOAT, 0, 0, 0);

   //tex coords
   glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*texCoords.size(), texCoords.data(), GL_STATIC_DRAW);
   glEnableVertexAttribArray(shader.getTexCoordHandle());
   glVertexAttribPointer(shader.getTexCoordHandle(), 2, GL_FLOAT, 0, 0, 0);

   //indices
   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffers[2] );
   glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(triangle)*indices.size(), indices.data(), GL_STATIC_DRAW );

   glBindVertexArray(0);

	std::string imageName = fileName.substr(0,fileName.length()-4)+".png";
   dr.setVAO(tmpVAO);
   dr.setTex(fromImage(imageName));//outlineTexture());
   dr.setNElements(nElements);

   return dr;

}


Drawable initQuad(JShader& shader){
   const int nVert=4, dim=3, nIndices=4;
   const int vStride = nVert*dim*sizeof(GLint);
   const int tStride = nVert*2*sizeof(GLfloat);
   const int iStride = nIndices*sizeof(GLuint);

	Drawable dr;

   const GLint vertices[nVert][dim] = {
		{0, 0, 0}, {1, 0, 0},
		{0, 1, 0}, {1, 1, 0}
	};

	//why 3?
   const GLfloat texCoords[4][3] = {
      {0.f, 0.f}, {1.f, 0.f},
      {0.f, 1.f}, {1.f, 1.f},
   };
   const GLuint indices[nIndices] = {0, 1, 2, 3};

   GLuint tmpVAO;
   glGenVertexArrays(1, &tmpVAO);
   glBindVertexArray(tmpVAO);

   GLuint buffers[3];
   glGenBuffers(3, buffers);

   //vertices
   glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
   glBufferData(GL_ARRAY_BUFFER, vStride, vertices, GL_STATIC_DRAW);
   glEnableVertexAttribArray(shader.getPosHandle());
   glVertexAttribPointer(shader.getPosHandle(), dim, GL_INT, 0, 0, 0);

   //tex coords
   glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
   glBufferData(GL_ARRAY_BUFFER, tStride, texCoords, GL_STATIC_DRAW);
   glEnableVertexAttribArray(shader.getTexCoordHandle());
   glVertexAttribPointer(shader.getTexCoordHandle(), dim, GL_FLOAT, 0, 0, 0);

   //indices
   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffers[2] );
   glBufferData( GL_ELEMENT_ARRAY_BUFFER, iStride, indices, GL_STATIC_DRAW );

   glBindVertexArray(0);

	dr.setVAO(tmpVAO);
	dr.setTex(fromImage("rect.png"));//outlineTexture());
	dr.setNElements(nIndices);

   return dr;
}

Drawable initCube(JShader& shader){
   const int nVert=24, dim=3, nIndices=36;
   const int vStride = nVert*dim*sizeof(GLint);
   const int tStride = nVert*2*sizeof(GLfloat);
   const int iStride = nIndices*sizeof(GLuint);

	Drawable dr;
	int verticeCount = 24;
	int faceCount = 12;

	float vertices[] = {
		0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,

		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f
	};
	float texCoords[] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
	};
	unsigned int faceIndex[] = {
		0,1,2,0,2,3,
		4,5,6,4,6,7,
		8,9,10,8,10,11,
		12,13,14,12,14,15,
		16,17,18,16,18,19,
		20,21,22,20,22,23
	};

   GLuint tmpVAO;
   glGenVertexArrays(1, &tmpVAO);
   glBindVertexArray(tmpVAO);

   GLuint buffers[3];
   glGenBuffers(3, buffers);

   //vertices
   glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
   glEnableVertexAttribArray(shader.getPosHandle());
   glVertexAttribPointer(shader.getPosHandle(), 4, GL_FLOAT, 0, 0, 0);

   //tex coords
   glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
   glEnableVertexAttribArray(shader.getTexCoordHandle());
   glVertexAttribPointer(shader.getTexCoordHandle(), 2, GL_FLOAT, 0, 0, 0);

   //indices
   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffers[2] );
   glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(faceIndex), faceIndex, GL_STATIC_DRAW );

	//unbind
   glBindVertexArray(0);

	dr.setVAO(tmpVAO);
	dr.setTex(outlineTexture());
	dr.setNElements(faceCount*3);

   return dr;
}
