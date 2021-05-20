#include "../detect_face.hpp"

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

int main()
{

    CascadeClassifier cascade;
    std::string cascadeName("../cascades/haarcascades/haarcascade_frontalface_alt.xml");
    if (!cascade.load(cascadeName)) {
        std::cerr << "WARNING: Could not load classifier cascade for nested objects" << std::endl;
        return -1;
    }

    try {

        boost::asio::io_service io_service;
        udp::socket socket(io_service, udp::endpoint(udp::v4(), 13));

        udp::endpoint remote_endpoint;
        std::array<char, 65508> recv_buffer{};

        std::function<void()> receive = [&]{
            socket.async_receive_from(boost::asio::buffer(recv_buffer), remote_endpoint, [&](boost::system::error_code, std::size_t len) {
                std::vector<uchar> img(recv_buffer.begin(), recv_buffer.begin() + static_cast<int>(len));

                Mat image = imdecode(img, 1);
                auto detect = example::detect_face(image, cascade);

                std::string message;
                if (!detect) {
                    message = "no detection!";
                } else {
                    message = "detected face at [" + std::to_string(detect->x) + ", " + std::to_string(detect->y) + "]";
                }

                socket.async_send_to(boost::asio::buffer(message), remote_endpoint, [&](boost::system::error_code, std::size_t) {
                    receive();
                });
            });
        };

        receive();

        std::thread t([&] {
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