#include <opencv2/opencv.hpp>
#include "Functions.hpp"
#include <vector>
#include <utility>
#include <fstream>

using namespace std;
using namespace cv;

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

extern std::map<std::string, std::string> settings;

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