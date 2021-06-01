#include <thrift/SpheroService.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include <iostream>

#include <boost/program_options.hpp>

using namespace example::thrift;

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;


int main(int argc, const char** argv)
{
    namespace po = boost::program_options;

    po::options_description desc("Options");
    desc.add_options()("port", po::value<int>(), "Remote port");
    desc.add_options()("host", po::value<std::string>(), "Remote host");

    if (argc == 1) {
        return -1;
    }

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);

    const auto host = vm["host"].as<std::string>();
    const auto port = vm["port"].as<int>();

    try {
        std::shared_ptr<TTransport> socket(new TSocket(host, port));
        std::shared_ptr<TFramedTransport> transport(new TFramedTransport(socket));
        std::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
        std::unique_ptr<SpheroServiceClient> client(new SpheroServiceClient(protocol));
        transport->open();

        SensorData data;
        client->sense(data);
        std::cout << data << std::endl;

        transport->close();


    } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }

    return 0;
}