#ifndef POPULATION_H
#define POPULATION_H

#include "Player.h"
#include "Obstacle.h"

typedef vector<unique_ptr<Obstacle> > ObsPtrVec;
typedef vector<unique_ptr<ActiveEnt> > AePtrVec;

using glm::vec4;

class Population{
	public:
		Population();
		Population(unique_ptr<Player> p);
		~Population();
		void setPlayer(unique_ptr<Player> p);
		void handleKey(int k);
		void update();
		void draw();//int, int);
		void addObs(unique_ptr<Obstacle> obs);
		void addActiveEnt(unique_ptr<ActiveEnt> aE);
		vec4 move();
		vec3 getPlayerCenter();
		Player * getPlayer();
	private:
		Player * player;
		ObsPtrVec obsVec;
		AePtrVec aeVec;
};

#endif
