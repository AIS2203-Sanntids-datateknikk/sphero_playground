#include <opencv2/opencv.hpp>
#include <thrift/ServoService.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include <iostream>

using namespace example::thrift;

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;

using namespace cv;

int main(int argc, const char** argv)
{
    CommandLineParser parser(argc, argv, "{host||}{port||}");
    const auto host = parser.get<std::string>("host");
    const auto port = parser.get<int>("port");

    std::shared_ptr<TTransport> socket(new TSocket(host, port));
    std::shared_ptr<TFramedTransport> transport(new TFramedTransport(socket));
    std::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    std::unique_ptr<ServoServiceClient> client(new ServoServiceClient(protocol));
    transport->open();

    double pos0 = client->get_servo_position(0, 90);
    double pos1 = client->get_servo_position(1, 90);

    namedWindow("input");

    int input;
    int step = 2;
    while (true) {

        input = waitKey(0);

        if (input == 119) { // w
            pos1 += step;
            client->move_servo_position(1, pos1, 90);
        } else if (input == 115) { // s
            pos1 -= step;
            client->move_servo_position(1, pos1, 90);
        } else if (input == 97) { // a
            pos0 += step;
            client->move_servo_position(0, pos0, 90);
        } else if (input == 100) { // s
            pos0 -= step;
            client->move_servo_position(0, pos0, 90);
        } else if (input == 113 || input == -1) { // q
            break;
        }
        std::cout << "[" << client->get_servo_position(0, 90) << ", " << client->get_servo_position(1, 90) << "]" << std::endl;
    }

    transport->close();

    return 0;
}