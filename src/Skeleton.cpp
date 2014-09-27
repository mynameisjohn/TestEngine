#include "Skeleton.h"

Skeleton::Skeleton() 
: root(nullptr){}

Skeleton::Skeleton(SceneGraph SG, Drawable * r)
: sg(move(SG)), root(r){
}

Skeleton::Skeleton(Drawable * r) 
: root(r) {}

void Skeleton::setRoot(Drawable * r){
	root = r;
}

void Skeleton::add(string k, unique_ptr<Drawable> v){
	pair<map<string, unique_ptr<Drawable> >::iterator, bool> p;
	p = sg.insert(pair<string, unique_ptr<Drawable> >(k,move(v)));
	if (p.second == false)
		cout <<"duplicate" << endl;
}

Drawable * Skeleton::getRoot(){
	return root;
}

Drawable * Skeleton::operator[](string idx){
	return sg.at(idx).get();
}
