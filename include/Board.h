// Board.h
#include <iostream>
#include "opencv2/opencv.hpp"
#include "Video.h"

class Board
{
public:
	Board();
	void update(bool);
	void calibrate();

	Video *vid;
	bool found;

private:
	void findCorners();
	void collectCalibrationImages();

	std::vector<cv::Point2f> corners;
	std::vector<cv::Point3f> points;
	std::vector< std::vector<cv::Point2f> > corner_set;
	std::vector< std::vector<cv::Point3f> > point_set;
	cv::Size size;
	cv::Mat frame;
};