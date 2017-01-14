/* Matt Martin
Jan 9, 2017
Square class implementation
*/

#include "square.h"

#define max(a,b) a<b?b:a

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

		// temporaroly drawing pieces here

		// cv::Point2f center = cv::Point2f(( projPoints[0].x+projPoints[2].x )/ 2.0, (projPoints[0].y+projPoints[2].y )/ 2.0);
		// float width = max((abs(projPoints[0].x-projPoints[1].x)+
		// 				   abs(projPoints[3].x-projPoints[2].x) ) / 2.0,
		// 				  (abs(projPoints[1].x-projPoints[2].x)+
		// 				   abs(projPoints[0].x-projPoints[3].x) ) / 2.0);
		// float height = max((abs(projPoints[0].y-projPoints[3].y)+
		// 					abs(projPoints[1].y-projPoints[2].y) ) / 2.0,
		// 				   (abs(projPoints[0].y-projPoints[1].y)+
		// 					abs(projPoints[3].y-projPoints[2].y) ) / 2.0);
		// cv::Size2f size = cv::Size2f( width, height );
		// float angle = atan2(projPoints[0].y - projPoints[1].y,
		// 					projPoints[0].x - projPoints[1].x);
		// const cv::RotatedRect rect = cv::RotatedRect(center,size,angle);
		// cv::ellipse(*frame,rect,cv::Scalar(255,0,0),-1);
	}
}

void Square::draw(aruco::Marker marker) {
	double modelview_matrix[16];
}