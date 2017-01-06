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

void modifyColor(cv::VideoCapture *cap) {
	for(;;) {
		cv::Mat frame;

		*cap >> frame; // get a new frame from the camera, treat as a stream
		
		int channels = frame.channels();
		int rows = frame.rows;
		int cols = frame.cols;
		printf("%d,%d,%d\n", rows,cols,channels);

		uchar* p;
		for (int i = 0; i < rows; ++i)
		{
			p = frame.ptr<uchar>(i);
			for (int j = 0; j < cols*channels; j+=channels)
			{
				int pixel[3];
				for (int k = 0; k < channels; ++k) {
					pixel[k] = p[j+k];
				}
				p[j+2] = 255;	
				//printf("(%d,%d,%d),", pixel[0],pixel[1],pixel[2]);
			}
			//printf("\n");
		}
		cv::imshow("Video", frame);
		
		if(cv::waitKey(30) % 255 > 0)
			break;
			

	}
}

int main(int argc, char *argv[]) {
	cv::VideoCapture *capdev;

	// open the video device
	capdev = new cv::VideoCapture(0);
	if( !capdev->isOpened() ) {
		printf("Unable to open video device\n");
		return(-1);
	}

	cv::namedWindow("Video", 1); // identifies a window?

	modifyColor(capdev);

	// terminate the video capture
	printf("Terminating\n");
	delete capdev;

	return(0);
}
