#ifndef CONNECT_COMMAND_H
#define CONNECT_COMMAND_H

#include "Command.h"
#include <vector>
#include <map>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

class ConnectCommand : public Command
{
	std::string ip;
	std::string port;
	std::map <std::string, std::string> varMap;
    std::map <std::string, std::string> bindMap;
	std::vector<std::string> bindedVariables;
	int sockfdGeneral;
public:
	int execute();
	void getFirstArgument(std::string arg){this->ip = arg;}
	void getSecondArgument(std::string arg);
	void getThirdArgument(std::string arg){};
	void setVarsAndBinds(std::map <std::string, std::string> _varMap,
		std::map <std::string, std::string> _bindMap,
		std::vector<std::string> _bindedVariables){
			varMap = _varMap;
			bindMap = _bindMap;
			bindedVariables = _bindedVariables;
	}
	void connectToServer(std::string value, std::string link);
};

#endif