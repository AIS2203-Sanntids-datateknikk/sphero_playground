
namespace cpp example.thrift

struct Point {
    1: i32 x,
    2: i32 y
}

service ImageService {

    Point detect_face(1: string data)

}