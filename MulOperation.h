#ifndef MUL_H
#define MUL_H

#include "Expression.h"
#include "admin.h"

class MulOperation : public Expression
{
    Expression* leftOperand;
    Expression* rightOperand;
public:
    MulOperation(std::string _leftOperand, std::string _rightOperand);
	std::string calculate();
};

#endif