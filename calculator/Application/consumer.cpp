#include <librdkafka/rdkafkacpp.h>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <fstream>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <chrono>
#include "../headers/calculator.h"
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_int.hpp>

constexpr static auto threads = 4U;

namespace mp = boost::multiprecision;

int main()
{

    RdKafka::Topic *topic = nullptr;
    RdKafka::Consumer *consumer = nullptr;
    RdKafka::Conf *conf = nullptr;
    RdKafka::Conf *tconf = nullptr;
    try
    {
        RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
        RdKafka::Conf *tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);

        std::string errstr;

        // Set bootstrap servers (replace localhost:9092 with your Kafka broker's address)
        if (conf->set("metadata.broker.list", "localhost:9092", errstr) != RdKafka::Conf::CONF_OK)
        {
            Logger::logError("Failed to set broker list: " + errstr, "main", "setBrokerList");
            throw std::runtime_error("Failed to set broker list: " + errstr);
        }

        // Create consumer instance
        RdKafka::Consumer *consumer = RdKafka::Consumer::create(conf, errstr);
        if (!consumer)
        {
            Logger::logError("Failed to create consumer: " + errstr, "main", "createConsumer");
            throw std::runtime_error("Failed to create consumer: " + errstr);
        }

        // Subscribe to the topic
        RdKafka::Topic *topic = RdKafka::Topic::create(consumer, "my-topic", tconf, errstr);
        if (!topic)
        {
            Logger::logError("Failed to create topic: " + errstr, "main", "createTopic");
            throw std::runtime_error("Failed to create topic: " + errstr);
        }

        RdKafka::ErrorCode err = consumer->start(topic, 0, RdKafka::Topic::OFFSET_END);

        if (err)
        {
            Logger::logError("Failed to start consumer: " + RdKafka::err2str(err), "main", "startConsumer");
            throw std::runtime_error("Failed to start consumer: " + RdKafka::err2str(err));
        }

        // Open the CSV file for writing
        std::ofstream outputFile("calculator.csv");
        if (!outputFile.is_open())
        {
            Logger::logError("Failed to open the output file.", "main", "fileOpen");
            return 1;
        }

        // Write the header to the CSV file
        outputFile << "Operation,Operand1,Operand2,Result\n";

        // Consume messages continuously
        while (true)
        {
            RdKafka::Message *message = consumer->consume(topic, 0, 1000);

            if (message->err())
            {
                if (message->err() != RdKafka::ERR__TIMED_OUT)
                {
                    Logger::logError("Error while consuming message: " + std::string(message->errstr()), "main", "consumeMessage");
                }
                delete message;
                continue;
            }

            // Extract and parse the message
            std::istringstream messageStream(static_cast<const char *>(message->payload()));
            int operation;
            double operand1, operand2;

            messageStream.ignore(std::numeric_limits<std::streamsize>::max(), ':');
            messageStream >> operation;
            messageStream.ignore(std::numeric_limits<std::streamsize>::max(), ':');
            messageStream >> operand1;
            messageStream.ignore(std::numeric_limits<std::streamsize>::max(), ':');
            messageStream >> operand2;

            // Perform the specified operation using Calculator class
            double result = 0.0;
            boost::multiprecision::cpp_int res = 0;
            switch (operation)
            {
            case 1:
                result = Calculator::add(operand1, operand2);
                std::cout << "Add: " << result << std::endl;
                break;
            case 2:
                result = Calculator::subtract(operand1, operand2);
                std::cout << "Subtract: " << result << std::endl;
                break;
            case 3:
                result = Calculator::multiply(operand1, operand2);
                   std::cout << "Multiply: " << result << std::endl;
                break;
            case 4:
                result = Calculator::divide(operand1, operand2);
                   std::cout << "Divide: " << result << std::endl;
                break;
            case 5:
                // result = Calculator::factorial(static_cast<unsigned long long int>(operand1));
                res = Calculator::factorial(static_cast<unsigned long long int>(operand1));

                std::cout << "Factorial: " << res << std::endl;
                break;
            default:
                Logger::logError("Invalid operation.", "main", "invalidOperation");
                result = 0.0;
            }

            // Print the result
            if (operation == 5) {
                Logger::logInfo("Received message: Operation=" + std::to_string(operation) +
                                ", Operand1=" + std::to_string(operand1) +
                                ", Factorial Result=" + res.str(), "main", "messageReceived");
                outputFile << Calculator::operationToString(operation) << "," << operand1  <<" " << res << "\n";
            } else {
                Logger::logInfo("Received message: Operation=" + std::to_string(operation) +
                                ", Operand1=" + std::to_string(operand1) +
                                ", Operand2=" + std::to_string(operand2) +
                                ", Result=" + std::to_string(result), "main", "messageReceived");
                outputFile << Calculator::operationToString(operation) << "," << operand1 << "," << operand2 << "," << result << "\n";
            }


            outputFile.flush(); // Flush the stream to ensure data is written immediately

            delete message;
        }
    }
    catch (const std::runtime_error &e)
    {
        Logger::logError("Runtime error caught: " + std::string(e.what()), "main", "runtimeErrorCaught");
    }
    catch (const std::exception &e)
    {
        Logger::logError("Exception caught: " + std::string(e.what()), "main", "exceptionCaught");
    }
    catch (...)
    {
        Logger::logError("Unknown exception caught.", "main", "unknownException");
    }

    // Cleanup (this part will never be reached in the loop)
    delete topic;
    consumer->stop(topic, 0);
    delete consumer;
    delete conf;
    delete tconf;

    return 0;
}
