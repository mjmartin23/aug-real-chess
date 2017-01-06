/*
Matt Martin
Jan 5, 2017
Board class
*/

#include "Board.h"

Board::Board() {
	vid = new Video(true,true); //initialize capture and show the video output
	size = cv::Size(7,7); // inner corners of 8x8 chessboard
}

void Board::findCorners() {
	cv::Mat frame = vid->curFrame;
	bool found = cv::findChessboardCorners(frame,size,corners,
		CV_CALIB_CB_ADAPTIVE_THRESH);
	//printf("%d\n", found);
	// if (found) {cv::cornerSubPix(frame,corners,cv::Size(8,8),cv::Size(-1,-1),
	// 	cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
	// }
	cv::drawChessboardCorners(frame,size,corners,found);
	vid->curFrame = frame;
	if (found) {
		printf("%f,%f\n",corners[0].x,corners[0].y);
		// for (std::vector<cv::Point2f>::iterator i = corners.begin(); i != corners.end(); ++i) {
		// 	printf("%f,%f\n", i->x,i->y);
		// }
	}
}

void Board::update() {
	vid->getFrame();
	findCorners();
	vid->updateFrame();
}

int main(int argc, char const *argv[]) {
	Board bd;
	for (;;) {
		bd.update();
		if(cv::waitKey(30) % 255 > 0)
			break;
	}
	return 0;
}