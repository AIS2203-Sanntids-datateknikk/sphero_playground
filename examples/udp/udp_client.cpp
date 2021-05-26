#include <boost/asio.hpp>
#include <opencv2/opencv.hpp>

#include <array>
#include <iostream>

using boost::asio::ip::udp;
using namespace cv;

namespace
{
    const int MAX_UDP_PACKET_SIZE = 65508;
}

int main(int argc, const char** argv)
{
    cv::CommandLineParser parser(argc, argv,"{host||}{port||}");

    const auto port = parser.get<int>("port");
    const auto host = parser.get<std::string>("host");

    Mat image = imread("../images/Lenna.png", 1);
    if (!image.data) {
        std::cout << "No image data" << std::endl;
        return -1;
    }

    cv::resize(image, image, cv::Size(), 0.5, 0.5);
    std::vector<uchar> buf;
    cv::imencode(".jpg", image, buf);

    if (buf.size() > MAX_UDP_PACKET_SIZE)
    {
        std::cerr << "Image to large to fit in a single UDP packet. Was: " << std::to_string(buf.size()) << std::endl;
        return -1;
    }

    try {

        boost::asio::io_service io_service;
        udp::socket socket(io_service);
        socket.open(udp::v4());

        udp::endpoint receiver_endpoint(boost::asio::ip::address::from_string(host), port);
        socket.send_to(boost::asio::buffer(buf), receiver_endpoint);

        udp::endpoint sender_endpoint;
        std::array<char, 128> recv_buf{};
        auto len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);
        std::string message(recv_buf.begin(), recv_buf.begin() + static_cast<int>(len));

        std::cout << "Server responded with: " << message  << std::endl;

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}