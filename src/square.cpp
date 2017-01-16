/* Matt Martin
Jan 9, 2017
Square class implementation
*/

#include "square.h"

#define max(a,b) a<b?b:a

Square::Square(int coln, int rown, int marker,float markerSizeParam,Piece *myPiece) {
	col = coln; 
	row = rown;
	markerId = marker;
	markerSize = markerSizeParam;
	piece = myPiece;
	isOccupied = (piece == nullptr) ? false : true;
	seen = false;
	lastSeen = 0;
	setCorners();
	setColor();
}

void Square::setColor() {
	if (( row + col ) % 2 == 0) {
		// draw white square
		color[0] = 0.98; color[1] = 0.98; color[2] = 0.98; color[3] = 1.0;
	} else {
		// draw black square
		color[0] = 0.01; color[1] = 0.01; color[2] = 0.01; color[3] = 1.0;
	}
	grey[0] = 0.5; grey[1] = 0.5; grey[2] = 0.5; grey[3] = 1.0;
    lowAmbient[0] = 0.2; lowAmbient[1] = 0.2; lowAmbient[2] = 0.2; lowAmbient[3] = 1.0;
    white[0] = 1.0; white[1] = 1.0; white[2] = 1.0; white[3] = 1,0;
}

void Square::setCorners() {
	float minx,maxx,miny,maxy;
	switch (col % 2) {
		case 0:
			switch (row % 2) {
				case 0:
					minx = -2.0;
					maxx = 0.;
					miny = -2.0;
					maxy = 0.;
					break;
				case 1:
					minx = 0.;
					maxx = 2.;
					miny = -2.0;
					maxy = 0.;
					break;
			}
			break;
		case 1:
			switch (row % 2) {
				case 0:
					minx = -2.0;
					maxx = 0.;
					miny = 0.;
					maxy = 2.0;
					break;
				case 1:
					minx = 0.;
					maxx = 2.;
					miny = 0.;
					maxy = 2.0;
					break;
			}
			break;
	}
	corners.push_back(cv::Point3f(minx*HALF_SQUARE,miny*HALF_SQUARE,0.f));
	corners.push_back(cv::Point3f(maxx*HALF_SQUARE,miny*HALF_SQUARE,0.f));
	corners.push_back(cv::Point3f(maxx*HALF_SQUARE,maxy*HALF_SQUARE,0.f));
	corners.push_back(cv::Point3f(minx*HALF_SQUARE,maxy*HALF_SQUARE,0.f));
}


void Square::draw(aruco::Marker* marker) {
	if (marker == nullptr) return;

    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,color);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,grey);
    glMaterialfv(GL_FRONT,GL_SPECULAR,white);
    glMaterialf(GL_FRONT,GL_SHININESS,128.0);
    glLightfv(GL_LIGHT0,GL_AMBIENT,lowAmbient);

	double modelview_matrix[16];

    marker->glGetModelViewMatrix(modelview_matrix);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLoadMatrixd(modelview_matrix);


    glTranslatef(0,0,0);
    glPushMatrix();
    // draw square
    
    glBegin(GL_POLYGON);
    glVertex3f(markerSize/4.15*corners[0].x,markerSize/4.15*corners[0].y,markerSize/4.15*corners[0].z);
    glVertex3f(markerSize/4.15*corners[1].x,markerSize/4.15*corners[1].y,markerSize/4.15*corners[1].z);
    glVertex3f(markerSize/4.15*corners[2].x,markerSize/4.15*corners[2].y,markerSize/4.15*corners[2].z);
    glVertex3f(markerSize/4.15*corners[3].x,markerSize/4.15*corners[3].y,markerSize/4.15*corners[3].z);
    glEnd();
    glPopMatrix();
}

void Square::drawPiece(aruco::Marker* marker) {
	this->piece->draw(marker,corners);
}