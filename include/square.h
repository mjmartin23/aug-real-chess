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

class Square {
public:
	Square(int,int,float,Piece*);
	void draw();
	void drawPiece();

	bool isOccupied;

private:
	void setCorners();
	void setColor();

	int row,col;
	
	GLfloat color[4],grey[4],lowAmbient[4],white[4];
	std::vector<cv::Point3f> corners;
	float squareSize;
	Piece* piece;
};