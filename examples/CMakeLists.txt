add_executable(display_image display_image.cpp)
target_link_libraries(display_image CONAN_PKG::opencv)

add_executable(edge edge.cpp)
target_link_libraries(edge CONAN_PKG::opencv)
