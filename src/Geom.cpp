#include "Geom.h"

#ifndef TIXML_USE_STL
#define TIXML_USE_STL
#endif
#include <tinyxml.h>
#include <sstream>
#include <glm/gtx/transform.hpp>
#include "Textures.h"

Rig initRigFromSVG(string fileName, JShader& shader){
	Rig r(&shader);
	geoInfo gI = SVGtoGeometry(fileName);
   GLuint VAO = genVAO(gI, shader);
	string imageName = fileName.substr(0,fileName.length()-4)+".png";
   r.setVAO(VAO);
   r.setTex(fromImage(imageName));//outlineTexture());
   r.setNElements(3*gI.indices.size());

	return r;
}

//assumes "x.svg" as input, as well as some "x.png" texture
Drawable initPolyFromSVG(string fileName, JShader& shader){
	Drawable dr(&shader,2);

	geoInfo gI = SVGtoGeometry(fileName);
	gI.weights = vector<vec3>();
	GLuint VAO = genVAO(gI, shader);
	
	string imageName = fileName.substr(0,fileName.length()-4)+".png";
   dr.setVAO(VAO);
   dr.setTex(fromImage(imageName));//outlineTexture());
   dr.setNElements(3*gI.indices.size());

   return dr;
}

Drawable initQuad(JShader& shader){
	Drawable dr(&shader,2);

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
	dr.setTex(fromImage("rect.png"));//outlineTexture());
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

map<string,string> getSVGPathMap(string svgFile){
	map<string,string> pathMap;

	TiXmlDocument doc(svgFile);
	if (!doc.LoadFile()){
		cout << "Unable to load SVG file\n";
	}

	TiXmlHandle mHandle(&doc);
	TiXmlElement * mElement = mHandle.FirstChild("svg").FirstChild("g").FirstChild("path").ToElement();
	if (!mElement){
		cout << "Couldn't find path element in SVG document\n";
	}
//	string ret = mElement->Attribute("d");
	while (mElement){
		pathMap[mElement->Attribute("id")] = mElement->Attribute("d");
//    cout << string(mElement->Attribute("id")) + "\n \n";
      mElement = mElement->NextSiblingElement("path");//->ToElement();
   }

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
geoInfo SVGtoGeometry(string svgFile){
   vector<vec4> vertices;
	vector<vec2> texCoords;
   vector<triangle> indices;
	vector<vec3> weights;


	map<string,string> pathMap = getSVGPathMap(svgFile);

	string outlineStr = pathMap.at("outline"), jointStr = pathMap.at("joints"), d1=" ", d2=",";

/*	
	vector<vec4>::iterator vIt;
	vector<vec4> temp;
	vec4 m,M;


	//Form texture coordinates	
	temp = getPathPoints(outlineStr);
	for (vIt=temp.begin(); vIt!=temp.end(); vIt++){
		m.x = min(m.x,vIt->x); m.y = min(m.y,vIt->y);
      M.x = max(M.x,vIt->x); M.y = max(M.y,vIt->y);
	}
	M -= m;
	for (vIt=temp.begin(); vIt!=temp.end(); vIt++){
		vec2 tC;
		vec4 vert;
		*vIt -= m;
		tC = vec2(*vIt/M);
		texCoords.push_back(tC);
		vIt->y=M.y-vIt->y;//flipping y direction
		vert = *vIt/max(M.x,M.y);
		vert.z=0; vert.w=1;
		vertices.push_back(vert);
		cout << vertices.back() << endl;
	}

	indices = getConvexIndices(vertices.size());

	temp = getPathPoints(jointStr);//temp.size() better be 3
	cout << temp.size() << endl;
	for (vIt=vertices.begin(); vIt!=vertices.end(); vIt++){
		vec3 w(
			(float)exp(-pow(temp[0].y-vIt->y,2)/.15f),
			(float)exp(-pow(temp[1].y-vIt->y,2)/.15f),
			(float)exp(-pow(temp[2].y-vIt->y,2)/.15f)
		);
//		weights.push_back(glm::normalize(w));
	}
*/


	vector<vec4> tmp = getPathPoints(outlineStr);

	if (outlineStr.length()<=1){
		cout << "Error loading path string\n";
		return {vertices, texCoords, indices};
	}
	bool relative = (outlineStr[0] == 'm');
	outlineStr = outlineStr.substr(2,outlineStr.length());
	size_t pos;
/*	
	while (pos != string::npos){
		switch (outlineStr[0]){
			case 'L':
			case 'l':
            pos = outlineStr.find(d1);
            outlineStr.erase(0,pos+d1.length());
            break;
         case 'z':
         case 'Z':
            pos = string::npos;
            break;
         default:
				vec2 p;
				pos = outlineStr.find(d2);
				string ptStr = outlineStr.substr(0,pos);
				if (!(stringstream(ptStr) >> p.x))
					cout << "Incorrect path string\n";
				outlineStr.erase(0,pos+d2.length());
				pos = outlineStr.find(d1);
				ptStr = outlineStr.substr(0,pos);
				if (!(stringstream(ptStr) >> p.y))
               cout << "Incorrect path string\n";
				outlineStr.erase(0,pos+d2.length());
				if (texCoords.size() && relative)
					p += texCoords.back();
//				texCoords.push_back(p);
		}
	}
*/
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
//		cout << vertices.back() << endl;
	}

	//Maybe change this to check for concavity...much later
	indices = getConvexIndices(vertices.size());

	//Hard coded for now....
	vector<vec2> BonePoints;
/*
	pos=0;
   relative = (jointStr[0] == 'm');
   jointStr = jointStr.substr(2,jointStr.length());
   while (pos != string::npos){
      switch (jointStr[0]){
         case 'L':
         case 'l':
            pos = jointStr.find(d1);
            jointStr.erase(0,pos+d1.length());
            break;
         case 'z':
         case 'Z':
            pos = string::npos;
            break;
         default:
            vec2 p;
            pos = jointStr.find(d2);
            string ptStr = jointStr.substr(0,pos);
            if (!(stringstream(ptStr) >> p.x))
               cout << "Incorrect path string\n";
            jointStr.erase(0,pos+d2.length());
            pos = jointStr.find(d1);
            ptStr = jointStr.substr(0,pos);
            if (!(stringstream(ptStr) >> p.y))
               cout << "Incorrect path string\n";
            jointStr.erase(0,pos+d2.length());
				p = (p-m)/M;
            if (BonePoints.size() && relative)
               p += BonePoints.back();
            //BonePoints.push_back(p);
      }
   }
*/
	tmp = getPathPoints(jointStr);

	for (int i=0;i<tmp.size();i++)
      BonePoints.push_back((vec2(tmp[i])-m)/M);

	for (int j=0;j<vertices.size();j++){
		vec3 w;//this is always 3...
//		cout << vertices[j] << endl;
		for (int i=0;i<BonePoints.size();i++)
			w[2-i]=(float)exp(-pow(BonePoints[i].y-vertices[j].y,2)/.15f);
		w=glm::normalize(w);
		weights.push_back(w);
	}

/*
	weights = {
{0.984471, 0.175527, 0.00261039},
 
{0.894276, 0.447171, 0.0175529},
 
{0.505167, 0.856476, 0.10609},
 
{0.284419, 0.934142, 0.215603},
 
{0.178233, 0.927066, 0.32982},
 
{0.0418761, 0.68865, 0.723883},
 
{0.00269803, 0.212666, 0.977121},
 
{0.00244645, 0.203027, 0.97917},
 
{0.0359489, 0.653881, 0.755742},
 
{0.144524, 0.910338, 0.38781},
 
{0.284419, 0.934142, 0.215603},
 
{0.514486, 0.851283, 0.103057},
 
{0.898393, 0.43887, 0.0168529},
 
{0.984471, 0.175527, 0.00261039}};
*/
	return {vertices, texCoords, indices, weights};
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
