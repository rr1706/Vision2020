#include "Functions.hpp"
#include <opencv2/videoio.hpp>

//xrot equation from Emerson (in degrees)
double calculateXrot(double imgWidth, cv::Point centeroftarget, double fovCameraX){
    double remapCenter = centeroftarget.x - imgWidth/2;
    return remapCenter/imgWidth*fovCameraX;
}

//based off this: https://www.pyimagesearch.com/2015/01/19/find-distance-camera-objectmarker-using-python-opencv/
double findDistance(double realWidth_in, double focalLength, double width_px){
    return (realWidth_in * focalLength) / width_px;
}

void setCam(cv::VideoCapture& camera){
	camera.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    camera.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
	camera.set(cv::CAP_PROP_BRIGHTNESS, 64);
	camera.set(cv::CAP_PROP_CONTRAST, 95);
	camera.set(cv::CAP_PROP_SATURATION, 0);
	camera.set(cv::CAP_PROP_HUE, 2000);
	camera.set(cv::CAP_PROP_WB_TEMPERATURE, 4000);
	camera.set(cv::CAP_PROP_GAMMA, 100);
	camera.set(cv::CAP_PROP_SHARPNESS, 5);
	camera.set(cv::CAP_PROP_BACKLIGHT, 1);
	camera.set(cv::CAP_PROP_EXPOSURE, 5000);
}