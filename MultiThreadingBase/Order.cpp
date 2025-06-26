#include "Order.h"
#include <corecrt.h>
#include <iostream>
#include <string>
#include "Utils.cpp"

int Order::_orderCounter = 0;
std::map<std::string, double> Order::_priceByName = {};
std::map<std::string, int> Order::_amountByName = {};

double Order::priceByName(std::string name)
{
	if (Order::_priceByName.count(name) != 1) { return 0.0; }
	return Order::_priceByName[name];
}

int Order::amountByName(std::string name, bool collectOne)
{
	if (Order::_amountByName.count(name) != 1) { return 0; }
	return collectOne ? _amountByName[name]-- : Order::_amountByName[name];
}

Order::Order(std::vector<std::string> items)
{
	_items = items;
	_id = ++Order::_orderCounter;
	_totalPrice = 0;
	_flowType = FlowType::Initiated;
}

Order::~Order(){}

void Order::validate()
{
	for (auto name : _items)
	{
		this->_status = priceByName(name) == 0 ? "InValid" : "Valid";
	}

	this->_flowType = FlowType::Validated;
	this->_flowTypeSteps += std::to_string(this->_flowType) + "; ";

	std::string message = 
		Utils::getCurrentThredIdStr() + " -> " +
		"validate(); Invoice id: " + std::to_string(this->_id) + 
		"; Status: " + this->_status + "\n";
	std::cout << message;
}

void Order::calculateTotalPrice()
{
	for (auto name : _items)
	{
		this->_totalPrice += priceByName(name);		
	}

	if (this->_status == "InValid") { this->_totalPrice = 0; }
	this->_flowType = FlowType::TotalPriceCalculated;
	this->_flowTypeSteps += std::to_string(this->_flowType) + "; ";

	std::string message =
		Utils::getCurrentThredIdStr() + " -> " +
		"calculateTotalPrice(); Invoice id: " + std::to_string(this->_id) +
		"; Total price: " + std::to_string(this->_totalPrice) + "\n";
	std::cout << message;
}

void Order::checkInventory()
{
	for (auto name : _items)
	{
		if (amountByName(name, true) == 0) {
			this->_status = "Not ordered";
			break;
		}
		else {
			this->_status =  "Ordered";
		}
	}

	if (this->_status == "Not ordered") { this->_totalPrice = 0; }
	this->_flowType = FlowType::InventoryChecked;
	this->_flowTypeSteps += std::to_string(this->_flowType) + "; ";

	std::string message =
		Utils::getCurrentThredIdStr() + " -> " +
		"checkInventory(); Invoice id: " + std::to_string(this->_id) +
		"; Status: " + this->_status + "\n";
	std::cout << message;
}

void Order::releaseInvoice()
{
	this->_flowType = FlowType::InvoicePrinted;
	this->_flowTypeSteps += std::to_string(this->_flowType) + "; ";

	std::string message =
		Utils::getCurrentThredIdStr() + " -> " +
		"releaseInvoice(); Invoice id: " + std::to_string(this->_id) +
		"; Flow steps: " + this->_flowTypeSteps +
		" Item count: " + std::to_string(this->_items.size()) +
		"; Status: " + this->_status + 
		"; Total price: " + std::to_string(this->_totalPrice) + "\n";
	std::cout << message;
}

int Order::getStatus()
{
	return this->_flowType;
}

void Order::init(
	std::vector<std::string> names, 
	std::vector<int> amounts, 
	std::vector<double> prices)
{
	for (size_t i = 0; i < names.size(); i++) {
		Order::_priceByName.insert({ names[i], prices[i]});
		Order::_amountByName.insert({ names[i], amounts[i] });
	}
}
