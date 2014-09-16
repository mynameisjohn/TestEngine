#include "Population.h"

Population::Population(){
	obsVec.clear();
	aeVec.clear();
}

Population::~Population(){
//NYI
}

Population::Population(unique_ptr<Player> p){
	obsVec.clear();
	aeVec.clear();
	aeVec.push_back(std::move(p));
	player = (Player *)aeVec[0].get();
}

void Population::setPlayer(unique_ptr<Player> p){
	aeVec[0] = std::move(p);
}

void Population::draw(){
	ObsPtrVec::iterator obsIt;
	AePtrVec::iterator aeIt;

	for (obsIt=obsVec.begin(); obsIt!=obsVec.end(); obsIt++)
		obsIt->get()->draw();//MVHandle, ColorHandle);
	
	for (aeIt=aeVec.begin(); aeIt!=aeVec.end(); aeIt++)
		aeIt->get()->draw();//MVHandle, ColorHandle);
}

vec3 Population::getPlayerCenter(){
	return player->center();//player.get()->center();
}

//These two vectors are held by smart pointers so I can polymorphize
void Population::addObs(unique_ptr<Obstacle> obs){
//default copy constructor???
	obsVec.push_back(std::move(obs));//unique_ptr<Obstacle>(new Obstacle(obs)));
}

void Population::addActiveEnt(unique_ptr<ActiveEnt> aE){
	aeVec.push_back(std::move(aE));//unique_ptr<ActiveEnt>(new ActiveEnt(aE)));
}

void Population::handleKey(int k){
	player->handleKey(k);
}

vec4 Population::move(){
	ObsPtrVec::iterator obsIt;
	AePtrVec::iterator aeIt;
	for (aeIt=aeVec.begin(); aeIt!=aeVec.end(); aeIt++){
		aeIt->get()->move();
		for (obsIt=obsVec.begin(); obsIt!=obsVec.end(); obsIt++){
			aeIt->get()->moveWRT_ent(obsIt->get());
		}
	}
	
	return vec4(getPlayerCenter(), fabs(player->getVel().x));
}

void Population::update(){
	ObsPtrVec::iterator obsIt;
	AePtrVec::iterator aeIt_i, aeIt_j;

	bool hit;
	for (aeIt_i=aeVec.begin(); aeIt_i!=aeVec.end(); aeIt_i++)
		for (aeIt_j=aeVec.begin(); aeIt_j!=aeVec.end(); aeIt_j++)
			if (aeIt_i != aeIt_j)
				if (aeIt_i->get()->overlapsWith(aeIt_j->get()))
					hit=true;//printf("hit\n");
	
	player->update();

	for (aeIt_i=aeVec.begin()+1; aeIt_i!=aeVec.end(); aeIt_i++){
		vec3 pos = aeIt_i->get()->getPos(), a;
		a = -player->getPotential(pos);
		for (obsIt=obsVec.begin(); obsIt!=obsVec.end(); obsIt++)//begin()+2; obsIt++)
			a += obsIt->get()->getPotential(pos);
		for (aeIt_j=aeVec.begin()+1; aeIt_j!=aeVec.end(); aeIt_j++)//begin()+2; obsIt++)
			if (aeIt_i != aeIt_j)
				a += aeIt_j->get()->getPotential(pos);
		aeIt_i->get()->update(a);
	}

	for (obsIt=obsVec.begin(); obsIt!=obsVec.end(); obsIt++)
		obsIt->get()->update();
}
Player * Population::getPlayer(){
	return player;
}
