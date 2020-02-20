#include <opencv2/opencv.hpp>
#include "Functions.hpp"
//#include <vector>
//#include <utility>
//#include <fstream>

//using namespace std;
//using namespace cv;

//xrot equation from Emerson (in degrees)
double calculateXrot(double imgWidth, cv::Point centeroftarget, double fovCameraX)
{
    double remapCenter = centeroftarget.x - imgWidth/2;
    return remapCenter/imgWidth*fovCameraX;
}

//based off this: https://www.pyimagesearch.com/2015/01/19/find-distance-camera-objectmarker-using-python-opencv/
double findDistance(double realWidth_in, double focalLength, double width_px){
    return (realWidth_in * focalLength) / width_px;
}

double calculateTY(double imgHeight, cv::Point centeroftarget, double fovCameraY)
{
    double remapCenter = centeroftarget.y - imgHeight/2;
    return remapCenter/imgHeight*fovCameraY;
}
