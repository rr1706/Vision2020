#include <opencv2/opencv.hpp>
#include "Functions.hpp"
#include <vector>

using namespace std;
using namespace cv;

//note: as of commenting this, this functions doesn't work with finding the center of the target
Point2f calculateCenter(vector<Point> contour)
{
    Moments mu;
    mu = moments(contour, false);
    return Point2f(mu.m10/mu.m00, mu.m01/mu.m00);
}

//xrot equation from Emerson (in degrees)
double emersonXrot(double imgWidth, cv::Point centeroftarget, double fovCamera, double botCenter)
{
    double remapCenter = centeroftarget.x - imgWidth/2;
    return remapCenter/imgWidth*fovCamera;

}

//xrot equation from 2017 made by connor (in radians)
double connorXrot(double imgWidth, cv::Point centeroftarget, double fovCamera, double botCenter)
{
    double remapCenter = centeroftarget.x - imgWidth/2;
    //the last value was added this year to fix some stuff
    return (remapCenter / botCenter * fovCamera / (imgWidth / botCenter)) + 1.88496;
}