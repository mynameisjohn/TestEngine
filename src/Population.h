#ifndef POPULATION_H
#define POPULATION_H

#include "Player.h"
#include "Obstacle.h"
#include <memory>

typedef std::vector<std::unique_ptr<Obstacle> > ObsPtrVec;
typedef std::vector<std::unique_ptr<ActiveEnt> > AePtrVec;

using glm::vec4;

class Population{
	public:
		Population();
		Population(std::unique_ptr<Player> p);
		~Population();
		void handleKey(int k);
		void update();
		void draw(int, int);
		vec4 move();
		vec3 getPlayerCenter();
		Player * getPlayer();
		void addObs(std::unique_ptr<Obstacle> obs);
		void addActiveEnt(std::unique_ptr<ActiveEnt> aE);
	private:
		std::unique_ptr<Player> player;
		ObsPtrVec obsVec;
		AePtrVec aeVec;
		//int obsIdx, aeIdx;
};

#endif
