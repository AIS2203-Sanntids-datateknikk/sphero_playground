
#include "curl_webcam_fetch.hpp" // has to go before opencv headers

namespace
{

size_t write_data(char* ptr, size_t size, size_t nmemb, void* userdata)
{
    auto stream = (std::vector<uchar>*)userdata;
    size_t count = size * nmemb;
    stream->insert(stream->end(), ptr, ptr + count);
    return count;
}

} // namespace

example::curl_webcam_fetcher::curl_webcam_fetcher(const std::string& host)
{
    std::string url("http://" + host + "/html/cam_pic.php");
    curl_ = curl_easy_init();
    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, write_data); // pass the writefunction
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &stream_); // pass the stream ptr to the writefunction
    curl_easy_setopt(curl_, CURLOPT_URL, url.c_str()); //the img url
}

bool example::curl_webcam_fetcher::fetch(cv::Mat& image)
{
    CURLcode res = curl_easy_perform(curl_);
    if (res == 0) {
        cv::imdecode(stream_, cv::IMREAD_UNCHANGED, &image);
        stream_.clear();
        return true;
    } else {
        stream_.clear();
        return false;
    }
}

example::curl_webcam_fetcher::~curl_webcam_fetcher()
{
    curl_easy_cleanup(curl_);
}
