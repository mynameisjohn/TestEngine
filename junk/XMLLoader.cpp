#include "XMLLoader.h"

#ifndef TIXML_USE_STL
#define TIXML_USE_STL
#endif
#include <tinyxml.h>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

float min(float v1, float v2){
   return (v1 < v2 ? v1 : v2);
}
float max(float v1, float v2){
   return (v1 > v2 ? v1 : v2);
}
float clamp(float v, float m, float M){
   return min(max(v,m),M);
}

geoInfo getGeomFromSVG(string fileName){
	std::vector<glm::vec2> texCoords;
   std::vector<glm::vec4> vertices;
   std::vector<triangle> indices;

   TiXmlDocument doc("drawing.svg");
   if (!doc.LoadFile()){
      std::cout << "Unable to load SVG file\n";
   }

   TiXmlHandle mHandle(&doc);
   TiXmlElement * mElement = mHandle.FirstChild("svg").FirstChild("g").FirstChild("path").ToElement();
   if (!mElement){
      std::cout << "Couldn't find path element in SVG document\n";
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
            }
            pathStr.erase(0,pos+d2.length());
            pos = pathStr.find(d1);
            ptStr = pathStr.substr(0,pos);
            if (!(std::stringstream(ptStr) >> p.y)){
               std::cout << "Incorrect path string\n";
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

	return {vertices, texCoords, indices};
}
