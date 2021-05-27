#include "curl/curl.h" // has to go before opencv headers

#include <opencv2/opencv.hpp>

#include <iostream>
#include <thread>
#include <vector>

using namespace cv;

namespace
{

size_t write_data(char* ptr, size_t size, size_t nmemb, void* userdata)
{
    auto* stream = (std::vector<uchar>*)userdata;
    size_t count = size * nmemb;
    stream->insert(stream->end(), ptr, ptr + count);
    return count;
}

} // namespace

int main(int argc, const char** argv)
{
    cv::CommandLineParser parser(argc, argv, "{host||}");
    const auto host = parser.get<std::string>("host");
    std::string url("http://" + host + "/html/cam_pic.php");

    curl_global_init(CURL_GLOBAL_ALL);

    namedWindow("Image output", WINDOW_AUTOSIZE);

    bool stop = false;
    std::thread t([&] {
        std::vector<uchar> stream;

        CURLcode res;
        CURL* curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); // pass the writefunction
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stream); // pass the stream ptr to the writefunction
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); //the img url

        Mat image;
        while (!stop) {

            res = curl_easy_perform(curl); // start curl

            if (res == 0) {
                image = imdecode(stream, IMREAD_UNCHANGED);
                stream.clear();
                if (image.data) {
                    imshow("Image output", image); // here's your car ;)
                }
            } else {
                break;
            }
        }

        curl_easy_cleanup(curl); // cleanup
    });

    waitKey(0);
    stop = true;
    t.join();

    curl_global_cleanup();

    return 1;
}