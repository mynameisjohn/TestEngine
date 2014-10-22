#include "Soup.h"
#include "Textures.h"
#include "Closet.h"
#include <glm/gtx/transform.hpp> 

#include "Projectile.h"

Collider getCollider(TiXmlElement * collider){
	string d=",";
	BoundBox bb;
	vector<BoundRect> recVec;
	for (TiXmlElement * box=collider->FirstChildElement("box"); box; box=box->NextSiblingElement("box")){
		vec3 dim, tr;
		string in = box->Attribute("S");
		size_t pos=0;
		for (int i=0;i<3;i++){
			pos=in.find(d);
			stringstream(in.substr(0,pos)) >> dim[i];
			in.erase(0,pos+d.length());
		}
/*
		in = box->Attribute("T");
		pos=0;
		for (int i=0;i<3;i++){
			pos=in.find(d);
			stringstream(in.substr(0,pos)) >> tr[i];
			in.erase(0,pos+d.length());
		}
*/
		bb = BoundBox(dim);
		for (TiXmlElement  * rect=box->FirstChildElement("rect"); rect; rect=rect->NextSiblingElement("rect")){
			vec2 rDim;
			pos=0;
			in = rect->Attribute("S");
			for (int i=0;i<2;i++){
				pos=in.find(d);
				stringstream(in.substr(0,pos)) >> rDim[i];
				in.erase(0,pos+d.length());
			}
			recVec.emplace_back(rDim);
		}
	}
	Collider c(bb);
	for (vector<BoundRect>::iterator it=recVec.begin(); it!=recVec.end(); it++)
		c.addSub(*it);

	return c;
}
/*
unique_ptr<Entity> loadEntFromFile(EntInfo eI, JShader& shader){
	string fileName = "res/"+eI.name+"/"+eI.name+".xml";
	TiXmlDocument doc(fileName);
   if (!doc.LoadFile()){
      cout << "Unable to load Graph File " << eI.name << endl;
      return nullptr;
   }

   TiXmlHandle h(&doc);
	TiXmlElement * entity(0), * skeleton(0), * collider(0), * projectile(0);
	bool collider_dbg;
	
	entity = h.FirstChild("Entity").ToElement();
	skeleton = entity->FirstChildElement("Skeleton");//h.FirstChild("Entity").FirstChild("Skeleton").ToElement();
	collider = entity->FirstChildElement("Collider");//h.FirstChild("Entity").FirstChild("Collider").ToElement();

	if (skeleton && collider){
		vec3 tr;
		Skeleton s = getSkeleton(skeleton, shader);
		mat4 MV = glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale);// * glm::translate(vec3(0,1,0));
		s.applyTransform(MV);

		Collider c = getCollider(collider);
		c.scale(glm::abs(eI.scale));
		c.translate(eI.translate);
	
		if (entity->Attribute("dbg")){
			if (!(stringstream(entity->Attribute("dbg")) >> collider_dbg))
				collider_dbg = false;

			if (collider_dbg){
				mat4 c_MV = glm::scale(c.getDim());// / eI.scale);
				Drawable c_dr = initCube(shader);
				c_dr.leftMultMV(glm::inverse(MV)*c_MV);
				c_dr.setColor(.3f,.4f,.5f,.6f);	
		
				s.addToRoot("collider",Ligament(c_dr));
	//unique_ptr<Drawable>(new Drawable(c_dr)));//initCube(shader))));
			}
		}

		unique_ptr<Entity> ret(new Entity(c,move(s)));//eI.translate, eI.scale, move(s)));
		return ret;
	}

	Closet c;

	cout << "Invalid XML File" << endl;
	return nullptr;
}

unique_ptr<Projectile> initProjectile(string fileName, JShader& shader){
	fileName = "res/"+fileName+"/"+fileName+".xml";
	TiXmlDocument doc(fileName);
	string imageFile;
	int num;
	vec3 scale(1,1,1);

	if (!doc.LoadFile()){
      cout << "Unable to load XML file " << fileName << endl;
      return nullptr;
   }

	TiXmlHandle h(&doc);
   TiXmlElement * projectile = h.FirstChild("Entity").FirstChild("Projectile").ToElement();

	if (projectile){
		imageFile = projectile->Attribute("fileName");
	
		if (!stringstream(projectile->Attribute("num")) >> num){
			cout << "Invalid projectile count in XML file " << fileName << endl;
			return nullptr;	
		}

		string in = projectile->Attribute("S");
      size_t pos=0;
		const string d = ",";
      for (int i=0;i<2;i++){
         pos=in.find(d);
         if (!(stringstream(in.substr(0,pos)) >> scale[i])){
				cout << "Invalid scale count in XML file " << fileName << endl;
	         return nullptr;
			}
         in.erase(0,pos+d.length());
      }
	}
	else{
		cout << "No Projectile element in XML file " << fileName << endl;
		return nullptr;
	}

//	cout << imageFile << "\n" << num << "\n" << scale << endl;

	Drawable dr = initTexQuad(imageFile, shader);
	mat4 MV = glm::scale(scale);//glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale);
	dr.leftMultMV(MV);

	//for local origin....
//	dr.leftMultMV(glm::translate(-scale/2.f));

	Skeleton s(dr);

	//this is the strangest thing i've ever seen...
	Collider c;//(BoundBox(scale));
	c.setBB(BoundBox(scale));
	Entity e(c,move(s));

	return unique_ptr<Projectile>(new Projectile(&e));
}

//Somewhere here i've got to add projectile functionality
unique_ptr<Player> initPlayer(EntInfo eI, JShader& shader){
	unique_ptr<Player> playerPtr(new Player(loadEntFromFile(eI, shader).get()));

	unique_ptr<Projectile> p = move(initProjectile(eI.name, shader));

	if (p){
		for (int i=0;i<eI.numProj;i++)
		playerPtr.get()->addProjectile(p.get());
	}

//	playerPtr.get()->setRig();

	return playerPtr;
}

unique_ptr<Obstacle> initObstacle(EntInfo eI, JShader& shader){ 
	const string name = "dr";
	Drawable dr = initCube(shader);
	Collider c(BoundBox(eI.scale));
	mat4 MV = glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale);
	dr.leftMultMV(MV);//s.applyTransform(MV);//[name]->leftMultMV(MV);
	dr.setColor(eI.color);
	Skeleton s(dr);
	unique_ptr<Obstacle> obs(new Obstacle(new Entity(c, move(s))));
	obs.get()->translate(eI.translate);	
	return obs;
}

unique_ptr<Seeker> initSeeker(EntInfo eI, JShader& shader){
	const string name = "dr";
	Drawable dr = initCube(shader);
	Collider c(BoundBox(eI.scale));
	mat4 MV = glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale);
	dr.leftMultMV(MV);//s.applyTransform(MV);
	dr.setColor(eI.color);
	Skeleton s(dr);
	unique_ptr<Seeker> seek(new Seeker(new Entity(c, move(s))));
	seek.get()->translate(eI.translate);	

	return seek;
}

unique_ptr<ActiveEnt> initAe(EntInfo eI, JShader& shader){
	const string name = "dr";
	Drawable dr = initCube(shader);
	Collider c(BoundBox(eI.scale));
	mat4 MV = glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale);
	dr.leftMultMV(MV);//s.applyTransform(MV);
	dr.setColor(eI.color);
	Skeleton s(dr);
	unique_ptr<ActiveEnt> aE(new ActiveEnt(new Entity(c, move(s))));
	aE.get()->translate(eI.translate);	
	return aE;
}

unique_ptr<Wall> initWall(EntInfo eI, char orientation, JShader& shader){
	const string name = "dr";
	Drawable dr = initQuad(shader);
	dr.setColor(eI.color);
	mat4 MV = glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale);
	dr.leftMultMV(MV);
	Skeleton s(dr);
	unique_ptr<Wall> w(new Wall(eI.translate, eI.scale, orientation));
	w.get()->setSkeleton(move(s));
	return w;
}
*/
/*
//this doesn't need to be in Soup's namespace
Collider Soup::getCollider(TiXmlElement * collider){
	const string d=",";
	BoundBox bb;
	vector<BoundRect> recVec;
	for (TiXmlElement * box=collider->FirstChildElement("box"); box; box=box->NextSiblingElement("box")){
		vec3 dim, tr;
		string in = box->Attribute("S");
		size_t pos=0;
		for (int i=0;i<3;i++){
			pos=in.find(d);
			stringstream(in.substr(0,pos)) >> dim[i];
			in.erase(0,pos+d.length());
		}

		bb = BoundBox(dim);
		for (TiXmlElement  * rect=box->FirstChildElement("rect"); rect; rect=rect->NextSiblingElement("rect")){
			vec2 rDim;
			pos=0;
			in = rect->Attribute("S");
			for (int i=0;i<2;i++){
				pos=in.find(d);
				stringstream(in.substr(0,pos)) >> rDim[i];
				in.erase(0,pos+d.length());
			}
			recVec.emplace_back(rDim);
		}
	}
	Collider c(bb);
	for (vector<BoundRect>::iterator it=recVec.begin(); it!=recVec.end(); it++)
		c.addSub(*it);

	return c;
}
*/
unique_ptr<Entity> Soup::loadEntFromFile(EntInfo eI, JShader& shader){
	Closet c(dMapPtr);
	string fileName = "res/"+eI.name+"/"+eI.name+".xml";
	TiXmlDocument doc(fileName);
   if (!doc.LoadFile()){
      cout << "Unable to load Graph File " << eI.name << endl;
      return nullptr;
   }

   TiXmlHandle h(&doc);
	TiXmlElement * entity(0), * skeleton(0), * collider(0), * projectile(0);
	bool collider_dbg;
	
	entity = h.FirstChild("Entity").ToElement();
	skeleton = entity->FirstChildElement("Skeleton");
	collider = entity->FirstChildElement("Collider");

	if (skeleton && collider){
		vec3 tr;
		Skeleton s = c.createSkeleton(skeleton, shader);
		mat4 MV = glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale);
		s.applyTransform(MV);

		Collider c = getCollider(collider);
		c.scale(glm::abs(eI.scale));
		c.translate(eI.translate);
	
		if (entity->Attribute("dbg")){
			if (!(stringstream(entity->Attribute("dbg")) >> collider_dbg))
				collider_dbg = false;
/*
			if (collider_dbg){
				mat4 c_MV = glm::scale(c.getDim());
				Drawable c_dr = initCube(shader);
				c_dr.leftMultMV(glm::inverse(MV)*c_MV);
				c_dr.setColor(.3f,.4f,.5f,.6f);	
		
				s.addToRoot("collider",Ligament(c_dr));
			}
*/
		}

		unique_ptr<Entity> ret(new Entity(c,move(s)));
		return ret;
	}

	cout << "Invalid XML File" << endl;
	return nullptr;
}

Projectile Soup::initProjectile(string fileName, JShader& shader){
//unique_ptr<Projectile> Soup::initProjectile(string fileName, JShader& shader){
	fileName = "res/"+fileName+"/"+fileName+".xml";
	TiXmlDocument doc(fileName);
	string imageFile;
	int num;
	vec3 scale(1);

	if (!doc.LoadFile()){
      cout << "Unable to load XML file " << fileName << endl;
      return nullptr;
   }

	TiXmlHandle h(&doc);
   TiXmlElement * projectile = h.FirstChild("Entity").FirstChild("Projectile").ToElement();

	if (projectile){
		imageFile = projectile->Attribute("fileName");
	
		if (!stringstream(projectile->Attribute("num")) >> num){
			cout << "Invalid projectile count in XML file " << fileName << endl;
			return nullptr;	
		}

		string in = projectile->Attribute("S");
      size_t pos=0;
		const string d = ",";
      for (int i=0;i<2;i++){
         pos=in.find(d);
         if (!(stringstream(in.substr(0,pos)) >> scale[i])){
				cout << "Invalid scale count in XML file " << fileName << endl;
	         return nullptr;
			}
         in.erase(0,pos+d.length());
      }
	}
	else{
		cout << "No Projectile element in XML file " << fileName << endl;
		return nullptr;
	}
	
	if (dMapPtr->find(imageFile) == dMapPtr->end()){
		unique_ptr<Drawable> drPtr(new Drawable(initTexQuad(imageFile, shader)));
		mat4 MV = glm::scale(scale);
		drPtr->leftMultMV(MV);
		(*dMapPtr)[imageFile] = move(drPtr);
	}
	Skeleton s(dMapPtr->find(imageFile)->second.get());

	//this is the strangest thing i've ever seen...
	Collider c;
	c.setBB(BoundBox(scale));
	Entity e(c,move(s));

	return Projectile(&e);//unique_ptr<Projectile>(new Projectile(&e));
}

unique_ptr<Player> Soup::initPlayer(EntInfo eI, JShader& shader){
	unique_ptr<Player> playerPtr(new Player(loadEntFromFile(eI, shader).get()));

/*
	unique_ptr<Projectile> p = move(initProjectile(eI.name, shader));
	if (p){
		for (int i=0;i<eI.numProj;i++)
			playerPtr.get()->addProjectile(p.get());
			cout << p.get() << "\t" << eI.numProj << endl;
	}
*/
	Projectile p = initProjectile(eI.name, shader);
//	Projectile p(&(initProjectile(eI.name, shader)));

	playerPtr->setProjectile(&p);
/*	
	for (int i=0;i<eI.numProj;i++)
		playerPtr.get()->addProjectile(p);
*/	

	return playerPtr;
}


unique_ptr<Obstacle> Soup::initObstacle(EntInfo eI, JShader& shader){ 
	if (dMapPtr->find(eI.name) == dMapPtr->end()){
		unique_ptr<Drawable> drPtr(new Drawable(initCube(shader)));
      drPtr->leftMultMV(glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale));
      drPtr->setColor(eI.color);
      (*dMapPtr)[eI.name] = move(drPtr);
   }
   Skeleton s(dMapPtr->find(eI.name)->second.get());
   Collider c(BoundBox(eI.scale));
	unique_ptr<Obstacle> obs(new Obstacle(new Entity(c, move(s))));
   obs.get()->translate(eI.translate);

   return obs;
}

unique_ptr<Seeker> Soup::initSeeker(EntInfo eI, JShader& shader){
	if (dMapPtr->find(eI.name) == dMapPtr->end()){
		unique_ptr<Drawable> drPtr(new Drawable(initCube(shader)));
      drPtr->leftMultMV(glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale));
      drPtr->setColor(eI.color);
      (*dMapPtr)[eI.name] = move(drPtr);
   }
   Skeleton s(dMapPtr->find(eI.name)->second.get());
   Collider c(BoundBox(eI.scale));
   unique_ptr<Seeker> seek(new Seeker(new Entity(c, move(s))));
   seek.get()->translate(eI.translate);

   return seek;
}

unique_ptr<ActiveEnt> Soup::initAe(EntInfo eI, JShader& shader){
	if (dMapPtr->find(eI.name) == dMapPtr->end()){
		unique_ptr<Drawable> drPtr(new Drawable(initCube(shader)));
		drPtr->leftMultMV(glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale));
		drPtr->setColor(eI.color);
		(*dMapPtr)[eI.name] = move(drPtr);
	}
	Skeleton s(dMapPtr->find(eI.name)->second.get());
	Collider c(BoundBox(eI.scale));
	unique_ptr<ActiveEnt> aE(new ActiveEnt(new Entity(c, move(s))));

	return aE;
}

unique_ptr<Wall> Soup::initWall(EntInfo eI, char orientation, JShader& shader){
   if (dMapPtr->find(eI.name) == dMapPtr->end()){
      unique_ptr<Drawable> drPtr(new Drawable(initQuad(shader)));
      drPtr->leftMultMV(glm::rotate(eI.rotate.w, vec3(eI.rotate)) * glm::scale(eI.scale));
      drPtr->setColor(eI.color);
      (*dMapPtr)[eI.name] = move(drPtr);
   }
	Skeleton s(dMapPtr->find(eI.name)->second.get());
	unique_ptr<Wall> w(new Wall(eI.translate, eI.scale, orientation));
   w.get()->setSkeleton(move(s));
   return w;
}
