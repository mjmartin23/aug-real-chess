/*
Matt Martin
Jan 5, 2017
Video class
*/

#include "Video.h"

Video::Video(bool initialize, bool showWindow) {
	show = showWindow;
	if (initialize) {
		on = (initializeCapture() == 0);
		if (show && on) cv::namedWindow("Video", 1);
	} else {
		on = false;
	}
}

int Video::initializeCapture() {
	capdev = new cv::VideoCapture(0); 
	if( !capdev->isOpened() ) {
		printf("Unable to open video device\n");
		return(-1);
	}
	return 0;
}

void Video::getFrame() {
	cv::Mat frame;
	*capdev >> frame;
	curFrame = frame; // get a new frame from the camera, treat as a stream	
}

void Video::updateFrame() {
	if (show) cv::imshow("Video", curFrame);
}