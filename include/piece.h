/* Matt Martin
Jan 14, 2017
Piece class header
*/

#include <iostream>
#include "aruco/aruco.h"

#include "GL/gl.h"
#include "GL/glut.h"
#include "objloader.h"

class Piece
{
public:
	Piece();
	Piece(int , float, const char *);

	void draw(aruco::Marker* marker, std::vector<cv::Point3f> corners);

private:
	void setColor();

	float scale;

	int team; //1 is white 0 is black
	OBJLoader obj;
	std::vector<Vertex> verts;
    std::vector<NormalVector> norms;
    std::vector<Face> faces;
    GLfloat color[4],grey[4],lowAmbient[4],white[4];

};