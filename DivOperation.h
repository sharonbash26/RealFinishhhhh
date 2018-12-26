#ifndef DIV_H
#define DIV_H

#include "Expression.h"
#include "admin.h"

class DivOperation : public Expression
{
    Expression* leftOperand;
    Expression* rightOperand;
public:
    DivOperation(std::string _leftOperand, std::string _rightOperand);
	std::string calculate();
};

#endif