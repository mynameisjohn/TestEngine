#include "XMLReader.h"
#include <exception>

template <typename T>
void fillIn(TiXmlElement * el, string id, T& v){
	try{
      stringstream(el->Attribute(id.c_str())) >> v;
   }
   catch (exception& e){/*
      cout << "XML attribute " << id << "in element " << 
		string(el->Attribute("id")) << " did not fit in variable. " << endl;*/
   }
}

void fillIt(TiXmlElement * el, string id, unsigned int &v){
	try{
      stringstream(el->Attribute(id.c_str())) >> v;
   }
   catch (exception& e){/*
      cout << "XML attribute " << id << "in element " << 
		string(el->Attribute("id")) << " did not fit in variable. " << endl;*/
   }
}

vector<triangle> getConvexIndices(int n){
	vector<triangle> indices;
	for (unsigned int i=0;i<n/2-1;i++){
		indices.push_back({i,n-i-1,n-i-2});//odd
		indices.push_back({i,i+1,n-i-2});//even
	}
	return indices;
}

vector<Cycle> getRigCycles(string svgFile){//TiXmlElement * rig){
	vector<Cycle> cycles;
	vector<Pose> poses;
	//vector<fdualquat> joints;
	vector<QuatVec> Joints;
	vec3 T;
	vec4 R;

	TiXmlDocument doc(svgFile);
   if (!doc.LoadFile()){
      cout << "Unable to load SVG file " << svgFile << "\n";
   }

   TiXmlHandle mHandle(&doc);
	TiXmlElement * rig, * cycle, * pose, * transform;

	rig = mHandle.FirstChild("rig").ToElement();//.FirstChild("g").FirstChild("rig").ToElement();
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
//cout << R << "\n" << getRQ(R) << "\n" << glm::dot(getRQ(R),getRQ(vec4(R.x-25,R.y,R.z,R.w))) << "\n" << endl;
				Joints.push_back((Joints.size() ? Joints.back() : QuatVec())*QuatVec(T,getRQ(R)));
			}
			float t=.1, dt=.1;
			fillIn(pose, "t", t);
			fillIn(pose, "dt", dt);
			poses.emplace_back(Joints);//, t, dt);
			Joints.clear();
		}
		unsigned int C=1;
		fillIn(cycle,"C", C);
		cycles.emplace_back(poses, C);
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
		switch (pathStr[0]){//todo: add support for Q and C
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

		//This assumes a lot (i.e joints drawn bottom-up)
		for (int j=0;j<vertices.size();j++){
			vec3 w;//this is always 3...
			for (int i=0;i<BonePoints.size();i++)
				w[i]=(float)exp(-pow(glm::length(BonePoints[i]-vec2(vertices[j])),2)/.1f);
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
