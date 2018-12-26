#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>

class Expression
{
public:
	virtual std::string calculate() = 0;
};

#endif