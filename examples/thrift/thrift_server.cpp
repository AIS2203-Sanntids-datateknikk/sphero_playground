
#include "ImageServiceHandler.hpp"

#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include <iostream>
#include <thread>

using namespace example;
using namespace example::thrift;

using namespace ::apache::thrift;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;

int main()
{
    const int port = 9090;

    std::shared_ptr<ImageServiceHandler> handler(new ImageServiceHandler());
    std::shared_ptr<TProcessor> processor(new ImageServiceProcessor(handler));

    std::shared_ptr<TTransportFactory> transportFactory(new TFramedTransportFactory());
    std::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
    std::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));

    TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);

    std::thread t([&] {
        server.serve();
    });

    std::cout << "Press a key to continue..." << std::endl;
    while (std::cin.get() != '\n') { };

    server.stop();
    t.join();

    return 0;
}