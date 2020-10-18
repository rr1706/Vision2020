#include "opencv2/opencv.hpp"

//#include <filesystem>
#include <unistd.h>

#include "Functions.hpp"
#ifdef WITH_NETWORK
#include "Network.hpp"
#endif

using namespace cv;


VideoCapture camera;
//camera specific
const float focalLength = 1559.29f;
//size of the target
const float widthOfHex = 39.25f;
const int heightOfHex = 17;
//fov of the camera (check again)
const int FovX = 120;
//3x3 black and white image used for erode dialate
Mat kernel = (cv::Mat_ < unsigned char >(3, 3) << 1,0, 1, 0, 1, 0, 1, 0, 1);
//press esc key to close program
char esc;


void runCamera(Mat base) {

	Mat threshed;

	//used in contours
	std::vector <std::vector<Point2i>> contours;
	std::vector <Vec4i> hierarchy;

	cvtColor(base, base, COLOR_BGR2GRAY);
	threshold(base, threshed, 30, 255, THRESH_BINARY);

	erode(threshed, threshed, kernel); //look into reducing this to one line
	erode(threshed, threshed, kernel);
	dilate(threshed, threshed, kernel);
	dilate(threshed, threshed, kernel);

	//contours
	findContours(threshed, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	#ifdef WITH_HEAD
	drawContours(base, contours, -1, Scalar(255, 10, 100), 1);
	#endif


	//only complete if a countour is found
	for (std::vector<Point2i> currentContour : contours){

		// test contour area
		if (contourArea(currentContour) < 100 || contourArea(currentContour) > 10000) {
			continue;
		}
		std::vector<Point2i> h, p; //make a hull variable and a polygon variable, thinking about re-naming
		convexHull(currentContour, h);
		approxPolyDP(h, p, 12, true);
		std::cout << "POLYGON SIZE " << p.size() << std::endl;

		#ifdef WITH_HEAD
		vector<vector<Point2i>> draw{p}; //wraps polygon to be draw in draw contours
		drawContours(base, draw, -1, Scalar(255, 150, 50));
		#endif

		if (p.size() == 4 && !currentContour.empty()){	

			RotatedRect rotRect = minAreaRect(currentContour);

			//find and send values
			double Xrot = calculateXrot(base.cols, rotRect.center, FovX);	// maybe find new tx formula
			double distToTarget = findDistance(heightOfHex, focalLength, rotRect.size.height);

			std::cout << "contour area: " << std::to_string(contourArea(currentContour)) << std::endl;
			std::cout << "Distance to target (in): " + std::to_string(distToTarget) << std::endl;
			std::cout << "Xrot: " << Xrot << std::endl;

			#ifdef WITH_HEAD
			putText(base, "Distance: " + to_string(distToTarget),Point(20, 40), FONT_HERSHEY_COMPLEX, 1, Scalar(255,50,200));
			putText(base, "Xrot: " + to_string(Xrot), Point(20, 90), FONT_HERSHEY_COMPLEX, 1, Scalar(255, 50, 200));
			#endif

			#ifdef WITH_NETWORK
			sendDouble("Xrot", Xrot);
			sendDouble("Distance", distToTarget);
			#endif
		}
	}

	//show final images
	#ifdef WITH_HEAD
	imshow("Normal", base);
	//imshow("Thresh", threshed);
	#endif
}

int main() {
	//check if it's possible to get around this
	//while (!std::filesystem::exist("/dev/video0")) {
	//	sleep(1);
	//}
	//sets camera params
	std::system("/usr/local/bin/setCam.sh");
	
	#ifdef WITH_NETWORK
	startTable();
	#endif

	camera.open(0);
	camera.set(CAP_PROP_FRAME_WIDTH, 640);
    camera.set(CAP_PROP_FRAME_HEIGHT, 480);
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
	return 0;
}