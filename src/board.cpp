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
	generateSquares();
}

void Board::generateSquares() {
	for (int i = 0; i < std::get<0>(size); ++i) {
		for (int j = 0; j < std::get<1>(size); ++j) {
			std::tuple<int,int> pos = std::make_tuple(j,i);
			Piece* p = new Piece(1,markerSize,"../data/models/bishop.obj");
			Square* sq = new Square(j,i,
									markerBoardPositions[std::make_tuple(j/2,i/2)],markerSize,
									p);
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


void Board::update(std::vector<aruco::Marker> visibleMarkers) {
	//cout << "Updating board" << endl;
	markers = visibleMarkers;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
	// draw squares
	for (int i = 0; i < std::get<0>(size); ++i) {
		for (int j = 0; j < std::get<1>(size); ++j) {
			std::tuple<int,int> pos = std::make_tuple(j,i);
			Square *square = squares[pos];
			aruco::Marker* m = getMarkerById(square->markerId);
			square->draw(m);
		}
	}
	//draw pieces
	for (int i = 0; i < std::get<0>(size); ++i) {
		for (int j = 0; j < std::get<1>(size); ++j) {
			std::tuple<int,int> pos = std::make_tuple(j,i);
			Square *square = squares[pos];
			aruco::Marker* m = getMarkerById(square->markerId);
			if (square->isOccupied) {
				square->drawPiece(m);
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