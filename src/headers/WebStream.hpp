#pragma once

#include <opencv4/opencv2/core.hpp>

class Webstream {
	public:
	Webstream();
	~Webstream();

    void send(cv::Mat frame);
	void bindSocket( const char *address, int port );
};
