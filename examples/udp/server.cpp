#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <opencv2/opencv.hpp>

#include <array>
#include <ctime>
#include <iostream>
#include <memory>
#include <thread>

using namespace cv;
using boost::asio::ip::udp;

namespace
{
class udp_server
{
public:
    explicit udp_server(boost::asio::io_service& io_service)
        : socket_(io_service, udp::endpoint(udp::v4(), 13))
    , capture_(0)
    {
        start_receive();
    }

private:
    void start_receive()
    {
        socket_.async_receive_from(
            boost::asio::buffer(recv_buffer_), remote_endpoint_,
            boost::bind(&udp_server::handle_receive, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }

    void handle_receive(const boost::system::error_code& error,
        std::size_t /*bytes_transferred*/)
    {
        if (!error || error == boost::asio::error::message_size) {
            Mat image;
            capture_ >> image;
            cv::resize(image, image, cv::Size(), 0.5, 0.5);
            std::vector<uchar> buf;
            cv::imencode(".jpg", image, buf);
            if (buf.size() > 65508) {
                std::cerr << "image size to large: " << std::to_string(buf.size()) << std::endl;
            } else {
                socket_.async_send_to(boost::asio::buffer(buf), remote_endpoint_,
                    boost::bind(&udp_server::handle_send, this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
            }

            start_receive();
        }
    }


    void handle_send(
        const boost::system::error_code& /*error*/,
        std::size_t bytes_transferred)
    {
        //std::cout << "sending data:" << std::to_string(bytes_transferred) << std::endl;
    }

    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    std::array<char, 1> recv_buffer_;
    VideoCapture capture_;
};
} // namespace

int main()
{
    try {

        boost::asio::io_service io_service;
        udp_server server(io_service);
        std::thread t([&io_service] {
            io_service.run();
        });

        std::cout << "Press a key to continue..." << std::endl;
        while (std::cin.get() != '\n') { };

        io_service.stop();

        t.join();

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}