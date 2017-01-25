/*
Matt Martin
Jan 5, 2017
Board class
*/

#include "board.h"

Board::Board() {

}


Board::Board(float markerSizeParam) {
	this->markerSize = markerSizeParam;
	this->size = std::make_tuple(8,8);
	this->firstPicked = std::make_tuple(-1,-1);
	generateSquaresandPieces();
	generateValidCommandStarts();
}

void Board::reset() {
	this->firstPicked = std::make_tuple(-1,-1);
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

void Board::generateValidCommandStarts() {
	validCommandStarts[0] = 'a';validCommandStarts[1] = 'b';validCommandStarts[2] = 'c';
	validCommandStarts[3] = 'd';validCommandStarts[4] = 'e';validCommandStarts[5] = 'f';
	validCommandStarts[6] = 'g';validCommandStarts[7] = 'h';validCommandStarts[8] = 'q';
	validCommandStarts[9] = 'r';validCommandStarts[10] = 'p';
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
			} else if (j == 4) {
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
			} else if (j == 4) {
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

void Board::updateGraphics(cv::Point3f pointer) {
	//cout << "Updating board" << endl;
	// draw squares
	for (int i = 0; i < std::get<0>(size); ++i) {
		for (int j = 0; j < std::get<1>(size); ++j) {
			std::tuple<int,int> pos = std::make_tuple(j,i);
			Square *square = squares[pos];
			// check if pointer is in this square
			square->contains(pointer);
			//cout<<square->selected<<endl;
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

void Board::pickSquare() {
	for (int i = 0; i < std::get<0>(size); ++i) {
		for (int j = 0; j < std::get<1>(size); ++j) {
			std::tuple<int,int> pos = std::make_tuple(j,i);
			Square *square = squares[pos];
			// check if pointer is in this square
			if (square->selected) {
				cout<<"square at "<<j<<","<<i<<" selected"<<endl;
				if (this->firstPicked == std::make_tuple(-1,-1)) {
					this->firstPicked = pos;
					//cout<<"fp"<<endl;
				} else {
					this->makeMove(this->firstPicked, pos);
					this->firstPicked = std::make_tuple(-1,-1);
					//cout<<"moving"<<endl;
				}
			}
		}
	}
}

void Board::executeCommand(string cmd) {
	char* end = validCommandStarts + sizeof(validCommandStarts) / sizeof(validCommandStarts[0]);
	if (std::find(validCommandStarts, end, cmd[0]) != end) {
		//it exists
		std::regex r("[a-h][1-8][a-h][1-8]");
		if (cmd == "quit" || cmd == "q") {
			cout<<"quitting"<<endl;
      		exit(0);
		} else if (cmd == "reset" || cmd == "r") {
			cout<<"resetting"<<endl;
			this->reset();
		// } else if (cmd == "pick" || cmd == "p") {
		// 	this->pickSquare();
		} else if (std::regex_match(cmd,r)) {
			//cout<<cmd<<" matched with regex"<<endl;
			std::tuple<int,int> startPos = std::make_tuple(((int)cmd[0])-97,(int)cmd[1]-49);
			std::tuple<int,int> endPos = std::make_tuple(((int)cmd[2])-97,(int)cmd[3]-49);
			makeMove(startPos,endPos);
		} else {
			cout<<"invalid input here: "<<cmd<<endl;
		}
	} else {
		cout<<"invalid input: "<<cmd<<endl;
	}
}

void Board::makeMove(std::tuple<int,int> start, std::tuple<int,int> end) {
	Square *startSquare = squares[start];
	Square *endSquare = squares[end];
	if (startSquare->isOccupied) {
		//move startSquare's Piece
		Piece* piece = startSquare->piece;
		cv::Point move = cv::Point( std::get<0>(start) - std::get<0>(end), std::get<1>(start) - std::get<1>(end));
		//cout<<move<<endl;
		// for (cv::Point p : piece->moveSet) {
		// 	cout<<p<<endl;
		// }
		if (piece->inMoveSet(move)) {
			//in move set of piece
			Piece *endPiece = endSquare->piece;
			if (endSquare->isOccupied) {
				if (endPiece->team == piece->team) {
					cout<<"invalid move: your team owns the destination"<<endl;
				} else {
					cout<<"moving"<<endl;
					endSquare->removePiece();
					endSquare->receivePiece(piece);
					startSquare->removePiece();
				}
			} else {
				cout<<"moving"<<endl;
				endSquare->receivePiece(piece);
				startSquare->removePiece();
			}
		}
	} else {
		cout<<"invalid move: there is no piece at start"<<endl;
	}
}

void Board::updateGame() {
	// update valid movesets for each piece
	// see if check or checkmate
}