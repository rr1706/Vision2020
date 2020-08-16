#include <opencv2/opencv.hpp>
#include "Functions.hpp"

//xrot equation from Emerson (in degrees)
double calculateXrot(double imgWidth, cv::Point centeroftarget, double fovCameraX){
    double remapCenter = centeroftarget.x - imgWidth/2;
    return remapCenter/imgWidth*fovCameraX;
}

//based off this: https://www.pyimagesearch.com/2015/01/19/find-distance-camera-objectmarker-using-python-opencv/
double findDistance(double realWidth_in, double focalLength, double width_px){
    return (realWidth_in * focalLength) / width_px;
}