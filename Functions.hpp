#include "opencv2/opencv.hpp"
//#include <vector>
//#include <utility>

//using namespace cv;
//using namespace std;

double connorXrot(double imgWidth, cv::Point centeroftarget, double fovCamera, double botCenter);

double calculateXrot(double imgWidth, cv::Point centeroftarget, double fovCamera);

double calculateTY(double imgHeight, cv::Point centeroftarget, double fovCameraY);

double findDistance(double realWidth_in, double focalLength, double width_px);
