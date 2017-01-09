/*
Matt Martin
Jan 5, 2017
Board class
*/

#include "Board.h"

Board::Board() {
	vid = new Video(true,true); //initialize capture and show the video output
	size = cv::Size(7,7); // inner corners of 8x8 chessboard
	found = false;
}

void Board::findCorners() {
	frame = vid->curFrame;
	found = cv::findChessboardCorners(frame,size,corners,
		CV_CALIB_CB_ADAPTIVE_THRESH);
	//printf("%d\n", found);
	// if (found) {cv::cornerSubPix(frame,corners,cv::Size(8,8),cv::Size(-1,-1),
	// 	cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
	// }
	cv::drawChessboardCorners(frame,size,corners,found);
	vid->curFrame = frame;
	if (found) {
		// printf("%f,%f\n",corners[0].x,corners[0].y);
		// printf("%f,%f\n\n",corners[1].x,corners[1].y);
		// for (std::vector<cv::Point2f>::iterator i = corners.begin(); i != corners.end(); ++i) {
		// 	printf("%f,%f\n", i->x,i->y);
		// }
	}
	points.clear();
	for (float i = 0.f; i < size.height; ++i) {
		for (float j = 0.f; j < size.width; ++j) {
			cv::Point3f point(-j,i,0.f);
			points.push_back(point);
		}
	}
}

void Board::calibrate() {
	printf("calibrating\n");
	float camera_mat_data[9] = {1.,0.,frame.cols/2.,0.,1.,frame.rows/2.,0.,0.,1.};
	cv::Mat camera_matrix = cv::Mat(3,3,CV_64FC1, camera_mat_data);
	std::cout << "camera_matrix = "<< std::endl << " "  << camera_matrix << std::endl << std::endl;
	cv::Mat distCoeffs = cv::Mat::zeros(8, 1, CV_64F);
	std::vector<cv::Mat> rvecs, tvecs;
	double error = cv::calibrateCamera(point_set,corner_set,
		size,camera_matrix,distCoeffs,rvecs,tvecs,CV_CALIB_FIX_ASPECT_RATIO);
	std::cout << "camera_matrix = "<< std::endl << " "  << camera_matrix << std::endl << std::endl;
	std::cout << "distCoeffs = "<< std::endl << " "  << distCoeffs << std::endl << std::endl;
	printf("error: %f\n", error);
}

void Board::collectCalibrationImages() {
	if (found) {
		printf("capturing calib image\n");
		corner_set.push_back(corners);
		point_set.push_back(points);
	}
}

void Board::update(bool collectCalib) {
	vid->getFrame();
	findCorners();
	if (collectCalib) collectCalibrationImages();
	vid->updateFrame();
}

int main(int argc, char const *argv[]) {
	Board bd;
	int count = 0;
	for (;;) {
		bd.update((count % 5 == 0 && count < 50 && bd.found));
		if(cv::waitKey(30) % 255 > 0)
			break;
		if (bd.found) count++;
		if (count == 51) bd.calibrate();
	}
	return 0;
}