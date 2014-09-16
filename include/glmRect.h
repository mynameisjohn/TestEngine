#ifndef GLM_RECT
#define GLM_RECT

#include "Util.h"

/*
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

#include <glm/glm.hpp>
#include <iostream>

#ifndef VEC2_PRINT
#define VEC2_PRINT
std::ostream& operator<<(std::ostream& os, const glm::vec2& vec);
#endif

using glm::vec2;
*/
class glmRect{
public:
	glmRect();
	glmRect(vec2 dim);
	glmRect(vec2 pos, vec2 dim);
	float left();
	float right();
	float bottom();
	float top();
	vec2 getPos();
	vec2 getDim();
protected:
	vec2 mPos;
	vec2 mDim;
};

#endif
