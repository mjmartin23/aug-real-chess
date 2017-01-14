/* Matt Martin
Jan 13, 2016
OBJ loader header
*/

#include <iostream>
#include <vector>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <GL/gl.h>
#include <GL/glut.h>
using namespace std;


 struct NormalVector
 {
     GLfloat x, y, z;
 };

 struct Vertex
 {
     GLfloat x, y, z, w;
 };

 struct Face
 {
     int v1,v2,v3, vn1,vn2,vn3;
     char type;
     GLfloat x,y,z;
 };

class OBJLoader {
public:
	OBJLoader();
	OBJLoader(const char *);
	OBJLoader(const char * ,std::vector<Vertex> *, std::vector<NormalVector> *, std::vector<Face> *);

	void load(std::vector < Vertex > *, std::vector<NormalVector>*, std::vector<Face>*);

private:
	std::vector<std::string> split(string, string);

	const char * file;
	string line;

};