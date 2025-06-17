#include <iostream>
#include "FileManager.h"
#include "Sensor.h"

int main()
{
	// Test that uses FileManager to write text to a file.
	try {
	    FileManager myFile("example.txt", "w");
	    myFile.write("Hello with RAII-03");
	}
	catch (const std::exception& e) {
	    std::cerr << "Error: " << e.what() << "\n";
	}

	// Simulate simultaneous use of shared sensor data:
	// Create two Sensor instances that share the same 
	// std::vector<int> via a std::shared_ptr.
	Sensor s1 = Sensor();
	Sensor s2 = s1;

	// Add data from sensor 1
	s1.add(1);
	s1.add(2);

	std::cout << "\n\nSensor 1:\n";
	s1.print();

	// Add data from sensor 2
	s2.add(3);
	s2.add(4);

	std::cout << "\n\nSensor 2:\n";
	s2.print();

	std::cout << "\n\nSensor 1:\n";
	s1.print();

	std::cout << "\n\n";
}
