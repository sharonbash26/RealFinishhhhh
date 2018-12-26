#include "admin.h"

Command* globalOpenDataServerCommand = NULL;
Command* globalConnectCommand = NULL;
bool isScriptParcingFinished = false;
bool stopForClientsConnection = false;
std::map <std::string, std::string> varMap;

void createMainTread(std::string file) {
    admin admin;

    std::ifstream readFile(file.c_str());
    std::string lineFromFile;
    std::string script;

    if (!readFile.is_open()) {
        return;
    }

    while (std::getline(readFile, lineFromFile)) {
        script += '|';
        script += lineFromFile;
    }

    script += '|';

    readFile.close();

    std::vector<std::string> fff = admin.lexer(script);

    for (int i = 0; i < fff.size(); i++) {

        auto it_front = fff.begin();
        advance(it_front, i);
        std::string str = *it_front;
    }

    admin.parcer(admin.lexer(script));
}

void createClient() {
    while (isScriptParcingFinished == false) {
        if (!stopForClientsConnection) {
            if (globalConnectCommand != NULL) {
                globalConnectCommand->execute();
            }
        }
    }
}

void createServer() {

    admin admin;

    while (isScriptParcingFinished == false) {
        if (globalOpenDataServerCommand != NULL) {
            if (!stopForClientsConnection) {
                varMap 
                    = ((OpenDataServerCommand*)globalOpenDataServerCommand)->updateValues();
            }
        }
    }
}

int main(int argc, char* argv[]) {
    std::string file = argv[1];
    thread mainThread(createMainTread,file);
    thread clientThread(createClient);
    thread serverThread(createServer);

    clientThread.join();
    serverThread.join();
    mainThread.join();

    return 0;
}

std::vector<std::string> admin::lexer (std::string input) {

    std::vector<std::string> scriptsList;
    int len = input.length();
    std::string curItem;

    bool isCommandReadNow = false;
    bool isExpressionReadNow = false;

    int commandCurrentSequenceLength = 0;
    int expressionCurrentSequenceLength = 0;
    int spaceCurrentSequenceLength = 0;

    char lastNonSpaceChar = ' ';

    for (int i = 0; i < len; i++) {

        if (curItem.length() == 0 && (input[i] == ' ' || input[i] == '\n'
                                        || input[i] == '\t')) {

            continue;
        }

        if (input[i] == '|') {
            for(int i2 = 0; i2 < curItem.length(); i2++) {
                if (curItem[i2] == ' ' || curItem[i2] == '\t' || curItem[i2] == '\n') {
                    std::string tempStr = curItem.substr(i2 + 1, curItem.length() - i2 - 1);
                    curItem = curItem.substr(0, i2) + tempStr;
                    i2--;
                }
            }

            scriptsList.push_back(curItem);
            scriptsList.push_back("|");
            curItem.clear();
        }

        if (input[i] == '}' && curItem.length() > 0) {
            for(int i2 = 0; i2 < curItem.length(); i2++) {
                if (curItem[i2] == ' ' || curItem[i2] == '\t' || curItem[i2] == '\n') {
                    std::string tempStr = curItem.substr(i2 + 1, curItem.length() - i2 - 1);
                    curItem = curItem.substr(0, i2) + tempStr;
                    i2--;
                }
            }

            scriptsList.push_back(curItem);
            scriptsList.push_back("|");
            scriptsList.push_back("}");
            scriptsList.push_back("|");
            curItem.clear();
            continue;
        }

        if (input[i] == '{' && curItem.length() > 0) {
            for(int i2 = 0; i2 < curItem.length(); i2++) {
                if (curItem[i2] == ' ' || curItem[i2] == '\t' || curItem[i2] == '\n') {
                    std::string tempStr = curItem.substr(i2 + 1, curItem.length() - i2 - 1);
                    curItem = curItem.substr(0, i2) + tempStr;
                    i2--;
                }
            }

            scriptsList.push_back(curItem);
            scriptsList.push_back("|");
            scriptsList.push_back("{");
            scriptsList.push_back("|");
            curItem.clear();
            continue;
        }

        if (input[i] == ',' && expressionCurrentSequenceLength > 0 
            && curItem.length() > 0 && !this->isContainsSpacesOnly(curItem)) {

            for(int i2 = 0; i2 < curItem.length(); i2++) {
                if (curItem[i2] == ' ' || curItem[i2] == '\t' || curItem[i2] == '\n') {
                    std::string tempStr = curItem.substr(i2 + 1, curItem.length() - i2 - 1);
                    curItem = curItem.substr(0, i2) + tempStr;
                    i2--;
                }
            }
            scriptsList.push_back(curItem);
            scriptsList.push_back(",");
            curItem.clear();

            commandCurrentSequenceLength = 0;
            expressionCurrentSequenceLength = 0;
            continue;
        }

        int curChar = input[i];

        if((curChar >= 97 && curChar <= 122) || (curChar >= 65 && curChar <= 90)
            || input[i] == '"' || input[i] == '=' || input[i] == '<' || input[i] == '>'
            || input[i] == '_' || input[i] == '{' || input[i] == '}') {

            if (curItem.length() > 0 && commandCurrentSequenceLength == 0
                && !this->isContainsSpacesOnly(curItem)) {

                for(int i2 = 0; i2 < curItem.length(); i2++) {
                    if (curItem[i2] == ' ' || curItem[i2] == '\t' || curItem[i2] == '\n') {
                        std::string tempStr = curItem.substr(i2 + 1, curItem.length() - i2 - 1);
                        curItem = curItem.substr(0, i2) + tempStr;
                        i2--;
                    }
                }

                scriptsList.push_back(curItem);
                curItem.clear();
            }

            isCommandReadNow = true;
            isExpressionReadNow = false;

            commandCurrentSequenceLength++;
            expressionCurrentSequenceLength = 0;
        }

        else if(input[i] == '(' || input[i] == ')'
                || input[i] == '+' || input[i] == '*' || input[i] == ' ' 
                || input[i] == '\n' || input[i] == '\t'|| input[i] == '.'
                || (input[i] == '-'  && expressionCurrentSequenceLength == 0
                    && commandCurrentSequenceLength == 0)
                || (isdigit(input[i]) && expressionCurrentSequenceLength == 0
                    && commandCurrentSequenceLength == 0)) {

            if (curItem.length() > 0 && expressionCurrentSequenceLength == 0
                && !this->isContainsSpacesOnly(curItem)) {

                for(int i2 = 0; i2 < curItem.length(); i2++) {
                    if (curItem[i2] == ' ' || curItem[i2] == '\t' || curItem[i2] == '\n') {
                        std::string tempStr = curItem.substr(i2 + 1, curItem.length() - i2 - 1);
                        curItem = curItem.substr(0, i2) + tempStr;
                        i2--;
                    }
                }

                scriptsList.push_back(curItem);
                curItem.clear();
            }

            isCommandReadNow = false;
            isExpressionReadNow = true;

            commandCurrentSequenceLength = 0;
            expressionCurrentSequenceLength++;
        }

        if (((curChar >= 97 && curChar <= 122) || (curChar >= 65 && curChar <= 90)
            || input[i] == '"' || input[i] == '=' || input[i] == '<' || input[i] == '>'
            || input[i] == '-' || input[i] == '/' || isdigit(input[i]) || input[i] == '_'
            || input[i] == '{' || input[i] == '}') 
            && commandCurrentSequenceLength > 0) {
            
            curItem += input[i];
        }

        else if ((isdigit(input[i]) || input[i] == '(' || input[i] == ')'
                || input[i] == '+' || input[i] == '-' || input[i] == '*'
                || input[i] == '/' || input[i] == ' ' || input[i] == '\n'
                || input[i] == '\t'|| input[i] == '.') && expressionCurrentSequenceLength > 0) {

            if (isdigit(lastNonSpaceChar) && spaceCurrentSequenceLength > 0 && isdigit(input[i])) {
                if (curItem.length() > 0 && !this->isContainsSpacesOnly(curItem)) {

                    for(int i2 = 0; i2 < curItem.length(); i2++) {
                        if (curItem[i2] == ' ' || curItem[i2] == '\t' || curItem[i2] == '\n') {
                            std::string tempStr = curItem.substr(i2 + 1, curItem.length() - i2 - 1);
                            curItem = curItem.substr(0, i2) + tempStr;
                            i2--;
                        }
                    }

                    scriptsList.push_back(curItem);
                    curItem.clear();

                    commandCurrentSequenceLength = 0;
                    expressionCurrentSequenceLength = 1;
                }
            }

            curItem += input[i];
        }

        if (!(input[i] == ' ' || input[i] == '\n' || input[i] == '\t')) {

            lastNonSpaceChar = input[i];
            spaceCurrentSequenceLength = 0;
        }

        else if(input[i] == ' ' || input[i] == '\n' || input[i] == '\t') {
            spaceCurrentSequenceLength++;
        }

        if ((i == len -1) && curItem.length() > 0) {
            if (!this->isContainsSpacesOnly(curItem)) {

                for(int i2 = 0; i2 < curItem.length(); i2++) {
                    if (curItem[i2] == ' ' || curItem[i2] == '\t' || curItem[i2] == '\n') {
                        std::string tempStr = curItem.substr(i2 + 1, curItem.length() - i2 - 1);
                        curItem = curItem.substr(0, i2) + tempStr;
                        i2--;
                    }
                }

                scriptsList.push_back(curItem);
                curItem.clear();
            }
        }
    }

    return scriptsList;
}

std::vector<std::string> admin::substituteVariablesValues(std::vector<std::string> items
    , int startLineIndex, int endLineIndex) {

    for (int i2 = startLineIndex; i2 < endLineIndex; i2++) {

            auto it_front = items.begin();
            advance(it_front, i2);
            std::string item = *it_front;

            it_front = items.begin();
            advance(it_front, i2+1);
            std::string nextItem = *it_front;

            if (item == "var") {

                varMap[nextItem] = "0";
                items.erase(items.begin() + i2);
                i2--;
                endLineIndex--;

                for (int i3 = 0; i3 < newLineIndexes.size(); i3++) {
                    auto it_front = newLineIndexes.begin();
                    advance(it_front, i3);

                    int curEndLineIndex = *it_front;
                    curEndLineIndex--;
                    newLineIndexes.at(i3) = curEndLineIndex;
                }
            }

            std::string curVar = varMap[item];
            if (curVar.length() > 0 && nextItem != "=") {
                items.at(i2) = curVar;
            }
        }
        return items;
}

std::vector<std::string> admin::fixSyntaxCorrection(std::vector<std::string> items
    , int startLineIndex, int endLineIndex) {
    
        for (int i2 = startLineIndex; i2 < endLineIndex; i2++) {
            auto it_front = items.begin();
            advance(it_front, i2);
            std::string item = *it_front;

            std::string prevItem;
            
            if (i2 > 0) {
                it_front = items.begin();
                advance(it_front, i2 - 1);
                prevItem = *it_front;
            }

            if (prevItem.length() > 0
                && (item[0] == ')' || item[0] == '+' || item[0] == '*' || item[0] == '/'
                || (item[0] == '-' && prevItem != "," && prevItem != "="
                    && commandsMap.find(item) != commandsMap.end()))) {

                prevItem += item;
                items.erase(items.begin() + i2);
                i2--;
                items.at(i2) = prevItem;
                endLineIndex--;

                for (int i3 = 0; i3 < newLineIndexes.size(); i3++) {
                    auto it_front = newLineIndexes.begin();
                    advance(it_front, i3);

                    int curEndLineIndex = *it_front;
                    curEndLineIndex--;
                    newLineIndexes.at(i3) = curEndLineIndex;
                }
            }
        }

        for (int i2 = endLineIndex - 1; i2 >= startLineIndex; i2--) {
            auto it_front = items.begin();
            advance(it_front, i2);
            std::string item = *it_front;

            std::string nextItem;
            
            if (i2 + 1 < items.size()) {
                it_front = items.begin();
                advance(it_front, i2 + 1);
                nextItem = *it_front;
            }

            if (nextItem.length() > 0
                && (item[item.length() - 1] == '(' 
                || ((item[item.length() - 1] == '+' 
                || item[item.length() - 1] == '*' || item[item.length() - 1] == '/'
                || item[item.length() - 1] == '-') && (item.length() - 1 != 0)))) {
                item += nextItem;
                items.erase(items.begin() + i2 + 1);
                items.at(i2) = item;
                endLineIndex--;

                for (int i3 = 0; i3 < newLineIndexes.size(); i3++) {
                    auto it_front = newLineIndexes.begin();
                    advance(it_front, i3);

                    int curEndLineIndex = *it_front;
                    curEndLineIndex--;
                    newLineIndexes.at(i3) = curEndLineIndex;
                }
            }
        }
        return items;
}

std::vector<std::string> admin::calculateInternalExpressions(
    std::vector<std::string> items, int startLineIndex, int endLineIndex) {

            for (int i2 = startLineIndex; i2 < endLineIndex; i2++) {

            auto it_front = items.begin();
            advance(it_front, i2);
            std::string item = *it_front;
            
            std::string prevItem;
            std::string prevPrevItem;
            std::string nextItem;

            if (i2 - 2 >= 0) {
                it_front = items.begin();
                advance(it_front, i2 - 1);
                prevItem = *it_front;

                it_front = items.begin();
                advance(it_front, i2 - 2);
                prevPrevItem = *it_front;
            }

            if (i2 < items.size() - 1) {
                it_front = items.begin();
                advance(it_front, i2 + 1);
                nextItem = *it_front;
            }

            if (this->isExpressionToCalculation(item)) {
                item = this->determineCurrentOperation(item)->calculate();
                items.at(i2) = item;

                if (prevPrevItem.length() > 0) {
                    if (prevItem == "=") {

                        varMap[prevPrevItem] = item;

                        Command* conectCommand = commandsMap["connect"];
                        if (conectCommand != NULL) {
                            ((ConnectCommand*)conectCommand)->setVarsAndBinds(varMap, bindMap
                            , bindedVariables);
                        }
                        
                        Command* openDataServerCommand = commandsMap["openDataServer"];
                        if (openDataServerCommand != NULL) {
                            ((OpenDataServerCommand*)openDataServerCommand)->setVarsAndBinds(
                                varMap, bindMap, bindedVariables);
                        }
                    }
                }
            }

            if (item == "bind") {
                if (prevPrevItem.length() > 0 && nextItem.length() > 0) {
                    bindMap[prevPrevItem] = nextItem;
                    bindedVariables.push_back(prevPrevItem);

                    Command* conectCommand = commandsMap["connect"];
                    if (conectCommand != NULL) {
                        ((ConnectCommand*)conectCommand)->setVarsAndBinds(varMap, bindMap
                        , bindedVariables);
                    }
                        
                    Command* openDataServerCommand = commandsMap["openDataServer"];
                    if (openDataServerCommand != NULL) {
                        ((OpenDataServerCommand*)openDataServerCommand)->setVarsAndBinds(
                            varMap, bindMap, bindedVariables);
                    }
                }
            }
        }
        return items;
}

void admin::parcer (std::vector<std::string> script) {

    commandsMap["openDataServer"] = new OpenDataServerCommand();
    commandsMap["connect"] = new ConnectCommand();
    commandsMap["="] = NULL;
    commandsMap["bind"] = NULL;
    commandsMap["if"] = NULL;
    commandsMap["while"] = NULL;
    commandsMap["<"] = new SmallerCommand();
    commandsMap[">"] = new BiggerCommand();
    commandsMap["<="] = new SmallerEqualsCommand();
    commandsMap[">="] = new BiggerEqualsCommand();
    commandsMap["=="] = new EqualsCommand();
    commandsMap["sleep"] = new SleepCommand();
    commandsMap["print"] = new PrintCommand();

    std::vector<std::string> items;

    for (int i = 0; i < script.size(); i++) {

        auto it_front = script.begin();
        advance(it_front, i);
        std::string item = *it_front;

        items.push_back(item);

        if (item == "|") {
            newLineIndexes.push_back(i);
            newLineIndexesOriginal.push_back(i);
        }
    }

 //   std::vector<std::string> items = script;

    for (int i = 0; i < newLineIndexes.size(); i++) {

        if (i == newLineIndexes.size() - 1) {
            isScriptParcingFinished = true;
        }

        while (!items.empty()) {
            items.pop_back();
        }

        for (int i2 = 0; i2 < script.size(); i2++) {
            items.push_back(script[i2]);
        }

        while (!newLineIndexes.empty()) {
            newLineIndexes.pop_back();
        }

        for (int i2 = 0; i2 < newLineIndexesOriginal.size(); i2++) {
            newLineIndexes.push_back(newLineIndexesOriginal[i2]);
        }

        auto it_front = newLineIndexes.begin();
        advance(it_front, i);

        int endLineIndex = *it_front;
        int startLineIndex = 0;
        
        if (i != 0) {
            it_front = newLineIndexes.begin();
            advance(it_front, i - 1);
            startLineIndex = *it_front + 1;
        }


        items = this->substituteVariablesValues(items, startLineIndex, endLineIndex);

        items = this->fixSyntaxCorrection(items, startLineIndex, endLineIndex);

        items = this->calculateInternalExpressions(items, startLineIndex, endLineIndex);

        for (int i2 = startLineIndex; i2 < endLineIndex; i2++) {

            auto it_front = items.begin();
            advance(it_front, i2);
            std::string item = *it_front;
            
            std::string prevItem;
            std::string prevPrevItem;
            std::string nextItem;

            if (i2 - 2 >= 0) {
                it_front = items.begin();
                advance(it_front, i2 - 1);
                prevItem = *it_front;

                it_front = items.begin();
                advance(it_front, i2 - 2);
                prevPrevItem = *it_front;
            }

            if (i2 < items.size() - 1) {
                it_front = items.begin();
                advance(it_front, i2 + 1);
                nextItem = *it_front;
            }

            if (item == "connect") {
                Command* conectCommand = commandsMap[item];
                if (items[++i2] == ",") {
                    ++i2;
                }
                conectCommand->getFirstArgument(items[i2]);
                if (items[++i2] == ",") {
                    ++i2;
                }
                conectCommand->getSecondArgument(items[i2]);
                ((ConnectCommand*)conectCommand)->setVarsAndBinds(varMap, bindMap
                    , bindedVariables);
                
                globalConnectCommand = conectCommand;

                continue;
            }

            if (item == "openDataServer") {
                Command* openDataServerCommand = commandsMap[item];
                if (items[++i2] == ",") {
                    ++i2;
                }
                openDataServerCommand->getFirstArgument(items[i2]);
                if (items[++i2] == ",") {
                    ++i2;
                }

                stopForClientsConnection = true;

                ((OpenDataServerCommand*)openDataServerCommand)
                        ->setVarsAndBinds(varMap, bindMap, bindedVariables);

                globalOpenDataServerCommand = openDataServerCommand;
                ((OpenDataServerCommand*)openDataServerCommand)->getConnection();
                stopForClientsConnection = false;

                continue;
            }

            if (item == "sleep" && i2 + 1 < items.size()) {
                double milisecondsToSleep = std::stod(items[i2 + 1]);
                milisecondsToSleep /= 1000;
                std::this_thread::sleep_for(std::chrono::seconds((int)milisecondsToSleep));
            }

            if (item == "print" && i2 + 1 < items.size()) {
                std::string str = items[i2 + 1];
                if (str[0] == '\"'
                            && str[str.length() - 1] == '\"') {
            
                    str = str.substr(1, str.length() - 2);
                }
                std::cout<<str<<"\n";
            }

            if (item == "while" || item == "if") {
                if (i2 + 3 < items.size()) {
                    Command *conditionCommand = commandsMap[items[i2 + 2]];
                    conditionCommand->getFirstArgument(items[i2 + 1]);
                    conditionCommand->getSecondArgument(items[i2 + 3]);
                    int result = conditionCommand->execute();

                    int numOfOpenedBraces = 0;

                    for (int i3 = i; i3 < newLineIndexes.size() - 1; i3++) {
                        int indexOfCurLine = newLineIndexes[i3] + 1;

                        if (items[indexOfCurLine] == "{") {
                            numOfOpenedBraces++;
                        }
                        if (items[indexOfCurLine] == "}") {
                            numOfOpenedBraces--;
                            if (numOfOpenedBraces == 0) {
                                beginBlockIndexesSteak.push_back(i);
                                endBlockIndexesSteak.push_back(i3);
                                nameOfBlocksSteak.push_back(item);
                            }
                        }
                    }

                    if (result == 0) {

                        i = endBlockIndexesSteak[endBlockIndexesSteak.size() - 1];

                        beginBlockIndexesSteak.pop_back();
                        endBlockIndexesSteak.pop_back();
                        nameOfBlocksSteak.pop_back();

                        if (i + 1 < newLineIndexes.size() - 1) {
                            i++;
                            i2 = newLineIndexes[i] + 1;
                            continue;
                        }
                        else {
                            i = newLineIndexes.size() - 1;
                            break;
                        }
                    }
                }
            }

            if (item == "}") {

                if (nameOfBlocksSteak[nameOfBlocksSteak.size() - 1] == "if") {
                    beginBlockIndexesSteak.pop_back();
                    endBlockIndexesSteak.pop_back();
                    nameOfBlocksSteak.pop_back();

                }
                else if (nameOfBlocksSteak[nameOfBlocksSteak.size() - 1] == "while") {

                    i = beginBlockIndexesSteak[beginBlockIndexesSteak.size() - 1];

                    i--;

                    beginBlockIndexesSteak.pop_back();
                    endBlockIndexesSteak.pop_back();
                    nameOfBlocksSteak.pop_back();

                    break;
                }
            }
        }  
    }
    this->print("after", items);
}

void admin::print(std::string str, std::vector<std::string> items) {
    for (int i = 0; i < items.size(); i++) {
        auto it_front = items.begin();
        advance(it_front, i);
        std::string item = *it_front;
    }
}

bool admin::isExpressionToCalculation(std::string input) {

    if (input == "+" || input == "-" || input == "*" || input == "/") {
        return false;
    }

    int numOFBrackets = 0;

    for (int i = 0; i < input.length(); i++) {

        if (input[i] == '(' || input[i] == ')') {
            numOFBrackets++;
        }

        if (!(input[i] == '.' || input[i] == '(' || input[i] == ')'
            || input[i] == '+' || input[i] == '-' || input[i] == '*'
            || input[i] == '/' || isdigit(input[i]))) {
                
            return false;
        }
    }

    if (numOFBrackets % 2 == 1) {
        return false;
    }

    return true;
}

Expression* admin::determineCurrentOperation(std::string expression) {

    int len = expression.length();

    // if an expression in shape "(exp)", remove brackets to get shape "exp"
    if (expression[0] == '(' && expression[len - 1] == ')') {
        expression = expression.substr(1, len - 2);
        len -= 2;
    }

    if (expression.empty()) {
        return new Number("0");
    }
/*
    if (expression[0] == '-') {

        std::string restOfEcpression = expression.substr(1, expression.length() - 1);

         if (restOfEcpression[0] != '(' && restOfEcpression[restOfEcpression.length() - 1] != ')') {
            std::string temp = "(";
            temp += restOfEcpression;
            temp += ")";
            restOfEcpression = temp;
        }

        return new NegOperation(restOfEcpression);
    }
*/
    bool isNumber = true;

    for (int i = 0; i < len; i++) {
        if (!isdigit(expression[i]) && expression[i] != '.') {
            isNumber = false;
            break;
        }
    }

    if (isNumber) {
        return new Number(expression);
    }

    int numOfOpenedBrackets = 0;

    for (int i = 0; i < len; i++) {

        if (expression[i] == '(') {
            numOfOpenedBrackets++;
        }

        if (expression[i] == ')') {
            numOfOpenedBrackets--;
        }

        if (numOfOpenedBrackets == 0 && (expression[i] == '+' || expression[i] == '-'
                                        || expression[i] == '*' || expression[i] == '/')) {
            
            if (expression[i] == '+') {
                std::string leftOperand = expression.substr(0, i);
                std::string rightOperand = expression.substr(i + 1, len - i -1);

                if (leftOperand[0] != '(' && leftOperand[leftOperand.length() - 1] != ')') {
                    std::string temp = "(";
                    temp += leftOperand;
                    temp += ")";
                    leftOperand = temp;
                }

                if (rightOperand[0] != '(' && rightOperand[rightOperand.length() - 1] != ')') {
                    std::string temp = "(";
                    temp += rightOperand;
                    temp += ")";
                    rightOperand = temp;
                }

                return new PlusOperation(leftOperand, rightOperand);
            }

            if (expression[i] == '-' && i != 0) {

                std::string leftOperand = expression.substr(0, i);
                std::string rightOperand = expression.substr(i + 1, len - i -1);

                if (leftOperand[0] != '(' && leftOperand[leftOperand.length() - 1] != ')') {
                    std::string temp = "(";
                    temp += leftOperand;
                    temp += ")";
                    leftOperand = temp;
                }

                if (rightOperand[0] != '(' && rightOperand[rightOperand.length() - 1] != ')') {
                    std::string temp = "(";
                    temp += rightOperand;
                    temp += ")";
                    rightOperand = temp;
                }

                return new MinusOperation(leftOperand, rightOperand);
            }
        }
    }

    numOfOpenedBrackets = 0;

    for (int i = 0; i < len; i++) {

        if (expression[i] == '(') {
            numOfOpenedBrackets++;
        }

        if (expression[i] == ')') {
            numOfOpenedBrackets--;
        }

        if (numOfOpenedBrackets == 0 && (expression[i] == '+' || expression[i] == '-'
                                        || expression[i] == '*' || expression[i] == '/')) {

            if (expression[i] == '*') {

                std::string leftOperand = expression.substr(0, i);
                std::string rightOperand = expression.substr(i + 1, len - i -1);

                if (leftOperand[0] != '(' && leftOperand[leftOperand.length() - 1] != ')') {
                    std::string temp = "(";
                    temp += leftOperand;
                    temp += ")";
                    leftOperand = temp;
                }

                if (rightOperand[0] != '(' && rightOperand[rightOperand.length() - 1] != ')') {
                    std::string temp = "(";
                    temp += rightOperand;
                    temp += ")";
                    rightOperand = temp;
                }

                return new MulOperation(leftOperand, rightOperand);
            }

            if (expression[i] == '/') {

                std::string leftOperand = expression.substr(0, i);
                std::string rightOperand = expression.substr(i + 1, len - i -1);

                if (leftOperand[0] != '(' && leftOperand[leftOperand.length() - 1] != ')') {
                    std::string temp = "(";
                    temp += leftOperand;
                    temp += ")";
                    leftOperand = temp;
                }

                if (rightOperand[0] != '(' && rightOperand[rightOperand.length() - 1] != ')') {
                    std::string temp = "(";
                    temp += rightOperand;
                    temp += ")";
                    rightOperand = temp;
                }


                return new DivOperation(leftOperand, rightOperand);
            }
        }
    }

    numOfOpenedBrackets = 0;

    for (int i = 0; i < len; i++) {

        if (expression[i] == '(') {
            numOfOpenedBrackets++;
        }

        if (expression[i] == ')') {
            numOfOpenedBrackets--;
        }

        if (numOfOpenedBrackets == 0 && (expression[i] == '+' || expression[i] == '-'
                                        || expression[i] == '*' || expression[i] == '/')) {

        
            if (expression[i] == '-' && i == 0) {
                std::string restOfEcpression = expression.substr(1, expression.length() - 1);

                if (restOfEcpression[0] != '(' 
                    && restOfEcpression[restOfEcpression.length() - 1] != ')') {
                    std::string temp = "(";
                    temp += restOfEcpression;
                    temp += ")";
                    restOfEcpression = temp;
                }

            return new NegOperation(restOfEcpression);
            }
        }
    }
}

bool admin::isContainsSpacesOnly(std::string input) {

    for (int i = 0; i < input.length(); i++) {
        if (!(input[i] == ' ' || input[i] == '\t' || input[i] == '\n')) {
            return false;
        }
    }

    return true;
}