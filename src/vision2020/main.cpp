#include "Functions.hpp"
#include "opencv4/opencv2/core.hpp"
#include "opencv4/opencv2/highgui.hpp"
#include "opencv4/opencv2/imgproc.hpp"
#include "opencv4/opencv2/video.hpp"

#include <filesystem>
#include <unistd.h>

#ifdef WITH_NETWORK
	#include "Network.hpp"
#endif

using namespace cv;

#define PORT 8000
VideoCapture camera;
// camera specific
const float focalLength = 1559.29f;
// size of the target
const float widthOfHex = 39.25f;
const int heightOfHex = 17;
// fov of the camera (check again)
const int FovX = 120;
// 3x3 black and white image used for erode dialate
Mat kernel = ( cv::Mat_<unsigned char>( 3, 3 ) << 1, 0, 1, 0, 1, 0, 1, 0, 1 );
// press esc key to close program
char esc;

void runCamera( Mat& base ) {
	Mat threshed;

	// used in contours
	std::vector<std::vector<Point2i>> contours;
	std::vector<Vec4i> hierarchy;

	cvtColor( base, base, COLOR_BGR2GRAY );
	threshold( base, threshed, 30, 255, THRESH_BINARY );

	erode( threshed, threshed, kernel );
	erode( threshed, threshed, kernel );
	dilate( threshed, threshed, kernel );
	dilate( threshed, threshed, kernel );

	// contours
	findContours( threshed, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE );
#ifdef WITH_HEAD
	drawContours( base, contours, -1, Scalar( 255, 10, 100 ), 1 );
#endif


	// only complete if a countour is found
	for( std::vector<Point2i> currentContour : contours ) {
		// test contour area
		if( contourArea( currentContour ) < 100 || contourArea( currentContour ) > 10000 ) { continue; }
		std::vector<Point2i> h, p; // make a hull variable and a polygon variable, thinking about re-naming
		convexHull( currentContour, h );
		approxPolyDP( h, p, 12, true );
		std::cout << "POLYGON SIZE " << p.size() << std::endl;

#ifdef WITH_HEAD
		std::vector<std::vector<Point2i>> draw { p }; // wraps polygon to be draw in draw contours
		drawContours( base, draw, -1, Scalar( 255, 150, 50 ) );
#endif

		if( p.size() == 4 && !currentContour.empty() ) {
			RotatedRect rotRect = minAreaRect( currentContour );

			// find and send values
			double Xrot = calculateXrot( base.cols, rotRect.center, FovX ); // maybe find new tx formula
			double distToTarget = findDistance( heightOfHex, focalLength, rotRect.size.height );

			// std::cout << "contour area: " << std::to_string(contourArea(currentContour)) << std::endl;
			std::cout << "Distance to target (in): " + std::to_string( distToTarget ) << std::endl;
			std::cout << "Xrot: " << Xrot << std::endl;

#ifdef WITH_HEAD
			putText(
				base, "Distance: " + std::to_string( distToTarget ), Point( 20, 40 ), FONT_HERSHEY_COMPLEX, 1,
				Scalar( 255, 50, 200 ) );
			putText(
				base, "Xrot: " + std::to_string( Xrot ), Point( 20, 90 ), FONT_HERSHEY_COMPLEX, 1,
				Scalar( 255, 50, 200 ) );
#endif

#ifdef WITH_NETWORK
			Net.sendDouble( "Xrot", Xrot );
			Net.sendDouble( "Distance", distToTarget );
#endif
		}
	}

// show final images
#ifdef WITH_HEAD
	imshow( "Normal", base );
// imshow("Thresh", threshed);
#endif
}

//=========================================================================================================

int main() {
	// check if it's possible to get around this
	while( !std::filesystem::exists( "/dev/video0" ) ) { usleep( 500 ); }

#ifdef WITH_NETWORK
	// sets network table default values
	Network Net;
	Net.init();
#endif

	// testing
	//	WebStream Stream;

	camera.open( 0 );
	setCam( camera );

	// display with with camera
	Mat base;
	while( camera.isOpened() ) {
		camera >> base;
		// flip(base, base, 0); //only needed if cam is upside down
		//		Stream.write( base );
		runCamera( base );

		esc = waitKey( 33 );
		if( esc == 27 ) { // TODO: have this properly deconstruct everything when it closes
			camera.release();
			break;
		}
	}
	return 0;
}
