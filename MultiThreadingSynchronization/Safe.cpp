//#include <mutex>
#include <condition_variable>
#include <mutex>
#include "BankStatisticsBase.h"
#include "BankAccountBase.h"

class SafeBankAccount :public BankAccountBase {
private:
    mutable std::mutex _balance_mutex;
    std::condition_variable _cv;
    bool _balanceInEditMode = false;

public:
    SafeBankAccount(IBankStatistics* stat) :BankAccountBase(stat) {}

    virtual bool withdraw(double amount)
    {
        if (get_balance() < amount) { return false; }

        return BankAccountBase::withdraw(amount);
    }

    virtual double get_balance()
    {
        std::unique_lock<std::mutex> lock(_balance_mutex);
        _cv.wait(lock, [this] {return !_balanceInEditMode;});

        return BankAccountBase::get_balance();
    }

    virtual void edit_balance(double amount)
    {
        std::lock_guard<std::mutex> lock(_balance_mutex);

        _balanceInEditMode = true;
        balance += amount;
        _balanceInEditMode = false;

        _cv.notify_one();
    }
};

class SafeBankStatistics:public IBankStatistics {
private:
    std::atomic<int> transaction_count{ 0 };
    std::atomic<double> total_amount{ 0.0 }; 

public:
    void record_transaction(double amount) {
        transaction_count++;
        total_amount = total_amount.load() + amount;
    }

    int get_total_transactions() const { return transaction_count.load(); }

    double get_total_amount() const { return total_amount.load(); }
};
