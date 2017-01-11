/*
Matt Martin
Jan 5, 2017
Board class
*/

#include "board.h"

Board::Board(aruco::CameraParameters camParams) {
	cameraMatrix = camParams.CameraMatrix;
	cameraDistortion = camParams.Distorsion;
	size = std::make_tuple(8,8);
	pairMarkersWithBoardPositions(); 
	generateSquares();
}

void Board::generateSquares() {
	for (int i = 0; i < std::get<0>(size); ++i) {
		for (int j = 0; j < std::get<1>(size); ++j) {
			std::tuple<int,int> pos = std::make_tuple(j,i);
			Square* sq = new Square(j,i,markerBoardPositions[std::make_tuple(j/2,i/2)]);
			squares[pos] = sq;
		}
	}
}

aruco::Marker* Board::getMarkerById(int id) {
	for (int i = 0; i < markers.size(); ++i) {
		if (id == markers[i].id) {
			return &markers[i];
		}
	}
	return nullptr;
}

void Board::pairMarkersWithBoardPositions() {
	// generate dictionary for wuick lookup of which marker is in which position
	markerBoardPositions[std::make_tuple(0,0)] = 86;
	markerBoardPositions[std::make_tuple(1,0)] = 79;
	markerBoardPositions[std::make_tuple(2,0)] = 58;
	markerBoardPositions[std::make_tuple(3,0)] = 81;
	markerBoardPositions[std::make_tuple(0,1)] = 213;
	markerBoardPositions[std::make_tuple(1,1)] = 73;
	markerBoardPositions[std::make_tuple(2,1)] = 152;
	markerBoardPositions[std::make_tuple(3,1)] = 63;
	markerBoardPositions[std::make_tuple(0,2)] = 163;
	markerBoardPositions[std::make_tuple(1,2)] = 30;
	markerBoardPositions[std::make_tuple(2,2)] = 113;
	markerBoardPositions[std::make_tuple(3,2)] = 192;
	markerBoardPositions[std::make_tuple(0,3)] = 69;
	markerBoardPositions[std::make_tuple(1,3)] = 144;
	markerBoardPositions[std::make_tuple(2,3)] = 206;
	markerBoardPositions[std::make_tuple(3,3)] = 244;
}


void Board::update(cv::Mat *frame, std::vector<aruco::Marker> visibleMarkers) {
	cout << "Updating board" << endl;
	markers = visibleMarkers;

	// draw squares
	for (int i = 0; i < std::get<0>(size); ++i) {
		for (int j = 0; j < std::get<1>(size); ++j) {
			std::tuple<int,int> pos = std::make_tuple(j,i);
			Square *square = squares[pos];
			aruco::Marker* m = getMarkerById(square->markerId);
			square->draw(frame,m,cameraMatrix,cameraDistortion);
		}
	}
	// draw pieces
}
