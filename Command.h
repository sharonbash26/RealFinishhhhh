#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <iostream>

class Command
{
	
public:
	virtual int execute() = 0;
	virtual void getFirstArgument(std::string arg) = 0;
	virtual void getSecondArgument(std::string arg) = 0;
	virtual void getThirdArgument(std::string arg)  = 0;
};

#endif