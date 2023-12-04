#include <gtest/gtest.h>
#include "../headers/calculator.h"  
#include <cmath>
TEST(CalculatorTest, Add) {
    EXPECT_EQ(Calculator::add(7, 9), 16);
    EXPECT_DOUBLE_EQ(Calculator::add(2.0, 3.0), 5.0);
    EXPECT_DOUBLE_EQ(Calculator::add(-1.0, 1.0), 0.0);

 
  
}

TEST(CalculatorTest, Subtract) {
    EXPECT_EQ(Calculator::subtract(7, 9), -2);  // Basic subtraction
    EXPECT_DOUBLE_EQ(Calculator::subtract(5.0, 3.0), 2.0);  // Basic subtraction with floating-point numbers
    EXPECT_EQ(Calculator::subtract(0, 0), 0);  // Subtracting zero from zero
    EXPECT_EQ(Calculator::subtract(-5, 3), -8);  // Subtracting a positive number from a negative number
    EXPECT_DOUBLE_EQ(Calculator::subtract(0.1, 0.2), -0.1);  // Subtracting a smaller floating-point number from a larger one
  
   
}

TEST(CalculatorTest, Multiply) {
    EXPECT_EQ(Calculator::multiply(6, 5), 30);
    EXPECT_DOUBLE_EQ(Calculator::multiply(2.0, 3.0), 6.0);
    EXPECT_DOUBLE_EQ(Calculator::multiply(-1.0, 1.0), -1.0);

   
}

TEST(CalculatorTest, Divide) {
   
    EXPECT_EQ(Calculator::divide(9, 3), 3);
    EXPECT_DOUBLE_EQ(Calculator::divide(6.0, 3.0), 2.0);
    EXPECT_DOUBLE_EQ(Calculator::divide(1.0, 0.0), 0.0);  // Division by zero should return 0.0
   
}


TEST(CalculatorTest, Factorial) {
    EXPECT_THROW(Calculator::factorial(-1), std::invalid_argument);  // Test factorial of a negative number
    EXPECT_EQ(Calculator::factorial(0), boost::multiprecision::cpp_int(1));
    EXPECT_EQ(Calculator::factorial(5), boost::multiprecision::cpp_int(120));
  
}


    




int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
