#include <opencv2/opencv.hpp>
#include "Functions.hpp"
#include <vector>
#include <utility>
#include <fstream>

using namespace std;
using namespace cv;

//xrot equation from Emerson (in degrees)
double calculateXrot(double imgWidth, cv::Point centeroftarget, double fovCameraX)
{
    double remapCenter = centeroftarget.x - imgWidth/2;
    return remapCenter/imgWidth*fovCameraX;
}

//based off this: https://stackoverflow.com/questions/14038002/opencv-how-to-calculate-distance-between-camera-and-object-using-image
/*double findDistance1(double realObjectWidth_mm, double  ObjectWidth_px, double focalLength_mm){
    
    double px_per_mm = ;
    double widthOnImageSensor_mm = ;
    return realObjectWidth_mm * focalLength_mm / widthOnImageSensor_mm;
}*/
double findDistance2(double realObjectWidth_mm, double  objectWidth_px, double focalLength_mm, double px_per_mm){
    double widthOnImageSensor_mm = objectWidth_px/px_per_mm;
    return realObjectWidth_mm * focalLength_mm / widthOnImageSensor_mm;
}

//based off this: https://www.pyimagesearch.com/2015/01/19/find-distance-camera-objectmarker-using-python-opencv/
double findDistance3(double realWidth_in, double focalLength, double width_px){
    return (realWidth_in * focalLength) / width_px;
}

double calculateTY(double imgHeight, cv::Point centeroftarget, double fovCameraY)
{
    double remapCenter = centeroftarget.y - imgHeight/2;
    return remapCenter/imgHeight*fovCameraY;
}


/*extern std::map<std::string, std::string> settings;

void loadConfig(string filename){
    string line;
    ifstream myfile(filename);
    if (myfile.is_open())
    {
        while ( getline(myfile,line))
        {

            int xx = line.find('=');
            int jf = line.size();
            string key = line.substr(0, xx);
            xx += 1;
            string value = line.substr(xx, jf - xx);
            settings[key] = value;
        }
        myfile.close();
    }
    else printf("unable to open file\n");
}
*/