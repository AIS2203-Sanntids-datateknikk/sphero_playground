#include <opencv2/opencv.hpp>

#include <iostream>

using namespace cv;

int main()
{
    Mat image = imread("../images/Lenna.png", 1);
    if (!image.data) {
        std::cout << "No image data" << std::endl;
        return -1;
    }
    namedWindow("Display Image", WINDOW_AUTOSIZE);
    imshow("Display Image", image);
    waitKey(0);
    return 0;
}