#include "WebStream.hpp"
#include <opencv2/videoio.hpp>

WebStream::WebStream(){
    cv::VideoWriter out("./video.avi", cv::VideoWriter::fourcc('M','J','P','G'), 30.0, cv::Size(640, 480));
}

WebStream::~WebStream(){
    out.release();
}

void WebStream::write(cv::Mat base){
    WebStream::out.write(base);
}