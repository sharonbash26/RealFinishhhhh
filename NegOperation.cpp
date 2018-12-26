#include "NegOperation.h"

NegOperation::NegOperation(std::string input) {
    admin admin;
    this->operand = admin.determineCurrentOperation(input);
}

std::string NegOperation::calculate() {
    std::string operandResult = this->operand->calculate();

    double operandResultDouble = std::stod(operandResult);

    double sum = (-1) * operandResultDouble;

    return std::to_string(sum);
}