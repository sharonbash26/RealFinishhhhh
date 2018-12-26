#ifndef UNTITLED13_SLEEPCOMMAND_H
#define UNTITLED13_SLEEPCOMMAND_H

#include <string>
#include "Command.h"
//#include <chrono>
#include <thread>

class SleepCommand: public Command {
    std::string operand1;
public:
    int execute() override ;
    void getFirstArgument(std::string arg){this->operand1 = arg;}
    void getSecondArgument(std::string arg){}
    void getThirdArgument(std::string arg){}
};


#endif