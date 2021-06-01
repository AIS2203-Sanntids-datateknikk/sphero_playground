
add_library(curl_fetch curl_webcam_fetch.cpp)
target_include_directories(curl_fetch PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(curl_fetch
        PUBLIC
        CONAN_PKG::opencv
        CONAN_PKG::libcurl)

add_executable(curl_fetch_demo curl_fetch_demo.cpp)
target_link_libraries(curl_fetch_demo curl_fetch)
