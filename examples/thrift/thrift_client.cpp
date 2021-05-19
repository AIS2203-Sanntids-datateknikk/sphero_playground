
#include <opencv2/opencv.hpp>
#include <thrift/ImageService.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include <iostream>

using namespace example::thrift;

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;

using namespace cv;

namespace
{

    cv::Rect convert(const example::thrift::Rect& r)
    {
        return cv::Rect(r.x, r.y, r.width, r.height);
    }

}

int main()
{
    Mat image = imread("../images/Lenna.png", 1);
    if (!image.data) {
        std::cout << "No image data" << std::endl;
        return -1;
    }

    std::vector<uchar> buf;
    cv::imencode(".jpg", image, buf);

    try {
        std::shared_ptr<TTransport> socket(new TSocket("localhost", 9090));
        std::shared_ptr<TFramedTransport> transport(new TFramedTransport(socket));
        std::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
        std::unique_ptr<ImageServiceClient> client(new ImageServiceClient(protocol));
        transport->open();

        example::thrift::FaceDetection detection;
        std::string data(buf.begin(), buf.end());
        client->detect_face(detection, data);
        transport->close();

        if (detection.detected) {
            std::cout << "Detected face: x=" << std::to_string(detection.rect.x) << ", y=" << std::to_string(detection.rect.y) << std::endl;

            namedWindow("Display Image", WINDOW_AUTOSIZE);
            rectangle(image, convert(detection.rect), cv::Scalar(255, 0, 0), 3, 8);
            imshow("Display Image", image);
            waitKey(0);

        }

    } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }

    return 0;
}