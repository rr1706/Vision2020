//basically save a file then push file to network
#pragma once

#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>

class WebStream{
    public:
        void write(cv::Mat base);
        WebStream();
        ~WebStream();
    
    private:
       cv::VideoWriter out;
}; 