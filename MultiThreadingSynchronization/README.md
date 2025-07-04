# HW 5. Synchronization and thread safety

## Task Description
Task: Multi-threaded Banking System
Problem Condition
You are developing a system for a bank where several cashiers (threads) serve customers simultaneously. You need to implement secure operations with accounts and statistics.

Requirements:

BankAccount class with methods:

deposit(amount) - depositing funds
withdraw(amount) - withdrawing funds (checking sufficiency)
get_balance() - getting balance

BankStatistics class for collecting statistics:

record_transaction(amount) - recording a transaction
get_total_transactions() - total number
get_total_amount() - total amount of transactions
Bank simulation:

5 cashiers (threads) serve customers simultaneously
Each cashier makes 100 random operations
Operations: deposit (50-500) or withdrawal (10-200)

## Files
- `MultiThreadingSynchronization.cpp` - main program file
- `BankAccountBase.h` - bank account base class declaration
- `BankAccountBase.cpp` - bank account base class implementation
- `BankStatisticsBase.h` - bank statistic base class declaration
- `Safe.cpp` - safe class implementation
- `Unsafe.cpp` - unsafe class implementation
- `Utils.cpp` - some helper methods implementation

## Compilation and Run
```bash
g++ -o program MultiThreadingSynchronization.cpp
./program