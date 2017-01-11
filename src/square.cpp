/* Matt Martin
Jan 9, 2017
Square class implementation
*/

#include "square.h"

Square::Square(int coln, int rown, int marker) {
	col = coln; 
	row = rown;
	markerId = marker;
	
	seen = false;
	lastSeen = 0;
	setCorners();
	setColor();
}

void Square::setColor() {
	if (( row + col ) % 2 == 0) {
		// draw white square
		color = cv::Scalar(200,200,200);
	} else {
		// draw black square
		color = cv::Scalar(20,20,20);
	}
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

void Square::draw(cv::Mat *frame, aruco::Marker* marker, cv::Mat cameraMatrix, cv::Mat cameraDistortion) {
	if (marker != nullptr) {
		seen = true;
		lastSeen = 0;
		projPoints.clear();
		
		cv::projectPoints(corners,marker->Rvec,marker->Tvec,cameraMatrix,cameraDistortion,projPoints);

	    for (int c = 0; c < projPoints.size(); ++c) {
	    	projPointsInt[0][c] = (cv::Point2i) projPoints[c] ;
		}
	} else {
		lastSeen++;
	}

	if (seen && lastSeen < 3) {
		int nCorners[] = {4};
		const cv::Point2i* ppts[1] = { projPointsInt[0] };
		cv::fillPoly(*frame,ppts,nCorners,1,color);
	}
}