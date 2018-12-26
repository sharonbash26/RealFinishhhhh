#ifndef ADMIN_H
#define ADMIN_H

#include <vector>
#include <map>
#include <string.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <thread>
#include <chrono>

#include "Command.h"
#include "Expression.h"
#include "Number.h"
#include "PlusOperation.h"
#include "MinusOperation.h"
#include "MulOperation.h"
#include "DivOperation.h"
#include "NegOperation.h"

#include "OpenDataServerCommand.h"
#include "ConnectCommand.h"
#include "BiggerCommand.h"
#include "BiggerEqualsCommand.h"
#include "SmallerCommand.h"
#include "SmallerEqualsCommand.h"
#include "EqualsCommand.h"
#include "SleepCommand.h"
#include "PrintCommand.h"
#include "OpenDataServerCommand.h"

using namespace std;

class admin {
    
    std::map <std::string, std::string> bindMap;
    std::vector<std::string> bindedVariables;
    std::vector<int> newLineIndexes;
    std::vector<int> newLineIndexesOriginal;
    std::vector<int> beginBlockIndexesSteak;
    std::vector<int> endBlockIndexesSteak;
    std::vector<std::string> nameOfBlocksSteak;
    std::map <std::string, Command*> commandsMap;
public:
    std::vector<std::string> lexer (std::string input);
    void parcer (std::vector<std::string> items);
    Expression* determineCurrentOperation(std::string expression);
    bool isExpressionToCalculation(std::string input);
    bool isContainsSpacesOnly(std::string input);
    std::vector<std::string> substituteVariablesValues(std::vector<std::string> items
        , int startLineIndex, int endLineIndex);
    std::vector<std::string> fixSyntaxCorrection(std::vector<std::string> items
        , int startLineIndex, int endLineIndex);
    std::vector<std::string> calculateInternalExpressions(
        std::vector<std::string> items, int startLineIndex, int endLineIndex);
    void print(std::string str, std::vector<std::string> items);
};

void createClient();
void createServer();
void createMainTread(std::string file);

#endif