// Board.h
#include <iostream>
#include <tuple>
#include "opencv2/opencv.hpp"
#include "aruco/aruco.h"
#include "square.h"

#DEFINE HALF_SQUARE 1.015f

class Board {
public:
	Board(cv::Mat, std::vector<aruco::Marker>, aruco::CameraParameters );

	std::map<int,std::tuple<int,int>> markerBoardPositions;
	std::map<std::tuple<int,int>,Square> squares;

private:
	void Board::pairMarkersWithBoardPositions();
	void Board::generateSquares();

	cv::Mat frame, cameraMatrix, cameraDistortion;
	std::vector<aruco::Marker> markers;
	std::tuple<int,int> size;
};