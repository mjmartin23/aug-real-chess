/*
Matt Martin
Jan 5, 2017
Board class
*/

#include "board.h"
#include "GL/gl.h"
#include "GL/glut.h"

Board::Board() {

}

Board::Board(aruco::CameraParameters camParams) {
	cameraMatrix = camParams.CameraMatrix;
	cameraDistortion = camParams.Distorsion;
	size = std::make_tuple(8,8);

	pairMarkersWithBoardPositions(); 
	generateSquares();
}

Board::Board(aruco::CameraParameters camParams, float markerSize) {
	cameraMatrix = camParams.CameraMatrix;
	cameraDistortion = camParams.Distorsion;
	markerSize = markerSize;
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


void Board::update(cv::Mat *frame,std::vector<aruco::Marker> visibleMarkers) {
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
}

void Board::update(std::vector<aruco::Marker> visibleMarkers) {
	GLfloat black[] = {0.0,0.0,0.0,1.0};
    GLfloat red[] = {1.0,0.0,0.0,1.0};
    GLfloat green[] = {0.0,1.0,0.0,1.0};
    GLfloat blue[] = {0.0,0.0,1.0,1.0};
    GLfloat white[] = {1.0,1.0,1.0,1.0};
    GLfloat grey[] = {0.5,0.5,0.5,1.0};
    GLfloat lowAmbient[] = {0.2,0.2,0.2,1.0};


	
    //now, for each marker,
    double modelview_matrix[16];
    for (unsigned int m=0;m<markers.size();m++)
    {
        if (m == 0) {
            glMaterialfv(GL_FRONT,GL_AMBIENT,black);
            glMaterialfv(GL_FRONT,GL_DIFFUSE,grey);
            glMaterialfv(GL_FRONT,GL_SPECULAR,white);
            glMaterialf(GL_FRONT,GL_SHININESS,128.0);
            glLightfv(GL_LIGHT0, GL_AMBIENT, lowAmbient);


            markers[m].glGetModelViewMatrix(modelview_matrix);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glLoadMatrixd(modelview_matrix);

            //glColor3f(0.4,0.4,0.4);
            glTranslatef(0, 0, markerSize/2);
            glRotatef(90.f,1.f,0.f,0.f);
            glPushMatrix();
            //glutWireCube( markerSize );
            glutSolidTeapot(markerSize/2 );  
            glPopMatrix();
        }
        float size = markerSize/2;
        glColor3f (1,0,0 );
	    glBegin(GL_LINES);
	    glVertex3f(0.0f, 0.0f, 0.0f); // origin of the line
	    glVertex3f(size,0.0f, 0.0f); // ending point of the line
	    glEnd( );

	    glColor3f ( 0,1,0 );
	    glBegin(GL_LINES);
	    glVertex3f(0.0f, 0.0f, 0.0f); // origin of the line
	    glVertex3f( 0.0f,size, 0.0f); // ending point of the line
	    glEnd( );


	    glColor3f (0,0,1 );
	    glBegin(GL_LINES);
	    glVertex3f(0.0f, 0.0f, 0.0f); // origin of the line
	    glVertex3f(0.0f, 0.0f, size); // ending point of the line
	    glEnd( );
    }
}
