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
	generateValidCommandStarts();
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
	validCommandStarts[9] = 'r';
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

void Board::updateGraphics() {
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

void Board::executeCommand(string cmd) {
	char* end = validCommandStarts + sizeof(validCommandStarts) / sizeof(validCommandStarts[0]);
	if (std::find(validCommandStarts, end, cmd[0]) != end) {
		//it exists
		std::regex r("[a-h][1-8][a-h][1-8]");
		if (cmd == "quit") {
			cout<<"quitting"<<endl;
		} else if (cmd == "reset") {
			cout<<"resetting"<<endl;
		} else if (std::regex_match(cmd,r)) {
			//cout<<cmd<<" matched with regex"<<endl;
			makeMove(cmd.substr(0,2),cmd.substr(2,2));
		} else {
			cout<<"invalid input here: "<<cmd<<endl;
		}
	} else {
		cout<<"invalid input: "<<cmd<<endl;
	}
}

void Board::makeMove(string start, string end) {
	std::tuple<int,int> startPos = std::make_tuple(((int)start[0])-97,(int)start[1]-49);
	std::tuple<int,int> endPos = std::make_tuple(((int)end[0])-97,(int)end[1]-49);
	Square *startSquare = squares[startPos];
	Square *endSquare = squares[endPos];
	//cout<<"got squares: "<<std::get<0>(startPos)<<" "<<std::get<1>(startPos)<<","<<std::get<0>(endPos)<<" "<<std::get<1>(endPos)<<endl;
	if (startSquare->isOccupied) {
		//move startSquare's Piece
		Piece* piece = startSquare->piece;
		endSquare->receivePiece(piece);
		startSquare->removePiece();
	} else {
		cout<<"invalid move: there is no piece at "<<start<<endl;
	}
}

void Board::updateGame() {
	// check for input if no input do nothing
	// std::string name;
	// //getline(std::cin, name);

	// if (name.empty()) {
	//     return;
	// } else {
	// 	cout<<"entered: "<<name<<endl;
	// }
}