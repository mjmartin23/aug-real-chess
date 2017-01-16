/*
Matt Martin
Jan 5, 2017
Board class
*/

#include "board.h"

Board::Board() {

}


Board::Board(aruco::CameraParameters camParams, float markerSizeParam) {
	cameraMatrix = camParams.CameraMatrix;
	cameraDistortion = camParams.Distorsion;
	markerSize = markerSizeParam;
	size = std::make_tuple(8,8);
	//obj = OBJLoader("../data/models/bishop.obj",&verts,&norms,&faces);
	//cout<<"back in board"<<endl;
	pairMarkersWithBoardPositions(); 
	generateSquaresandPieces();
}

void Board::generateSquaresandPieces() {
	for (int i = 0; i < std::get<0>(size); ++i) {
		for (int j = 0; j < std::get<1>(size); ++j) {
			std::tuple<int,int> pos = std::make_tuple(j,i);
			Piece* p = determinePieceType(j,i);
			Square* sq = new Square(j,i,
									markerBoardPositions[std::make_tuple(j/2,i/2)],
									markerSize,
									p);
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
		//cout<<"assigning nullptr "<<j<<i<<endl;
		return nullptr;
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
	markerBoardPositions[std::make_tuple(0,0)] = 244;
	markerBoardPositions[std::make_tuple(1,0)] = 206;
	markerBoardPositions[std::make_tuple(2,0)] = 144;
	markerBoardPositions[std::make_tuple(3,0)] = 69;
	markerBoardPositions[std::make_tuple(0,1)] = 192;
	markerBoardPositions[std::make_tuple(1,1)] = 113;
	markerBoardPositions[std::make_tuple(2,1)] = 30;
	markerBoardPositions[std::make_tuple(3,1)] = 163;
	markerBoardPositions[std::make_tuple(0,2)] = 63;
	markerBoardPositions[std::make_tuple(1,2)] = 152;
	markerBoardPositions[std::make_tuple(2,2)] = 73;
	markerBoardPositions[std::make_tuple(3,2)] = 213;
	markerBoardPositions[std::make_tuple(0,3)] = 81;
	markerBoardPositions[std::make_tuple(1,3)] = 58;
	markerBoardPositions[std::make_tuple(2,3)] = 79;
	markerBoardPositions[std::make_tuple(3,3)] = 86;
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
			//aruco::Marker* m = getMarkerById(square->markerId);
			if (square->isOccupied) {
				square->drawPiece();
			}
		}
	}
}

// void Board::update(std::vector<aruco::Marker> visibleMarkers) {
// 	markers = visibleMarkers;
// 	GLfloat black[] = {0.0,0.0,0.0,1.0};
//     GLfloat red[] = {1.0,0.0,0.0,1.0};
//     GLfloat green[] = {0.0,1.0,0.0,1.0};
//     GLfloat blue[] = {0.0,0.0,1.0,1.0};
//     GLfloat white[] = {1.0,1.0,1.0,1.0};
//     GLfloat grey[] = {0.5,0.5,0.5,1.0};
//     GLfloat lowAmbient[] = {0.2,0.2,0.2,1.0};


// 	glEnable(GL_DEPTH_TEST);
// 	glEnable(GL_NORMALIZE);
//     glShadeModel(GL_SMOOTH);

//     glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,lowAmbient);
//     glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,grey);
//     glMaterialfv(GL_FRONT,GL_SPECULAR,white);
//     glMaterialf(GL_FRONT,GL_SHININESS,128.0);
//     glLightfv(GL_LIGHT0,GL_AMBIENT,lowAmbient);
//     //now, for each marker,
//     double modelview_matrix[16];


//     //markers[m].glGetModelViewMatrix(modelview_matrix);
//     glMatrixMode(GL_MODELVIEW);
//     glLoadIdentity();
//     glLoadMatrixd(modelview_matrix);

//     //glColor3f(0.4,0.4,0.4);

//     glTranslatef(markerSize/3.0, markerSize/3.0, markerSize/4.0);
//     glRotatef(90.f,1.f,0.f,0.f);
//     glPushMatrix();
//     //glutWireCube( markerSize );
//     //glutSolidTeapot(markerSize/2 );
//     drawOBJ();
//     glPopMatrix();
        
    
// }

// void Board::drawOBJ() {    

//     //glColor3f(0,1,0);
//     glBegin(GL_TRIANGLES);
//     for (int i = 0; i < faces.size(); ++i) {

//         glNormal3f(markerSize/4.0*norms[(faces[i].vn1-1)].x, 
//         			markerSize/4.0*norms[(faces[i].vn1-1)].y, 
//         			markerSize/4.0*norms[(faces[i].vn1-1)].z);
//         glVertex3f(markerSize/4.0*verts[(faces[i].v1-1)].x, 
//         			markerSize/4.0*verts[(faces[i].v1-1)].y, 
//         			markerSize/4.0*verts[(faces[i].v1-1)].z);

//         //cout<<verts[(faces[i].v1-1)].x<<","<<verts[(faces[i].v1-1)].y<<","<< verts[(faces[i].v1-1)].z<<endl;

//         glNormal3f(markerSize/4.0*norms[(faces[i].vn2-1)].x, 
//         			markerSize/4.0*norms[(faces[i].vn2-1)].y, 
//         			markerSize/4.0*norms[(faces[i].vn2-1)].z);
//         glVertex3f(markerSize/4.0*verts[(faces[i].v2-1)].x, 
//         			markerSize/4.0*verts[(faces[i].v2-1)].y, 
//         			markerSize/4.0*verts[(faces[i].v2-1)].z);

//         glNormal3f(markerSize/4.0*norms[(faces[i].vn3-1)].x, 
//         			markerSize/4.0*norms[(faces[i].vn3-1)].y, 
//         			markerSize/4.0*norms[(faces[i].vn3-1)].z);
//         glVertex3f(markerSize/4.0*verts[(faces[i].v3-1)].x, 
//         			markerSize/4.0*verts[(faces[i].v3-1)].y, 
//         			markerSize/4.0*verts[(faces[i].v3-1)].z);
//     }
//     glEnd();


// }