#ifndef GEOM_H
#define GEOM_H

#include <map>

#include "Rig.h"

typedef struct triangle{
   GLuint A,B,C;
} triangle;

typedef struct geoInfo{
	vector<vec4> vertices;
	vector<vec2> texCoords;
	vector<triangle> indices;
	vector<vec3> weights;
	vec3 offset;
} geoInfo;

Drawable initQuad(JShader& shader);
Drawable initCube(JShader& shader);
Drawable initPolyFromSVG(string fileName, JShader& shader);
Rig initRigFromSVG(string fileName, JShader& shader);
vector<triangle> getConvexIndices(int n);
vector<Cycle> getRigCycles(string svgFile);
map<string,string> getSVGPathMap(string svgFile);
geoInfo SVGtoGeometry(string svgFile, bool rigged);
GLuint genVAO(geoInfo gI, JShader& shader);

#endif
