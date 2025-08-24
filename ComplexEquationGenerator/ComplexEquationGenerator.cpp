#include <iostream>
#include <string>
#include <limits>
#include "MathEngine.h"

int main() {
    using namespace MathEngine::Generator;

    while (true) {
        std::cout << "Enter a number (or 'q' to quit): ";
        std::string input;
        std::cin >> input;

        if (input == "q" || input == "Q") {
            std::cout << "Exiting...\n";
            break;
        }

        try {
            double value = std::stod(input);
            std::string result = generateEquation(value);
            std::cout << "Generated Equation: " << result << "\n";
        }
        catch (const std::invalid_argument&) {
            std::cout << "[!] Invalid input. Please enter a number.\n";
        }
        catch (const std::out_of_range&) {
            std::cout << "[!] Number out of range.\n";
        }
    }

    return 0;
}
