#ifndef PLAYER_H
#define PLAYER_H

#include "ActiveEnt.h"
#include "KeyboardHandler.h"
#include "Rig.h"

class Player: public ActiveEnt{
	public:
		Player();
		Player(vec3 translate, vec3 scale);
		Player(Collider c);
		~Player();
		int setChildren();
		void setRig();
		void update();
		void getHandleInfo();
		void handleKey(int k);
	private:
		KeyboardHandler mHandler;
		float mDash;
};

#endif
