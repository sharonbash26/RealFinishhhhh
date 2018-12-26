#ifndef MINUS_H
#define MINUS_H

#include "Expression.h"
#include "admin.h"

class MinusOperation : public Expression
{
    Expression* leftOperand;
    Expression* rightOperand;
public:
    MinusOperation(std::string _leftOperand, std::string _rightOperand);
	std::string calculate();
};

#endif