#pragma once

#include "BankStatisticsBase.h"

class BankAccountBase
{
private: 
    IBankStatistics* _statPnt;
protected:
    double balance = 1000.0;
public:
    BankAccountBase(IBankStatistics* statistic);
    virtual void deposit(double amount);
    virtual bool withdraw(double amount);
    virtual void edit_balance(double amount);
    virtual double get_balance() const;
    virtual void cashier_work();
};

