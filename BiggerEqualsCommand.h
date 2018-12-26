#ifndef BIGGER_EQUALS_COMMAND_H
#define BIGGER_EQUALS_COMMAND_H

#include "Command.h"

class BiggerEqualsCommand : public Command
{	
	std::string operand1;
	std::string operand2;
public:
	int execute();
	void getFirstArgument(std::string arg){this->operand1 = arg;}
	void getSecondArgument(std::string arg){this->operand2 = arg;}
	void getThirdArgument(std::string arg){}
};

#endif