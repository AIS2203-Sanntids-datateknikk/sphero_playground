
namespace cpp example.thrift

service ServoService {

    i32 get_pwm_frequency()

    double get_servo_position(1: i32 channel, 2: i32 swing)

    void move_servo_position(1: i32 channel, 2: double pos, 3: i32 swing)

    bool set_pwm_frequency(1: i32 frequency)

    void restart()

}