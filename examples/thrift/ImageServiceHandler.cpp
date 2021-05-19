
#include "ImageServiceHandler.hpp"

#include <utility>

#include "../detect_face.hpp"

using namespace cv;
using namespace example;

ImageServiceHandler::ImageServiceHandler() {
    std::string cascadeName("../cascades/haarcascades/haarcascade_frontalface_alt.xml");
    std::string nestedCascadeName("../cascades/haarcascades/haarcascade_eye_tree_eyeglasses.xml");

    if (!nestedCascade_.load(nestedCascadeName))
        std::cerr << "WARNING: Could not load classifier cascade for nested objects" << std::endl;
    if (!cascade_.load(cascadeName)) {
        std::cerr << "ERROR: Could not load classifier cascade" << std::endl;
    }
}

void ImageServiceHandler::detect_face(thrift::Point& _return, const std::string& data)
{
    std::vector<uchar> tmp(data.begin(), data.begin());
    Mat image = imdecode(tmp, 1);
    auto point = example::detect_face(image, cascade_, nestedCascade_);
    if (point) {
        _return.x = point->x;
        _return.y = point->y;
    } else {
        _return.x = -1;
        _return.y = -1;
    }
}
