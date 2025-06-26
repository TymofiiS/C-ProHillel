#pragma once
#include <vector>
#include <string>
#include <map>
class Order
{
private:
    int _id;
    std::vector<std::string> _items;
    double _totalPrice;
    std::string _status;
    int _flowType;
    std::string _flowTypeSteps;

    double priceByName(std::string name);
    int amountByName(std::string name, bool collectOne);

    static int _orderCounter;
    static std::map<std::string, double> _priceByName;
    static std::map<std::string, int> _amountByName;

public:
    Order(std::vector<std::string> _items);
    ~Order();

    void validate();
    void calculateTotalPrice();
    void checkInventory();
    void releaseInvoice();
    int getStatus();

    static void init(
        std::vector<std::string> names,
        std::vector<int> amounts,
        std::vector<double> prices);

    enum FlowType {
        Initiated,
        Validated,
        TotalPriceCalculated,
        InventoryChecked,
        InvoicePrinted,
    };
};

