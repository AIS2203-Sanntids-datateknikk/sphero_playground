#include "../../curl/curl_webcam_fetch.hpp"

#include <boost/program_options.hpp>
#include <thrift/SpheroService.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include <iostream>
#include <chrono>

using namespace example::thrift;

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;


namespace {

    int new_heading(int oldHeading, int increment)
    {
        int heading = oldHeading + increment;
        if (heading > 359) {
            heading = heading - 359;
        } else if (heading < 0) {
            heading = 359 + heading;
        }
        return heading;
    }

}

int main(int argc, const char** argv)
{
    namespace po = boost::program_options;

    po::options_description desc("Options");
    desc.add_options()("port", po::value<int>(), "Remote port");
    desc.add_options()("host", po::value<std::string>(), "Remote host");

    if (argc == 1) {
        return -1;
    }

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);

    const auto host = vm["host"].as<std::string>();
    const auto port = vm["port"].as<int>();

    try {
        std::shared_ptr<TTransport> socket(new TSocket(host, port));
        std::shared_ptr<TFramedTransport> transport(new TFramedTransport(socket));
        std::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
        std::unique_ptr<SpheroServiceClient> client(new SpheroServiceClient(protocol));
        transport->open();

        SensorData data;
        client->sense(data);
        std::cout << data << std::endl;



        bool stopDriving = false;
        int heading = 0;
        int speed = 0;
        int flags = 0;

        std::thread t([&]{

            int input;
            cv::namedWindow("Input", cv::WINDOW_AUTOSIZE);

            while (true) {

                input = cv::waitKey(0);

                if (input == 119) { // w
                    flags = 0;
                    speed = 32;
                } else if (input == 115) { // s
                    flags = 1;
                    speed = 32;
                } else if (input == 97) { // a
                    heading = new_heading(heading, -10);
                } else if (input == 100) { // s
                    heading = new_heading(heading, 10);
                } else if (input == 32) { //space
                    speed = 0;
                    flags = 0;
                } else if (input == 113 || input == -1) { // q or e
                    break;
                }

               //client->drive_with_heading(speed, heading, flags);

            }

            stopDriving = true;

        });

        std::thread t1([&]{
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            while (!stopDriving) {
                client->drive_with_heading(speed, heading, flags);
            }
        });

        t.join();
        t1.join();

        transport->close();


    } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }

    return 0;
}