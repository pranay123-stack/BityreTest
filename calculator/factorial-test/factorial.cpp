#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>
#include <thread>
#include <mutex>
#include <atomic>

#define THREAD_COUNT 4

std::atomic<int> thread_num(1); // global variable
std::mutex mu;                   // mutex for protecting shared resource

void threaded_factorial(unsigned long long int num, boost::multiprecision::cpp_int& bigInt)
{
    int threadid = thread_num++; // thread_num is atomic, so this is safe
    boost::multiprecision::cpp_int N = 1;
    for (unsigned long long int i = threadid; i <= num; i = i + THREAD_COUNT)
    {
        N *= (i);
    }

    std::lock_guard<std::mutex> lock(mu); // race condition --> mutex needed
    bigInt *= N;
}

boost::multiprecision::cpp_int fact(unsigned long long int num)
{
    boost::multiprecision::cpp_int result = 1;
    std::thread workers[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; ++i)
    {
        workers[i] = std::thread(threaded_factorial, num, std::ref(result));
    }

    for (int i = 0; i < THREAD_COUNT; ++i)
    {
        workers[i].join();
    }

    return result;
}

int main()
{
    unsigned long long int num;

    std::cout << "Enter a number to calculate its factorial: ";
    std::cin >> num;

    boost::multiprecision::cpp_int result = fact(num);

    std::cout << "Factorial of " << num << " is: " << result << std::endl;

    return 0;
}
