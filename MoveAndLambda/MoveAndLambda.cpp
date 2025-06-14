/*

Requirements:

Implement a constructor, copy constructor, copy operator, move constructor, and move operator.

Create a function that returns a BigData object(by value) to demonstrate how moving works.

Add logs(console output) to each constructor / destructor / operator to see when an object is copied and when it is moved.

Write a Lambda expression that grabs any STL container you like by reference.
It also takes a specific value as a parameter, by which it increments each element of this container.
*/

#include <iostream>
#include "BigData.h"
#include <vector>

int main()
{
    // Initial instance
    BigData initial(5);

    std::cout << "\nInitial print";
    initial.print();

    // Function that returns a BigData object(by value) to demonstrate how moving works.
    auto bigDataObjectByValueMove = [](BigData& obj) {
            return std::move(obj);
        };
    BigData final = bigDataObjectByValueMove(initial);

    std::cout << "\nPrint after move";
    initial.print();
    final.print();

    // Apply data modification function
    final.modifyData([](std::vector<int>& v, int inc)
        {
            for (int i = 0; i < v.size(); i++){ v[i] += inc; }
        },
        10);

    std::cout << "\nPrint after modification";
    final.print();

    return 0;
}

