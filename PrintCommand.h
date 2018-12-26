#ifndef UNTITLED13_PRINTCOMMAND_H
#define UNTITLED13_PRINTCOMMAND_H

#include "Command.h"

class PrintCommand : public Command {
    std::string operand1;
    std::string operand2;

public:
    int execute() ;
    void getFirstArgument(std::string arg){this->operand1 = arg;}
    void getSecondArgument(std::string arg){}
    void getThirdArgument(std::string arg){}
};

#endif