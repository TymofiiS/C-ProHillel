#include <iostream>
#include <vector>
#include <limits>
#include "MathLib.h"  // Include our MathLib header

using namespace MathLib;

// Utility function: clear input buffer if invalid input
void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Menu display
void showMenu() {
    std::cout << "\n==== Math Calculator ====\n";
    std::cout << "1. Basic operations (+, -, *, /)\n";
    std::cout << "2. Mathematical functions (power, sqrt, abs)\n";
    std::cout << "3. Array operations (min, max, sum, avg, sort)\n";
    std::cout << "4. Integer functions (factorial, prime, gcd, fibonacci)\n";
    std::cout << "5. Demonstrate all functions (sample data)\n";
    std::cout << "6. Exit\n";
    std::cout << "Choose an option: ";
}

// Option 1: Basic operations
void basicOperations() {
    double a, b;
    std::cout << "Enter two numbers: ";
    if (!(std::cin >> a >> b)) {
        clearInput();
        std::cout << "Invalid input!\n";
        return;
    }
    std::cout << "Addition: " << add(a, b) << "\n";
    std::cout << "Subtraction: " << subtract(a, b) << "\n";
    std::cout << "Multiplication: " << multiply(a, b) << "\n";
    try {
        std::cout << "Division: " << divide(a, b) << "\n";
    }
    catch (const std::exception& e) {
        std::cout << "Division error: " << e.what() << "\n";
    }
}

// Option 2: Mathematical functions
void mathFunctions() {
    double base, value;
    int exponent;

    std::cout << "Enter base and exponent for power: ";
    if (!(std::cin >> base >> exponent)) {
        clearInput();
        std::cout << "Invalid input!\n";
        return;
    }
    std::cout << "Result: " << power(base, exponent) << "\n";

    std::cout << "Enter value for square root: ";
    if (!(std::cin >> value)) {
        clearInput();
        std::cout << "Invalid input!\n";
        return;
    }
    try {
        std::cout << "Square root: " << squareRoot(value) << "\n";
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }

    std::cout << "Enter value for absolute: ";
    if (!(std::cin >> value)) {
        clearInput();
        std::cout << "Invalid input!\n";
        return;
    }
    std::cout << "Absolute: " << absoluteValue(value) << "\n";
}

// Option 3: Array operations
void arrayOperations() {
    int size;
    std::cout << "Enter array size: ";
    if (!(std::cin >> size) || size <= 0) {
        clearInput();
        std::cout << "Invalid size!\n";
        return;
    }

    std::vector<double> arr(size);
    std::cout << "Enter " << size << " numbers:\n";
    for (int i = 0; i < size; i++) {
        if (!(std::cin >> arr[i])) {
            clearInput();
            std::cout << "Invalid input!\n";
            return;
        }
    }

    std::cout << "Max: " << findMax(arr.data(), size) << "\n";
    std::cout << "Min: " << findMin(arr.data(), size) << "\n";
    std::cout << "Sum: " << calculateSum(arr.data(), size) << "\n";
    std::cout << "Average: " << calculateAverage(arr.data(), size) << "\n";

    sortArray(arr.data(), size);
    std::cout << "Sorted: ";
    for (double val : arr) std::cout << val << " ";
    std::cout << "\n";
}

// Option 4: Integer functions
void integerFunctions() {
    int n, a, b;

    std::cout << "Enter number for factorial: ";
    if (!(std::cin >> n)) { clearInput(); std::cout << "Invalid input!\n"; return; }
    try { std::cout << "Factorial: " << factorial(n) << "\n"; }
    catch (const std::exception& e) { std::cout << "Error: " << e.what() << "\n"; }

    std::cout << "Enter number for prime check: ";
    if (!(std::cin >> n)) { clearInput(); std::cout << "Invalid input!\n"; return; }
    std::cout << (isPrime(n) ? "Prime\n" : "Not prime\n");

    std::cout << "Enter two numbers for GCD: ";
    if (!(std::cin >> a >> b)) { clearInput(); std::cout << "Invalid input!\n"; return; }
    std::cout << "GCD: " << gcd(a, b) << "\n";

    std::cout << "Enter n for Fibonacci: ";
    if (!(std::cin >> n)) { clearInput(); std::cout << "Invalid input!\n"; return; }
    std::cout << "Fibonacci(" << n << "): " << fibonacci(n) << "\n";
}

// Option 5: Demonstrate all functions with sample data
void demoAll() {
    std::cout << "\n=== Demo All Functions ===\n";
    std::cout << "add(5, 3): " << add(5, 3) << "\n";
    std::cout << "power(2, 4): " << power(2, 4) << "\n";
    std::cout << "squareRoot(16): " << squareRoot(16) << "\n";
    std::cout << "absoluteValue(-9.5): " << absoluteValue(-9.5) << "\n";
    std::cout << "factorial(5): " << factorial(5) << "\n";
    std::cout << "isPrime(7): " << (isPrime(7) ? "Yes" : "No") << "\n";
    std::cout << "gcd(48, 18): " << gcd(48, 18) << "\n";
    std::cout << "fibonacci(10): " << fibonacci(10) << "\n";

    double arr[] = { 3.1, 2.4, 7.8, 1.2 };
    int size = 4;
    std::cout << "Array Max: " << findMax(arr, size) << "\n";
    std::cout << "Array Min: " << findMin(arr, size) << "\n";
    std::cout << "Array Sum: " << calculateSum(arr, size) << "\n";
    std::cout << "Array Average: " << calculateAverage(arr, size) << "\n";
    sortArray(arr, size);
    std::cout << "Sorted Array: ";
    for (double v : arr) std::cout << v << " ";
    std::cout << "\n";
}

int main() {
    int choice;
    do {
        showMenu();
        if (!(std::cin >> choice)) {
            clearInput();
            std::cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        switch (choice) {
        case 1: basicOperations(); break;
        case 2: mathFunctions(); break;
        case 3: arrayOperations(); break;
        case 4: integerFunctions(); break;
        case 5: demoAll(); break;
        case 6: std::cout << "Exiting program...\n"; break;
        default: std::cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 6);

    return 0;
}
