
#include <curl_webcam_fetch.hpp>

#include <thread>

using namespace cv;

int main(int argc, const char** argv)
{
    cv::CommandLineParser parser(argc, argv, "{host||}");
    const auto host = parser.get<std::string>("host");

    if (host.empty()) {
        std::cerr << "Please specify the remote host using --host" << std::endl;
        return -1;
    }

    namedWindow("Image output", WINDOW_AUTOSIZE);

    bool stop = false;
    std::thread t([&] {

        Mat image;
        example::curl_webcam_fetcher fetcher(host);
        while (!stop) {

            if (fetcher.fetch(image)) {
                if (image.data) {
                    imshow("Image output", image);
                }
            } else {
                break;
            }

        }

    });

    waitKey(0);
    stop = true;
    t.join();

    return 0;
}