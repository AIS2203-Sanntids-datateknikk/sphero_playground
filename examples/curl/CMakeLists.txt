add_executable(curl_fetch curl_fetch.cpp)
target_link_libraries(curl_fetch CONAN_PKG::opencv CONAN_PKG::libcurl)
