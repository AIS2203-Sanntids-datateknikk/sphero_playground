
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

void ImageServiceHandler::detect_face(thrift::FaceDetection& _return, const std::string& data)
{
    std::vector<uchar> tmp(data.begin(), data.end());
    std::cout << data.size() << std::endl;
    Mat image = imdecode(tmp, 1);
    auto detect = example::detect_face(image, cascade_);
    if (detect) {
        _return.detected = true;
        _return.rect.x = detect->x;
        _return.rect.y = detect->y;
        _return.rect.width = detect->width;
        _return.rect.height = detect->height;
        _return.__set_rect(_return.rect);

    } else {
        _return.detected = false;
    }
}
