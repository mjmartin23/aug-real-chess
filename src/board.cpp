/*
Matt Martin
Jan 5, 2017
Board class
*/

#include "board.h"

Board::Board() {

}


Board::Board(float markerSizeParam) {
	markerSize = markerSizeParam;
	size = std::make_tuple(8,8);
	generateSquaresandPieces();
}

void Board::generateSquaresandPieces() {
	for (int i = 0; i < std::get<0>(size); ++i) {
		for (int j = 0; j < std::get<1>(size); ++j) {
			std::tuple<int,int> pos = std::make_tuple(j,i);
			Piece* p = determinePieceType(j,i);
			Square* sq = new Square(j,i,markerSize,p);
			squares[pos] = sq;
			cout<<"gen square at "<<i<<","<<j<<endl;
		}
	}
}

Piece* Board::determinePieceType(int j, int i) {
	int team;
	if (i == 0 || i == 1) {
		//white team
		team = 1;
		if (i == 0) {
			//back row
			if (j == 0 || j == 7) {
				// rook
				return new Rook(team,markerSize);
			} else if (j == 1 || j == 6) {
				return new Knight(team,markerSize);
			} else if (j == 2 || j == 5) {
				return new Bishop(team,markerSize);
			} else if (j == 3) {
				return new King(team,markerSize);
			} else {
				return new Queen(team,markerSize);
			}
		} else {
			//pawns
			return new Pawn(team,markerSize);
		}
	} else if (i == 6 || i == 7) {
		//black team
		team = 0;
		if (i == 7) {
			//back row
			if (j == 0 || j == 7) {
				return new Rook(team,markerSize);
			} else if (j == 1 || j == 6) {
				return new Knight(team,markerSize);
			} else if (j == 2 || j == 5) {
				return new Bishop(team,markerSize);
			} else if (j == 3) {
				return new King(team,markerSize);
			} else {
				return new Queen(team,markerSize);
			}
		} else {
			//pawns
			return new Pawn(team,markerSize);
		}
	} else {
		return nullptr;
	}
}

void Board::update() {
	//cout << "Updating board" << endl;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
	// draw squares
	for (int i = 0; i < std::get<0>(size); ++i) {
		for (int j = 0; j < std::get<1>(size); ++j) {
			std::tuple<int,int> pos = std::make_tuple(j,i);
			Square *square = squares[pos];
			square->draw();
		}
	}
	//draw pieces
	for (int i = 0; i < std::get<0>(size); ++i) {
		for (int j = 0; j < std::get<1>(size); ++j) {
			std::tuple<int,int> pos = std::make_tuple(j,i);
			Square *square = squares[pos];
			if (square->isOccupied) {
				square->drawPiece();
			}
		}
	}
}