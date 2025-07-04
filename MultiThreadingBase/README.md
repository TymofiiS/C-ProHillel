# HW 4. Multitasking (basics)

## Task Description
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

## Files
- `MultiThreadingBase.cpp` - main program file
- `Order.h` - order class declaration
- `Order.cpp` - order class implementation
- `OrderProcessor.h` - order processor class declaration
- `OrderProcessor.cpp` - order processor class implementation
- `SimpleThreadPool.cpp` - simple thread pool class implementation
- `Utils.cpp` - some helper methods implementation

## Compilation and Run
```bash
g++ -o program MultiThreadingBase.cpp
./program