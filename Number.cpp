#include "Number.h"

Number::Number(std::string input) {
    this->number = input;
}

std::string Number::calculate() {
    return this->number;
}