
#include "opencv2/opencv.hpp"
#include <vector>
#include <cmath>
#include "Functions.hpp"
#include "Network.hpp"

using namespace cv;
using namespace std;

VideoCapture camera;

//press esc key to close program
char esc;

//constents
double widthOfHex = 39.25;
double focalLength = 308.46268657;
double targetHeight = 91;
double camFov = 120;
double camHeight =  29.75;
double cameraHexDif = targetHeight - camHeight;
double robotCenter = 10.795;

//threshing value, will need to be changed if you use something other then ir or gray scale
int tMin = 100;

//sets kernal to a cross, the shape of the kernal is determained by the shape of the target
Mat kernel = (cv::Mat_<unsigned char>(3,3) << 1,0,1,0,1,0,1,0,1);


void runCamera(Mat base, Mat second)
{	
	//used for a threshed image
	Mat threshed;

	//used in contours
	vector<vector<Point2i>> contours;
	vector<Vec4i> hierarchy;
	vector<Point2i> last_contours;

	//note: optimize threshing
	threshold(second, threshed, tMin, 255, THRESH_BINARY); //try adaptive threshold
	dilate(threshed, threshed, kernel);
	erode(threshed, threshed, kernel);

    //contours
	findContours(threshed, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	#ifdef HEADLESS
	drawContours(base, contours, -1, Scalar(0, 255, 0), 1);
	#endif
	if(contours.size()<1){
	tMin=65;
	}
	//only complete if a countour is found
	for (auto contour : contours){

		if(contourArea(contour) > 300){
                	
			tMin = 220;
                }
		if(contourArea(contour) < 50|| contours.size() < 1){
                        tMin = 65;
                }

		cout << contourArea(contour)<<endl;
		//bounding box
		Rect bound = boundingRect(contour);
		//RotatedRect rotatedBound = minAreaRect(contour);
		#ifdef HEADLESS
		rectangle(base, bound.tl(), bound.br(), Scalar(255, 0, 0), 2);
		#endif
		/*// test values
		if (rotatedBound.size.width < 1 || rotatedBound.size.height < 1)
			continue;

		Point2f vertices[4];
		rotatedBound.points(vertices);	

		{ // draw the rotated rectangle
			vector<vector<Point2i> > drawRot;
			vector<Point2i> rot;
			for (int i = 0; i < 4; ++i)
				rot.push_back(vertices[i]);
			drawRot.push_back(rot);

			drawContours(base, drawRot, -1, cv::Scalar(0,0,255));
		}*/
		
		int width = bound.width;

		//finds target center and places crosshair
		Point2f centerOfTarget = Point(bound.x+bound.width/2, bound.y+bound.height/2);
		#ifdef HEADLESS
		drawMarker(base, Point(centerOfTarget), Scalar(255, 0, 0), MARKER_CROSS, 20, 5);
		#endif

		//Draw crosshair on the center of the image
		int imgWidth = base.cols;
		int imgHeight = base.rows;
		#ifdef HEADLESS
		drawMarker(base, Point(imgWidth / 2, imgHeight / 2), Scalar(255, 0, 0), MARKER_CROSS, 20, 5);
		#endif

		//find and send values
		double distance = (widthOfHex * focalLength) / width;			
		double Xrot = emersonXrot(imgWidth, centerOfTarget, camFov, robotCenter);
		double distanceToBase = sqrt(pow(distance, 2.0) - pow(cameraHexDif, 2.0));
		
		cout << to_string(tMin) << endl;
		if(distance || distanceToBase < 500){
			cout << "Distance to target center: " + to_string(distance) << endl;
			sendMessage("targetDist", distance);
			cout << "XRot: " << to_string(Xrot) << endl;
			sendMessage("Xrot", Xrot);
			cout << "distance to base: " + to_string(distanceToBase) << endl;
			sendMessage("distToBase", distanceToBase);
			cout << " " << endl;
		}
	}
	//show final images
	#ifdef HEADLESS
	imshow("Normal", base);
	imshow("Thresh", threshed);
	imshow("gray", second);
	#endif
}

int main(int argc, char** argv)
{
	startTable();
	camera.open(0);
	//note: make sure this isn't broken
	//camera.set(CAP_PROP_EXPOSURE, 1);
	//camera.set(CAP_PROP_AUTO_EXPOSURE, 1000);

	//display with with camera
	Mat base, gray;
	while(camera.isOpened())
	{
		camera >> base;
		//flip(base, base, 0); //only needed if cam is upside down
        	cvtColor(base, gray, COLOR_BGR2GRAY);
        	runCamera(base, gray);

		esc = waitKey(33);
		if (esc == 27)
		{
			break;
		}
	}
	camera.release();
	return 0;
}
