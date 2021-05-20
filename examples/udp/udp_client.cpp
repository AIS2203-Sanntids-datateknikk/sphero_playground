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

int main()
{

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

        udp::resolver resolver(io_service);
        udp::resolver::query query(udp::v4(), "localhost", "daytime");
        udp::endpoint receiver_endpoint = *resolver.resolve(query);

        udp::socket socket(io_service);
        socket.open(udp::v4());

        socket.send_to(boost::asio::buffer(buf), receiver_endpoint);

        udp::endpoint sender_endpoint;
        std::array<char, 128> recv_buf{};
        auto len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);
        std::string message(recv_buf.begin(), recv_buf.begin() + len);

        std::cout << "server responded with: " << message  << std::endl;

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}