#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include "../headers/logger.h"
#include <boost/multiprecision/cpp_int.hpp>

class Calculator {
public:
    static double add(double operand1, double operand2);
    static double subtract(double operand1, double operand2);
    static double multiply(double operand1, double operand2);
    static double divide(double operand1, double operand2);
    // static int  factorial(int n);
    static boost::multiprecision::cpp_int factorial(unsigned long long int num);
    static std::string operationToString(int operation);



};

#endif // CALCULATOR_H
