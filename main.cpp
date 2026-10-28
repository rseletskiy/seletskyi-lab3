#include "prylad.hpp"

#include <iostream>

int main() {
    Prylad first;
    const Prylad second("Lviv", "Kyiv", 2);

    std::cout << "Default constructor:\n";
    first.output();

    std::cout << "\nParameterized constructor:\n";
    second.output();

    std::cout << "\nEnter route data:\n";
    if (!first.input()) return 1;

    std::cout << "\nAfter input:\n";
    first.output();
}
