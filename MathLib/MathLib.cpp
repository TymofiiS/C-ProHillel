#include "MathLib.h"
#include "pch.h"

namespace MathLib {

    // Basic arithmetic operations
    double add(double a, double b) { return a + b; }
    double subtract(double a, double b) { return a - b; }
    double multiply(double a, double b) { return a * b; }
    double divide(double a, double b) {
        if (b == 0) throw std::invalid_argument("Division by zero!");
        return a / b;
    }

    // Mathematical functions
    double power(double base, int exponent) { return std::pow(base, exponent); }
    double squareRoot(double value) {
        if (value < 0) throw std::invalid_argument("Square root of a negative number!");
        return std::sqrt(value);
    }
    double absoluteValue(double value) { return std::fabs(value); }

    // Integer functions
    int factorial(int n) {
        if (n < 0) throw std::invalid_argument("Factorial of a negative number!");
        int result = 1;
        for (int i = 1; i <= n; i++) result *= i;
        return result;
    }

    bool isPrime(int number) {
        if (number < 2) return false;
        for (int i = 2; i <= std::sqrt(number); i++) {
            if (number % i == 0) return false;
        }
        return true;
    }

    int gcd(int a, int b) {
        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        return std::abs(a);
    }

    // Array operations
    double findMax(const double arr[], int size) {
        if (size <= 0) throw std::invalid_argument("Empty array!");
        double maxVal = arr[0];
        for (int i = 1; i < size; i++)
            if (arr[i] > maxVal) maxVal = arr[i];
        return maxVal;
    }

    double findMin(const double arr[], int size) {
        if (size <= 0) throw std::invalid_argument("Empty array!");
        double minVal = arr[0];
        for (int i = 1; i < size; i++)
            if (arr[i] < minVal) minVal = arr[i];
        return minVal;
    }

    double calculateSum(const double arr[], int size) {
        double sum = 0;
        for (int i = 0; i < size; i++) sum += arr[i];
        return sum;
    }

    double calculateAverage(const double arr[], int size) {
        if (size <= 0) throw std::invalid_argument("Empty array!");
        return calculateSum(arr, size) / size;
    }

    void sortArray(double arr[], int size) {
        std::sort(arr, arr + size);
    }

    // Utility functions
    bool isEven(int number) { return number % 2 == 0; }
    bool isOdd(int number) { return number % 2 != 0; }

    int fibonacci(int n) {
        if (n < 0) throw std::invalid_argument("Invalid index for Fibonacci!");
        if (n == 0) return 0;
        if (n == 1) return 1;
        int a = 0, b = 1, c;
        for (int i = 2; i <= n; i++) {
            c = a + b;
            a = b;
            b = c;
        }
        return b;
    }

} // namespace MathLib
