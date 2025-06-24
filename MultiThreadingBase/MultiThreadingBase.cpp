/*
Task 1: Parallel Calculation of Mathematical Functions
Create a program that calculates different mathematical functions in separate threads:
Thread 1: calculates the factorial of 15
Thread 2: finds the sum of squares of numbers from 1 to 1000
Thread 3: calculates Fibonacci numbers up to the 30th element
Thread 4: finds all prime numbers up to 100
Each thread outputs its result independently of the others. 
Threads should be in a thread pool, not created separately!


Task 2: Create a system where:
Flow 1: validates initial orders (checks format, data correctness)
Flow 2: waits for flow 1 to complete, calculates prices for valid orders
Flow 3: waits for flow 2, checks for stock availability
Flow 4: waits for flow 3, generates invoices and sends to customers
*/

#include <vector>
#include <iostream>
#include "Order.h"
#include "OrderProcessor.h"
#include <thread>
#include "SimpleThreadPool.cpp"
#include "Utils.cpp"

int main() 
{ 
	std::cout << "\n=== Task 1 ===\n";

	SimpleThreadPool pool(4);

	// Thread 1: calculates the factorial of 15
	pool.addTask([](int n)
		{
			std::string message =
				"\n" + Utils::getCurrentThredIdStr() +
				" -> Factorial of " + std::to_string(n) +
				" is " + std::to_string(Utils::factorial(n));
			std::cout << message;
		}, 15);
	
	// Thread 2: finds the sum of squares of numbers from 1 to 1000
	pool.addTask([](int n)
		{
			std::string message =
				"\n" + Utils::getCurrentThredIdStr() +
				" -> The sum of squares of numbers from 1 to " + std::to_string(n) +
				" is " + std::to_string(Utils::sumSquares(n));
			std::cout << message;
		}, 1000);

	// Thread 3: calculates Fibonacci numbers up to the 30th element
	pool.addTask([](int n)
		{
			std::string message =
				"\n" + Utils::getCurrentThredIdStr() +
				" -> Fibonacci numbers up to the " + std::to_string(n) +
				" is " + std::to_string(Utils::fib(n));
			std::cout << message;
		}, 30);

	// Thread 4: finds all prime numbers up to 100
	pool.addTask([](int n)
		{
			std::string message =
				"\n" + Utils::getCurrentThredIdStr() +
				" -> Prime numbers up to " + std::to_string(n) +
				" is " + Utils::primeNumbers(n);
			std::cout << message;
		}, 100);

	// Wait for all threads
	pool.waitAll();


	std::cout << "\n\n=== Task 2 ===\n\n";

	// Initial data
	Order::init(
		{ "item1", "item2", "item3" }, 
		{ 1,2,3 }, 
		{ 10.1, 20.2, 30.3 }
	);
	
	std::vector<Order> orders = {
		Order({ "item1", "item2"}),		
		Order({ "item3", "item1", "item2"}),
		Order({ "item1", "item4"}) 
	};

	auto orderProcessor = OrderProcessor();

	// Flow 1: validates initial orders (checks format, data correctness)	
	std::thread validateOrders(
		&OrderProcessor::validateOrders, 
		&orderProcessor, 
		std::ref(orders));		

	// Flow 2: waits for flow 1 to complete, calculates prices for valid orders	
	std::thread calculatePricing(
		&OrderProcessor::calculatePricing, 
		&orderProcessor, 
		std::ref(orders));	

	// Flow 3: waits for flow 2, checks for stock availability
	std::thread checkInventory(
		&OrderProcessor::checkInventory,
		&orderProcessor,
		std::ref(orders));	

	// Flow 4: waits for flow 3, generates invoices and sends to customers
	std::thread generateInvoices(
		&OrderProcessor::generateInvoices,
		&orderProcessor,
		std::ref(orders));	

	// Wait for all threads
	validateOrders.join();
	calculatePricing.join();
	checkInventory.join();
	generateInvoices.join();
	
	return 0;
}