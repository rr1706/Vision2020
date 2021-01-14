// basically save a file then push file to network
#pragma once

#include "opencv2/opencv.hpp"

class Webstream {
	public:
	Webstream();
	~Webstream();

    void sendFrame(Mat frame);
	void bindSocket( const char *address, int port );

	private:
	int sock;
};
