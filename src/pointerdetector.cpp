/* Matt Martin
* Jan 18, 2017
* Expo marker detector
*/

#include "pointerdetector.h"

PointerDetector::PointerDetector() {}

PointerDetector::PointerDetector(int* lh,int*hh,int*ls,int*hs,int*lv,int*hv) {
	lowH = lh; highH = hh;
	lowS = ls; highS = hs;
	lowV = lv; highV = hv;
	x=0;y=0;
	this->focalLength = 19200.0;
	this->size = 0.025;
}

PointerDetector::PointerDetector(int lh,int hh,int ls,int hs,int lv,int hv) {
	lowH = &lh; highH = &hh;
	lowS = &ls; highS = &hs;
	lowV = &lv; highV = &hv;
	x=0;y=0;
	this->focalLength = 19200.0;
	this->size = 0.025;
}

void PointerDetector::detect(cv::Mat frame,int show) {
	if (show) cv::imshow("orig",frame);
	cv::Mat hsv;
	// cv::cvtColor(frame,hsv,cv::COLOR_RGB2BGR);
	// if (show) cv::imshow("back to bgr",frame);
	cv::cvtColor(frame,hsv,cv::COLOR_BGR2HSV);
	//if (show) cv::imshow("hsv",frame);
	// for other than red
	cv::inRange(hsv, cv::Scalar(*lowH,*lowS,*lowV),cv::Scalar(*highH,*highS,*highV),thresh);
	// for red
	// cv::Mat lred, hred;
	// cv::inRange(hsv, cv::Scalar(0,*lowS,*lowV),cv::Scalar(10,*highS,*highV),lred);
	// cv::inRange(hsv, cv::Scalar(150,*lowS,*lowV),cv::Scalar(179,*highS,*highV),hred);
	// cv::addWeighted(lred, 1.0, hred, 1.0, 0.0, thresh);




	cv::erode(thresh,thresh,cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(8,8)));
	if (show) cv::imshow("eroded",thresh);
	cv::dilate(thresh,thresh,cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(8,8)));
	if (show) cv::imshow("dilate",thresh);
	if (show) if (cv::waitKey(10) == 27) return;

	// cv::dilate(thresh,thresh,cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(5,5)));
	// cv::erode(thresh,thresh,cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(5,5)));
	//cout<<thresh<<endl;

}

void PointerDetector::calcCenter() {	
	cv::Moments moms = cv::moments(thresh);
	double m01 = moms.m01;
	double m10 = moms.m10;
	double area = moms.m00;
	if (area > 10) {
		this->x = m10/area;
		this->y = m01/area;
	}
}

void PointerDetector::calcDistance() {
	cv::Mat nonZero;
	cv::findNonZero(thresh,nonZero);
	int count = nonZero.total();
	if (count > 50) this->distance = this->size * this->focalLength / count;
}

void PointerDetector::configFocalLength() {
	cv::Mat nonZero;
	cv::findNonZero(thresh,nonZero);
	int count = nonZero.total();
	float fl = count * 0.2794 / this->size;
	cout<<fl<<endl;
}

void PointerDetector::update(cv::Mat frame, int show) {
	this->detect(frame,show);
	this->calcDistance();
	this->calcCenter();
}

void PointerDetector::histogram(cv::Mat *frame, cv::Mat *out) {
	// cv::Mat hsv,thresh;
	// cv::cvtColor(*frame,hsv,cv::COLOR_BGR2HSV);
	// cv::inRange(hsv, cv::Scalar(*lowH,*lowS,*lowV),cv::Scalar(*highH,*highS,*highV),thresh);

	// for (int i = 0; i < frame->rows; ++i) {
	// 	for (int j = 0; j < frame->cols; ++j) {
	// 		cv::Vec3f pixel = frame->at<cv::Vec3b>(i,j);
	// 		float blue = pixel.val[0];
	// 		float green = pixel.val[1];
	// 		float red = pixel.val[2];

	// 		double sum = red+green+blue;
	// 		double b = blue/sum;
	// 		double g = green/sum;
	// 		double r = red/sum;

	// 		out->data[out->step[0]*i + out->step[1]* j + 0] = (b*255);
 //           	out->data[out->step[0]*i + out->step[1]* j + 1] = (g*255);
 //           	out->data[out->step[0]*i + out->step[1]* j + 2] = (r*255); 
	// 	}
	// }

	int histSize = 256;
	float range[] = {0,256};
	const float * histRange = { range };
	bool uniform = true; bool accumulate = true;

	cv::Mat gHist;
	std::vector<cv::Mat> bgrPlanes;
	cv::split(*frame,bgrPlanes);

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
// 	int iLowH = 90;
// 	int iHighH = 120;

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

// 	PointerDetector ed = PointerDetector(&iLowH,&iHighH,&iLowS,&iHighS,&iLowV,&iHighV);
// 	cv::Mat origImg;
// 	while (1) {
// 		cap.read(origImg);
// 		ed.update(origImg,1);
// 		cout<<ed.x<<","<<ed.y<<": "<<ed.distance<<endl;
// 		//ed.configFocalLength();
// 		//ed.histogram(&origImg,&out);
// 		//cv::imshow("out",out);
// 		cv::imshow("orig",origImg);
// 		if (cv::waitKey(10) == 27) break;
// 	}

// 	return 0;
// }