/* Matt Martin
* Jan 18, 2017
* Expo marker detector
*/

#include "expodetector.h"

ExpoDetector::ExpoDetector() {}

ExpoDetector::ExpoDetector(int* lh,int*hh,int*ls,int*hs,int*lv,int*hv) {
	lowH = lh; highH = hh;
	lowS = ls; highS = hs;
	lowV = lv; highV = hv;
	x=0;y=0;
}

ExpoDetector::ExpoDetector(int lh,int hh,int ls,int hs,int lv,int hv) {
	lowH = &lh; highH = &hh;
	lowS = &ls; highS = &hs;
	lowV = &lv; highV = &hv;
	x=0;y=0;

}

void ExpoDetector::detect(cv::Mat *frame,cv::Mat *thresh) {
	cv::Mat hsv;
	cv::cvtColor(*frame,hsv,cv::COLOR_BGR2HSV);
	cv::inRange(hsv, cv::Scalar(*lowH,*lowS,*lowV),cv::Scalar(*highH,*highS,*highV),*thresh);

	cv::imshow("simple thresh",*thresh);

	cv::erode(*thresh,*thresh,cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(4,4)));
	cv::imshow("eroded",*thresh);
	cv::dilate(*thresh,*thresh,cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(8,8)));
	cv::imshow("dilsate",*thresh);

	// cv::dilate(*thresh,*thresh,cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(5,5)));
	// cv::erode(*thresh,*thresh,cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(5,5)));
	cv::Mat nonZero;
	cv::findNonZero(*thresh,nonZero);
	int sumx = 0; int sumy = 0;
	for (int i = 0; i < nonZero.total(); i++) {
		sumx += nonZero.at<cv::Point>(i).x;
		sumy += nonZero.at<cv::Point>(i).y;
	}
	x = sumx/nonZero.total();
	y = sumy/nonZero.total();
}

void ExpoDetector::histogram(cv::Mat *frame, cv::Mat *out) {
	// cv::Mat hsv,thresh;
	// cv::cvtColor(*frame,hsv,cv::COLOR_BGR2HSV);
	// cv::inRange(hsv, cv::Scalar(*lowH,*lowS,*lowV),cv::Scalar(*highH,*highS,*highV),thresh);

	for (int i = 0; i < frame->rows; ++i) {
		for (int j = 0; j < frame->cols; ++j) {
			cv::Vec3f pixel = frame->at<cv::Vec3b>(i,j);
			float blue = pixel.val[0];
			float green = pixel.val[1];
			float red = pixel.val[2];

			double sum = red+green+blue;
			double b = blue/sum;
			double g = green/sum;
			double r = red/sum;

			out->data[out->step[0]*i + out->step[1]* j + 0] = (b*255);
           	out->data[out->step[0]*i + out->step[1]* j + 1] = (g*255);
           	out->data[out->step[0]*i + out->step[1]* j + 2] = (r*255); 
		}
	}

	int histSize = 256;
	float range[] = {0,256};
	const float * histRange = { range };
	bool uniform = true; bool accumulate = true;

	cv::Mat gHist;
	std::vector<cv::Mat> bgrPlanes;
	cv::split(*out,bgrPlanes);

	cv::calcHist(&bgrPlanes[1],1,0,cv::Mat(),gHist,1,&histSize,&histRange,uniform,accumulate);

	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound( (double) hist_w/histSize );

	cv::Mat histImage( hist_h, hist_w, CV_8UC3, cv::Scalar( 0,0,0) );

	cv::normalize(gHist,gHist,0,histImage.rows,cv::NORM_MINMAX,-1,cv::Mat());

	for ( int i = 1; i < histSize; i++ ) {
		cv::line(histImage,cv::Point(bin_w*(i-1), hist_h - cvRound(gHist.at<float>(i-1)) ),
							cv::Point( bin_w*(i), hist_h - cvRound(gHist.at<float>(i)) ),
                     		cv::Scalar( 0, 255, 0), 2, 8, 0  );
	}
	cv::imshow("hist1",histImage);

}


// int main(int argc, char const *argv[])
// {
// 	cv::VideoCapture cap(0);
// 	if (!cap.isOpened()) {
// 		cout<<"vid not open"<<endl;
// 		return -1;
// 	}
// 	cv::namedWindow("Control",CV_WINDOW_AUTOSIZE);
// 	int iLowH = 44;
// 	int iHighH = 100;

// 	int iLowS = 74; 
// 	int iHighS = 255;

// 	int iLowV = 75;
// 	int iHighV = 255;

// 	//Create trackbars in "Control" window
// 	cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
// 	cvCreateTrackbar("HighH", "Control", &iHighH, 179);

// 	cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
// 	cvCreateTrackbar("HighS", "Control", &iHighS, 255);

// 	cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
// 	cvCreateTrackbar("HighV", "Control", &iHighV, 255);

// 	ExpoDetector ed = ExpoDetector(&iLowH,&iHighH,&iLowS,&iHighS,&iLowV,&iHighV);
// 	cv::Mat origImg,out;
// 	while (1) {
// 		cap.read(origImg);
// 		//out = cv::Mat(origImg.size(),origImg.type());
// 		ed.detect(&origImg,&out);
// 		//cv::imshow("out",out);
// 		cv::imshow("orig",origImg);
// 		if (cv::waitKey(10) == 27) break;
// 	}

// 	return 0;
// }