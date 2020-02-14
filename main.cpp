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
