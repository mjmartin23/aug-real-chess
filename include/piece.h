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
	Piece(int , float);

	void draw(std::vector<cv::Point3f> corners);

protected:
	void setColor();

	float scale;
	cv::Point3f center;
	int team; //1 is white 0 is black
	OBJLoader obj;
	std::vector<Vertex> verts;
    std::vector<NormalVector> norms;
    std::vector<Face> faces;
    GLfloat colorA[4],colorD[4],grey[4],lowAmbient[4],white[4];

    std::vector<cv::Point2i> moveSet;
    const char * objPath;
    float zTranslate;

};

class King : public Piece {
public:
	King(int, float);
};

class Queen : public Piece {
public:
	Queen(int, float);
};

class Bishop : public Piece {
public:
	Bishop(int, float);
};

class Knight : public Piece {
public:
	Knight(int, float);
};

class Rook : public Piece {
public:
	Rook(int, float);
};

class Pawn : public Piece {
public:
	Pawn(int, float);
};