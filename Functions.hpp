#include "opencv2/opencv.hpp"
#include <vector>
#include <utility>

using namespace cv;
using namespace std;

double connorXrot(double imgWidth, cv::Point centeroftarget, double fovCamera, double botCenter);

double calculateXrot(double imgWidth, cv::Point centeroftarget, double fovCamera);

double calculateTY(double imgHeight, cv::Point centeroftarget, double fovCameraY);

double findDistance2(double realObjectWidth_mm, double objectWidth_px, double focalLength_mm, double px_per_mm);

double findDistance3(double realWidth_in, double focalLength, double width_px);

//extern std::map<std::string, std::string> settings;

//void loadConfig(string filename);
