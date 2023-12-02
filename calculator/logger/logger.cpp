#include "../headers/logger.h"

void Logger::logError(const std::string &errorMessage, const std::string &className = "", const std::string &functionName = "")
{
    std::ofstream logFile("logger.txt", std::ios::app); // Open file in append mode
    if (logFile.is_open())
    {
        logFile << getTimeStamp() << " Error";
        logLocationInfo(logFile, className, functionName);
        logFile << ": " << errorMessage << std::endl;
        logFile.close();
    }
    // If unable to open the file, log to standard error
    else
    {
        std::cerr << getTimeStamp() << " Error";
        logLocationInfo(std::cerr, className, functionName);
        std::cerr << ": " << errorMessage << std::endl;
    }
}

void Logger::logInfo(const std::string &infoMessage, const std::string &className = "", const std::string &functionName = "")
{
    std::ofstream logFile("logger.txt", std::ios::app); // Open file in append mode
    if (logFile.is_open())
    {
        logFile << getTimeStamp() << " Info";
        logLocationInfo(logFile, className, functionName);
        logFile << ": " << infoMessage << std::endl;
        logFile.close();
    }
    // If unable to open the file, log to standard output
    else
    {
        std::cout << getTimeStamp() << " Info";
        logLocationInfo(std::cout, className, functionName);
        std::cout << ": " << infoMessage << std::endl;
    }
}

void Logger::logLocationInfo(std::ostream &output, const std::string &className, const std::string &functionName)
{
    if (!className.empty())
    {
        output << " [" << className << "]";
    }

    if (!functionName.empty())
    {
        output << " [" << functionName << "]";
    }
}

std::string Logger::getTimeStamp()
{
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now), "%F %T");
    return ss.str();
}
