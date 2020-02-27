#include "opencv2/cudaimgproc.hpp"
#include "opencv2/cudafilters.hpp"
#include "opencv2/core/cuda.hpp"
#include "opencv2/opencv.hpp"
#include <vector>
#include <iostream>
#include "Functions.hpp"
#include <opencv2/core/utils/filesystem.hpp>
#include <fstream>
#ifdef WITH_NETWORK
#include "Network.hpp"
#endif
#include <unistd.h>

using namespace cv;
using namespace std;
//using namespace cv::cuda;

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

void runCamera(cuda::GpuMat base);

//press esc key to close program
char esc;


int main(int argc, char **argv)
{
	cout << to_string(0) << endl; //;;
	while (!utils::fs::exists("/dev/video0")) {
		usleep(500);
	}

	system("/usr/local/bin/setCam.sh");
	cout << to_string(1) << endl; //;;
	#ifdef WITH_NETWORK
	startTable();
	#endif
	camera.open(0);
	#ifdef WITH_NETWORK
	sendString("On?", "Yes");
	#endif
	//display with with camera
	Mat base;
	cuda::GpuMat gbase, smol;
	cout << to_string(2) << endl; //;;
	while (camera.isOpened()) {

		camera >> base;
		cout << to_string(2.09) << endl; //;;
		flip(base, base, 0); //only needed if cam is upside down
		base.copyTo(gbase);
		cout << to_string(2.1) << endl; //;;
		//Size newSize( base.size().width / 2 , base.size().height / 2 );
		//cuda::resize(gbase, smol, newSize, 360, 920, INTER_AREA);
		cout << to_string(3) << endl; //;;
		runCamera(gbase);
		esc = waitKey(33);
		if (esc == 27) {
			break;
		}
	}
	#ifdef WITH_NETWORK
	sendString("On?", "No");
	#endif
	return 0;
}

void runCamera(cuda::GpuMat base)
{

	//used for a threshed image
	cuda::GpuMat threshed;

	//used in contours
	vector <vector<Point2i> > contours;
	vector <Vec4i> hierarchy;

	cout << to_string(4) << endl; //;;

	cuda::cvtColor(base, base, COLOR_BGR2GRAY);
	cout << to_string(4.1) << endl; //;;
	cuda::threshold(base, threshed,tMin,255,THRESH_BINARY);
	//cv::adaptiveThreshold(threshed, threshed, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, 2);
	erode(threshed, threshed, kernel); //look into reducing this to one line
	erode(threshed, threshed, kernel);
	dilate(threshed, threshed, kernel);
	dilate(threshed, threshed, kernel);

	cout << to_string(5) << endl; //;;

	//contours
	findContours(threshed, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	#ifdef WITH_HEAD
	drawContours(base, contours, -1, Scalar(255, 10, 100), 1);
	#endif

	cout << to_string(6) << endl; //;;

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
