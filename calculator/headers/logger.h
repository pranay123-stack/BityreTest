#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>

class Logger {
public:
    static void logError(const std::string& errorMessage, const std::string& className , const std::string& functionName );
    static void logInfo(const std::string& infoMessage, const std::string& className , const std::string& functionName );

private:
    static void logLocationInfo(std::ostream& output, const std::string& className, const std::string& functionName);
    static std::string getTimeStamp();
};

#endif // LOGGER_H
