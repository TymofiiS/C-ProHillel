#include "BankStatisticsBase.h"
#include "BankAccountBase.h"

class UnsafeBankAccount:public BankAccountBase {
public:
    UnsafeBankAccount(IBankStatistics* stat):BankAccountBase(stat){}
};

class UnsafeBankStatistics:public IBankStatistics {
private:
    int transaction_count = 0;
    double total_amount = 0.0;

public:
    virtual void record_transaction(double amount) {
        transaction_count++;
        total_amount += amount;
    }

    virtual int get_total_transactions() const { return transaction_count; }
    virtual double get_total_amount() const { return total_amount; }
};
