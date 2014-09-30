#ifndef XML_READER_H
#define XML_READER_H

#ifndef TIXML_USE_STL
#define TIXML_USE_STL
#endif

#include <tinyxml.h>
#include <map>
#include "Util.h"
#include "Cycle.h"

typedef struct triangle{
   unsigned int A,B,C;
} triangle;

typedef struct geoInfo{
	vector<vec4> vertices;
	vector<vec2> texCoords;
	vector<triangle> indices;
	vector<vec3> weights;
	vec3 offset;
} geoInfo;

template <typename T>
void fillIn(TiXmlElement * el, string id, T& v); 
vector<triangle> getConvexIndices(int n);
vector<Cycle> getRigCycles(string svgFile);
map<string, string> getSVGPathMap(string svgFile);
geoInfo SVGtoGeometry(string svgFile, bool rigged);

void fillIn(string s, unsigned int v);

#endif
