
// Write a Sensor class that:
// Stores sensor data in a std::shared_ptr<std::vector<int>>.
// Provides functions for adding and printing sensor data.

#include "Sensor.h"
#include <iostream>

Sensor::Sensor()
{
	_data = std::make_shared<std::vector<int>>();
}

void Sensor::add(int d)
{
	_data->push_back(d);
}

void Sensor::print()
{
	std::cout << "Data address: " << _data << " from sensor address: " << this << "\n";

	for (size_t i = 0; i < _data->size(); i++)
	{
		std::cout << _data->at(i) << "; ";
	}	
}
