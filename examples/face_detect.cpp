#include "detect_face.hpp"

#include <iostream>

using namespace cv;

int main()
{
    Mat frame, image;
    CascadeClassifier cascade, nestedCascade;

    std::string inputName("../images/Lenna.png");

    image = imread(inputName, IMREAD_COLOR);
    if (image.empty()) {
        std::cout << "Could not read " << inputName << std::endl;
        return 1;
    }

    std::string cascadeName("../cascades/haarcascades/haarcascade_frontalface_alt.xml");
    std::string nestedCascadeName("../cascades/haarcascades/haarcascade_eye_tree_eyeglasses.xml");

    if (!nestedCascade.load(nestedCascadeName))
        std::cerr << "WARNING: Could not load classifier cascade for nested objects" << std::endl;
    if (!cascade.load(cascadeName)) {
        std::cerr << "ERROR: Could not load classifier cascade" << std::endl;
        return -1;
    }

    std::cout << "Detecting face in " << inputName << std::endl;
    auto center = detect_face(image, cascade, nestedCascade);
    if (center) {
        std::cout << "Detected face at " << *center << std::endl;
    }
    imshow("result", image);
    waitKey(0);

    return 0;
}
