/* Matt Martin
Jan 9, 2017
Square class implementation
*/

#include "square.h"

#define max(a,b) a<b?b:a

Square::Square(int coln, int rown,float markerSizeParam, Piece *myPiece) {
	col = coln; 
	row = rown;
	squareSize = markerSizeParam/2.0;
	piece = myPiece;
	isOccupied = (piece == nullptr) ? false : true;
	this->selected = false;
	setCorners();
	setColor();
}

void Square::setColor() {
	if (( row + col ) % 2 == 1) {
		// draw white square
		color[0] = 0.98; color[1] = 0.98; color[2] = 0.98; color[3] = 1.0;
	} else {
		// draw black square
		color[0] = 0.01; color[1] = 0.01; color[2] = 0.01; color[3] = 1.0;
	}
	highlight[0] = 0.2; highlight[1] = 0.8; highlight[3] = 0.2; highlight[3] = 1.0;
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

void Square::receivePiece(Piece* p) {
	piece = p;
	piece->moves++;
	isOccupied = (piece == nullptr) ? false : true;
}

void Square::removePiece() {
	piece = nullptr;
	isOccupied = false;
}

void Square::contains(cv::Point3f pt) {
	cv::Mat ab,am,bc,bm;
	float v0[2],v1[2],v2[2],v3[2],d0,d1,d2,d3;

	v0[0] = corners[1].x-corners[0].x; v0[1] = corners[1].y - corners[0].y;
	ab = cv::Mat(2,1,CV_32F,v0);

	v1[0] = pt.x - corners[0].x; v1[1] = pt.y - corners[0].y;
	am = cv::Mat(2,1,CV_32F,v1);

	v2[0] = corners[2].x - corners[1].x; v2[1] = corners[2].y - corners[1].y;
	bc = cv::Mat(2,1,CV_32F,v2);

	v3[0] = pt.x - corners[1].x; v3[1] = pt.y - corners[1].y;
	bm = cv::Mat(2,1,CV_32F,v3);

	d0 = ab.dot(am);
	d1 = ab.dot(ab);
	d2 = bc.dot(bm);
	d3 = bc.dot(bc);

	if (0 <= d0 && d0 <= d1 && 0 <= d2 && d2 <= d3 ) {
		this->selected = true;
	} else {
		this->selected = false;
	}
}

void Square::draw() {
	//cout<<this->selected<<endl;

	//set color
	if (!this->selected) {
	    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,color);
	    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,color);
	} else {
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,highlight);
	    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,highlight);
	}

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