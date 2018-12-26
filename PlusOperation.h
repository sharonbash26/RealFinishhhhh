#ifndef PLUS_H
#define PLUS_H

#include "Expression.h"
#include "admin.h"

class PlusOperation : public Expression
{
    Expression* leftOperand;
    Expression* rightOperand;
public:
    PlusOperation(std::string _leftOperand, std::string _rightOperand);
	std::string calculate();
};

#endif