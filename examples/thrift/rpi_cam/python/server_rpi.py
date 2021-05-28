import io
import time
import argparse

from image_service import CameraService

from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol
from thrift.server import TServer

import picamera


class CameraHandler:

    def __init__(self):
        self.camera = picamera.PiCamera()
        self.camera.start_preview()
        time.sleep(2)

    def fetch_image(self):
        stream = io.BytesIO()
        self.camera.capture(stream, use_video_port=True, format='jpeg')
        stream.seek(0)
        return stream.read()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--port', type=int,
                        help='Remote port')

    args = parser.parse_args()

    try:

        handler = CameraHandler()
        processor = CameraService.Processor(handler)
        transport = TSocket.TServerSocket(port=args.port)
        tfactory = TTransport.TFramedTransportFactory()
        pfactory = TBinaryProtocol.TBinaryProtocolFactory()
        server = TServer.TThreadedServer(processor, transport, tfactory, pfactory)

        server.serve()

    except Exception as e:
        print(f"something went wrong: {e}")


if __name__ == "__main__":
    main()
