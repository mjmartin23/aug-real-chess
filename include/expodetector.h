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
	ExpoDetector(int*,int*,int*,int*,int*,int*);

	void detect(cv::Mat *,cv::Mat*);

private:
	int *lowH,*highH,*lowS,*highS,*lowV,*highV;
	
};