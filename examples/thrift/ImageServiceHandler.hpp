
#ifndef OPENCV___IMAGESERVICEHANDLER_HPP
#define OPENCV___IMAGESERVICEHANDLER_HPP

#include <opencv2/objdetect.hpp>
#include <thrift/ImageService.h>

namespace example
{

class ImageServiceHandler : virtual public thrift::ImageServiceIf
{
public:
    ImageServiceHandler();
    void detect_face(thrift::Point& _return, const std::string& data) override;

private:
    cv::CascadeClassifier cascade_;
};

} // namespace example


#endif //OPENCV___IMAGESERVICEHANDLER_HPP
