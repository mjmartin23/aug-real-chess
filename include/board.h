// Board.h
#include <iostream>
#include <fstream>
#include <sstream>
#include "GL/gl.h"
#include "GL/glut.h"
#include "opencv2/opencv.hpp"
#include "aruco/aruco.h"
#include "square.h"



class Board {
public:
	Board();
	Board( float );
	void update(  );

	std::map<std::tuple<int,int>,Square*> squares;

private:
	void generateSquaresandPieces();
	Piece* determinePieceType(int, int );

	std::tuple<int,int> size;
	float markerSize;
};