/*
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
Operations: deposit (50-500 UAH) or withdrawal (10-200 UAH)
*/

#include <iostream>
#include "Unsafe.cpp"
#include "Safe.cpp"
#include "BankStatisticsBase.h"
#include "../MultiThreadingBase/SimpleThreadPool.cpp"

int main()
{
	// Initiate Unsafe simulation
	IBankStatistics* unsafeStatPnt = new UnsafeBankStatistics();
	UnsafeBankAccount unsafeBankAcc(unsafeStatPnt);

	std::cout << "\n\n===  Unsafe Bank Simulation started ===";
	SimpleThreadPool pool(5);
	for (int i = 0; i < 5; i++) 
	{
		pool.addTask(&UnsafeBankAccount::cashier_work, &unsafeBankAcc);
	}
	pool.waitAll();

	std::cout << "\n\n===  Unsafe Bank Simulation Results ===";
	std::cout << "\nInitial balance : " << "1000";
	std::cout << "\nFinal balance : " << unsafeBankAcc.get_balance();
	std::cout << "\nTotal transactions : " << unsafeStatPnt->get_total_transactions();
	std::cout << "\nTotal transaction amount : " << unsafeStatPnt->get_total_amount();
	std::cout << std::endl;

	delete unsafeStatPnt;


	// Initiate Safe simulation
	IBankStatistics* safeStatPnt = new SafeBankStatistics();
	SafeBankAccount safeBankAcc(safeStatPnt);

	std::cout << "\n\n===  Safe Bank Simulation started ===";
	for (int i = 0; i < 5; i++)
	{
		pool.addTask(&SafeBankAccount::cashier_work, &safeBankAcc);
	}
	pool.waitAll();

	std::cout << "\n\n===  Safe Bank Simulation Results ===";
	std::cout << "\nInitial balance : " << "1000";
	std::cout << "\nFinal balance : " << safeBankAcc.get_balance();
	std::cout << "\nTotal transactions : " << safeStatPnt->get_total_transactions();
	std::cout << "\nTotal transaction amount : " << safeStatPnt->get_total_amount();
	std::cout << std::endl;

	delete safeStatPnt;
}