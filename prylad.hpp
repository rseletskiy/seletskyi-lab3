#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <utility>

class Prylad {
private:
    std::string startPoint;
    std::string endPoint;
    int durationDays = 0;

public:
    Prylad() = default;

    Prylad(std::string startPoint, std::string endPoint, int durationDays)
        : startPoint(std::move(startPoint)), endPoint(std::move(endPoint)),
          durationDays(durationDays) {}

    bool input() {
        std::string start, end, line;
        int days = 0;

        std::cout << "Start point: ";
        if (!std::getline(std::cin, start)) return false;

        std::cout << "End point: ";
        if (!std::getline(std::cin, end)) return false;

        while (true) {
            std::cout << "Duration (days): ";
            if (!std::getline(std::cin, line)) return false;

            std::istringstream value(line);
            if (value >> days && days > 0 && (value >> std::ws).eof()) break;

            std::cout << "Enter a positive integer.\n";
        }

        startPoint = std::move(start);
        endPoint = std::move(end);
        durationDays = days;
        return true;
    }

    void output() const {
        std::cout << "Start point: " << startPoint << '\n'
                  << "End point: " << endPoint << '\n'
                  << "Duration (days): " << durationDays << '\n';
    }
};
