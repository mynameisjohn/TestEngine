#include "Wall.h"

Wall::Wall()
: Obstacle(){
	orientation='u';
}

Wall::Wall(vec3 translate, vec3 scale, char o){
	float tmp;
	switch(o){
		case 'x':
			tmp = scale.z;
			scale.z = scale.x;
			scale.x = tmp;
		case 'y':
			tmp = scale.z;
			scale.z = scale.y;
			scale.y = tmp;
		case 'z':
			break;
	}
	orientation=o;
	mCollider = Collider(BoundBox(scale));
	mCollider.addSub(BoundRect(vec2(scale)));
	this->translate(translate);
}

Wall::~Wall(){
	//NYI
}

void Wall::update(){
	//NYI
}
