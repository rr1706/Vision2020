//#include "WebStream.hpp"
//
//
//void WebStream::init(cv::Mat base){
//
//    /* VideoWriter: 'videoconvert' converts the 'BGR' images into 'YUY2' raw frames to be fed to
//    'jpegenc' encoder since 'jpegenc' does not accept 'BGR' images.*/
//    WebStream::out = (base, cv::CAP_GSTREAMER, 0, 30, cv::Size(640,480), true);
//}