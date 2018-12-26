#ifndef NEG_OPERATION_H
#define NEG_OPERATION_H

#include "Expression.h"
#include "admin.h"
#include <string>

class NegOperation : public Expression
{
    Expression* operand;
public:
    NegOperation(std::string input);
	std::string calculate();
};

#endif