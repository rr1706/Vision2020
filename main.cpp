#include "opencv2/opencv.hpp"
#include <utility>
#include <vector>
#include <cmath>
#include "Functions.hpp"
#include <opencv2/core/utils/filesystem.hpp>
#include <fstream>
#ifdef NETWORK
#include "Network.hpp"
#endif
#include <unistd.h>

using namespace cv;
using namespace std;

VideoCapture camera;

//constents: need to be changed when camera moves/replaced
double tYOffset = 30;
double camHeight =  22.25;
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

/*std::map<std::string, std::string> settings = {
    {"mode", "run"},
    {"frontCam", "/dev/video0"}
};*/


int main(int argc, char** argv)
{
	while(!utils::fs::exists("/dev/video0")){
		usleep(500);
	}

	//loadConfig("../VisionSettings.conf");
	system("/usr/local/bin/setCam.sh");
	#ifdef NETWORK
	startTable();
	#endif
	camera.open(0);
	#ifdef NETWORK
	sendString("On?", "Yes");
	#endif
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
	#ifdef NETWORK
	sendString("On?", "No");
	#endif
	camera.release();
	return 0;
}

void runCamera(Mat base)
{
	//used for a threshed image
	Mat threshed, splitChannels[3], merged;

	//used in contours
	vector<vector<Point2i>> contours;
	vector<Vec4i> hierarchy;

	inRange(base,Scalar(25,25,25),Scalar(255,255,255),threshed);
	//dilate(threshed, threshed, kernel);
	//erode(threshed, threshed, kernel);

    	//contours
	findContours(threshed, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	#ifdef HEAD
	drawContours(base, contours, -1, Scalar(0, 255, 0), 1);
	#endif
	if(contours.size()<1){
		tMin=70;
	}
	vector<vector<Point2i>> hull(contours.size());
	//only complete if a countour is found
	for(size_t i = 0; i <  contours.size(); i++){

		vector<Point2i> contour = contours[0];

		if(contourArea(contour) > 300){
            		tMin = 200;
        	}
		if(contourArea(contour) < 70 || contours.size() < 1){
            		tMin = 70;
		}
		if(contourArea(contour) < 100){

			convexHull(contours[i], hull[i]);
			#ifdef HEAD
			drawContours(base, hull, (int)i, Scalar(255,0,255));
			#endif
			//bounding box
			Rect bound = boundingRect(contour);
			#ifdef HEAD
			rectangle(base, bound.tl(), bound.br(), Scalar(255, 0, 0), 3);
			#endif
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

			cout << "Thresh value: " << to_string(tMin) << endl;
			if(distToTarget < 500){
				cout << "Distance to target center: " + to_string(distToTarget) << endl;
				#ifdef NETWORK
				sendDouble("Distance", distToTarget);
				#endif
			}
			cout << "tY: " << to_string(tY) << endl;
			#ifdef NETWORK
			sendDouble("tY", tY);
			#endif
			cout << "XRot: " << to_string(Xrot) << endl;
			#ifdef NETWORK
			sendDouble("Xrot", Xrot);
			#endif
			cout << " " << endl;
		}
	}
	//show final images
	#ifdef HEAD
	imshow("Normal", base);
	imshow("Thresh", threshed);
	#endif
}
