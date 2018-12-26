#ifndef NUMBER_H
#define NUMBER_H

#include "Expression.h"

class Number : public Expression
{
    std::string number;
public:
    Number(std::string input);
	std::string calculate();
};

#endif