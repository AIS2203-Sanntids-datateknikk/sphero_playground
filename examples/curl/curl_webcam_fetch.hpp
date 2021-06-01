
#ifndef OPENCV___CURL_WEBCAM_FETCH_HPP
#define OPENCV___CURL_WEBCAM_FETCH_HPP

#include "curl/curl.h" // has to go before opencv headers

#include <opencv2/opencv.hpp>

#include <string>
#include <vector>

namespace example
{

class curl_webcam_fetcher
{

public:
    explicit curl_webcam_fetcher(const std::string& host);

    bool fetch(cv::Mat& image);

    ~curl_webcam_fetcher();

private:
    CURL* curl_;
    std::vector<uchar> stream_;
};

}

#endif //OPENCV___CURL_WEBCAM_FETCH_HPP
