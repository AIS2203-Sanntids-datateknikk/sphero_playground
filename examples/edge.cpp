#include "opencv2/core/utility.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <cstdio>

using namespace cv;
using namespace std;

namespace {

    int edgeThresh = 1;
    int edgeThreshScharr = 1;
    Mat image, gray, blurImage, edge1, edge2, cedge;
    const char *window_name1 = "Edge map : Canny default (Sobel gradient)";
    const char *window_name2 = "Edge map : Canny with custom gradient (Scharr)";

    // define a trackbar callback
    void onTrackbar(int, void *) {
        blur(gray, blurImage, Size(3, 3));
        // Run the edge detector on grayscale
        Canny(blurImage, edge1, edgeThresh, edgeThresh * 3, 3);
        cedge = Scalar::all(0);
        image.copyTo(cedge, edge1);
        imshow(window_name1, cedge);
        Mat dx, dy;
        Scharr(blurImage, dx, CV_16S, 1, 0);
        Scharr(blurImage, dy, CV_16S, 0, 1);
        Canny(dx, dy, edge2, edgeThreshScharr, edgeThreshScharr * 3);
        cedge = Scalar::all(0);
        image.copyTo(cedge, edge2);
        imshow(window_name2, cedge);
    }

}// namespace

int main() {
    string filename = "../images/Lenna.png";
    image = imread(filename, IMREAD_COLOR);
    if (image.empty()) {
        printf("Cannot read image file: %s\n", filename.c_str());
        return -1;
    }
    cedge.create(image.size(), image.type());
    cvtColor(image, gray, COLOR_BGR2GRAY);
    // Create a window
    namedWindow(window_name1, 1);
    namedWindow(window_name2, 1);
    // create a toolbar
    createTrackbar("Canny threshold default", window_name1, &edgeThresh, 100, onTrackbar);
    createTrackbar("Canny threshold Scharr", window_name2, &edgeThreshScharr, 400, onTrackbar);
    // Show the image
    onTrackbar(0, nullptr);
    // Wait for a key stroke; the same function arranges events processing
    waitKey(0);
    return 0;
}