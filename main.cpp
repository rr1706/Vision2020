//remember to un-comment the install and other stuff, also random errors might mean check the other commented stuff, also update readme
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
double camHeight =  22.25; //height off ground
double focalLength = 319.97856484; //find new fl
double px_per_mm = 0; //find
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
int tMin = 170;

//sets kernal to a cross, the shape of the kernal is determained by the shape of the target
Mat kernel = (cv::Mat_<unsigned char>(3,3) << 1,0,1,0,1,0,1,0,1); //look for new kernal

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
		//flip(base, base, 0); //only needed if cam is upside down
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
	Mat threshed;

	//used in contours
	vector<vector<Point2i>> contours;
	vector<Vec4i> hierarchy;

	//try canny
	//inRange(base,Scalar(1,1,1),Scalar(255,255,255),threshed);
	threshold(base,threshed,tMin,255,THRESH_BINARY);
	erode(threshed, threshed, kernel);
	dilate(threshed, threshed, kernel);
	//Canny(base, threshed, 50, 100);

    //contours
	findContours(threshed, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	#ifdef HEAD
	drawContours(base, contours, -1, Scalar(0, 255, 0), 1);
	#endif

	//optimize contour auto thresh
	if(contours.size()<1){
		tMin--;
	}
	vector<vector<Point2i>> hull(contours.size());
	//only complete if a countour is found
	for(size_t i = 0; i <  contours.size(); i++){

		vector<Point2i> contour = contours[i];

		if(contourArea(contour) > 300){
            tMin++;
        }
		if(contourArea(contour) < 70 || contours.size() < 1){
            tMin--;
		}
		if(contourArea(contours[i]) < 1000){

			convexHull(contours[i], hull[i]);
			#ifdef HEAD
			drawContours(base, hull, (int)i, Scalar(255,0,255));
			#endif

			//based off https://www.pyimagesearch.com/2016/02/08/opencv-shape-detection/
			vector<double> approx;
			double peri = arcLength(contour, true);
			approxPolyDP(contour, approx, 0.04 * peri, true);
			cout << "approx points: " << to_string(approx.size) << endl;

			if(approx.size == 4){
				
				//bounding box
				Rect bound = boundingRect(contours[i]);
				#ifdef HEAD
				RotatedRect rotRect = minAreaRect(contours[i]);
				rectangle(base, bound.tl(), bound.br(), Scalar(255, 0, 0), 5);
				#endif
				//finds target center and places crosshair
				Point2f centerOfTarget = Point(bound.x+bound.width/2, bound.y+bound.height/2);
				#ifdef HEAD
				drawMarker(base, Point(centerOfTarget), Scalar(255, 0, 0), MARKER_CROSS, 20, 5);
				#endif
				//Draw crosshair on the center of the image
				int imgWidth = base.cols;
				int imgHeight = base.rows;
				double pxWidth = (double) bound.width;
				#ifdef HEAD
				drawMarker(base, Point(imgWidth / 2, imgHeight / 2), Scalar(255, 0, 0), MARKER_CROSS, 20, 5);
				#endif

				//find and send values
				double tY = calculateTY(imgHeight, centerOfTarget, FovY); //find new ty formula
				double Xrot = calculateXrot(imgWidth, centerOfTarget, FovX); // maybe find new tx formula
				double distToTarget = findDistance2(widthOfHex, pxWidth, focalLength, px_per_mm); //finish

				cout << "contour area: " << to_string(contourArea(contour)) << endl;
				cout << "Thresh value: " << to_string(tMin) << endl;
				if(distToTarget < 500){
					cout << "Distance to target: " + to_string(distToTarget) << endl;
					#ifdef HEAD
					putText(base, "Distance: " + to_string(distToTarget), Point(30,30), FONT_HERSHEY_COMPLEX, 5, Scalar(255,50,200));
					#endif
					#ifdef NETWORK
					sendDouble("Distance", distToTarget);
					#endif
				}
				cout << "tY: " << to_string(tY) << endl;
				#ifdef Head
				putText(base, "tY: " + to_string(tY), Point(60,60), FONT_HERSHEY_COMPLEX, 5, Scalar(255,50,200));
				#endif
				#ifdef NETWORK
				sendDouble("tY", tY);
				#endif
				cout << "Xrot: " << to_string(Xrot) << endl;
				#ifdef HEAD
				putText(base, "Xrot: " + to_string(Xrot), Point(90,90), FONT_HERSHEY_COMPLEX, 5, Scalar(255,50,200));
				#endif
				#ifdef NETWORK
				sendDouble("Xrot", Xrot);
				#endif
				cout << " " << endl;
			}
		}
	}
	//show final images
	#ifdef HEAD
	//may have the synax backwards
	namedWindow("Normal", WINDOW_FREERATIO);
	namedWindow("Thresh", WINDOW_FREERATIO);
	imshow("Normal", base);
	imshow("Thresh", threshed);
	#endif
}