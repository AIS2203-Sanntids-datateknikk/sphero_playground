
add_executable(qwiic_subscriber qwiic_subscriber.cpp)
target_link_libraries(qwiic_subscriber
        PUBLIC
        CONAN_PKG::boost
        CONAN_PKG::nlohmann_json
        CONAN_PKG::paho-mqtt-cpp)