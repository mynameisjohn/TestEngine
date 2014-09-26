#ifndef CLOSET_H
#define CLOSET_H

#ifndef TIXML_USE_STL
#define TIXML_USE_STL
#endif

#include <tinyxml.h>

#include "Geom.h"
#include <memory>

typedef std::map<std::string, std::unique_ptr<Drawable> > SceneGraph;

SceneGraph initSceneGraph(TiXmlElement * root, JShader& shader);

#endif
