// Board.h
#include <iostream>
#include "opencv2/opencv.hpp"
#include "Video.h"

class Board
{
public:
	Board();
	void findCorners();
	void update();
	Video *vid;
private:
	std::vector<cv::Point2f> corners;
	cv::Size size;
};