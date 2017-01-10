/*
Matt Martin
Jan 5, 2017
Board class
*/

#include "board.h"

Board::Board(cv::Mat frame, std::vector<aruco::Marker> markers, aruco::CameraParameters camParams, std::tuple<int,int> size=std::make_tuple(8,8)) {
	frame = frame;
	markers = markers;
	cameraMatrix = camParams.CameraMatrix;
	camerDistortion = camParams.Distortion;
	size = size;
	pairMarkersWithBoardPositions();
	generateSquares();
}

void Board::generateSquares() {
	for (int i = 0; i < size; ++i)
	{
		/* code */
	}
}

void Board::pairMarkersWithBoardPositions() {
	markerBoardPositions[86] = std::make_tuple(0,0);
	markerBoardPositions[79] = std::make_tuple(1,0);
	markerBoardPositions[58] = std::make_tuple(2,0);
	markerBoardPositions[81] = std::make_tuple(3,0);
	markerBoardPositions[213] = std::make_tuple(0,1);
	markerBoardPositions[73] = std::make_tuple(1,1);
	markerBoardPositions[152] = std::make_tuple(2,1);
	markerBoardPositions[63] = std::make_tuple(3,1);
	markerBoardPositions[163] = std::make_tuple(0,2);
	markerBoardPositions[30] = std::make_tuple(1,2);
	markerBoardPositions[113] = std::make_tuple(2,2);
	markerBoardPositions[192] = std::make_tuple(3,2);
	markerBoardPositions[69] = std::make_tuple(0,3);
	markerBoardPositions[144] = std::make_tuple(1,3);
	markerBoardPositions[206] = std::make_tuple(2,3);
	markerBoardPositions[244] = std::make_tuple(3,3);
}


int main(int argc, char const *argv[]) {
	Board bd;
	
	return 0;
}