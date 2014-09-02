#ifndef KEYBOARDHANDLER_H
#define KEYBOARDHANDLER_H

#include <stdio.h>
//#include <SDL2/SDL.h>

class KeyboardHandler{
public:
	KeyboardHandler(){
	   for (int i=0; i<256; i++)
			keyState[i]=false;
	}

	inline int getKeyState(int key){
		return keyState[key & 0xFF];
	}

	//just toggles for now	
	inline void handleKey(int key){
		//if (key>0 && key<255)
		key &= 0xFF;
		keyState[key] = !keyState[key];
	}

protected:
	bool keyState[256];
};

#endif
