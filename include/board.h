// Board.h
#include <iostream>
#include <fstream>
#include <sstream>
#include "opencv2/opencv.hpp"
#include "aruco/aruco.h"
#include "square.h"
#include "GL/gl.h"
#include "GL/glut.h"
#include "objloader.h"


class Board {
public:
	Board();
	Board( aruco::CameraParameters camParams  );
	Board( aruco::CameraParameters camParams , float );
	void update( cv::Mat*, std::vector<aruco::Marker> visibleMarkers);
	void update( std::vector<aruco::Marker> visibleMarkers);

	std::map<std::tuple<int,int>,int> markerBoardPositions;
	std::map<std::tuple<int,int>,Square*> squares;

private:
	void pairMarkersWithBoardPositions();
	void generateSquares();
	aruco::Marker* getMarkerById(int);
	void drawOBJ();

	cv::Mat frame, cameraMatrix, cameraDistortion;
	std::vector<aruco::Marker> markers;
	std::tuple<int,int> size;
	OBJLoader obj;
	std::vector<Vertex> verts;
    std::vector<NormalVector> norms;
    std::vector<Face> faces;
	float markerSize;
};