//tried to optimize blind, may work

/*#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"*/
#include "opencv2/opencv.hpp"
#include <vector>
#include "Functions.hpp"
#include <opencv2/core/utils/filesystem.hpp>
#include <fstream>
#ifdef WITH_NETWORK
#include "Network.hpp"
#endif
#include <unistd.h>

using namespace cv;
using namespace std;

VideoCapture camera;

//constents: need to be changed when camera moves/replaced
double focalLength = 1559.29;
//Won't need changed
double widthOfHex = 39.25;
//may need changed
double FovX = 120;
//find y fov maybe

//basic thresh value
int tMin = 30;

//sets kernal to a cross, the shape of the kernal is determained by the shape of the target
Mat kernel = (cv::Mat_ < unsigned char >(3, 3) << 1,0, 1, 0, 1, 0, 1, 0, 1);	//look for new kernal

void runCamera(Mat base);

//press esc key to close program
char esc;


int main(int argc, char **argv)
{
	while (!utils::fs::exists("/dev/video0")) {
		usleep(500);
	}

	system("/usr/local/bin/setCam.sh");
	#ifdef WITH_NETWORK
	startTable();
	#endif
	camera.open(0);
	#ifdef WITH_NETWORK
	sendString("On?", "Yes");
	#endif
	//display with with camera
	Mat base;
	while (camera.isOpened()) {
		camera >> base;
		//resize(base, base, Size(), xFactor, yFactor, INTER_LANCZOS4); //find values to resize to
		//flip(base, base, 0); //only needed if cam is upside down
		runCamera(base);
		esc = waitKey(33);
		if (esc == 27) {
			break;
		}
	}
	#ifdef WITH_NETWORK
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
	vector <vector<Point2i> > contours;
	vector <Vec4i> hierarchy;

	cvtColor(base, base, COLOR_BGR2GRAY);
	threshold(base, threshed,tMin,255,THRESH_BINARY);
	//cv::adaptiveThreshold(threshed, threshed, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, 2);
	erode(threshed, threshed, kernel); //look into reducing this to one line
	erode(threshed, threshed, kernel);
	dilate(threshed, threshed, kernel);
	dilate(threshed, threshed, kernel);

	//contours
	findContours(threshed, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	#ifdef WITH_HEAD
	drawContours(base, contours, -1, Scalar(255, 10, 100), 1);
	#endif

	vector<Point2i> contour;

	//only complete if a countour is found
	for (size_t i = 0; i < contours.size(); i++) {
		vector<Point2i> c = contours[i];
		// test contour area
		if (contourArea(c) < 100 || contourArea(c) > 10000) {
			continue;
		}
		vector<Point2i> h, p; //make a hull variable and a polygon variable, thinking about re-naming
		convexHull(c, h);
		approxPolyDP(h, p, 12, true);
		cout << "POLYGON SIZE " << p.size() << endl;

		#ifdef WITH_HEAD
		vector<vector<Point2i>> draw{p}; //wraps polygon to be draw in draw contours
		drawContours(base, draw, -1, Scalar(255, 150, 50));
		#endif

		if (p.size() == 4)
		{
			contour = c;
		}
	}

	if (!contour.empty())
	{
		Rect bound = boundingRect(contour);
		RotatedRect rotRect = minAreaRect(contour);
		double boundHeight = rotRect.size.height;
		
		//finds target center and places crosshair
		Point2f centerOfTarget = Point(bound.x + bound.width / 2, bound.y + bound.height / 2);
		
		#ifdef WITH_HEAD
		drawMarker(base, Point(centerOfTarget), Scalar(255, 0, 0), MARKER_CROSS, 20, 5);
		#endif
		//Draw crosshair on the center of the image
		int imgWidth = base.cols;

		//find and send values
		//double tY = calculateTY(imgHeight, centerOfTarget, FovY); //find new ty formula
		double Xrot = calculateXrot(imgWidth, centerOfTarget, FovX);	// maybe find new tx formula
		double distToTarget = findDistance(17 /*hex height*/, focalLength, boundHeight);

		cout << "contour area: " << to_string(contourArea(contour)) << endl;
		cout << "Thresh value: " << to_string(tMin) << endl;
		cout << "Distance to target (in): " + to_string(distToTarget) << endl;
		cout << "Xrot: " << to_string(Xrot) << endl;

		#ifdef WITH_HEAD
		putText(base, "Distance: " + to_string(distToTarget),Point(20, 40), FONT_HERSHEY_COMPLEX, 1, Scalar(255,50,200));
		putText(base, "Xrot: " + to_string(Xrot), Point(20, 90), FONT_HERSHEY_COMPLEX, 1, Scalar(255, 50, 200));
		putText(base, "Bound Height: " + to_string(boundHeight), Point(20, 120), FONT_HERSHEY_COMPLEX, 1, Scalar(255, 50, 200));
		#endif

		#ifdef WITH_NETWORK
		sendDouble("Xrot", Xrot);
		sendDouble("Distance", distToTarget);
		#endif
		
		cout << " " << endl;

	}

	//show final images
	#ifdef WITH_HEAD
	imshow("Normal", base);
	//imshow("Thresh", threshed);
	#endif
}
