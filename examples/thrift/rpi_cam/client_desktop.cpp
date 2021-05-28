
#include <opencv2/opencv.hpp>
#include <thrift/CameraService.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include <../../detect_face.hpp>
#include <iostream>
#include <thread>
#include <vector>

using namespace cv;

using namespace example::thrift;

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;


int main(int argc, const char** argv)
{
    CommandLineParser parser(argc, argv, "{host||}{port||}");
    const auto host = parser.get<std::string>("host");
    const auto port = parser.get<int>("port");

    CascadeClassifier cascade;
    std::string cascadeName("../cascades/haarcascades/haarcascade_frontalface_alt.xml");
    if (!cascade.load(cascadeName)) {
        std::cerr << "WARNING: Could not load classifier cascade for nested objects" << std::endl;
        return -1;
    }

    try {

        std::shared_ptr<TTransport> socket(new TSocket(host, port));
        std::shared_ptr<TFramedTransport> transport(new TFramedTransport(socket));
        std::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
        std::unique_ptr<CameraServiceClient> client(new CameraServiceClient(protocol));
        transport->open();

        bool stop = false;
        namedWindow("Display Image", WINDOW_AUTOSIZE);
        std::thread t([&] {
            std::string binary;
            while (!stop) {
                client->fetch_image(binary);

                std::vector<uchar> buf(binary.begin(), binary.end());

                Mat image = imdecode(buf, 1);
                example::detect_face(image, cascade);
                imshow("Display Image", image);

                binary.clear();
            }
        });

        waitKey(0);
        stop = true;
        t.join();
        transport->close();

    } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }

    return 0;
}