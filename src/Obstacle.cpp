#include "Obstacle.h"

Obstacle::Obstacle(){
	colorOsc = 0.f;
}

Obstacle::Obstacle(vec3 translate, vec3 scale)
: Entity(translate,scale){
	colorOsc = 0.f;
}
void Obstacle::update(){
	float color = 0.5f + 0.5f*sin(colorOsc);
	colorOsc += 0.05f;
	mDrawables[0].setColor(vec3(color, 1.f-color, 0.5f*color));
}
