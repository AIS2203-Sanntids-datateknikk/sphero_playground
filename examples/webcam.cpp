
#include "detect_face.hpp"

#include <iostream>
#include <thread>

using namespace cv;

int main()
{
    VideoCapture capture(0);
    if (capture.isOpened()) {

        Mat image;
        bool stop = false;
        namedWindow("Display Image", WINDOW_AUTOSIZE);

        CascadeClassifier cascade, nestedCascade;
        std::string cascadeName("../cascades/haarcascades/haarcascade_frontalface_alt.xml");
        std::string nestedCascadeName("../cascades/haarcascades/haarcascade_eye_tree_eyeglasses.xml");

        if (!nestedCascade.load(nestedCascadeName))
            std::cerr << "WARNING: Could not load classifier cascade for nested objects" << std::endl;
        if (!cascade.load(cascadeName)) {
            std::cerr << "ERROR: Could not load classifier cascade" << std::endl;
        }

        std::thread t([&stop, &capture, &image, &cascade, &nestedCascade] {
            while (!stop) {
                capture >> image;
                example::detect_face(image, cascade, nestedCascade);
                imshow("Display Image", image);
            }
        });

        waitKey(0);
        stop = true;

        t.join();

    } else {
        std::cerr << "Unable to open camera.." << std::endl;
    }

    return 0;
}