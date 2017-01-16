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
	squareSize = markerSizeParam/2.0;
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
	corners.push_back(cv::Point3f((col-4)*squareSize,(row-4)*squareSize,0.f));
	corners.push_back(cv::Point3f((col-3)*squareSize,(row-4)*squareSize,0.f));
	corners.push_back(cv::Point3f((col-3)*squareSize,(row-3)*squareSize,0.f));
	corners.push_back(cv::Point3f((col-4)*squareSize,(row-3)*squareSize,0.f));
}


void Square::draw() {

	//set color
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,color);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,grey);
    glMaterialfv(GL_FRONT,GL_SPECULAR,white);
    glMaterialf(GL_FRONT,GL_SHININESS,128.0);
    glLightfv(GL_LIGHT0,GL_AMBIENT,lowAmbient);

    // draw square
    
    glBegin(GL_POLYGON);
    glVertex3f(corners[0].x,corners[0].y,corners[0].z);
    glVertex3f(corners[1].x,corners[1].y,corners[1].z);
    glVertex3f(corners[2].x,corners[2].y,corners[2].z);
    glVertex3f(corners[3].x,corners[3].y,corners[3].z);
    glEnd();
}

void Square::drawPiece() {
	this->piece->draw(corners);
}