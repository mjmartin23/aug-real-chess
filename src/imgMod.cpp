/*
	Bruce A. Maxwell
	J16 
	Simple example of reading, manipulating, displaying, and writing an image

	Compile command

	clang++ -o imod -I /opt/local/include imgMod.cpp -L /opt/local/lib -lopencv_core -lopencv_highgui 

*/
#include <cstdio>
#include <cstring>
#include "opencv2/opencv.hpp"

int main(int argc, char *argv[]) {
	cv::Mat src;
	char filename[256];

	// usage
	if(argc < 2) {
		printf("Usage %s <image filename>\n", argv[0]);
		exit(-1);
	}
	strcpy(filename, argv[1]);

	// read the image
	src = cv::imread(filename);

	// test if the read was successful
	if(src.data == NULL) {
		printf("Unable to read image %s\n", filename);
		exit(-1);
	}

	// print out information about the image
	printf("filename:         %s\n", filename);
	printf("Image size:       %d rows x %d columns\n", (int)src.size().height, (int)src.size().width);
	printf("Image dimensions: %d\n", (int)src.channels());
	printf("Image depth:      %d bytes/channel\n", (int)src.elemSize()/src.channels());

	// create a window
	cv::namedWindow(filename, 1);

	// edit the source image here

	// show the image in a window
	cv::imshow(filename, src);

	// wait for a key press (indefinitely)
	cv::waitKey(0);

	// get rid of the window
	cv::destroyWindow(filename);

	// terminate the program
	printf("Terminating\n");

	return(0);
}
