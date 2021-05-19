
#include "ImageServiceHandler.hpp"

#include "../detect_face.hpp"

#include <utility>

using namespace cv;
using namespace example;

ImageServiceHandler::ImageServiceHandler()
{
    std::string cascadeName("../cascades/haarcascades/haarcascade_frontalface_alt.xml");
    if (!cascade_.load(cascadeName)) {
        std::cerr << "WARNING: Could not load classifier cascade for nested objects" << std::endl;
    }
}

void ImageServiceHandler::detect_face(thrift::Point& _return, const std::string& data)
{
    std::vector<uchar> tmp(data.begin(), data.end());
    std::cout << data.size() << std::endl;
    Mat image = imdecode(tmp, 1);
    auto point = example::detect_face(image, cascade_);
    if (point) {
        _return.x = point->x;
        _return.y = point->y;
    } else {
        _return.x = -1;
        _return.y = -1;
    }
}
