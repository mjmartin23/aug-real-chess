// Video.h

#include "opencv2/opencv.hpp"

class Video {
public:
	Video(bool,bool);
	int initializeCapture();
	void getFrame();
	void updateFrame();

	cv::VideoCapture *capdev;
	cv::Mat curFrame;

private:
	bool on;
	bool show;
};