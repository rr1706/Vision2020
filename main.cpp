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
double camHeight = 22.25;		//height off ground
double focalLength = 1559.29;		//find new fl
double px_per_mm = 1.52048426;	//find
//double focalLength = 309.15445743;
//double robotCenter = 10.795;

//Won't need changed
double widthOfHex = 39.25 * 25.4;
double targetHeight = 91;
double cameraHexDif = targetHeight - camHeight;

//may need changed
double FovX = 120;
double FovY = 72.05474677;


//basic thresh value
int tMin = 20;

//sets kernal to a cross, the shape of the kernal is determained by the shape of the target
Mat kernel = (cv::Mat_ < unsigned char >(3, 3) << 1, 0, 1, 0, 1, 0, 1, 0, 1);	//look for new kernal

void runCamera(Mat base);

//press esc key to close program
char esc;


int main(int argc, char **argv)
{
	while (!utils::fs::exists("/dev/video0")) {
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
	while (camera.isOpened()) {
		camera >> base;
		//flip(base, base, 0); //only needed if cam is upside down
		runCamera(base);
		esc = waitKey(33);
		if (esc == 27) {
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
	std::cout << "Frame rate " << base.cols << std::endl;
	//used for a threshed image
	Mat threshed;

	//used in contours
	vector <vector<Point2i> > contours;
	vector <Vec4i> hierarchy;

	cv::cvtColor(base, threshed, COLOR_BGR2GRAY);
	//cv::threshold(threshed, threshed,tMin,255,THRESH_BINARY);
	cv::adaptiveThreshold(threshed, threshed, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, 2);
	erode(threshed, threshed, kernel);
	erode(threshed, threshed, kernel);
	cv::imshow("Erode", threshed);
	dilate(threshed, threshed, kernel);
	dilate(threshed, threshed, kernel);

	//contours
	findContours(threshed, contours, hierarchy, RETR_EXTERNAL,
				 CHAIN_APPROX_SIMPLE);
#ifdef WITH_HEAD
	drawContours(base, contours, -1, Scalar(0, 255, 0), 1);
#endif

	vector<Point2i> contour;

	//only complete if a countour is found
	for (size_t i = 0; i < contours.size(); i++) {
		vector<Point2i> c = contours[i];
		// test contour area
		if (contourArea(c) < 100 || contourArea(c) > 10000) {
			continue;
		}
		vector<Point2i> h, p;
		convexHull(c, h);
		approxPolyDP(h, p, 12, true);
		std::cout << "POLYGON SIZE " << p.size() << std::endl;
#ifdef WITH_HEAD
		std::vector<std::vector<Point2i>> draw{p};
		drawContours(base, draw, -1, Scalar(64, 128, 255));
#endif
		if (p.size() == 4)
		{
			contour = c;
		}
	}

	vector<Point2i> hull, polygon;

	if (!contour.empty())
	{
		auto bound = boundingRect(contour);
		RotatedRect rotRect = minAreaRect(contour);
		double height = rotRect.size.height;
		//finds target center and places crosshair
		Point2f centerOfTarget = Point(bound.x + bound.width / 2,
									   bound.y + bound.height / 2);
#ifdef WITH_HEAD
		drawMarker(base, Point(centerOfTarget), Scalar(255, 0, 0),
				   MARKER_CROSS, 20, 5);
#endif
		//Draw crosshair on the center of the image
		int imgWidth = base.cols;
		int imgHeight = base.rows;
		double pxWidth = (double) bound.width;
#ifdef WITH_HEAD
		drawMarker(base, Point(imgWidth / 2, imgHeight / 2), Scalar(255, 0, 0), MARKER_CROSS, 20, 5);
		putText(base, "THeight: " + to_string(height), Point(20, 120), FONT_HERSHEY_COMPLEX, 1, Scalar(255, 50, 200));
#endif

		//find and send values
		//double tY = calculateTY(imgHeight, centerOfTarget, FovY); //find new ty formula
		double Xrot = calculateXrot(imgWidth, centerOfTarget, FovX);	// maybe find new tx formula
		//double distToTarget = findDistance2(widthOfHex, pxWidth, focalLength, px_per_mm);	//finish
		double distToTarget = findDistance3(17, focalLength, height);	//finish

		cout << "contour area: " << to_string(contourArea(contour)) <<
			endl;
		cout << "Thresh value: " << to_string(tMin) << endl;

		cout << "Distance to target (in): " + to_string(distToTarget) << endl;
#ifdef WITH_HEAD
		putText(base, "Distance: " + to_string(distToTarget),
				Point(20, 40), FONT_HERSHEY_COMPLEX, 1, Scalar(255,
															   50,
															   200));
#endif
#ifdef NETWORK
		sendDouble("Distance", distToTarget);
#endif

		cout << "Xrot: " << to_string(Xrot) << endl;
#ifdef WITH_HEAD
		putText(base, "Xrot: " + to_string(Xrot), Point(20, 90),
				FONT_HERSHEY_COMPLEX, 1, Scalar(255, 50, 200));
#endif
#ifdef NETWORK
		sendDouble("Xrot", Xrot);
#endif
		cout << " " << endl;

	}

	//show final images
#ifdef WITH_HEAD
	imshow("Normal", base);
	//imshow("Thresh", threshed);
#endif
}
