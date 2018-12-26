#include "DivOperation.h"

DivOperation::DivOperation(std::string _leftOperand, std::string _rightOperand) {
    admin admin;
    this->leftOperand = admin.determineCurrentOperation(_leftOperand);
    this->rightOperand = admin.determineCurrentOperation(_rightOperand);
}

std::string DivOperation::calculate() {
    std::string leftOperandResult = this->leftOperand->calculate();
    std::string rightOperandResult = this->rightOperand->calculate();

    double leftOperandResultDouble = std::stod(leftOperandResult);
    double rightOperandResultDouble = std::stod(rightOperandResult);

    double sum = leftOperandResultDouble / rightOperandResultDouble;

    return std::to_string(sum);
}