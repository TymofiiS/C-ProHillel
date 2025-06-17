#pragma once
#include <memory>
#include <vector>
class Sensor
{
private:
	std::shared_ptr<std::vector<int>> _data;
public:
	Sensor();
	void add(int);
	void print();
};

