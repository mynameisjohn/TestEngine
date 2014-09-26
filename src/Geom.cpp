#include "Geom.h"
#include "Textures.h"

#ifndef TIXML_USE_STL
#define TIXML_USE_STL
#endif

#include <tinyxml.h>
#include <sstream>
#include <glm/gtx/transform.hpp>

Rig initRigFromSVG(string fileName, JShader& shader){
	Rig r(&shader);
	r.setCycles(getRigCycles(fileName));
	geoInfo gI = SVGtoGeometry(fileName,1);
   GLuint VAO = genVAO(gI, shader);
	string imageName = fileName.substr(0,fileName.length()-4)+".png";
	r.leftMultMV(glm::translate(gI.offset));
	r.setVAO(VAO);
   r.setTex(fromImage(imageName));//outlineTexture());
   r.setNElements(3*gI.indices.size());

	return r;
}

//assumes "x.svg" as input, as well as some "x.png" texture
Drawable initPolyFromSVG(string fileName, JShader& shader){
	Drawable dr(&shader,2);

	geoInfo gI = SVGtoGeometry(fileName,0);
	gI.weights = vector<vec3>();
	GLuint VAO = genVAO(gI, shader);	
	string imageName = fileName.substr(0,fileName.length()-4)+".png";
   dr.setVAO(VAO);
   dr.setTex(fromImage(imageName));//outlineTexture());
   dr.setNElements(3*gI.indices.size());

   return dr;
}

Drawable initTexQuad(string imageName, JShader& shader){
	Drawable dr = initQuad(shader);
	dr.setTex(fromImage(imageName));
	return dr;
}

Drawable initQuad(JShader& shader){
	Drawable dr(&shader,1);

	vector<vec4> vertices={
		{0,0,0,1}, {1,0,0,1},
		{1,1,0,1}, {0,1,0,1}
	};
	vector<vec2> texCoords={
		{0,0}, {1,0},
		{1,1}, {0,1}
	};
	vector<triangle> indices=getConvexIndices(vertices.size());
	vector<vec3> weights;
	GLuint VAO = genVAO({vertices, texCoords, indices, weights}, shader);
	
	dr.setVAO(VAO);
	dr.setNElements(3*indices.size());

   return dr;
}

//I don't even want to touch this
Drawable initCube(JShader& shader){
   const int nVert=24, dim=3, nIndices=36;
   const int vStride = nVert*dim*sizeof(GLint);
   const int tStride = nVert*2*sizeof(GLfloat);
   const int iStride = nIndices*sizeof(GLuint);

	Drawable dr(&shader,2);
	int verticeCount = 24;
	int faceCount = 12;

	float vertices[] = {
		0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,

		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f
	};
	float texCoords[] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,

		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
	};
	unsigned int faceIndex[] = {
		0,1,2,0,2,3,
		4,5,6,4,6,7,
		8,9,10,8,10,11,
		12,13,14,12,14,15,
		16,17,18,16,18,19,
		20,21,22,20,22,23
	};

   GLuint tmpVAO;
   glGenVertexArrays(1, &tmpVAO);
   glBindVertexArray(tmpVAO);

   GLuint buffers[3];
   glGenBuffers(3, buffers);

   //vertices
   glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
   glEnableVertexAttribArray(shader.getPosHandle());
   glVertexAttribPointer(shader.getPosHandle(), 4, GL_FLOAT, 0, 0, 0);

   //tex coords
   glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
   glEnableVertexAttribArray(shader.getTexCoordHandle());
   glVertexAttribPointer(shader.getTexCoordHandle(), 2, GL_FLOAT, 0, 0, 0);

   //indices
   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffers[2] );
   glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(faceIndex), faceIndex, GL_STATIC_DRAW );

	//unbind
   glBindVertexArray(0);

	dr.setVAO(tmpVAO);
	dr.setTex(outlineTexture());
	dr.setNElements(faceCount*3);

   return dr;
}

vector<triangle> getConvexIndices(int n){
	vector<triangle> indices;
	for (GLuint i=0;i<n/2-1;i++){
		indices.push_back({i,n-i-1,n-i-2});//odd
		indices.push_back({i,i+1,n-i-2});//even
	}
	return indices;
}

vector<Cycle> getRigCycles(string svgFile){//TiXmlElement * rig){
	vector<Cycle> cycles;
	vector<Pose> poses;
	vector<fdualquat> joints;
	vec3 T;
	vec4 R;

	TiXmlDocument doc(svgFile);
   if (!doc.LoadFile()){
      cout << "Unable to load SVG file " << svgFile << "\n";
   }

   TiXmlHandle mHandle(&doc);
	TiXmlElement * rig, * cycle, * pose, * transform;

	rig = mHandle.FirstChild("svg").FirstChild("g").FirstChild("rig").ToElement();
		if (!rig) 
			return cycles;

	for (cycle = rig->FirstChildElement("cycle"); cycle; cycle=cycle->NextSiblingElement("cycle")){
		for (pose = cycle->FirstChildElement("pose"); pose; pose=pose->NextSiblingElement("pose")){
			for (transform=pose->FirstChildElement("trans"); transform; transform=transform->NextSiblingElement("trans")){
				string inStr = transform->Attribute("T");
				size_t pos=0;
				string d=",";
				for (int i=0;i<3;i++){
					pos = inStr.find(d);
					stringstream(inStr.substr(0,pos)) >> T[i];
					inStr.erase(0,pos+d.length());
				}
				inStr = transform->Attribute("R");
				pos=0;
				for (int i=0;i<4;i++){
					pos = inStr.find(d);//in degrees
					stringstream(inStr.substr(0,pos)) >> R[i];
					inStr.erase(0,pos+d.length());
				}
				//float th = degToRad(R.w)/2;
				//R = fquat(cos(th), sin(th)*vec3(R.x,R.y,R.z));
				//joints.emplace_back(T,R);
				joints.push_back((joints.size() ? joints.back() : fdualquat())*createDQ_t(T) * createDQ_r(R));
			}
			poses.emplace_back(joints);
			joints.clear();
		}
		cycles.emplace_back(poses);
		poses.clear();
	}
	return cycles;
}

map<string,string> getSVGPathMap(string svgFile){
	map<string,string> pathMap;

	TiXmlDocument doc(svgFile);
	if (!doc.LoadFile()){
		cout << "Unable to load SVG file " << svgFile << "\n";
	}

	TiXmlHandle mHandle(&doc);
	TiXmlElement * mElement = mHandle.FirstChild("svg").FirstChild("g").FirstChild("path").ToElement();
	if (!mElement){
		cout << "Couldn't find path element in SVG document\n";
	}
	
	while (mElement){
		pathMap[mElement->Attribute("id")] = mElement->Attribute("d");
      mElement = mElement->NextSiblingElement("path");//->ToElement();
   }
	
	mElement = mHandle.FirstChild("svg").FirstChild("g").FirstChild("rig").ToElement();
	
	return pathMap;
}

vector<vec4> getPathPoints(string pathStr){
	vector<vec4> ret;
	vec4 p;
	const string d1=" ", d2=",";
	size_t pos=0;
	bool relative;
	relative = (pathStr[0] == 'm');
	pathStr = pathStr.substr(2,pathStr.length());
	while (pos != string::npos){
		switch (pathStr[0]){
         case 'L':
         case 'l':
            pos = pathStr.find(d1);
            pathStr.erase(0,pos+d1.length());
            break;
         case 'z':
         case 'Z':
            pos = string::npos;
            break;
         default:	
	         pos = pathStr.find(d2);
            string ptStr = pathStr.substr(0,pos);
            if (!(stringstream(ptStr) >> p.x))
               cout << "Incorrect path string\n";
            pathStr.erase(0,pos+d2.length());
            pos = pathStr.find(d1);
            ptStr = pathStr.substr(0,pos);
            if (!(stringstream(ptStr) >> p.y))
               cout << "Incorrect path string\n";
            pathStr.erase(0,pos+d2.length());
            if (ret.size() && relative)
               p += ret.back();
            ret.push_back(p);
      }
	}

	return ret;
}

geoInfo SVGtoGeometry(string svgFile, bool rigged){
   vector<vec4> vertices;
	vector<vec2> texCoords;
   vector<triangle> indices;
	vector<vec3> weights;
	vec3 offset;

	map<string,string> pathMap = getSVGPathMap(svgFile);

	string outlineStr = pathMap.at("outline"), d1=" ", d2=",";

	vector<vec4> tmp = getPathPoints(outlineStr);

	if (outlineStr.length()<=1){
		cout << "Error loading path string\n";
		return {vertices, texCoords, indices};
	}
	bool relative = (outlineStr[0] == 'm');
	outlineStr = outlineStr.substr(2,outlineStr.length());
	size_t pos;
	for (int i=0;i<tmp.size();i++)
		texCoords.push_back(vec2(tmp[i]));	

	vec2 m=texCoords.front(), M=texCoords.front();
	vector<vec2>::iterator it;
	for (it=texCoords.begin(); it!=texCoords.end(); it++){
		m.x = min(m.x,it->x); m.y = min(m.y,it->y);
		M.x = max(M.x,it->x); M.y = max(M.y,it->y);
	}
	M -= m;
	for (it=texCoords.begin(); it!=texCoords.end(); it++){
		*it -= m;
		vec2 vert = vec2(*it);
		*it /= M;
		vert.y = M.y-vert.y;
		vertices.push_back(vec4(vert/max(M.x,M.y),0,1));
	}

	//Maybe change this to check for concavity...much later
	indices = getConvexIndices(vertices.size());

	if (rigged){
		string jointStr = pathMap.at("joints");
		vector<vec2> BonePoints;
		tmp = getPathPoints(jointStr);

		for (int i=0;i<tmp.size();i++)
			BonePoints.push_back((vec2(tmp[i])-m)/max(M.x,M.y));

		//Use this opportunity to switch the transform origin
		for (int j=0;j<vertices.size();j++){
			vec3 w;//this is always 3...
			for (int i=0;i<BonePoints.size();i++)
				w[2-i]=(float)exp(-pow(BonePoints[i].y-vertices[j].y,2)/.15f);
			w=glm::normalize(w);
			weights.push_back(w);
		}

		float len = 0;
		int k=0;
		for (int i=0;i<BonePoints.size();i++){
			if (glm::length(BonePoints[i]) > len){
				k=i;
				len=glm::length(BonePoints[i]);
			}
		}

		for (int i=0;i<vertices.size();i++)
			vertices[i] -= vec4(BonePoints[k],0,0);
		offset = vec3(BonePoints[k],0);
	}

	return {vertices, texCoords, indices, weights, offset};
}

GLuint genVAO(geoInfo gI, JShader& shader){
	GLuint VAO;
   glGenVertexArrays(1, &VAO);
   glBindVertexArray(VAO);

   GLuint buffers[4];
   glGenBuffers(4, buffers);

   //vertices
   glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vec4)*gI.vertices.size(), gI.vertices.data(), GL_STATIC_DRAW);
   glEnableVertexAttribArray(shader.getPosHandle());
   glVertexAttribPointer(shader.getPosHandle(), 4, GL_FLOAT, 0, 0, 0);

   //tex coords
   glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vec2)*gI.texCoords.size(), gI.texCoords.data(), GL_STATIC_DRAW);
   glEnableVertexAttribArray(shader.getTexCoordHandle());
   glVertexAttribPointer(shader.getTexCoordHandle(), 2, GL_FLOAT, 0, 0, 0);

   //indices
   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffers[2] );
   glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(triangle)*gI.indices.size(), gI.indices.data(), GL_STATIC_DRAW );
	
	//weights
	if (gI.weights.size()){
		glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*gI.weights.size(), gI.weights.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(shader.getWeightHandle());
		glVertexAttribPointer(shader.getWeightHandle(), 3, GL_FLOAT, 0, 0, 0);
	}
   
	glBindVertexArray(0);

	return VAO;
}
