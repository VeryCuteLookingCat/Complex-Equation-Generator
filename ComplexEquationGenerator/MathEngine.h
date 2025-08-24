#pragma once
#include <string>
namespace MathEngine {
    namespace Generator {

        // Generates a complex equation string that approximates the given input value.
        // @param input: The target number the equation should evaluate to.
        // @return: A string representation of the generated equation.
        std::string generateEquation(double input);

        // Returns a string representing a "zero" equation using random combinations of SI/SF and operations.
        // Useful for building larger equations with redundant or filler operations.
        // @return: A string equation that evaluates to zero.
        std::string randomZero();

        // Returns a string representing a "one" equation using random combinations of SI/SF and operations.
        // Useful for building larger equations with redundant or filler operations.
        // @return: A string equation that evaluates to one.
        std::string randomOne();

        // Generates a redundant equation that incorporates the input value without changing its meaning.
        // Essentially creates an "obfuscated" version of a number.
        // @param input: The numeric value to embed in the equation.
        // @return: A string equation equivalent to input.
        std::string randomRedundant(double input);

        // Generates a random equation from predefined templates and computes its numeric output.
        // @param output: Reference to a double that receives the evaluated numeric result of the equation.
        // @return: The string representation of the random equation.
        std::string randomEquation(double& output);

        // Replaces placeholders like SI/SF in a given equation string with actual numeric values.
        // @param equation: The equation string with placeholders.
        // @return: The equation string with actual numbers substituted.
        std::string opcodeToNum(std::string equation);

        // Returns a random integer between min and max (inclusive) using a thread-safe RNG.
        // @param min: Minimum integer value.
        // @param max: Maximum integer value.
        // @return: A random integer in the range [min, max].
        int randomInt(int min, int max);

        // Returns a random floating-point number between min and max.
        // @param min: Minimum float value.
        // @param max: Maximum float value.
        // @return: A random double in the range [min, max].
        double randomFloat(float min, float max);
    }

    namespace Util {

        // Replaces all occurrences of a substring in a string with another string.
        // @param input: The original string.
        // @param search: The substring to replace.
        // @param replace: The string to insert instead.
        // @return: Modified string with replacements.
        std::string strReplace(std::string input, std::string search, std::string replace);

        // Replaces the first occurrence of a substring in a string with another string.
        // @param input: Reference to the string to modify.
        // @param search: The substring to replace.
        // @param replace: The string to insert instead.
        // @return: True if a replacement was made, false if not found.
        bool strSingleReplace(std::string& input, std::string search, std::string replace);

        // Inserts a sub-equation into a larger equation string with an operator and optional random filler.
        // @param final: Reference to the string being built.
        // @param equation: The equation to insert.
        // @param op: Operator to combine with existing equation ("+", "-", etc.).
        void plugInEquation(std::string & final, std::string equation, std::string op);

        // Cleans up a generated equation string by removing unnecessary trailing zeros and dots.
        // @param equation: The equation string to clean.
        // @return: A cleaner, more readable equation string.
        std::string cleanEquation(std::string equation);
    }
}
