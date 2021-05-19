
add_executable(server server.cpp)
target_link_libraries(server CONAN_PKG::opencv CONAN_PKG::boost)

add_executable(client client.cpp)
target_link_libraries(client CONAN_PKG::opencv CONAN_PKG::boost)
