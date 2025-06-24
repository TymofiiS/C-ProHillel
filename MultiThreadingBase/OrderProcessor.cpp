#include "OrderProcessor.h"
#include <functional>
#include <iostream>

std::vector<Order> OrderProcessor::validateOrders(std::vector<Order>& rawOrders)
{
	return iteratorHelper(
		rawOrders,
		(int)Order::FlowType::Initiated, 
		[](Order* orderPnt) {orderPnt->validate();},
		"validateOrders");
}

std::vector<Order> OrderProcessor::calculatePricing(std::vector<Order>& validOrders)
{
	return iteratorHelper(
		validOrders,
		(int)Order::FlowType::Validated,
		[](Order* orderPnt) {orderPnt->calculateTotalPrice();},
		"calculatePricing");
}

std::vector<Order> OrderProcessor::checkInventory(std::vector<Order>& pricedOrders)
{
	return iteratorHelper(
		pricedOrders,
		(int)Order::FlowType::TotalPriceCalculated,
		[](Order* orderPnt) {orderPnt->checkInventory();},
		"checkInventory");
}

void OrderProcessor::generateInvoices(std::vector<Order>& finalOrders)
{
	iteratorHelper(
		finalOrders,
		(int)Order::FlowType::InventoryChecked,
		[](Order* orderPnt) {orderPnt->releaseInvoice();},
		"generateInvoices");
}

std::vector<Order> OrderProcessor::iteratorHelper(
	std::vector<Order>& orders,
	int status,
	std::function<void(Order*)> callBack,
	std::string methodName ) {

	int counter = 0;
	while (counter != orders.size())
	{
		for (auto& order : orders) {
			if (order.getStatus() != (Order::FlowType)status) { continue; }
			callBack(&order);
			counter++;
		}
	}

	return orders;
}