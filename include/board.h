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
	Board( aruco::CameraParameters, float );
	void update(  );

	std::map<std::tuple<int,int>,int> markerBoardPositions;
	std::map<std::tuple<int,int>,Square*> squares;

private:
	void pairMarkersWithBoardPositions();
	void generateSquaresandPieces();
	Piece* determinePieceType(int, int );
	aruco::Marker* getMarkerById(int);
	//void drawOBJ();

	cv::Mat frame, cameraMatrix, cameraDistortion;
	std::vector<aruco::Marker> markers;
	std::tuple<int,int> size;
	OBJLoader obj;
	std::vector<Vertex> verts;
    std::vector<NormalVector> norms;
    std::vector<Face> faces;
	float markerSize;
};