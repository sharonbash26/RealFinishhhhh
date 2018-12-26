#include "EqualsCommand.h"

int EqualsCommand::execute() {

    double operandDouble1 = std::stod(this->operand1);
    double operandDouble2 = std::stod(this->operand2);

    if (operandDouble1 == operandDouble2) {
        return 1;
    }

    return 0;
}