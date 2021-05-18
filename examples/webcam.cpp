
#include "detect_face.hpp"

#include <iostream>
#include <thread>

using namespace cv;

int main()
{

    namedWindow("Display Image", WINDOW_AUTOSIZE);

    CascadeClassifier cascade, nestedCascade;
    std::string cascadeName("../cascades/haarcascades/haarcascade_frontalface_alt.xml");
    std::string nestedCascadeName("../cascades/haarcascades/haarcascade_eye_tree_eyeglasses.xml");

    if (!nestedCascade.load(nestedCascadeName))
        std::cerr << "WARNING: Could not load classifier cascade for nested objects" << std::endl;
    if (!cascade.load(cascadeName)) {
        std::cerr << "ERROR: Could not load classifier cascade" << std::endl;
        return -1;
    }

    bool stop = false;

    Mat image;
    VideoCapture capture(0);
    std::thread t([&stop, &capture, &image, &cascade, &nestedCascade] {
        while (!stop && capture.isOpened()) {
            capture >> image;
            detect(image, cascade, nestedCascade);
            imshow("Display Image", image);
        }
    });

    waitKey(0);
    stop = true;

    t.join();

    return 0;
}