#include "prylad.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

void expect(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

std::string describe(const Prylad& route) {
    std::ostringstream output;
    auto* previous = std::cout.rdbuf(output.rdbuf());
    route.output();
    std::cout.rdbuf(previous);
    return output.str();
}

bool enter(Prylad& route, const std::string& text) {
    std::istringstream input(text);
    std::ostringstream prompts;
    auto* previousInput = std::cin.rdbuf(input.rdbuf());
    auto* previousOutput = std::cout.rdbuf(prompts.rdbuf());
    std::cin.clear();
    const bool result = route.input();
    std::cin.rdbuf(previousInput);
    std::cin.clear();
    std::cout.rdbuf(previousOutput);
    return result;
}

int main() {
    try {
        const Prylad empty;
        expect(describe(empty) == "Start point: \nEnd point: \nDuration (days): 0\n",
               "Default constructor must initialize an empty route");

        const Prylad initialized("Lviv", "Kyiv", 2);
        expect(describe(initialized) == "Start point: Lviv\nEnd point: Kyiv\nDuration (days): 2\n",
               "Parameterized constructor must preserve all three values");

        Prylad route;
        expect(enter(route, "New York\nLos Angeles\n1\n"), "Valid console input");
        expect(describe(route) == "Start point: New York\nEnd point: Los Angeles\nDuration (days): 1\n",
               "Names with spaces and minimum duration must be preserved");

        const std::string invalidValues[] = {"0", "-2", "abc", "2.5", "7days", "999999999999999999999999"};
        for (const auto& invalid : invalidValues) {
            expect(enter(route, "Lviv\nKyiv\n" + invalid + "\n3\n"), "Retry invalid duration");
            expect(describe(route) == "Start point: Lviv\nEnd point: Kyiv\nDuration (days): 3\n",
                   "Invalid duration must not be accepted: " + invalid);
        }

        expect(enter(route, "Odesa\nDnipro\n  4  \n"), "Surrounding whitespace is valid");
        expect(describe(route) == "Start point: Odesa\nEnd point: Dnipro\nDuration (days): 4\n",
               "Duration with whitespace must be parsed");

        const std::string incompleteInputs[] = {"", "Lviv\n", "Lviv\nKyiv\n", "Lviv\nKyiv\n-1\n"};
        for (const auto& input : incompleteInputs) {
            expect(!enter(route, input), "EOF must stop input");
            expect(describe(route) == "Start point: Odesa\nEnd point: Dnipro\nDuration (days): 4\n",
                   "Incomplete input must preserve the previous route");
        }

        expect(enter(route, "Lviv\nKyiv\n5"), "Last line need not end with a newline");
        expect(describe(route) == "Start point: Lviv\nEnd point: Kyiv\nDuration (days): 5\n",
               "Final value without a newline must be stored");

        std::cout << "All tests passed\n";
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
