/* Matt Martin
Jan 9,2017
Square class for chessboard
*/

#include "aruco/aruco.h"
#include "opencv2/opencv.hpp"
#include <iostream>

#define HALF_SQUARE 1.275f

class Square {
public:
	Square(int,int,int);
	void draw(cv::Mat *, aruco::Marker *, cv::Mat, cv::Mat);

	int markerId;

private:
	void setCorners();
	void setColor();

	bool seen;
	int row,col;
	//int nCorners;
	
	cv::Scalar color;
	std::vector<cv::Point3f> corners;
	std::vector<cv::Point2f> projPoints;
	cv::Point2i projPointsInt[1][4];
	//std::vector<cv::Point2i> lastProjPointsInt;
	//cv::Mat rvec,tvec;
};