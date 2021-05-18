#include <boost/asio.hpp>
#include <opencv2/opencv.hpp>

#include <array>
#include <iostream>

using boost::asio::ip::udp;
using namespace cv;

int main()
{
    try {
        namedWindow("Display Image", WINDOW_AUTOSIZE);

        bool stop = false;
        std::thread t([&stop] {
            Mat image;

            boost::asio::io_service io_service;

            udp::resolver resolver(io_service);
            udp::resolver::query query(udp::v4(), "localhost", "daytime");
            udp::endpoint receiver_endpoint = *resolver.resolve(query);

            udp::socket socket(io_service);
            socket.open(udp::v4());

            std::array<char, 65535> recv_buf{};
            std::array<char, 1> send_buf = {{0}};

            while (!stop) {

                socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

                udp::endpoint sender_endpoint;
                size_t len = socket.receive_from(
                    boost::asio::buffer(recv_buf), sender_endpoint);

                std::vector<uchar> img(recv_buf.begin(), recv_buf.begin() + len);

                image = imdecode(img, 1);
                imshow("Display Image", image);
            }

        });

        waitKey(0);
        stop = true;
        t.join();

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}