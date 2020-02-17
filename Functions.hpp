#include "opencv2/opencv.hpp"
#include <vector>
#include <utility>

using namespace cv;
using namespace std;

double connorXrot(double imgWidth, cv::Point centeroftarget, double fovCamera, double botCenter);

Point2f calculateCenter(vector<Point> contour);

double emersonXrot(double imgWidth, cv::Point centeroftarget, double fovCamera);

double calculateTY(double imgHeight, cv::Point centeroftarget, double fovCameraY);

//extern std::map<std::string, std::string> settings;

//void loadConfig(string filename);

void runCamera(Mat base);
