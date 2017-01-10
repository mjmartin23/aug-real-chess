// Board.h
#include <iostream>
#include "opencv2/opencv.hpp"
#include "aruco/aruco.h"
#include "square.h"

class Board {
public:
	Board( aruco::CameraParameters camParams );
	void update(cv::Mat *frame, std::vector<aruco::Marker> visibleMarkers);

	std::map<std::tuple<int,int>,int> markerBoardPositions;
	std::map<std::tuple<int,int>,Square*> squares;

private:
	void pairMarkersWithBoardPositions();
	void generateSquares();
	aruco::Marker* getMarkerById(int id);

	cv::Mat frame, cameraMatrix, cameraDistortion;
	std::vector<aruco::Marker> markers;
	std::tuple<int,int> size;
};