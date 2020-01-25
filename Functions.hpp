#include "opencv2/opencv.hpp"
#include <vector>

using namespace cv;
using namespace std;

double connorXrot(double imgWidth, cv::Point centeroftarget, double fovCamera, double botCenter);

Point2f calculateCenter(vector<Point> contour);

double emersonXrot(double imgWidth, cv::Point centeroftarget, double fovCamera, double botCenter);