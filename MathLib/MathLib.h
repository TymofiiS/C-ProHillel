#pragma once

#include "pch.h"

namespace MathLib {

	// Basic arithmetic operations
	double add(double a, double b);
	double subtract(double a, double b);
	double multiply(double a, double b);
	double divide(double a, double b); // Handle division by zero

	// Mathematical functions
	double power(double base, int exponent);
	double squareRoot(double value);
	double absoluteValue(double value);

	// Integer functions
	int factorial(int n);
	bool isPrime(int number);
	int gcd(int a, int b);

	// Array operations
	double findMax(const double arr[], int size);
	double findMin(const double arr[], int size);
	double calculateSum(const double arr[], int size);
	double calculateAverage(const double arr[], int size);
	void sortArray(double arr[], int size);

	// Utility functions
	bool isEven(int number);
	bool isOdd(int number);
	int fibonacci(int n);

} // namespace MathLib
