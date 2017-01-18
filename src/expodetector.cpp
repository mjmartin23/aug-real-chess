/* Matt Martin
* Jan 18, 2017
* Expo marker detector
*/

#include "expodetector.h"

ExpoDetector::ExpoDetector(int* lh,int*hh,int*ls,int*hs,int*lv,int*hv) {
	lowH = lh; highH = hh;
	lowS = ls; highS = hs;
	lowV = lv; highV = hv;
}

void ExpoDetector::detect(cv::Mat *frame,cv::Mat *thresh) {
	cv::Mat hsv;
	cv::cvtColor(*frame,hsv,cv::COLOR_BGR2HSV);
	cv::inRange(hsv, cv::Scalar(*lowH,*lowS,*lowV),cv::Scalar(*highH,*highS,*highV),*thresh);

	// cv::erode(*thresh,*thresh,cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(5,5)));
	// cv::dilate(*thresh,*thresh,cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(5,5)));

	// cv::dilate(*thresh,*thresh,cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(5,5)));
	// cv::erode(*thresh,*thresh,cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(5,5)));

}


int main(int argc, char const *argv[])
{
	cv::VideoCapture cap(0);
	if (!cap.isOpened()) {
		cout<<"vid not open"<<endl;
		return -1;
	}
	cv::namedWindow("Control",CV_WINDOW_AUTOSIZE);
	int iLowH = 49;
	int iHighH = 91;

	int iLowS = 74; 
	int iHighS = 215;

	int iLowV = 75;
	int iHighV = 182;

	//Create trackbars in "Control" window
	cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
	cvCreateTrackbar("HighH", "Control", &iHighH, 179);

	cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
	cvCreateTrackbar("HighS", "Control", &iHighS, 255);

	cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
	cvCreateTrackbar("HighV", "Control", &iHighV, 255);

	ExpoDetector ed = ExpoDetector(&iLowH,&iHighH,&iLowS,&iHighS,&iLowV,&iHighV);

	while (1) {
		cv::Mat origImg,out;
		cap.read(origImg);
		ed.detect(&origImg,&out);
		cv::imshow("thresh",out);
		cv::imshow("orig",origImg);
		if (cv::waitKey(10) == 27) break;
	}

	return 0;
}