
add_executable(server udp_server.cpp)
target_link_libraries(server CONAN_PKG::opencv CONAN_PKG::boost)

add_executable(client udp_client.cpp)
target_link_libraries(client CONAN_PKG::opencv CONAN_PKG::boost)
