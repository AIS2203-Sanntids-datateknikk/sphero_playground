
from service import ImageService

from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol

import cv2 as cv


def main():

    try:

        transport = TSocket.TSocket('localhost', 9090)
        transport = TTransport.TFramedTransport(transport)
        protocol = TBinaryProtocol.TBinaryProtocol(transport)
        transport.open()

        client = ImageService.Client(protocol)

        img = cv.imread("../../../images/Lenna.png")
        encode = cv.imencode(".jpg", img)[1].tostring()

        detect = client.detect_face(encode)
        if detect.detected:

            rect = detect.rect
            cv.rectangle(img, (rect.x, rect.y), (rect.x+rect.width, rect.y + rect.height), (255, 0, 0), 3, 8)
            cv.imshow('image', img)

            cv.waitKey(0)
        else:
            print("No image detected")
        # Close!
        transport.close()
    except Exception as e:
        print(f"something went wrong: {e}")


if __name__ == "__main__":
    main()
