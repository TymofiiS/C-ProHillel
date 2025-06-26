#include "../MultiThreadingBase/Utils.cpp"
#include "BankAccountBase.h"
#include "BankStatisticsBase.h"
#include "Utils.cpp"
#include <iostream>
#include <string>

BankAccountBase::BankAccountBase(IBankStatistics* statPnt)
{
	_statPnt = statPnt;
}

void BankAccountBase::deposit(double amount)
{
	edit_balance(amount);
	_statPnt->record_transaction(amount);
}

bool BankAccountBase::withdraw(double amount)
{
	if (balance < amount) { return false; }

	edit_balance((-1)*amount);
	_statPnt->record_transaction(amount);

	return true;
}

void BankAccountBase::edit_balance(double amount)
{
	balance += amount;
}

double BankAccountBase::get_balance() const
{
	return balance;
}

void BankAccountBase::cashier_work()
{
	std::string message = "\nCashier N " +
		Utils::getCurrentThredIdStr(false) + " started work";
	std::cout << message;

	Randomiser randomDeposit(50, 500);
	Randomiser randomWithdrawal(10, 200);

	int deposited = 0;
	int withdrawn = 0;
	for (int i = 0; i < 50; i++) {
		int curDep = randomDeposit.randomInt();
		int curWith = randomWithdrawal.randomInt();
		deposited += curDep;
		withdrawn += curWith;
		deposit(curDep);
		withdraw(curWith);
	}

	message = "\nCashier N " +
		Utils::getCurrentThredIdStr(false) + " finished work" +
		"; Work statistic: +" + std::to_string(deposited) + 
		"; -" + std::to_string(withdrawn);
	std::cout << message;
}
