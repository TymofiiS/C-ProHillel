#pragma once
#include <vector>
#include <functional>
class BigData {
private:
	int* data;
	size_t size;

	void moveHelper(BigData& other) noexcept;
	void copyHelper(const BigData& other);

public:
	BigData(size_t size);
	~BigData();

	// Copy constructor
	BigData(const BigData& other);

	// Copy operator
	BigData& operator=(const BigData& other);

	// Move constructor
	BigData(BigData&& other) noexcept;

	// Move operator
	BigData& operator=(BigData&& other) noexcept;

	void print() const;

	void modifyData(std::function< void(std::vector<int>&, int)> callBack, int);
};


