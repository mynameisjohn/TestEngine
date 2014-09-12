#ifndef XMLLOADER_H
#define XMLLOADER_H

#include <vector>
#include <glm/glm.hpp>

typedef struct triangle{
	unsigned int A,B,C;
} triangle;

typedef struct geoInfo{
	std::vector<glm::vec4> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<triangle> indices;
} geoInfo;

#endif
