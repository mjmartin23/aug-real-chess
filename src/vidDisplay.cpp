/*
	Bruce A. Maxwell
	J16 
	Simple example of video capture and manipulation
	Based on OpenCV tutorials

	Compile command

	clang++ -o vid -I /opt/local/include vidDisplay.cpp -L /opt/local/lib -lopencv_core -lopencv_highgui -lopencv_video -lopencv_videoio

*/
#include <cstdio>
#include "opencv2/opencv.hpp"

int main(int argc, char *argv[]) {
	cv::VideoCapture *capdev;

	// open the video device
	capdev = new cv::VideoCapture(0);
	if( !capdev->isOpened() ) {
		printf("Unable to open video device\n");
		return(-1);
	}

	cv::namedWindow("Video", 1); // identifies a window?

	for(;;) {
		cv::Mat frame;

		*capdev >> frame; // get a new frame from the camera, treat as a stream

		cv::imshow("Video", frame);

		if(cv::waitKey(10) >= 0)
			break;

	}

	// terminate the video capture
	printf("Terminating\n");
	delete capdev;

	return(0);
}
