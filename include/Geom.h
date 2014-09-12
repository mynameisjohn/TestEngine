#ifndef GEOM_H
#define GEOM_H

#include "JShader.h"
#include "Drawable.h"
#include <glm/glm.hpp>

float min(float v1, float v2);
float max(float v1, float v2);
float clamp(float v, float m, float M);

typedef struct triangle{
   GLuint A,B,C;
} triangle;

typedef struct geoInfo{
	std::vector<glm::vec4> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<triangle> indices;
} geoInfo;

std::vector<triangle> getConvexIndices(int n);
std::string getSVGPathStr(std::string svgFile);
geoInfo SVGtoGeometry(std::string svgFile);
GLuint genVAO(geoInfo gI, JShader& shader);
Drawable initPolyFromSVG(std::string fileName, JShader& shader);
Drawable initQuad(JShader& shader);
Drawable initCube(JShader& shader);

#endif
