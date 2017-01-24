/* Matt Martin
* Jan 18, 2017
* Expo marker detector declaration
*/
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;

class PointerDetector
{
public:
	PointerDetector();
	PointerDetector(int*,int*,int*,int*,int*,int*);
	PointerDetector(int,int,int,int,int,int);

	void update(cv::Mat,int);
	int x,y;
	float distance,size,focalLength;

private:
	void detect(cv::Mat ,int);
	void calcCenter();
	void histogram(cv::Mat*,cv::Mat*);
	void configFocalLength();
	void calcDistance();
	
	int *lowH,*highH,*lowS,*highS,*lowV,*highV;
	cv::Mat thresh;
	
};