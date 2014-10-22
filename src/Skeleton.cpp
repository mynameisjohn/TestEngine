#include "Skeleton.h"
#include "Ligament.h"


/*
	See the Entity::Entity(Entity * f) constructor in Entity.cpp
	for details regarding a bug involving this class. 
*/

Skeleton::Skeleton() 
: root(nullptr){}

Skeleton::Skeleton(unordered_map<string, int> nM, vector<Ligament> lV)
: nameMap(std::move(nM)), lVec(std::move(lV)) {}

/*
Skeleton::Skeleton(const Skeleton& other){

}
*/
/*
Skeleton::Skeleton(SceneGraph SG, Drawable * r)
: sg(move(SG)), root(r){
}
*/
Skeleton::Skeleton(map<string, Ligament> lM, string r)//{// Ligament * l)
: lMap(move(lM)){//, root(l), MV(){
//	lMap = move(lM);
	itRoot = lMap.find(r);
	R = r;
}

Skeleton::Skeleton(Drawable * r) {
	nameMap.clear();
	nameMap["root"]=0;
	lVec.clear();
	lVec.emplace_back(r);

	lMap.clear();
	lMap["root"] = Ligament(r);
	itRoot = lMap.find("root");
	root = &(lMap["root"]);
	R = "root";
}
/*
Skeleton::Skeleton(const Drawable& dr){
*/
/*
	sg.clear();
	sg["root"]=unique_ptr<Drawable>(new Drawable(dr));
	root = sg["root"].get();
*/
/*
	lMap.clear();
	lMap["root"] = Ligament(dr);
	root = &(lMap["root"]);
*/
//unique_ptr<Drawable>(new Drawable(dr));
/*
}
*/
/*
void Skeleton::setRoot(Ligament * l){
	root = l;
}
*/
void Skeleton::update(){
	vector<Ligament>::iterator lIt;
	for (lIt=lVec.begin(); lIt!=lVec.end(); lIt++)
		lIt->update();

	map<string, Ligament>::iterator it;
	for (it = lMap.begin(); it!=lMap.end();it++)
		it->second.update();
}

void Skeleton::print(){
/*	unordered_map<string, int>::iterator it;
	for (it=nameMap.begin();it!=nameMap.end();it++){
		cout << 
	}

	map<string, Ligament>::iterator it;
   for (it = lMap.begin(); it!=lMap.end();it++)
		cout << it->first << "\t" << &(it->second) << "\t" << it->second.mDrawable << endl;
*/
	cout << "holding off on this for now" << endl;
}

/*
void Skeleton::add(string k, Ligament l){//unique_ptr<Drawable> v){
	pair<map<string, Ligament >::iterator, bool> p;
	p = lMap.insert(pair<string, Ligament>(k,move(v)));
	if (p.second == false)
		cout <<"duplicate" << endl;
}
*/
/*
void Skeleton::addToRoot(string k, Ligament l){//unique_ptr<Drawable> v){
//	add(k,move(v));
//	root->addChild(sg[k].get());
	lMap[k]=l;//Ligament(move(v));
	root->addChild(&(lMap[k]));
}
*/
void Skeleton::draw(mat4 transform){
//	lMap["body"].draw(transform*MV);
//	root->draw(transform * MV);
//	print();
	lVec.front().draw(transform*MV);
//	itRoot->second.draw(transform*MV);
/*	if (root)
		root->draw(transform);
	else	
		root->draw(transform, 0);
*/
}

void Skeleton::applyTransform(mat4 m){
	MV = m * MV;
//	root->leftMultMV(m);
}

void Skeleton::resetLigaments(){
	vector<Ligament>::iterator lIt;
	for (lIt=lVec.begin(); lIt!=lVec.end(); lIt++)
		lIt->resetTransform();

	map<string, Ligament>::iterator it;
	for (it=lMap.begin(); it!=lMap.end(); it++)
		it->second.resetTransform();
}

Ligament * Skeleton::getRoot(){
	return &(lVec.front());//&(lMap[R]);//root;
}

Ligament * Skeleton::operator[](string idx){
	return &(lVec[nameMap[idx]]);
//	return &(lMap.at(idx));
}
