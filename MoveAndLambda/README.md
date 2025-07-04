# HW 2. Movement semantics and lambda expressions

## Task Description
Implement a constructor, copy constructor, copy operator, move constructor, and move operator.
Create a function that returns a BigData object(by value) to demonstrate how moving works.
Add logs(console output) to each constructor / destructor / operator to see when an object is copied and when it is moved.
Write a Lambda expression that grabs any STL container you like by reference.
It also takes a specific value as a parameter, by which it increments each element of this container.

## Files
- `MoveAndLambda.cpp` - main program file
- `BigData.h` - big data class declaration
- `BigData.cpp` - big data class implementation

## Compilation and Run
```bash
g++ -o program MoveAndLambda.cpp
./program