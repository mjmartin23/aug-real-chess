// Board.h
#include <iostream>
#include <fstream>
#include <sstream>
#include "opencv2/opencv.hpp"
#include "aruco/aruco.h"
#include "square.h"


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
	aruco::Marker* getMarkerById(int id);

	cv::Mat frame, cameraMatrix, cameraDistortion;
	std::vector<aruco::Marker> markers;
	std::tuple<int,int> size;
	float markerSize;
};