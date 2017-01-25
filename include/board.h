// Board.h
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <regex>
#include "GL/gl.h"
#include "GL/glut.h"
#include "opencv2/opencv.hpp"
#include "aruco/aruco.h"
#include "square.h"



class Board {
public:
	Board();
	Board( float );
	void updateGraphics( cv::Point3f );
	void updateGame();
	void executeCommand(string); 
	void pickSquare();

	std::map<std::tuple<int,int>,Square*> squares;

private:
	void generateSquaresandPieces();
	void generateValidCommandStarts();
	Piece* determinePieceType(int, int );
	void makeMove(std::tuple<int,int>,std::tuple<int,int>);
	void reset();

	std::tuple<int,int> size,firstPicked;
	float markerSize;
	char validCommandStarts[11];
};