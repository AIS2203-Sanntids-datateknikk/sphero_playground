
namespace cpp example.thrift

struct Rect {
    1: i32 x,
    2: i32 y,
    3: i32 width,
    4: i32 height
}

struct FaceDetection {
    1: bool detected,
    2: optional Rect rect,
}

service ImageService {

    FaceDetection detect_face(1: string data)

}