#include "opencv2/cudaimgproc.hpp"
#include "opencv2/cudafilters.hpp"
#include "opencv2/core/cuda.hpp"
//#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"
#ifdef WITH_HEAD
#include "opencv2/highgui.hpp"
#endif
#include <thread>
#include <vector>
#include <iostream>
#include "Functions.hpp"
#include <opencv2/core/utils/filesystem.hpp>
#include <fstream>
#ifdef WITH_NETWORK
#include "Network.hpp"
#endif
#include <unistd.h>
//try opencl and umat? https://kezunlin.me/post/59afd8b3/

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
	while (!utils::fs::exists("/dev/video0")) {
		usleep(500);
	}

	system("/usr/local/bin/setCam.sh");
	#ifdef WITH_NETWORK
	startTable();
	#endif
	camera.open(0);
	//display with with camera
	Mat base;
	cuda::GpuMat gbase, smol;
	while (camera.isOpened()) {

		camera >> base;
		flip(base, base, 0); //only needed if cam is upside down
		gbase = cuda::GpuMat(base);
		//cuda::resize(gbase, smol, Size(920,400), 0, 0, INTER_AREA);
		runCamera(gbase);
		esc = waitKey(33);
		if (esc == 27) {
			break;
		}
	}
	return 0;
}

void runCamera(cuda::GpuMat gbase)
{

	//used for a threshed image
	cuda::GpuMat gthreshed;
	Mat base, threshed;

	//used in contours
	vector <vector<Point2i> > contours;
	vector <Vec4i> hierarchy;

	cuda::cvtColor(gbase, gbase, COLOR_BGR2GRAY);
	cuda::threshold(gbase, gthreshed,tMin,255,THRESH_BINARY);
	//cv::adaptiveThreshold(threshed, threshed, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, 2);
	//end of cuda :(
	base = (Mat) gbase;
	threshed = (Mat) gthreshed;

	thread th1(erode, threshed, threshed, kernel); //look into reducing this to one line
	thread th2(erode, threshed, threshed, kernel);
	dilate(threshed, threshed, kernel);
	dilate(threshed, threshed, kernel);

	//contours
	findContours(threshed, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	#ifdef WITH_HEAD
	thread th4 (drawContours, base, contours, -1, Scalar(255, 10, 100), 1);
	#endif


	vector<Point2i> contour;

	//only complete if a countour is found
	for (vector<Point2i> currentContour : contours){
		//vector<Point2i> c = contours[i];
		// test contour area
		if (contourArea(currentContour) < 100 || contourArea(currentContour) > 10000) {
			continue;
		}
		vector<Point2i> h, p; //make a hull variable and a polygon variable, thinking about re-naming
		convexHull(currentContour, h);
		approxPolyDP(h, p, 12, true);
		cout << "POLYGON SIZE " << p.size() << endl;

		#ifdef WITH_HEAD
		vector<vector<Point2i>> draw{p}; //wraps polygon to be draw in draw contours
		drawContours(base, draw, -1, Scalar(255, 150, 50));
		#endif

		if (p.size() == 4)
		{
			contour = currentContour;
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
		//putText(base, "Bound Height: " + to_string(boundHeight), Point(20, 120), FONT_HERSHEY_COMPLEX, 1, Scalar(255, 50, 200));
		#endif

		#ifdef WITH_NETWORK
		sendDouble("Xrot", Xrot);
		sendDouble("Distance", distToTarget);
		#endif

		cout << " " << endl;

	}

	th1.join();
	th2.join();
	th4.join();

	//show final images
	#ifdef WITH_HEAD
	imshow("Normal", base);
	//imshow("Thresh", threshed);
	#endif
}
