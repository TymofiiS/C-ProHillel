#pragma once
#include "Order.h"
#include <functional>
#include <string>
#include <vector>
class OrderProcessor {
public:
    std::vector<Order> validateOrders(std::vector<Order>& rawOrders);
    std::vector<Order> calculatePricing(std::vector<Order>& validOrders);
    std::vector<Order> checkInventory(std::vector<Order>& pricedOrders);
    void generateInvoices(std::vector<Order>& finalOrders);

    std::vector<Order> iteratorHelper(
        std::vector<Order>& orders, 
        int status, 
        std::function<void(Order*)> callBack,
        std::string methodName);
};

