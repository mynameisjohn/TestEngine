/**
TODO
	-- comment code properly
	-- create library/character libraries
	-- projectiles
	-- art
	-- sound
	-- skinning
**/

#include "Level.h"
#include "BaseEngine.h"

#include <iostream>
#include <string>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

BaseEngine::BaseEngine(){
	//NYI
}

BaseEngine::~BaseEngine(){
	glDeleteProgram(shader.getProgramID());
	glBindVertexArray(0);
}

bool BaseEngine::init(std::string vertShaderSrc, std::string fragShaderSrc){
	//Load Vertex/Fragment Shader files
   if (!shader.loadVert(vertShaderSrc) ||
       !shader.loadFrag(fragShaderSrc))
      return false;

   //Generate Shader Program
   if (!shader.loadProgram())
      return false;
	
	pop = std::move(initLevel(shader));
	return true;
}

void BaseEngine::update(){
	pop->update();
}

void BaseEngine::move(){
	cam.push(pop->move());
}

//Try and get SDL out of the picture
void BaseEngine::handleEvent(SDL_Event& e){
	if (e.key.repeat == 0 && (e.type == SDL_KEYUP || e.type == SDL_KEYDOWN)){
      pop->handleKey(keyCode(e));
	}
}

void BaseEngine::render(){
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader.bind();

	cam.updateProj(shader.getProjHandle());
	pop->draw(shader.getMVHandle(), shader.getColorHandle());

	shader.unbind();
}

int keyCode(SDL_Event& e){
   return (int)e.key.keysym.sym;
}
/*
if (keyCode(e) == SDLK_RIGHT)
	cam.rightMult(glm::rotate(0.01f, vec3(0, 1, 0)));
if (keyCode(e) == SDLK_LEFT)
	cam.rightMult(glm::rotate(-0.01f, vec3(0, 1, 0)));
if (keyCode(e) == SDLK_UP)
	cam.rightMult(glm::rotate(-0.01f, vec3(1, 0, 0)));
if (keyCode(e) == SDLK_DOWN)
	cam.rightMult(glm::rotate(0.01f, vec3(1, 0, 0)));
*/
