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
//	aeVec.push_back(std::move(p));
//	player = (Player *)aeVec[0].get();
	player = std::move(p);
}

void Population::setPlayer(unique_ptr<Player> p){
	player = std::move(p);
}

void Population::draw(){
	ObsPtrVec::iterator obsIt;
	AePtrVec::iterator aeIt;
	SeekPtrVec::iterator sIt;

	player.get()->draw();

	for (obsIt=obsVec.begin(); obsIt!=obsVec.end(); obsIt++)
		obsIt->get()->draw();
	
	for (aeIt=aeVec.begin(); aeIt!=aeVec.end(); aeIt++)
		aeIt->get()->draw();

	for (sIt=seekVec.begin(); sIt!=seekVec.end(); sIt++)
		sIt->get()->draw();
}

vec3 Population::getPlayerCenter(){
	return player.get()->center();//player.get()->center();
}

//These two vectors are held by smart pointers so I can polymorphize
void Population::addObs(unique_ptr<Obstacle> obs){
	obsVec.push_back(std::move(obs));
}

void Population::addActiveEnt(unique_ptr<ActiveEnt> aE){
	aeVec.push_back(std::move(aE));
}

void Population::addSeeker(unique_ptr<Seeker>seek){
	seekVec.push_back(std::move(seek));
}

//for_each?
//move all active ents WRT obstacles
vec4 Population::move(){
	ObsPtrVec::iterator obsIt;
	AePtrVec::iterator aeIt;
	SeekPtrVec::iterator sIt;

	player.get()->move();
	for (obsIt=obsVec.begin(); obsIt!=obsVec.end(); obsIt++)
		player.get()->moveWRT_ent(obsIt->get());

	for (aeIt=aeVec.begin(); aeIt!=aeVec.end(); aeIt++){
		aeIt->get()->move();
		for (obsIt=obsVec.begin(); obsIt!=obsVec.end(); obsIt++){
			aeIt->get()->moveWRT_ent(obsIt->get());
		}
	}

	for (sIt=seekVec.begin(); sIt!=seekVec.end(); sIt++){
		sIt->get()->move();
		for (obsIt=obsVec.begin(); obsIt!=obsVec.end(); obsIt++){
			sIt->get()->moveWRT_ent(obsIt->get());
		}
	}
	
	return vec4(getPlayerCenter(), fabs(player.get()->getVel().x));
}

//Move player out of aeVEc, make a seeker vec I guess

void Population::update(){//EventInfo evInfo){
	ObsPtrVec::iterator obsIt;
	AePtrVec::iterator aeIt_i, aeIt_j;
	SeekPtrVec::iterator sIt_i, sIt_j;

	//This detects active ent on active ent collision
	bool hit;
	for (aeIt_i=aeVec.begin(); aeIt_i!=aeVec.end(); aeIt_i++)
		for (aeIt_j=aeVec.begin(); aeIt_j!=aeVec.end(); aeIt_j++)
			if (aeIt_i != aeIt_j)
				if (aeIt_i->get()->overlapsWith(aeIt_j->get()))
					hit=true;//printf("hit\n");
	
	for (aeIt_i=aeVec.begin(); aeIt_i!=aeVec.end(); aeIt_i++)
		bool playerHit = player.get()->overlapsWith(aeIt_i->get());

	for (sIt_i=seekVec.begin(); sIt_i!=seekVec.end(); sIt_i++)
		bool playerHit = player.get()->overlapsWith(sIt_i->get());

	player.get()->update();
	
	//This is for AI movement
	for (sIt_i=seekVec.begin(); sIt_i!=seekVec.end(); sIt_i++){
		vec3 pos = sIt_i->get()->getPos(), a;
		a = -player.get()->getPotential(pos);
		for (obsIt=obsVec.begin(); obsIt!=obsVec.end(); obsIt++)//begin()+2; obsIt++)
			a += obsIt->get()->getPotential(pos);
		for (sIt_j=seekVec.begin(); sIt_j!=seekVec.end(); sIt_j++)//begin()+2; obsIt++)
			if (sIt_i != sIt_j)
				a += sIt_j->get()->getPotential(pos);
		sIt_i->get()->update(a);
	}

	//Do whatever it is they need to do
	for (obsIt=obsVec.begin(); obsIt!=obsVec.end(); obsIt++)
		obsIt->get()->update();
}
Player * Population::getPlayer(){
	return player.get();
}
