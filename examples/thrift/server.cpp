
#include "ImageServiceHandler.hpp"

#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TTransportUtils.h>

using namespace example;
using namespace example::thrift;

using namespace ::apache::thrift;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;

int main() {


    std::unique_ptr<TSimpleServer> server;

    std::shared_ptr<ImageServiceHandler> handler(new ImageServiceHandler());
    std::shared_ptr<TProcessor> processor(new ImageServiceProcessor(handler));

    std::shared_ptr<TTransportFactory> transportFactory(new TFramedTransportFactory());
    std::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());


}