
set(generatedSourcesDir "${CMAKE_BINARY_DIR}/generated")
set(thriftCppSourcesDir "${generatedSourcesDir}/thrift")
set(thriftPythonSourcesDir "${CMAKE_CURRENT_SOURCE_DIR}/python")

file(MAKE_DIRECTORY "${generatedSourcesDir}")
file(MAKE_DIRECTORY "${thriftCppSourcesDir}")
file(MAKE_DIRECTORY "${thriftPythonSourcesDir}")

execute_process(
        COMMAND
        "${THRIFT_EXECUTABLE}"
        "--gen" "cpp:no_skeleton"
        "--out" "${thriftCppSourcesDir}"
        "${CMAKE_CURRENT_SOURCE_DIR}/sphero_service.thrift"
)

execute_process(
        COMMAND
        "${THRIFT_EXECUTABLE}"
        "--gen" "py"
        "--out" "${thriftPythonSourcesDir}"
        "${CMAKE_CURRENT_SOURCE_DIR}/sphero_service.thrift"
)

set(thriftGenerated
        "${thriftCppSourcesDir}/SpheroService.h"
        "${thriftCppSourcesDir}/SpheroService.cpp"
        "${thriftCppSourcesDir}/sphero_service_types.h"
        "${thriftCppSourcesDir}/sphero_service_types.cpp"
        )

add_library(sphero-service OBJECT ${thriftGenerated})
target_link_libraries(sphero-service PUBLIC CONAN_PKG::thrift)
list(APPEND generatedFiles "$<TARGET_OBJECTS:sphero-service>")

add_executable(sphero_client_test sphero_client_test.cpp ${generatedFiles})
target_include_directories(sphero_client_test
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${generatedSourcesDir}")
target_link_libraries(sphero_client_test
        PUBLIC
        CONAN_PKG::boost
        CONAN_PKG::thrift)
