#include "BigData.h"
#include <iostream>
#include <vector>


void BigData::moveHelper(BigData& other) noexcept
{
	this->data = other.data;
	this->size = other.size;

	other.data = nullptr;
	other.size = 0;
}

void BigData::copyHelper(const BigData& other)
{
	delete data;
	this->data = new int[other.size];

	for (int i = 0; i < other.size; ++i)
	{
		this->data[i] = other.data[i];
	}

	this->size = other.size;
}

BigData::BigData(size_t size)
{
	std::cout << "\nConstructor by size for " << this <<" started\n";
	this->size = size;
	this->data = new int[this->size];
	for (size_t i = 0; i < this->size; ++i){ this->data[i] = (int)i; }
}

BigData::~BigData()
{
	std::cout << "\nDestructor for " << this << " started\n";
	delete data;
}

BigData::BigData(const BigData& other)
{
	std::cout << "\nCopy constructor for " << this << " started\n";
	copyHelper(other);
}

BigData& BigData::operator=(const BigData& other)
{
	std::cout << "\nCopy operator for " << this << " started\n";
	copyHelper(other);
	return *this;
}

BigData::BigData(BigData&& other) noexcept
{
	std::cout << "\nMove constructor for " << this << " started\n";
	moveHelper(other);
}

BigData& BigData::operator=(BigData&& other) noexcept
{
	std::cout << "\nMove operator for " << this << " started\n";
	moveHelper(other);	
	return *this;
}

void BigData::print() const
{
	std::cout << "\nPrint method for " << this << " started\n";
	for (int i = 0; i < this->size; ++i) { std::cout << this->data[i] << "\n"; }
}

void BigData::modifyData(std::function<void(std::vector<int>&, int)> callBack, int inc)
{
	std::vector<int> v;
	for (size_t i = 0; i < this->size; ++i) { v.push_back( this->data[i]); }

	callBack(v, inc);

	for (size_t i = 0; i < this->size; ++i) { this->data[i] = v[i]; }
}


