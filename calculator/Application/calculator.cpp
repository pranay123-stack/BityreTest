#include "../headers/calculator.h"
#include <array>
#include <future>
#include <boost/multiprecision/cpp_int.hpp>
#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>
#include <thread>
#include <mutex>
#include <atomic>

#define THREAD_COUNT 4


std::atomic<int> thread_num(1); // global variable
std::mutex mu;   


std::string Calculator::operationToString(int operation)
{
    try
    {
        switch (operation)
        {
        case 1:
            return "add";
        case 2:
            return "subtract";
        case 3:
            return "multiply";
        case 4:
            return "divide";
        case 5:
            return "factorial";
        default:
            Logger::logError("Invalid operation in operationToString.", "Calculator", "operationToString");
            return "unknown";
        }
    }
    catch (...)
    {
        Logger::logError("Unknown error in operationToString.", "Calculator", "operationToString");
        throw std::runtime_error("Error in operationToString: Unknown error");
    }
}

double Calculator::add(double operand1, double operand2)
{
    try
    {
        double result = operand1 + operand2;
        Logger::logInfo("Addition result: Operand1=" + std::to_string(operand1) +
                            ", Operand2=" + std::to_string(operand2) +
                            ", Result=" + std::to_string(result),
                        "Calculator", "add");
        return result;
    }
    catch (...)
    {
        Logger::logError("Unknown error in add.", "Calculator", "add");
        throw std::runtime_error("Error in add: Unknown error");
    }
}

double Calculator::subtract(double operand1, double operand2)
{
    try
    {
        double result = operand1 - operand2;
        Logger::logInfo("Subtraction result: Operand1=" + std::to_string(operand1) +
                            ", Operand2=" + std::to_string(operand2) +
                            ", Result=" + std::to_string(result),
                        "Calculator", "subtract");
        return result;
    }
    catch (...)
    {
        Logger::logError("Unknown error in subtract.", "Calculator", "subtract");
        throw std::runtime_error("Error in subtract: Unknown error");
    }
}

double Calculator::multiply(double operand1, double operand2)
{
    try
    {
        double result = operand1 * operand2;
        Logger::logInfo("Multiplication result: Operand1=" + std::to_string(operand1) +
                            ", Operand2=" + std::to_string(operand2) +
                            ", Result=" + std::to_string(result),
                        "Calculator", "multiply");
        return result;
    }
    catch (...)
    {
        Logger::logError("Unknown error in multiply.", "Calculator", "multiply");
        throw std::runtime_error("Error in multiply: Unknown error");
    }
}

double Calculator::divide(double operand1, double operand2)
{
    try
    {
        if (operand2 != 0)
        {
            double result = operand1 / operand2;
            Logger::logInfo("Division result: Operand1=" + std::to_string(operand1) +
                                ", Operand2=" + std::to_string(operand2) +
                                ", Result=" + std::to_string(result),
                            "Calculator", "divide");
            return result;
        }
        else
        {
            Logger::logError("Error in divide: Division by zero. Operand1=" + std::to_string(operand1), "Calculator", "divide");
            return 0.0;
        }
    }
    catch (...)
    {
        Logger::logError("Unknown error in divide.", "Calculator", "divide");
        throw std::runtime_error("Error in divide: Unknown error");
    }
}

// int Calculator::factorial(int n) {
// //    int result[1000] = {0};
// //    if (n >= 0) {
// //       result[0] = 1;
// //       for (int i = 1; i <= n; ++i) {
// //          result[i] = i * result[i - 1];
// //       }
// //       return result[n];
// //    }
   


// }


void threaded_factorial(unsigned long long int start, unsigned long long int end, boost::multiprecision::cpp_int& bigInt)
{
    boost::multiprecision::cpp_int N = 1;
    for (unsigned long long int i = start; i <= end; ++i)
    {
        N *= i;
    }

    std::lock_guard<std::mutex> lock(mu);
    bigInt *= N;
}

boost::multiprecision::cpp_int Calculator::factorial(unsigned long long int num)
{
    if (num == 0) {
        return 1;
    }

    boost::multiprecision::cpp_int result = 1;
    std::thread workers[THREAD_COUNT];

    // Calculate the range for each thread
    unsigned long long int range = num / THREAD_COUNT;

    for (int i = 0; i < THREAD_COUNT; ++i)
    {
        unsigned long long int start = i * range + 1;
        unsigned long long int end = (i == THREAD_COUNT - 1) ? num : (i + 1) * range;

        workers[i] = std::thread(threaded_factorial, start, end, std::ref(result));
    }

    for (int i = 0; i < THREAD_COUNT; ++i)
    {
        workers[i].join();
    }

    return result;
}
