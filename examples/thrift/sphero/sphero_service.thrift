
namespace cpp example.thrift

struct IMU {
    1: bool is_valid
    2: double pitch
    3: double roll
    4: double yaw
}

struct Accelerometer {
    1: bool is_valid
    2: double x
    3: double y
    4: double z
}

struct AmbientLight {
    1: bool is_valid
    2: double value
}

struct Color {
    1: bool is_valid
    2: i32 r
    3: i32 g
    4: i32 b
    5: i32 index
    6: double confidence
}

struct SensorData {
    1: IMU imu
    2: Accelerometer accelerometer
    3: AmbientLight light
    5: Color color
}


service SpheroService {

    SensorData sense()

    void drive_with_heading(1: i32 speed, 2: i32 heading, 3: i32 flags)

}