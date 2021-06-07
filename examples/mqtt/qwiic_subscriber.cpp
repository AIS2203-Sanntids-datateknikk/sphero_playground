#include <boost/program_options.hpp>

#include <iostream>
#include <mqtt/callback.h>
#include <mqtt/async_client.h>


namespace
{

const auto TOPIC = "tof_readings";

/**
 * Local callback & listener class for use with the client connection.
 * This is primarily intended to receive messages, but it will also monitor
 * the connection to the broker. If the connection is lost, it will attempt
 * to restore the connection and re-subscribe to the topic.
 */
class callback : public virtual mqtt::callback

{
    // The MQTT client
    mqtt::async_client& cli_;

    // (Re)connection success
    void connected(const std::string& cause) override {
        cli_.subscribe(TOPIC, 1);
    }

    // Callback for when the connection is lost.
    // This will initiate the attempt to manually reconnect.
    void connection_lost(const std::string& cause) override {}

    // Callback for when a message arrives.
    void message_arrived(mqtt::const_message_ptr msg) override {
        std::cout << "Message arrived" << std::endl;
        std::cout << "\ttopic: '" << msg->get_topic() << "'" << std::endl;
        std::cout << "\tpayload: '" << msg->to_string() << "'\n" << std::endl;
    }

    void delivery_complete(mqtt::delivery_token_ptr token) override {}

public:
    explicit callback(mqtt::async_client& cli)
        : cli_(cli){}
};
}

int main(int argc, const char** argv)
{
    namespace po = boost::program_options;

    po::options_description desc("Options");
    desc.add_options()("port", po::value<int>(), "Remote port");
    desc.add_options()("host", po::value<std::string>(), "Remote host");

    if (argc == 1) {
        std::cerr << "Missing program arguments.." << std::endl;
        return -1;
    }

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);

    const auto host = vm["host"].as<std::string>();
    const auto port = vm["port"].as<int>();

    const std::string address("tcp://" + host + ":" + std::to_string(port));
    mqtt::async_client cli(address, "CLIENT_ID");


    auto connOpts = mqtt::connect_options_builder()
                        .keep_alive_interval(std::chrono::milliseconds(20))
                        .clean_session()
                        .finalize();

    callback cb(cli);
    cli.set_callback(cb);

    try {

        cli.connect(connOpts);

        std::this_thread::sleep_for(std::chrono::milliseconds(2500));

        cli.disconnect();
    } catch (const mqtt::exception& exc) {
        std::cerr << "Error: " << exc.what() << " ["
                  << exc.get_reason_code() << "]" << std::endl;
        return 1;
    }

    return 0;
}