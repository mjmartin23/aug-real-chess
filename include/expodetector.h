/* Matt Martin
* Jan 18, 2017
* Expo marker detector declaration
*/
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;

class ExpoDetector
{
public:
	ExpoDetector();
	ExpoDetector(int*,int*,int*,int*,int*,int*);
	ExpoDetector(int,int,int,int,int,int);

	void detect(cv::Mat *,cv::Mat*);
	void histogram(cv::Mat*,cv::Mat*);
	int x,y;

private:
	int *lowH,*highH,*lowS,*highS,*lowV,*highV;
	
};