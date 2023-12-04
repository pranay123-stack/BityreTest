#include <librdkafka/rdkafkacpp.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>
#include <iomanip>
#include <fstream>
#include "../headers/logger.h"

int main() {
    // Set up configuration
    RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    RdKafka::Topic *topic = nullptr;
    std::string errstr;

    try {
        // Set bootstrap servers (replace localhost:9092 with your Kafka broker's address)
        if (conf->set("metadata.broker.list", "localhost:9092", errstr) != RdKafka::Conf::CONF_OK) {
            Logger::logError("Failed to set broker list: " + errstr, "producer[main function]", "setBrokerList");
            throw std::runtime_error("Failed to set broker list: " + errstr);
        }

        // Create producer instance
        RdKafka::Producer *producer = RdKafka::Producer::create(conf, errstr);
        if (!producer) {
            Logger::logError("Failed to create producer: " + errstr, "producer[main function]", "createProducer");
            throw std::runtime_error("Failed to create producer: " + errstr);
        }

        // Create topic instance
        RdKafka::Topic *topic = RdKafka::Topic::create(producer, "my-topic", nullptr, errstr);
        if (!topic) {
            Logger::logError("Failed to create topic: " + errstr, "producer[main function]", "createTopic");
            delete producer;
            delete conf;
            throw std::runtime_error("Failed to create topic: " + errstr);
        }

        // Produce messages continuously
        while (true) {
            std::cout << "Choose an operation:\n"
                      << "1. Add\n"
                      << "2. Subtract\n"
                      << "3. Multiply\n"
                      << "4. Divide\n"
                      << "5. Factorial\n"
                      << "Enter the option (1-5): ";

            int choice;
            std::cin >> choice;

            std::ostringstream message;
            message << "Operation: " << choice;

            if (choice != 5) {
                double operand1, operand2;
                std::cout << "Enter two numbers: ";
                std::cin >> operand1 >> operand2;
                message << ", Operand1: " << operand1 << ", Operand2: " << operand2;
            } else {
                double operand1;
                std::cout << "Enter a number: ";
                std::cin >> operand1;
                message << ", Operand1: " << operand1;
            }

            RdKafka::ErrorCode resp = producer->produce(topic, RdKafka::Topic::PARTITION_UA,
                                                        RdKafka::Producer::RK_MSG_COPY,
                                                        const_cast<char*>(message.str().c_str()),
                                                        message.str().length(),
                                                        nullptr, 0, nullptr);

            if (resp != RdKafka::ERR_NO_ERROR) {
                Logger::logError("Failed to produce message: " + RdKafka::err2str(resp), "producer[main function]", "produceMessage");
            } else {
                Logger::logInfo("Produced message: " + message.str(), "producer[main function]", "produceMessage");
            }

            // Wait for a short interval before producing the next message
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    } catch (const std::runtime_error& e) {
        Logger::logError("Runtime error caught: " + std::string(e.what()), "producer[main function]", "runtimeErrorCaught");
    } catch (const std::exception& e) {
        Logger::logError("Exception caught: " + std::string(e.what()), "producer[main function]", "exceptionCaught");
    } catch (...) {
        Logger::logError("Unknown exception caught.", "producer[main function]", "unknownException");
    }

    // Cleanup (this part will never be reached in the loop)
    delete topic;
    delete conf;

    return 0;
}





