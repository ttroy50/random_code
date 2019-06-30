# Variant Based Polymorphism for C++11

Example of variant based polymorphism with C++11 and boost variant. 

This example is used to show how you can have dynamic runtime polymorphism without having to use inheritance to create a generic interface. This can be useful if you don't want to (or can't) change your classes to support a common interface.

In it I create a simple calculator that evaluates from left to right and supports addition, subtraction, or multiplication. The classes for doing the operation are not related but can be stored in a queue in the order they should be applied. 

A visitor is used to apply the correct operation and shows how the different classes do not have to have the same interface to be called. 

It is possible to write a cleaner version of this using C++17 (and probably C++14) but I needed to target C++11.

## Building

To build you need to have cmake, boost headers, and a compiler that supports C++11.

```
mkdir build
cd build
cmake ..
make
./vbp "1 + 2 * 3 - 1"
calculating arg 0 : 1 + 2 * 3 - 1 
calc is 8
```

## References

 * [Boost Variant](https://www.boost.org/doc/libs/1_70_0/doc/html/variant/tutorial.html)
