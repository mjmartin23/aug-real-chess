/* Matt Martin
Jan 9,2017
Square class for chessboard
*/

#include "aruco/aruco.h"
#include "opencv2/opencv.hpp"
#include "GL/gl.h"
#include "GL/glut.h"
// #include <iostream>
#include "piece.h"

#define HALF_SQUARE 1.275f

class Square {
public:
	Square(int,int,int,float,Piece*);
	void draw(aruco::Marker*);
	void drawPiece(aruco::Marker*);

	int markerId;
	bool isOccupied;

private:
	void setCorners();
	void setColor();

	bool seen;
	int row,col;
	int lastSeen;
	
	GLfloat color[4],grey[4],lowAmbient[4],white[4];
	std::vector<cv::Point3f> corners;
	float markerSize;
	Piece* piece;
};