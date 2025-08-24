#include "MathEngine.h"
#include <random>
#include <iostream>
#include <functional>
#include <iomanip>
#include <sstream>
#include <regex>

static thread_local std::mt19937 rng{ std::random_device{}() };
#define disableBitwise 1
#define debugMode 0

std::string floatToStringPrecise(double f, int precision = 9) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << f;
    return oss.str();
}

std::string MathEngine::Generator::opcodeToNum(std::string equation) {
    // Truly random, will never be the same twice.
    std::srand(std::time(nullptr) ^ randomInt(randomInt(1000,9999), randomInt(1000000,9999999)) / randomInt(randomInt(1000, 9999), randomInt(1000000, 9999999)) + randomInt(randomInt(1000, 9999), randomInt(1000000, 9999999)));
    std::srand(randomInt(randomInt(1000,9999), randomInt(1000000,9999999)) / randomInt(randomInt(1000, 9999), randomInt(1000000, 9999999)) + randomInt(randomInt(1000, 9999), randomInt(1000000, 9999999))); // Pure random
    
    int staticInt = randomInt(1000, 9999);
    double staticFloat = randomFloat(1000, 9999);

    equation = MathEngine::Util::strReplace(equation, "SI", std::to_string(staticInt));
    equation = MathEngine::Util::strReplace(equation, "SF", floatToStringPrecise(staticFloat));

#if debugMode
    std::cout << equation << std::endl;
#endif

    return equation;
}
std::string MathEngine::Generator::randomZero()
{
    std::vector<std::string> equations = {
        "(SI - SI)",
        "(SF - SF)",
        "(SI ^ SI)",
        "(SI & 0)",
        "(SF * 0)",
        "(SI * 0)",
        "((SI % 1) - (SI % 1))",
        "((SI & SI) ^ (SI & SI))",
        "((1 - 1) * SI)",
        "(SF - (1 * SF)) + SF",
        "((SI >> 5) << 5) - ((SI >> 5) << 5)",
        "((int)(SF - SF))",
#if !disableBitwise
        "((SI | SI) - (SI | SI))",
#endif
        "(SF - (SF * 1.0))",
        "(SI & ~SI)",
        "(~SI & SI)",
        "(SI ^ SI)",
        "((SI << 1) - (SI << 1))",
    };

    std::string equation = equations[randomInt(0, equations.size() - 1)];

    return opcodeToNum(equation);
}

std::string MathEngine::Generator::randomOne()
{
    std::vector<std::string> equations = {
        "(SF / SF)",
        "(SI / SI)",
        "(1 * SF)",
        "(1 * SI)",
        "(SI ^ SI ^ 1)",
        "((SI & SF) | 1)",
        "((SI >> 0) / (SI >> 0))",
        "((SF + 0) / (SF + 0))",
        "((SI & SI) / SI)",
        "((SI | 1) & 1)",
        "((SI ^ 0) * 1)",
        "(SF / (SF + 0))",
        "(1 + (SI - SI))",
        "((SI * 2) / (SI * 2))",
        "(SI / (SI | 1))",
        "(SI ^ (SI ^ 1))",
        "((SI & ~0) / (SI & ~0))",
        "((int)SF / (int)SF)",
    };
    std::string equation = equations[randomInt(0, equations.size()-1)];

    return opcodeToNum(equation);
}

std::string MathEngine::Generator::randomRedundant(double input)
{
    std::vector<std::string> equations = {
        "((Input + SI) - SI)",
        "((Input + SF) - SF)",
        "((Input * 1) + (SI - SI))",
        "((Input * 1.0) + (SF - SF))",
        "((Input + 0) * 1)",
        "((Input + 999 - 999))",
        "((SI + Input) - SI)",
        "(((Input + SI + SF) - SI - SF))",
        "(((Input * 2) / 2))",
        "((Input + (SF * 0)))",
        "((Input - (0 * SI)))",
        "((Input + (SI - SI) + (SF - SF)))"
    };

    std::string equation = equations[randomInt(0, equations.size() - 1)];

    equation = MathEngine::Util::strReplace(equation, "Input", std::to_string(input));

    return opcodeToNum(equation);
}
std::string MathEngine::Generator::randomEquation(double& output)
{
    struct EquationTemplate {
        std::string formula;
        std::function<float(int, float)> evaluator;
    };
    std::vector<EquationTemplate> equations = {
    { "(RI + RF - RF)",        [](int RI, double RF) { return RI + RF - RF; } },
    { "(RF * RI / RI)",        [](int RI, double RF) { return RI != 0 ? RF * RI / RI : 0.0; } },
    { "((RI | 1) ^ (RI & 0))", [](int RI, double) { return static_cast<double>((RI | 1) ^ (RI & 0)); } },
    { "((RI ^ RI) + RF)",      [](int RI, double RF) { return 0.0 + RF; } },
    { "((RI & ~0) | (RI & 0))",[](int RI, double) { return static_cast<double>((RI & ~0) | (RI & 0)); } },
    { "(RF - (RF * 0))",       [](int, double RF) { return RF - (RF * 0); } },
    { "((RI >> 1) + (RI << 1))",[](int RI, double) { return static_cast<double>((RI >> 1) + (RI << 1)); } },
    { "((RF / (RF + 1)) * RI)",[](int RI, double RF) { return RF != -1.0 ? (RF / (RF + 1)) * RI : 0.0; } },
    { "(RI * 2 - RI)",         [](int RI, double) { return static_cast<double>(RI * 2 - RI); } },
    { "((RI & 1) * (RF + 0))", [](int RI, double RF) { return static_cast<double>((RI & 1)) * (RF + 0); } },
    { "((RI + RI) - RI)",      [](int RI, double) { return static_cast<double>((RI + RI) - RI); } },
    { "(RF + (RI * 0))",       [](int RI, double RF) { return RF + (RI * 0); } },
#if !disableBitwise
    { "((RI | RI) & RI)",      [](int RI, double) { return static_cast<double>((RI | RI) & RI); } },
#endif
    { "((RI << 3) >> 3)",      [](int RI, double) { return static_cast<double>((RI << 3) >> 3); } },
    { "(RI % (RI + 1))",       [](int RI, double) { return RI != -1 ? static_cast<double>(RI % (RI + 1)) : 0.0; } },
    { "(RF * (RI / RI))",      [](int RI, double RF) { return RI != 0 ? RF * (static_cast<double>(RI) / RI) : 0.0; } },
    { "((RI ^ RI) | (RI & RI))",[](int RI, double) { return static_cast<double>((RI ^ RI) | (RI & RI)); } },
#if !disableBitwise
    { "(RI + RF - (RI | RF))", [](int RI, double RF) { return RI + RF - (RI | static_cast<int>(RF)); } },
#endif
    { "((RI - RI) + (RF * 1))",[](int RI, double RF) { return 0.0 + RF * 1; } },
    { "(RI / 10000.0)", [](int RI, double) { return static_cast<double>(RI) / 10000.0; } },
    { "(1.0 / RI)", [](int RI, double) { return RI != 0 ? 1.0 / RI : 0.0; } },
    };


    EquationTemplate equation = equations[randomInt(0, equations.size() - 1)];
    
    // reused, but requied to find the output
    std::srand(std::time(nullptr) ^ randomInt(randomInt(1000, 9999), randomInt(1000000, 9999999)) / randomInt(randomInt(1000, 9999), randomInt(1000000, 9999999)) + randomInt(randomInt(1000, 9999), randomInt(1000000, 9999999)));
    std::srand(randomInt(randomInt(1000, 9999), randomInt(1000000, 9999999)) / randomInt(randomInt(1000, 9999), randomInt(1000000, 9999999)) + randomInt(randomInt(1000, 9999), randomInt(1000000, 9999999))); // Pure random

    int staticInt = randomInt(1000, 9999);
    double staticFloat = randomFloat(1000, 9999);
    
    std::string formula = equation.formula;
    formula = MathEngine::Util::strReplace(formula, "RI", std::to_string(staticInt));
    formula = MathEngine::Util::strReplace(formula, "RF", floatToStringPrecise(staticFloat));

    output = equation.evaluator(staticInt, staticFloat);
#if debugMode
    std::cout << formula << " = " << output << std::endl;
#endif
    return formula;
}

int MathEngine::Generator::randomInt(int min, int max) {

    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);

}

double MathEngine::Generator::randomFloat(float min, float max)
{
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    float randomValue = dist(rng);
    float returnValue = min + (max - min) * (randomValue);

   // return returnValue;
    return randomInt(min, max);
} 

std::string MathEngine::Generator::generateEquation(double input)
{
    double current = 0;
    std::string finalEquation = "(";

    int steps = randomInt(5, 12); // configurable for equation size
    while((steps--) != 0) {
        
        double output;
        std::string ranEquation = randomEquation(output);

        switch (randomInt(0, 1)) {
        case 0:
            MathEngine::Util::plugInEquation(finalEquation, ranEquation, "+");
            current += output;

#if debugMode
            std::cout << (current - output) << "+" << output << " = " << current << std::endl;
#endif
            break;
        case 1:
            MathEngine::Util::plugInEquation(finalEquation, ranEquation, "-");
            current -= output;
#if debugMode
            std::cout << (current + output) << "-" << output << " = " << current << std::endl;
#endif
            break;
        }

    }
#if debugMode
    std::cout << current << " : " << input << std::endl;
#endif
    if (current > input) {
        finalEquation += ("-" + floatToStringPrecise(current-input));
    }
    else if(current < input) {
        finalEquation += ("+" + floatToStringPrecise(input-current));
    }

    finalEquation += ")";
    return MathEngine::Util::cleanEquation(finalEquation);
}

std::string MathEngine::Util::strReplace(std::string input, std::string search, std::string replace) {
    size_t startPos = 0;

    while ((startPos = input.find(search, startPos)) != std::string::npos) {
        input.replace(startPos, search.length(), replace);
        startPos += replace.length();
    }

    return input;
}

bool MathEngine::Util::strSingleReplace(std::string& input, std::string search, std::string replace) {

    size_t startPos = input.find(search);

    if (startPos == std::string::npos)
        return false;

    input = input.replace(startPos, search.length(), replace);
    
    return true;
}

void MathEngine::Util::plugInEquation(std::string& final, std::string equation, std::string op) {
    if (final.length() == 1) {
        final += MathEngine::Generator::randomZero() + op + "((" + equation+")";
    }
    else {
        final += op + "(("+equation+")";
    }

    int junkFiller = MathEngine::Generator::randomInt(1, 3);
    for (int i = 0; i <= junkFiller; i++) {
        switch (MathEngine::Generator::randomInt(0, 3)) {
        case 0:
            final += "+" + MathEngine::Generator::randomZero();
            break;
        case 1:
            final += "-" + MathEngine::Generator::randomZero();
            break;
        case 2:
            final += "*" + MathEngine::Generator::randomOne();
            break;
        case 3:
            final += "/" + MathEngine::Generator::randomOne();
            break;

        }
    }
    final += ")";
}
std::string MathEngine::Util::cleanEquation(std::string equation) {
    std::string cleaned = equation;

    std::regex floatRegex(R"((\d+)\.(\d*?)(0+)\b)");
    cleaned = std::regex_replace(cleaned, floatRegex, "$1.$2");

    std::regex trailingDot(R"((\d+)\.)");
    cleaned = std::regex_replace(cleaned, trailingDot, "$1");

    return cleaned;
}