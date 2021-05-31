
find_program(THRIFT_EXECUTABLE NAMES thrift)
if (NOT THRIFT_EXECUTABLE)
    message(FATAL_ERROR "The thrift compiler was not found. Cannot generate Thrift sources.")
endif ()

add_subdirectory(servos)
add_subdirectory(rpi_cam)
