#pragma once
class IBankStatistics
{
public:
    virtual ~IBankStatistics(){}
    virtual void record_transaction(double amount) = 0;
    virtual int get_total_transactions() const = 0;
    virtual double get_total_amount() const = 0;
};

