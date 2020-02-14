#include "opencv2/opencv.hpp"
#include <utility>
#include <fstream>
#include <vector>
#include <cmath>
#include "Functions.hpp"
#include "Network.hpp"
#include <unistd.h>
#include <opencv2/core/utils/filesystem.hpp>

using namespace cv;
using namespace std;

VideoCapture camera;

//constents: need to be changed when camera moves/replaced
double tYOffset = 0;
double camHeight =  24.25;
//double focalLength = 319.97856484;
//double focalLength = 309.15445743;
//double robotCenter = 10.795;

//Won't need changed
double widthOfHex = 39.25;
double targetHeight = 91;
double cameraHexDif = targetHeight - camHeight;

//may need changed
double FovX = 120;
double FovY = 72.05474677;


//basic thresh value
int tMin = 100;

//sets kernal to a cross, the shape of the kernal is determained by the shape of the target
Mat kernel = (cv::Mat_<unsigned char>(3,3) << 1,0,1,0,1,0,1,0,1);

void runCamera(Mat base);

//press esc key to close program
char esc;

std::map<std::string, std::string> settings = {
    {"mode", "run"},
    {"frontCam", "/dev/video0"}
};


int main(int argc, char** argv)
{
	while(!utils::fs::exists("/dev/video0")){
		usleep(500);
	}

	loadConfig("VisionSettings.conf");
	system(("v4l2ctrl -d " + settings["frontCam"] + " -l CamSettings.conf").c_str());

	startTable();
	camera.open(0);
	sendString("On?", "Yes");

	//display with with camera
	Mat base;
	while(camera.isOpened())
	{
		camera >> base;
		flip(base, base, 0); //only needed if cam is upside down
        runCamera(base);

		esc = waitKey(33);
		if (esc == 27)
		{
			break;
		}
	}
	sendString("On?", "No");
	camera.release();
	return 0;
}

void runCamera(Mat base)
{	
	//used for a threshed image
	Mat threshed, splitChannels[3];

	//used in contours
	vector<vector<Point2i>> contours;
	vector<Vec4i> hierarchy;
	//vector<Point2i> contour;

	//split color channels
	split(base, splitChannels);

	//note: optimize threshing and pick one
	threshold(splitChannels[0], threshed, tMin, 255, THRESH_BINARY); //try adaptive threshold
	//inRange(base, lowerVals, upperVals, threshed);
	dilate(threshed, threshed, kernel);
	erode(threshed, threshed, kernel);

    //contours
	findContours(threshed, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	#ifdef HEAD
	drawContours(base, contours, -1, Scalar(0, 255, 0), 1);
	#endif
	
	if(contours.size()<1){
		tMin=70;
	}
	
	//only complete if a countour is found
	for (vector<Point2i> contour : contours){

		if(contourArea(contour) > 300){
            tMin = 200;
        }
		if(contourArea(contour) < 70 || contours.size() < 1){
            tMin = 70;
		}

		//bounding box
		Rect bound = boundingRect(contour);
		#ifdef HEAD
		rectangle(base, bound.tl(), bound.br(), Scalar(255, 0, 0), 2);
		#endif
		
		//int width = bound.width;

		//finds target center and places crosshair
		Point2f centerOfTarget = Point(bound.x+bound.width/2, bound.y+bound.height/2);
		#ifdef HEAD
		drawMarker(base, Point(centerOfTarget), Scalar(255, 0, 0), MARKER_CROSS, 20, 5);
		#endif

		//Draw crosshair on the center of the image
		int imgWidth = base.cols;
		int imgHeight = base.rows;
		#ifdef HEAD
		drawMarker(base, Point(imgWidth / 2, imgHeight / 2), Scalar(255, 0, 0), MARKER_CROSS, 20, 5);
		#endif

		//find and send values
		double tY = calculateTY(imgHeight, centerOfTarget, FovY);	
		double Xrot = emersonXrot(imgWidth, centerOfTarget, FovX);
		double distToTarget = cameraHexDif/sin(tY+tYOffset);

		cout << to_string(tMin) << endl;
		if(distToTarget < 500){
			cout << "Distance to target center: " + to_string(distToTarget) << endl;
			sendMessage("Distance", distToTarget);
		}
		cout << "tY: " << to_string(tY) << endl;
		cout << "XRot: " << to_string(Xrot) << endl;
		sendMessage("Xrot", Xrot);
		cout << " " << endl;
	}
	//show final images
	#ifdef HEAD
	imshow("Normal", base);
	imshow("Thresh", threshed);
	imshow("B", splitChannels[0]);
	#endif
}
