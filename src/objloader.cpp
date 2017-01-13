/* Matt Martin
Jan 13, 2016
OBJ loader
*/

#include "objloader.h"

OBJLoader::OBJLoader() {

}

OBJLoader::OBJLoader(const char * filename) {
	file = filename;
}

std::vector<std::string> split(std::string str, string delimiter) {
	size_t pos = 0;
	std::string token;
	std::vector<std::string> splitted;
	while ((pos = str.find(delimiter)) != std::string::npos) {
	    token = str.substr(0, pos);
	    splitted.push_back( token );
	    str.erase(0, pos + delimiter.length());
	}
	return splitted;
}

void OBJLoader::load(std::vector<Vertex> verts, std::vector<NormalVector> norms, std::vector<Face> faces) {
	ifstream myFile(file);
	int vertCount = 0;
	int normCount = 0;
	int faceCount = 0;
	while (std::getline(myFile,line)) {
		std::vector<std::string> splitLine = split(line," ");
		if (splitLine[0] == "#") {
			continue;
		} else if (splitLine[0] == "v") {
			Vertex v;
			v.x = std::stof(splitLine[1]);
			v.y = std::stof(splitLine[2]);
			v.z = std::stof(splitLine[3]);
			verts[vertCount] = v;
			vertCount++;
		} else if (splitLine[0] == "vn") {
			NormalVector v;
			v.x = std::stof(splitLine[1]);
			v.y = std::stof(splitLine[2]);
			v.z = std::stof(splitLine[3]);
			norms[normCount] = v;
			normCount++;
		} else if (splitLine[0] == "f") {
			Face f;
			std::vector<std::string> inner;
			inner = split(splitLine[1],"//");
			f.v1 = stoi(inner[0]); f.vn1 = stoi(inner[1]);
			inner = split(splitLine[2],"//");
			f.v2 = stoi(inner[0]); f.vn2 = stoi(inner[1]);
			inner = split(splitLine[3],"//");
			f.v3 = stoi(inner[0]); f.vn3 = stoi(inner[1]);
			faces[faceCount] = f;
			faceCount++;
		} else {
			cerr<<"invalid first string "<<splitLine[0]<<endl;
		}
		
		//myFile.close();
	}
}

int main(int argc, char const *argv[])
{
	OBJLoader obj = OBJLoader("../data/models/bishop.obj");
	std::vector<Vertex> verts;
	std::vector<NormalVector> norms;
	std::vector<Face> faces;
	obj.load(verts,norms,faces);
	// cout<<verts[0]<<endl;
	// cout<<norms[0]<<endl;
	// cout<<faces[0]<<endl;
	return 0;
}